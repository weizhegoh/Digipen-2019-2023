/*****************************************************************
*\file		 LaserSystem.cpp
*\brief		 LaserSystem functions definitions.

*\author(s)   Lee Liang Ping	132 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <FakeEngine.h>
#include <LaserSystem.h>
#include <EntityCreation.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;
void LaserSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<Laser>());
	mGameECSmanager.SetSystemBitSignature<LaserSystem>(signature);
}

void LaserSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Timer& timer = gFakeEngine->GetGameManager<Timer>();
	//GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	if (timer.CheckSingletonTimer(TimerMap::LASER_IMAGE, 0.5f))
	{
		for (auto const& entity : mEntities)
		{
			Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
			if (0 == (flag.activeFlag & FLAG_ACTIVE))
				continue;
			EntityType& entitytype = mGameECSmanager.GetComponent<EntityType>(entity);
			if (entitytype.objType != ObjectType::TURRET)
				continue;
			Laser& laser = mGameECSmanager.GetComponent<Laser>(entity);
			if (laser.lifetimeCounter)
				laser.lifetimeCounter = false;
			else
				laser.lifetimeCounter = true;
		}
	}


	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;
		EntityType& entitytype = mGameECSmanager.GetComponent<EntityType>(entity);
		if (entitytype.objType != ObjectType::TURRET)
			continue;

		Laser& laser = mGameECSmanager.GetComponent<Laser>(entity);
		Position& position = mGameECSmanager.GetComponent<Position>(entity);

		if (laser.lifetimeCounter)
		{
			int randcounter{ mathEngine::RandNumberBetween(1, 3) };
			for (int i = 0; i < randcounter; ++i)
			{
				AABB& aabb = mGameECSmanager.GetComponent<AABB>(entity);
				mathEngine::Vector3D particleStartPos =
					mathEngine::Vector3D(mathEngine::RandNumberBetween(aabb.min.x, aabb.max.x),
						mathEngine::RandNumberBetween(aabb.min.y, aabb.max.y), 0.f);
				gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
					particleStartPos, 2.f, ObjectDirection::CIRCLE,
					ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS,
					std::make_pair<float, float>(10.f, 30.f),
					std::make_pair<float, float>(0.f, 0.f),
					std::make_pair<float, float>(8.f, 10.f),
					std::make_pair<float, float>(0.1f, 0.2f),
					std::vector<ParticleAction>(),
					{ 0.5f, 0.5f, 0.f }, { 0.8f,0.f,0.8f, WorldIntensity }, 1.f, ShaderOption::FULL_TEXTURE,
					ObjectShapes::SQUARE, TextureType::GLOWINGBALL, entity, mathEngine::RandNumberBetween(10, 30), false, PhysicFlag{ P_NORMAL }));
			}
			laser.lifetimeCounter = false;
		}
		if (laser.laserState == LaserState::LASER_IDLE && laser.lifetime >= 0.5f)
		{
			laser.laserState = LaserState::LASER_CHARGE;
		}
		else if (laser.laserState == LaserState::LASER_CHARGE && laser.lifetime >= 2.0f)
		{
			//timer.CheckSingletonTimer(TimerMap::LASER_CHARGE, 1.5f)
			//Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
			if (!laser.charging)
			{
				mathEngine::Vector3D particleStartPos = position.pos;
				//particleStartPos.x -= transform.scale.x / 2;
				CollisionList& cList = gFakeEngine->GetGameManager<CollisionList>();
				laser.charging_id = EntityCreation::CreateEntity(
					EntitySystemOption::GAME_ENTITY,
					EntityType{ ObjectType::LASER_PROJECTILE },
					State{ ObjectState::IDLE, ObjectDirection::LEFT },
					RenderTex{ gFakeEngine->GetTexture(TextureType::MONSTER) },
					RenderPos{ {10.f, 10.f} },
					Animation{ gFakeEngine->GetObjectAnimations(TextureType::MONSTER) },
					Render{ gFakeEngine->GetShapes(ObjectShapes::SQUARE),ShaderOption::ANIMATION_TEXTURE },
					Transform{ {10.f, 10.f, 0.f}, {0.f, 0.f, 0.f} , 180.f },
					Position{ particleStartPos },
					Rigidbody{ {0.f,0.f,0.f }, {0.f,0.f,0.f } },
					AABB{ {0.f,0.f }, {0.f,0.f },{0.3f, 0.3f} },
					Colour{ {1.f,0.f,0.f } },
					Light{ {1.f, 0.f, 1.f, WorldIntensity} },
					Flags{ FLAG_ACTIVE | FLAG_VISIBLE },
					ParentId{ entity },
					Laser{},
					LifeTime{},
					Force{},
					LightFlag{ LIGHT_ALL }
				);

				cList.AddCollisionParticleList(laser.charging_id, EntitySystemOption::GAME_ENTITY, CollisionType::CHARGING_LASER_PROJECTILE);
				gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
					particleStartPos, 60.f, ObjectDirection::CIRCLE,
					ParticleTypesEnum::PARTICLE_PULSE_INWARDS,
					std::make_pair<float, float>(60.f, 80.f),
					std::make_pair<float, float>(0.f, 0.f),
					std::make_pair<float, float>(8.f, 10.f),
					std::make_pair<float, float>(1.0f, 1.3f),
					std::vector<ParticleAction>(),
					{ 1.f, 1.f, 0.f }, { 1.f,0.f,1.f,WorldIntensity }, 1.f, ShaderOption::FULL_TEXTURE,
					ObjectShapes::SQUARE, TextureType::GLOWINGBALL, entity, mathEngine::RandNumberBetween(30, 50), false, PhysicFlag{ P_NORMAL }));
				laser.charging = true;
				if (mGameECSmanager.GetComponent<CollisionFlag>(entity).audioActivate && !input.GetInputBooleen().win_state && !input.GetInputBooleen().win_screen)
					gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::LASER_4), FMOD_LOOP_OFF, 0.05f);
			}
		}
		else if (laser.laserState == LaserState::LASER_FIRE)
		{
			if (mGameECSmanager.GetComponent<CollisionFlag>(entity).audioActivate && !input.GetInputBooleen().win_state && !input.GetInputBooleen().win_screen)
				gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::AI_STATIC_LASER), FMOD_LOOP_OFF, 0.4f);

			Rigidbody& rigidbody = mGameECSmanager.GetComponent<Rigidbody>(laser.charging_id);
			State& state = mGameECSmanager.GetComponent<State>(laser.charging_id);
			EntityType entityType = mGameECSmanager.GetComponent<EntityType>(laser.charging_id);
			rigidbody.velocity.x = -400.f;
			state.objectState = ObjectState::PUSHING_BARREL;
			laser.laserState = LaserState::LASER_FIRING;
		}
		laser.lifetime += g_dt;

		Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
		transform.angle += mathEngine::RandNumberBetween(1.f, 100.f) * g_dt;
		if (transform.angle >= 360.f)
			transform.angle = 0;
	}
}