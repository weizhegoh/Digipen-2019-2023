/*****************************************************************
*\file		 LaserProjectileSystem.cpp
*\brief		 LaserProjectileSystem functions definitions.

*\author(s)   Lee Liang Ping	103 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <FakeEngine.h>
#include <Collision.h>
#include <LaserProjectileSystem.h>
#include <EntityCreation.h>
extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;
void LaserProjectileSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	signature.set(mGameECSmanager.GetComponentType<AABB>());
	signature.set(mGameECSmanager.GetComponentType<Flags>());
	signature.set(mGameECSmanager.GetComponentType<Rigidbody>());
	mGameECSmanager.SetSystemBitSignature<LaserProjectileSystem>(signature);
}

void LaserProjectileSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Boolean& boolean = gFakeEngine->GetGameManager<Boolean>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	for (std::set<Entity>::const_reverse_iterator f_entity = mEntities.rbegin(); f_entity != mEntities.rend(); ++f_entity)
	{

		Flags& f_flag = mGameECSmanager.GetComponent<Flags>(*f_entity);
		if (0 == (f_flag.activeFlag & FLAG_ACTIVE))
			continue;

		EntityType& f_objecttype = mGameECSmanager.GetComponent<EntityType>(*f_entity);
		if (f_objecttype.objType != ObjectType::LASER_PROJECTILE)
			continue;
		
		State& f_state = mGameECSmanager.GetComponent<State>(*f_entity);
		if (f_state.objectState != ObjectState::PUSHING_BARREL)
			continue;

		Rigidbody& f_rigidbody = mGameECSmanager.GetComponent<Rigidbody>(*f_entity);
		AABB& f_aabb = mGameECSmanager.GetComponent<AABB>(*f_entity);
		for (std::set<Entity>::const_reverse_iterator s_entity = mEntities.rbegin(); s_entity != mEntities.rend(); ++s_entity)
		{
			Flags& s_flag = mGameECSmanager.GetComponent<Flags>(*s_entity);
			if (0 == (s_flag.activeFlag & FLAG_ACTIVE))
				continue;
			EntityType& s_objecttype = mGameECSmanager.GetComponent<EntityType>(*s_entity);
			if (s_objecttype.objType == ObjectType::LASER_PROJECTILE)
				continue;
			Position& s_position = mGameECSmanager.GetComponent<Position>(*s_entity);
			Rigidbody& s_rigidbody = mGameECSmanager.GetComponent<Rigidbody>(*s_entity);
			AABB& s_aabb = mGameECSmanager.GetComponent<AABB>(*s_entity);

			ObjectDirection direction = f_state.InverseDir();
			mathEngine::Vector3D position{ s_aabb.max.x, s_position.pos.y, 0.f };

			if (s_objecttype.objType == ObjectType::WALL || s_objecttype.objType == ObjectType::PUSHABLE_BARREL)
			{
				if (Collision::Aabb::Intersection_RectRect(f_aabb, f_rigidbody.velocity, s_aabb, s_rigidbody.velocity))
				{
					f_flag.activeFlag = FLAG_INACTIVE;
					gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
						position, 10.f, direction,
						ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS,
						std::make_pair<float, float>(60.f, 80.f),
						std::make_pair<float, float>(0.f, 0.f),
						std::make_pair<float, float>(8.f, 10.f),
						std::make_pair<float, float>(0.5f, 0.6f),
						std::vector<ParticleAction>(),
						{ 1.f, 1.f, 0.f }, { 1.f,0.f,1.f,0.9f }, 1.f, ShaderOption::FULL_TEXTURE,
						ObjectShapes::SQUARE, TextureType::GLOWINGBALL, NULL, mathEngine::RandNumberBetween(10, 30), false, PhysicFlag{ P_NORMAL }));

					ParentId& f_parentId = mGameECSmanager.GetComponent<ParentId>(*f_entity);
					EntityCreation::AssignEntity(EntitySystemOption::GAME_ENTITY, f_parentId.id, Laser({ LaserState::LASER_IDLE , false, 0, NULL }));
					mGameECSmanager.DestroyEntity(*f_entity);
					
					Inventory& mc_inventory = mGameECSmanager.GetComponent<Inventory>(globalVar.mainCharID);
					if(mc_inventory._currBoulder != NULL && s_objecttype.objType == ObjectType::PUSHABLE_BARREL)
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::LASER_EXPLODE), FMOD_LOOP_OFF, 0.5f);

					break;
				}
			}
			else if (s_objecttype.objType == ObjectType::MAIN_CHARACTER)
			{
				if (Collision::Aabb::Intersection_RectRect(f_aabb, f_rigidbody.velocity, s_aabb, s_rigidbody.velocity))
				{
					gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
						position, 10.f, direction,
						ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS,
						std::make_pair<float, float>(60.f, 80.f),
						std::make_pair<float, float>(0.f, 0.f),
						std::make_pair<float, float>(8.f, 10.f),
						std::make_pair<float, float>(0.5f, 0.6f),
						std::vector<ParticleAction>(),
						{ 1.f, 1.f, 0.f }, { 1.f,0.f,1.f,0.9f }, 1.f, ShaderOption::FULL_TEXTURE,
						ObjectShapes::SQUARE, TextureType::GLOWINGBALL, NULL, mathEngine::RandNumberBetween(10, 30), false, PhysicFlag{ P_NORMAL }));
							
					Input& input = gFakeEngine->GetGraphicManager<Input>();
					if (!input.GetInputBooleen().god_mode)
					{
						EntityCreation::AssignEntity(EntitySystemOption::GAME_ENTITY, *s_entity,
							State{ ObjectState::DEATH, ObjectDirection::NO_DIR });
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DEATH), FMOD_LOOP_OFF, gAudioEngine->volume);
						boolean.True(BooleanType::MOVEMENT_LOCK);
					}
					
					ParentId& f_parentId = mGameECSmanager.GetComponent<ParentId>(*f_entity);
					EntityCreation::AssignEntity(EntitySystemOption::GAME_ENTITY, f_parentId.id, Laser({ LaserState::LASER_IDLE , false, 0, NULL }));
					mGameECSmanager.DestroyEntity(*f_entity);
					break;
				}
			}
		}
	}
}