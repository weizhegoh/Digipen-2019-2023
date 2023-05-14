/*****************************************************************
*\file		 AnimationSystem.cpp
*\brief		 AnimationSystem functions definitions.

*\author(s)   Lee Jun Jie	84 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include "AnimationSystem.h"
#include <FakeEngine.h>
#include <SystemList.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;

void AnimationSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	signature.set(mGameECSmanager.GetComponentType<Animation>());
	mGameECSmanager.SetSystemBitSignature<AnimationSystem>(signature);
}

void AnimationSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();

	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE) || 0 == (flag.activeFlag & FLAG_VISIBLE))
			continue;
		FPS& fps = gFakeEngine->GetGraphicManager<FPS>();
		for (int step = 0; step < fps.GetCurrentNumberOfSteps(); ++step)
		{
			EntityType& type = mGameECSmanager.GetComponent<EntityType>(entity);
			if (type.objType == ObjectType::MAIN_CHARACTER || type.objType == ObjectType::MONSTER 
				|| type.objType == ObjectType::MONSTER_PATROL || type.objType == ObjectType::MONSTER_GUIDE
				|| type.objType == ObjectType::LASER_PROJECTILE || type.objType == ObjectType::LEVER_DOOR)
			{
				Animation& animation = mGameECSmanager.GetComponent<Animation>(entity);
				State& state = mGameECSmanager.GetComponent<State>(entity);
				if (state.objectState != animation.currentObjectState)
				{
					animation.currentFrameIndex = 0;
					animation.currentFrameTimer = 0;
					animation.currentObjectState = state.objectState;
				}
				if ((animation.currentFrameTimer += g_dt) >= animation.animationsList->animationList[state.objectState][static_cast<int>(animation.currentFrameIndex)].frameTime)
				{
					int prevFrameIndex = static_cast<int>(animation.currentFrameIndex);
					animation.currentFrameIndex = (animation.currentFrameIndex + 1l) % animation.animationsList->animationList[state.objectState].size();
					animation.currentFrameTimer = 0.0f;

					if (state.objectState == ObjectState::DEATH && animation.currentFrameIndex == 0 && prevFrameIndex != 0)
					{
						//main character Death;
						Boolean& boolean = gFakeEngine->GetGameManager<Boolean>();
						Position& position = mGameECSmanager.GetComponent<Position>(entity);
						State& s_state = mGameECSmanager.GetComponent<State>(entity);
						GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();

						s_state.objectState = ObjectState::IDLE;
						
						gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
							position.pos, 35.f, ObjectDirection::CIRCLE,
							ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS,
							std::make_pair<float, float>(100.f, 300.f),
							std::make_pair<float, float>(15.f, 60.f),
							std::make_pair<float, float>(7.f, 9.f),
							std::make_pair<float, float>(0.5f, 2.5f),
							std::vector<ParticleAction>(),
							{ 1.f, 1.f, 0.f }, { 1.f,1.f,1.f,0.9f }, 1.f ,ShaderOption::FULL_TEXTURE,
							ObjectShapes::SQUARE, TextureType::GLOWINGBALL,
							entity, 20, false, PhysicFlag{ P_NORMAL }));
						
						boolean.False(BooleanType::MOVEMENT_LOCK);
						globalVar.entrance_door_opened = true;
						if(globalVar.door.size())
							globalVar.door.pop_back();
						//position.pos = position.starting_pos;
						Input& input = gFakeEngine->GetGraphicManager<Input>();
						InputBoolean& inputBoolean = input.GetInputBooleen();
						gFakeEngine->GetECSGameManager<GameSystemsList>().hotSpotCollisionSystem->ResetPosition();
						gFakeEngine->GetECSLightManager<LightSystemsList>().mainchar_lightSystem->ResetAmbient();
						if (--mGameECSmanager.GetComponent<Inventory>(entity)._lifes < 1)
						{
							inputBoolean.defeat_state = true;
							gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::GAME_OVER), FMOD_LOOP_OFF, 0.9f);
						}

					}
					else if ((state.objectState == ObjectState::DOOR_OPEN || state.objectState == ObjectState::DOOR_CLOSE) && animation.currentFrameIndex == 0 && prevFrameIndex != 0)
					{
						//std::cout << EnumToString::Get(state.objectState) << std::endl;
						animation.endAni = true;
					}
								
				}
			}
		}
	}
}
