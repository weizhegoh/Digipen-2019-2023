/*****************************************************************
*\file		 GameInputSystem.cpp
*\brief		 GameInputSystem functions definitions.

*\author(s)   Lee Liang Ping	229 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <GameInputSystem.h>
#include <FakeEngine.h>
#include <AudioEngine.h>
#include <SystemList.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;
extern float g_dt;
extern unsigned _gameStateCurrent;

void GameInputSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	mGameECSmanager.SetSystemBitSignature<GameInputSystem>(signature);
	gFakeEngine->SubscribeEvent(this, &GameInputSystem::SetBoulderStatus, EventID::SetBoulderStatusEvent);
}

void GameInputSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Timer& timer = gFakeEngine->GetGameManager<Timer>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	Boolean& boolean = gFakeEngine->GetGameManager<Boolean>();
	ForceManager& forceManager = gFakeEngine->GetGameManager<ForceManager>();
	Camera2d& camera2d = gFakeEngine->GetGraphicManager<Camera2d>();
	if (input.CheckPress(GLFW_KEY_Z))
		camera2d.zoom_input = true;

	if (input.GetInputBooleen().activate_hack)
	{
		if (input.CheckTriggered(GLFW_KEY_LEFT_ALT))
		{
			if (input.CheckPress(GLFW_KEY_G))
				input.ToggleBoolean(input.GetInputBooleen().god_mode);

			if (input.CheckPress(GLFW_KEY_S))
				input.ToggleBoolean(input.GetInputBooleen().speed_hack);

			if (input.CheckPress(GLFW_KEY_W))
			{
				input.GetInputBooleen().win_state = true;
				boolean.True(BooleanType::MOVEMENT_LOCK);
			}
		}
	}
	else
	{
		input.GetInputBooleen().god_mode = false;
		input.GetInputBooleen().speed_hack = false;
	}

	//if (input.CheckTriggered(GLFW_KEY_LEFT_ALT))
	//{
	//	if (input.CheckPress(GLFW_KEY_I))
	//		input.ToggleBoolean(input.GetInputBooleen().activate_imgui);
	//}

	if (input.CheckPress(GLFW_KEY_R))
	{
		gFakeEngine->GetECSGameManager<GameSystemsList>().hotSpotCollisionSystem->ResetPosition();
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Push Boulder Activation
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	Inventory& inventory = mGameECSmanager.GetComponent<Inventory>(globalVar.mainCharID);
	if (input.CheckPress(GLFW_KEY_SPACE))
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::PUSH), FMOD_LOOP_OFF, 0.5f);

	if (input.CheckTriggered(GLFW_KEY_SPACE))
	{
		input.GetInputBooleen().space_key = true;
	}
	else
	{
		input.GetInputBooleen().space_key = false;

		if (inventory._currBoulder != NULL)
		{
			timer.SetTimerLoop(TimerMap::MOVEMENT_LOCK, 1);
			Rigidbody& rigidbody = mGameECSmanager.GetComponent<Rigidbody>(globalVar.mainCharID);
			forceManager.removeforce(globalVar.mainCharID);
			rigidbody.velocity = { 0.f, 0.f, 0.f };
			std::cout << "Set boulder" << std::endl;
			gFakeEngine->ExecuteEvent(new SetBoulderStatusEvent(inventory._currBoulder));
		}
		inventory._currBoulder = NULL;
	}


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// MOVEMENT
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (timer.CheckTimerLoop(TimerMap::MOVEMENT_LOCK) && boolean.Check(BooleanType::MOVEMENT_LOCK))
	{
		Rigidbody& rigidbody = mGameECSmanager.GetComponent<Rigidbody>(globalVar.mainCharID);
		State& state = mGameECSmanager.GetComponent<State>(globalVar.mainCharID);

		if (rigidbody.velocity.x == 0.f)
		{
			if (input.CheckTriggered(GLFW_KEY_UP))
			{
				MovementUpdate(ObjectDirection::UP);
			}
			else if (input.CheckTriggered(GLFW_KEY_DOWN))
			{
				MovementUpdate(ObjectDirection::DOWN);
			}
			else
			{
				if (timer.CheckSingletonTimer(TimerMap::PUSHING_BARREL_INPUT, 0.5f))
				{
					if (state.objectState != ObjectState::IDLE)
					{
						forceManager.removeforce(globalVar.mainCharID);
						state.objectState = ObjectState::IDLE;
					}
				}
			}
		}

		if (rigidbody.velocity.y == 0.f)
		{
			if (input.CheckTriggered(GLFW_KEY_LEFT))
			{
				MovementUpdate(ObjectDirection::LEFT);
			}
			else if (input.CheckTriggered(GLFW_KEY_RIGHT))
			{
				MovementUpdate(ObjectDirection::RIGHT);
			}
			else
			{
				if (timer.CheckSingletonTimer(TimerMap::PUSHING_BARREL_INPUT, 0.5f))
				{
					if (state.objectState != ObjectState::IDLE)
					{
						forceManager.removeforce(globalVar.mainCharID);
						state.objectState = ObjectState::IDLE;
					}
				}
			}
		}
	}


	if (input.GetInputBooleen().win_state)
	{
		mathEngine::Vector3D position{ mathEngine::RandNumberBetween((float)-1900, (float)1900), mathEngine::RandNumberBetween((float)-1000, (float)1000), 0 };
		mathEngine::Vector3D colour{ mathEngine::RandNumberBetween(0.f, 1.f), mathEngine::RandNumberBetween(0.f, 1.f), mathEngine::RandNumberBetween(0.f, 1.f) };
		gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
			position, 10.f, ObjectDirection::CIRCLE,
			ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS,
			std::make_pair<float, float>(60.f, 80.f),
			std::make_pair<float, float>(0.f, 0.f),
			std::make_pair<float, float>(8.f, 10.f),
			std::make_pair<float, float>(1.5f, 1.6f),
			std::vector<ParticleAction>(),
			{ 1.f, 1.f, 0.f }, { colour.x, colour.y, colour.z, 0.9f }, 1.f, ShaderOption::FULL_TEXTURE,
			ObjectShapes::SQUARE, TextureType::GLOWINGBALL, NULL, mathEngine::RandNumberBetween(10, 30), false, PhysicFlag{ P_NORMAL | P_GRAVITY }));

		if (!input.GetInputBooleen().win_audio)
		{
			gAudioEngine->UnloadAll();
			//gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::REACH_FOR_THE_STARS_SHORT), FMOD_LOOP_NORMAL, 0.5f);
			gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::WIN), FMOD_LOOP_NORMAL, 0.5f);
			gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::FIREWORK), FMOD_LOOP_NORMAL, 0.5f);
			input.GetInputBooleen().win_audio = true;
		}

		//if (globalVar.win_time >= 10.f)
		//	gFakeEngine->PublishEvent(new SetWinScreen(true, 1));
		//globalVar.win_time += g_dt;
	}
}

void GameInputSystem::SetBoulderStatus(SetBoulderStatusEvent* rEvent)
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	GlobalVar& globalvar = gFakeEngine->GetGameManager<GlobalVar>();
	ForceManager& forceManager = gFakeEngine->GetGameManager<ForceManager>();
	Position boulder_position = mGameECSmanager.GetComponent<Position>(rEvent->entityA);
	State& boulder_state = mGameECSmanager.GetComponent<State>(rEvent->entityA);
	Rigidbody& boulder_rigidbody = mGameECSmanager.GetComponent<Rigidbody>(rEvent->entityA);
	boulder_rigidbody.velocity = { 0.f, 0.f, 0.f };
	globalvar.ConvertScreenToBinary(boulder_position.pos);
	globalvar.SetCellValue(boulder_position.pos, ObjectType::PUSHABLE_BARREL);
	forceManager.removeforce(rEvent->entityA);
	boulder_state.objectState = ObjectState::IDLE;
	boulder_state.objectDir = ObjectDirection::NO_DIR;
}

void GameInputSystem::InputVelocity(Entity entity, ObjectDirection dir)
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	State& state = mGameECSmanager.GetComponent<State>(entity);
	Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	ForceManager& forceManager = gFakeEngine->GetGameManager<ForceManager>();
	ObjectState stateholder;

	if (input.GetInputBooleen().space_key)
		stateholder = ObjectState::PUSHING_BARREL;
	else
		stateholder = ObjectState::MOVING;
	float speed = 2000.f;
	if (input.GetInputBooleen().speed_hack)
		speed = 4000.f;

	switch (dir)
	{
	case ObjectDirection::UP:
		forceManager.addforce(entity, mathEngine::Vector3D(0.f, speed, 0.f), 5.f);
		state.objectDir = ObjectDirection::UP;
		transform.angle = 90.f;
		state.objectState = stateholder;
		//forceManager.removeforce(entity);
		break;
	case ObjectDirection::DOWN:
		forceManager.addforce(entity, mathEngine::Vector3D(0.f, -speed, 0.f), 5.f);
		state.objectDir = ObjectDirection::DOWN;
		transform.angle = 270.f;
		state.objectState = stateholder;
		break;
	case ObjectDirection::LEFT:
		forceManager.addforce(entity, mathEngine::Vector3D(-speed, 0.f, 0.f), 5.f);
		state.objectDir = ObjectDirection::LEFT;
		transform.angle = 180.f;
		state.objectState = stateholder;
		break;
	case ObjectDirection::RIGHT:
		forceManager.addforce(entity, mathEngine::Vector3D(speed, 0.f, 0.f), 5.f);
		state.objectDir = ObjectDirection::RIGHT;
		transform.angle = 0.f;
		state.objectState = stateholder;
		break;
	default:
		state.objectDir = ObjectDirection::NO_DIR;
		state.objectState = ObjectState::IDLE;
		break;
	}
	Inventory& inventory = mGameECSmanager.GetComponent<Inventory>(gFakeEngine->GetGameManager<GlobalVar>().mainCharID);
	inventory.inputDir = dir;
}

void GameInputSystem::MovementUpdate(ObjectDirection dir)
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	Inventory& inventory = mGameECSmanager.GetComponent<Inventory>(globalVar.mainCharID);

	if (input.GetInputBooleen().space_key && inventory._currBoulder != NULL)
	{
		State boulder_state = mGameECSmanager.GetComponent<State>(inventory._currBoulder);
		if (boulder_state.objectDir == dir)
			InputVelocity(globalVar.mainCharID, dir);
	}
	else
		InputVelocity(globalVar.mainCharID, dir);
}