/*****************************************************************
*\file		 Level_1.cpp
*\brief		 Level one Logics

*\author(s)   Lee Liang Ping		150 lines x 100% Code contribution
*\author(s)   Yip Ling Kit Justin	1   lines x   0.5% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>

#include <FakeEngine.h>
#include <EntityCreation.h>
#include <GameTypes.h>
#include <SystemList.h>
#include "../GameState/GameState.h"
#include "../Engine/Serialization/Serialization.h"

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;

void Stage1_Load()
{
	gFakeEngine->RegisterGameManager<GlobalVar>();
	gFakeEngine->RegisterGameManager<ParticlesInfo>();
	gFakeEngine->RegisterGameManager<Timer>();
	gFakeEngine->RegisterGameManager<SerializationVar>();
	gFakeEngine->RegisterGameManager<CollisionList>();
	gFakeEngine->RegisterGameManager<GameCounter>();
	gFakeEngine->RegisterGameManager<ForceManager>();
	gFakeEngine->RegisterGameManager<Boolean>();
	gFakeEngine->RegisterGameManager<TextManager>();

	gFakeEngine->RegisterGameManager<ImGuiVar>();
	gFakeEngine->RegisterGameManager<LightEffects>();

	//gFakeEngine->GetGameManager<StageChanger>().InitStage(StagesEnum::ACTUAL_STAGE);
	gFakeEngine->PushShapeAssets(ObjectShapes::CIRCLE);
	gFakeEngine->PushShapeAssets(ObjectShapes::TRIANGLE);
	gFakeEngine->PushShapeAssets(ObjectShapes::SQUARELESS);
}

void Stage1_Initialize()
{
	StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
	Serialization::load_object_FromFile();
	Serialization::load_sound_FromFile();

	gFakeEngine->PushTextureAssets(TextureType::GLOWINGBALL, "Assets/images/GlowingBall.png");
	gFakeEngine->PushTextureAssets(TextureType::SMOKE_1, "Assets/images/Smoke/Smoke_1.png");
	gFakeEngine->PushTextureAssets(TextureType::SMOKE_2, "Assets/images/Smoke/Smoke_2.png");
	gFakeEngine->PushTextureAssets(TextureType::SMOKE_3, "Assets/images/Smoke/Smoke_3.png");
	gFakeEngine->PushTextureAssets(TextureType::SMOKE_4, "Assets/images/Smoke/Smoke_4.png");
	gFakeEngine->PushTextureAssets(TextureType::SMOKE_5, "Assets/images/Smoke/Smoke_5.png");

	Serialization::creation_of_gameplay_stuff("Assets/Serialization/pause_menu.txt", stageChanger.allobjects);

	//check stuff 
	stageChanger.LoadStage();

	ParticlesInfo& particlesList = gFakeEngine->GetGameManager<ParticlesInfo>();
	particlesList.Init(2000);


	//Pre-transform the objects
	auto& mGameSystem = gFakeEngine->GetECSGameManager<GameSystemsList>();
	auto& mParticleSystem = gFakeEngine->GetECSParticlesManager<ParticleSystemsList>();
	mGameSystem.transformSystem->Update(EntitySystemOption::GAME_ENTITY);
	mParticleSystem.transformSystem->Update(EntitySystemOption::PARTICLE_ENTITY);

	gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::LEVEL_1), FMOD_LOOP_NORMAL, gAudioEngine->volume);

	TextManager& textManager = gFakeEngine->GetGameManager<TextManager>();
	textManager.HelperFunctionAddText();

	LightEffects& lightEffects = gFakeEngine->GetGameManager<LightEffects>();
	lightEffects.Init();

	mGameSystem.leverSystem->init_map();
	mGameSystem.teleporterSystem->init_map();
	mGameSystem.tutorialSystem->InitEntity();
}

void Stage1_Update()
{
	//Address getter
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	InputBoolean& inputBoolean = input.GetInputBooleen();
	Timer& timer = gFakeEngine->GetGameManager<Timer>();
	StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
	SerializationVar& serializationVar = gFakeEngine->GetGameManager<SerializationVar>();

	auto& mGameSystem = gFakeEngine->GetECSGameManager<GameSystemsList>();
	auto& mParticleSystem = gFakeEngine->GetECSParticlesManager<ParticleSystemsList>();
	auto& mLightSystem = gFakeEngine->GetECSLightManager<LightSystemsList>();

	timer.UpdateGlobalTime();
	if (inputBoolean.change_state_pause && timer.globalTime >= 3.f)
	{
		gFakeEngine->PublishEvent(new SetLoadingScreen(false));
		inputBoolean.change_state_pause = false;
	}

	//Input Update System
	window.PrintWindowTitle();
	mGameSystem.inputSystem->Update();
	stageChanger.StageUpdate();

	mGameSystem.mouseSystem->Update();

	mGameSystem.pauseMenu->Update();
	if (inputBoolean.activate_imgui)
		mGameSystem.imGuiSystem->Update();

	if(inputBoolean.pause_screen)
		mGameSystem.victory_screen->Update();

	if (!inputBoolean.pause_screen &&
		!inputBoolean.active_pause_screen &&
		!inputBoolean.change_state_pause && 
		!serializationVar.duringTutorial)
	{
		FPS& fps = gFakeEngine->GetGraphicManager<FPS>();
		TextManager& textManager = gFakeEngine->GetGameManager<TextManager>();
		ForceManager& forceManager = gFakeEngine->GetGameManager<ForceManager>();
		//Timer Update
		timer.UpdateTimer();
		mGameSystem.victory_screen->Update();
		mGameSystem.defeat_screen->Update();

		mGameSystem.tutorialSystem->BoxUpdate();

		mGameSystem.uiSystem->Update();

		//AI Object Systmes
		mGameSystem.gameInputSystem->Update();
		mGameSystem.aiSystem->Update();
		mParticleSystem.particleSystem->Update();
		mGameSystem.stateSystem->Update();
		mParticleSystem.particleSystem->GenerationUpdate();


		mLightSystem.lightSystem->Update();
		mLightSystem.mainchar_lightSystem->Update();
		mLightSystem.keyObjectives_lightSystem->Update();
		mLightSystem.keyObjectivesParts_lightSystem->Update();

		//Score System 
		mGameSystem.scoreSystem->Update();

		//Pre Collision System
		mGameSystem.preCollisionSystem->Update(EntitySystemOption::GAME_ENTITY);
		mParticleSystem.preCollisionSystem->Update(EntitySystemOption::PARTICLE_ENTITY);
		mGameSystem.doorSystem->Update();
		mGameSystem.leverSystem->Update();
		mGameSystem.teleporterSystem->Update();
		mGameSystem.laserSystem->Update();
		mGameSystem.tutorialSystem->Update();

		//Transform Systems
		mGameSystem.transformSystem->Update(EntitySystemOption::GAME_ENTITY);
		mParticleSystem.transformSystem->Update(EntitySystemOption::PARTICLE_ENTITY);

		for (int step = 0; step < fps.GetCurrentNumberOfSteps(); ++step)
		{
			//Physic Update System
			mGameSystem.cameraSystem->Update();
			mGameSystem.physicSystem->Update(EntitySystemOption::GAME_ENTITY);
			mParticleSystem.l_p_PhysicSystem->Update(EntitySystemOption::PARTICLE_ENTITY);
			//mLightSystem.l_p_PhysicSystem->Update(EntitySystemOption::LIGHT_ENTITY);

			forceManager.UpdateForce();

			//Collision Check Systems
			mParticleSystem.particleCollisionSystem->Update();
			mGameSystem.collisionSystem->Update();
			mGameSystem.laserProjectileSystem->Update();
			mGameSystem.hotSpotCollisionSystem->Update();
		}

		// Post Update System
		mGameSystem.inventorySystem->Update();
		mGameSystem.keyObjectiveSystem->Update();
		mGameSystem.textRenderSystem->Update();
		textManager.UpdateTextList();

	}
	else if (!inputBoolean.pause_screen)
	{

		mGameSystem.tutorialSystem->BoxUpdate();
		mGameSystem.transformSystem->Update(EntitySystemOption::GAME_ENTITY);

	}

	gFakeEngine->ExecuteQueuedEvents();
}

void Stage1_Draw()
{
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	InputBoolean& inputBoolean = input.GetInputBooleen();
	auto& mGameSystem = gFakeEngine->GetECSGameManager<GameSystemsList>();
	Graphics::Gfx::RenderFunctions& gRender = gFakeEngine->GetGraphicManager<Graphics::Gfx::RenderFunctions>();
	gRender.ToggleBlender(true);
	gRender.ClearBitBuffer(GL_COLOR_BUFFER_BIT);
	if (!inputBoolean.change_state_pause && !inputBoolean.win_screen)
	{
		//gFakeEngine->GetGameManager<ImGuiVar>().updte();
		mGameSystem.animationSystem->Update();
		mGameSystem.renderingSystem->Update(EntitySystemOption::GAME_ENTITY);

		if (inputBoolean.win_state)
			mGameSystem.scoreSystem->Draw();

		if (!inputBoolean.pause_screen)
		{
			auto& mParticleSystem = gFakeEngine->GetECSParticlesManager<ParticleSystemsList>();
			mParticleSystem.renderingSystem->Update(EntitySystemOption::PARTICLE_ENTITY);

			TextManager& textManager = gFakeEngine->GetGameManager<TextManager>();
			textManager.RenderTextList();
		}

		if (inputBoolean.activate_imgui)
			ImGui::Render();
	}
	else
	{
		mGameSystem.fLoadingSystem->Update();
	}
}

void Stage1_Free()
{
 
	//
	ImGui_ImplGlfwGL3_Shutdown();
	//const std::string free_sound = gAudioEngine->TypeToString(SoundType::FOLLOW_THE_STAR);
	gFakeEngine->GetECSGameManager<ECSManager>().FreeEntityList();
	gFakeEngine->GetECSParticlesManager<ECSManager>().FreeEntityList();
	gFakeEngine->GetECSLightManager<ECSManager>().FreeEntityList();
	//conditional
	gFakeEngine->GetGraphicManager<StageChanger>().Reset();

	//clear all
	clear(gFakeEngine->GetGameManager<GlobalVar>());
	clear(gFakeEngine->GetGameManager<Timer>());
	clear(gFakeEngine->GetGameManager<CollisionList>());
	clear(gFakeEngine->GetGameManager<ForceManager>());
	clear(gFakeEngine->GetGameManager<Boolean>());
	clear(gFakeEngine->GetGameManager<ImGuiVar>());
	clear(gFakeEngine->GetGameManager<SerializationVar>());
	clear(gFakeEngine->GetGraphicManager<Camera2d>());
	clear(gFakeEngine->GetGraphicManager<Input>());
	gAudioEngine->UnloadAll();
}

void Stage1_Unload()
{
	gFakeEngine->RemoveGameManagerList();
}