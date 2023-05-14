/*****************************************************************
*\file		 ParticleCollisionSystem.cpp
*\brief		 Particle collision systems functions definitions.

*\author(s)   Lee Liang Ping	82 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <FakeEngine.h>
#include <ParticleCollisionSystem.h>
#include <Collision.h>
#include <SystemList.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;

void ParticleCollisionSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSParticlesManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<ParticleType>());
	signature.set(mGameECSmanager.GetComponentType<ParticleStates>());
	mGameECSmanager.SetSystemBitSignature<ParticleCollisionSystem>(signature);
}

void ParticleCollisionSystem::Update()
{
	auto& mParticleECSmanager = gFakeEngine->GetECSManager<ECSManager>(EntitySystemOption::PARTICLE_ENTITY);
	auto& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(EntitySystemOption::GAME_ENTITY);
	CollisionList& collisionList = gFakeEngine->GetGameManager<CollisionList>();
	auto cList= collisionList.GetCollisionParticleList(EntitySystemOption::GAME_ENTITY, CollisionType::CHARGING_LASER_PROJECTILE);
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();

	for (auto const& f_entity : mEntities)
	{
		Flags& f_flag = mParticleECSmanager.GetComponent<Flags>(f_entity);
		if (0 == (f_flag.activeFlag & FLAG_ACTIVE))
			continue;

		AABB& f_aabb = mParticleECSmanager.GetComponent<AABB>(f_entity);
		Rigidbody& f_rigidbody = mParticleECSmanager.GetComponent<Rigidbody>(f_entity);
		ParentId& f_ParentId = mParticleECSmanager.GetComponent<ParentId>(f_entity);
		for (auto const& s_entity : cList)
		{
			Flags& s_flag = mGameECSmanager.GetComponent<Flags>(s_entity);
			if (0 == (s_flag.activeFlag & FLAG_ACTIVE))
				continue;

			EntityType& s_objecttype = mGameECSmanager.GetComponent<EntityType>(s_entity);
			if (s_objecttype.objType == ObjectType::LASER_PROJECTILE)
			{
				ParentId& s_ParentId = mGameECSmanager.GetComponent<ParentId>(s_entity);
				Rigidbody& s_rigidbody = mGameECSmanager.GetComponent<Rigidbody>(s_entity);
				AABB& s_aabb = mGameECSmanager.GetComponent<AABB>(s_entity);
				if (s_ParentId.id == f_ParentId.id)
				{
					if (Collision::Aabb::Intersection_RectRect(f_aabb, f_rigidbody.velocity, s_aabb, s_rigidbody.velocity))
					{
						Transform& s_transform = mGameECSmanager.GetComponent<Transform>(s_entity);
						s_transform.scale.y = s_transform.scale.x += 5.0f;
						if (s_transform.scale.x >= 80.f)
						{
							s_transform.scale.y = s_transform.scale.x = 80.f;
							Laser& laser = mGameECSmanager.GetComponent<Laser>(s_ParentId.id);
							laser.laserState = LaserState::LASER_FIRE;
							collisionList.RemoveCollisionList(s_entity, EntitySystemOption::GAME_ENTITY, CollisionType::CHARGING_LASER_PROJECTILE);
						}
						continue;
					}
				}
			}

		}



		ParticleType& type = mParticleECSmanager.GetComponent<ParticleType>(f_entity);
		if (type.pType == ParticleTypesEnum::PARTICLE_PICKUP)
		{
			ParentId& id = mParticleECSmanager.GetComponent<ParentId>(f_entity);
			AABB& s_aabb = mGameECSmanager.GetComponent<AABB>(id.id);
			Rigidbody& s_rigidbody = mGameECSmanager.GetComponent<Rigidbody>(id.id);

			if (Collision::Aabb::Intersection_RectRect(f_aabb, f_rigidbody.velocity, s_aabb, s_rigidbody.velocity))
			{
				LifeTime& lifetime = mParticleECSmanager.GetComponent<LifeTime>(f_entity);
				Position& position = mParticleECSmanager.GetComponent<Position>(f_entity);
				position = mGameECSmanager.GetComponent<Position>(globalVar.level_doorID);
				lifetime.isTherelifeTime = true;
				
				auto& mLightSystem = gFakeEngine->GetECSLightManager<LightSystemsList>();
				auto& mLightECSmanager = gFakeEngine->GetECSLightManager<ECSManager>();
				for (auto const& light_entity : mLightSystem.keyObjectives_lightSystem->mEntities)
				{
					ET_Light_KeyObjectives& et_Light_ko = mLightECSmanager.GetComponent<ET_Light_KeyObjectives>(light_entity);
					if (f_entity == et_Light_ko.followingId)
						et_Light_ko.state = TypeState::Active;
				}
			}

		}

	}
}