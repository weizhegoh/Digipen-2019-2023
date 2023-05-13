/*****************************************************************
*\file		 StateSystem.cpp
*\brief		 StateSystem functions definitions.

*\author(s)   Lee Liang Ping	89 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <StateSystem.h>
#include <FakeEngine.h>


extern std::unique_ptr<FakeEngine> gFakeEngine;

void StateSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	signature.set(mGameECSmanager.GetComponentType<State>());
	mGameECSmanager.SetSystemBitSignature<StateSystem>(signature);
}

void StateSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Timer& timer = gFakeEngine->GetGameManager<Timer>();
	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;
		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		if (objecttype.objType == ObjectType::MAIN_CHARACTER)
		{
			Position& position = mGameECSmanager.GetComponent<Position>(entity);
			State& state = mGameECSmanager.GetComponent<State>(entity);
			Light& light = mGameECSmanager.GetComponent<Light>(entity);
			if (state.objectState == ObjectState::IDLE && timer.CheckSingletonTimer(TimerMap::PARTICLE_PULSING_INTERVAL, 5.f))
				gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
					position.pos, 35.f,ObjectDirection::CIRCLE, 
					ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS,
					std::make_pair<float, float>(30.f, 60.f),
					std::make_pair<float, float>(0.f, 0.f),
					std::make_pair<float, float>(7.f, 9.f),
					std::make_pair<float, float>(0.3f, 0.5f),
					std::vector<ParticleAction>(),
					{1.f, 1.f, 0.f}, { light.highlight }, 1.f, ShaderOption::FULL_TEXTURE,
					ObjectShapes::SQUARE, TextureType::GLOWINGBALL, 
					entity, 20, false, PhysicFlag{ P_NORMAL }));

			else if (state.objectState == ObjectState::MOVING && timer.CheckSingletonTimer(TimerMap::PARTICLE_WALKING_INTERVAL, .2f))
				gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
					position.pos, 35.f, state.InverseDir(),
					ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS,
					std::make_pair<float, float>(30.f, 60.f),
					std::make_pair<float, float>(0.f, 0.f),
					std::make_pair<float, float>(7.f, 9.f),
					std::make_pair<float, float>(0.3f, 0.5f),
					std::vector<ParticleAction>(),
					{ 1.f, 1.f, 0.f }, { light.highlight }, 1.f, ShaderOption::FULL_TEXTURE,
					ObjectShapes::SQUARE, TextureType::GLOWINGBALL, 
					entity, mathEngine::RandNumberBetween(2,5), false, PhysicFlag{ P_NORMAL }));
			else if (state.objectState == ObjectState::PUSHING_BARREL && timer.CheckSingletonTimer(TimerMap::PARTICLE_WALKING_INTERVAL, .5f))
				gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
					position.pos, 35.f, state.InverseDir(),
					ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS,
					std::make_pair<float, float>(40.f, 80.f),
					std::make_pair<float, float>(0.f, 0.f),
					std::make_pair<float, float>(7.f, 9.f),
					std::make_pair<float, float>(0.5f, 0.6f),
					std::vector<ParticleAction>(),
					{ 1.f, 1.f, 0.f }, { light.highlight }, 1.f, ShaderOption::FULL_TEXTURE,
					ObjectShapes::SQUARE, TextureType::GLOWINGBALL, 
					entity, mathEngine::RandNumberBetween(10, 15), false, PhysicFlag{ P_NORMAL }));
		}
		else if (objecttype.objType == ObjectType::MONSTER || objecttype.objType == ObjectType::MONSTER_PATROL || objecttype.objType == ObjectType::MONSTER_GUIDE)
		{
			Position& position = mGameECSmanager.GetComponent<Position>(entity);
			State& state = mGameECSmanager.GetComponent<State>(entity);
			AI& ai = mGameECSmanager.GetComponent<AI>(entity);
			Light& light = mGameECSmanager.GetComponent<Light>(entity);
			if (state.objectState == ObjectState::MONSTER_MOVING && ai.lifetime > .2f)
			{
				gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
					position.pos, 35.f, state.InverseDir(),
					ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS,
					std::make_pair<float, float>(30.f, 60.f),
					std::make_pair<float, float>(0.f, 0.f),
					std::make_pair<float, float>(7.f, 9.f),
					std::make_pair<float, float>(0.3f, 0.5f),
					std::vector<ParticleAction>(),
					{ 1.f, 1.f, 0.f }, { light.highlight }, 1.f, ShaderOption::FULL_TEXTURE,
					ObjectShapes::SQUARE, TextureType::GLOWINGBALL,
					entity, mathEngine::RandNumberBetween(2, 5), false, PhysicFlag{ P_NORMAL }));
				ai.lifetime = 0.f;
			}
			ai.lifetime += g_dt;

		}
	}
}