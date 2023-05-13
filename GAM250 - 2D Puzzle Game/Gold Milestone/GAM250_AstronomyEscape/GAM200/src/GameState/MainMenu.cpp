/*****************************************************************
*\file		 MainMenu.cpp
*\brief		 Contains definition for main menu game states

*\author(s)   Goh Wei Zhe	146 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include  <FakeEngine.h>
#include <GameTypes.h>
#include <SystemList.h>
#include "../GameState/GameState.h"
#include "../Engine/Serialization/Serialization.h"

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;

void MainMenu_Load()
{
	gFakeEngine->RegisterGameManager<GlobalVar>();
	gFakeEngine->RegisterGameManager<SerializationVar>();
	gFakeEngine->RegisterGameManager<GameCounter>();
	gFakeEngine->RegisterGameManager<Timer>();
	gFakeEngine->RegisterGameManager<Boolean>();
	gFakeEngine->RegisterGameManager<ParticlesInfo>();
	//gFakeEngine->RegisterGameManager<StageChanger>();
}

void MainMenu_Initialize()
{
	StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/main_menu_bg.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/play.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/control.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/option.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/credit.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/quit.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/levelselect_menu_bg.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/level1.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/level2.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/level3.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/level4.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/level5.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/level_back.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/option_menu_bg.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/option_1920x1080.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/fullscreen.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/bgm.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/option_back.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/control_menu_bg_1.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/control_menu_bg_2.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/control_menu_bg_3.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/control_next.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/control_back.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/credit_menu_bg_1.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/credit_menu_bg_2.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/credit_menu_bg_3.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/credit_menu_bg_4.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/credit_menu_bg_5.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/credit_menu_bg_6.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/credit_back.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/credit_next.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/quit_menu_bg.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/quit_yes.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/quit_no.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/LoadingLogo.txt"));
	stageChanger.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/mouse.txt"));
	//check stuff 
	//Serialization::testObjData(stageChanger.allobjects);
	//Serialization::creation_of_gameplay_stuff("Assets/Serialization/main_menu.txt", stageChanger.allobjects);

	stageChanger.LoadStage(StagesEnum::MAIN_MENU);


	//std::cout << " finish creation " << std::endl;

	gFakeEngine->PushShapeAssets(ObjectShapes::CIRCLE);
	gFakeEngine->PushShapeAssets(ObjectShapes::TRIANGLE);
	gFakeEngine->PushShapeAssets(ObjectShapes::SQUARELESS);

	gFakeEngine->PushTextureAssets(TextureType::GLOWINGBALL, "Assets/images/GlowingBall.png");
	ParticlesInfo& particlesList = gFakeEngine->GetGameManager<ParticlesInfo>();
	particlesList.Init(2000);

	//Pre-transform the objects
	auto& mGameSystem = gFakeEngine->GetECSGameManager<GameSystemsList>();
	auto& mParticleSystem = gFakeEngine->GetECSParticlesManager<ParticleSystemsList>();
	mGameSystem.transformSystem->Update(EntitySystemOption::GAME_ENTITY);
	mParticleSystem.transformSystem->Update(EntitySystemOption::PARTICLE_ENTITY);


	Serialization::load_sound_FromFile();
	gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::SUBMERGED_SILENCE_SHORT), FMOD_LOOP_OFF, 0.02f);
	//gAudioEngine->PlaySound("Audio/INNOCENCE_Holidays.wav", FMOD_LOOP_NORMAL, 0.1);
}

void MainMenu_Update()
{
	// write window title with current fps ...
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	Timer& timer = gFakeEngine->GetGameManager<Timer>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	window.PrintWindowTitle();
	timer.UpdateGlobalTime();
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	InputBoolean& inputBoolean = input.GetInputBooleen();
	auto& mGameSystem = gFakeEngine->GetECSGameManager<GameSystemsList>();
	auto& globalbool = gFakeEngine->GetGraphicManager<Graphics::Storage>();

	if (globalbool.globalLogo)
	{
		if (input.CheckPress(GLFW_KEY_SPACE))
			inputBoolean.logoskip = true;
		
		if (globalVar.logo_time >= 5.f || inputBoolean.logoskip)
		{
			FakeLoadingSystem::ToggleLOGO();
			globalVar.logo_time = 0;
			inputBoolean.logoskip = false;
		}
		if (g_dt != INFINITY)
			globalVar.logo_time += g_dt;
	}
	else
	{
		timer.UpdateTimer();
		mGameSystem.mouseSystem->Update();
		auto& mParticleSystem = gFakeEngine->GetECSParticlesManager<ParticleSystemsList>();
		mParticleSystem.particleSystem->GenerationUpdate();
		mParticleSystem.particleSystem->Update();

		mGameSystem.inputSystem->Update();
		mGameSystem.preCollisionSystem->Update(EntitySystemOption::GAME_ENTITY);
		mGameSystem.mainMenuInputSystem->Update();
		mGameSystem.cameraSystem->Update();
		mGameSystem.transformSystem->Update(EntitySystemOption::GAME_ENTITY);
		mParticleSystem.transformSystem->Update(EntitySystemOption::PARTICLE_ENTITY);

		mParticleSystem.l_p_PhysicSystem->Update(EntitySystemOption::PARTICLE_ENTITY);
	
	}
	gFakeEngine->ExecuteQueuedEvents();
}

void MainMenu_Draw()
{
	auto& mGameSystem = gFakeEngine->GetECSGameManager<GameSystemsList>();
	auto& mParticleSystem = gFakeEngine->GetECSParticlesManager<ParticleSystemsList>();
	Graphics::Gfx::RenderFunctions& gRender = gFakeEngine->GetGraphicManager<Graphics::Gfx::RenderFunctions>();
	auto& globalbool = gFakeEngine->GetGraphicManager<Graphics::Storage>();
	gRender.ToggleBlender(true);
	gRender.ClearBitBuffer(GL_COLOR_BUFFER_BIT);

	if (!globalbool.globalLogo)
	{
		mGameSystem.renderingSystem->Update(EntitySystemOption::GAME_ENTITY);
		mParticleSystem.renderingSystem->Update(EntitySystemOption::PARTICLE_ENTITY);
	}
	else
	{
		mGameSystem.fLoadingSystem->Update();
	}

}

void MainMenu_Free()
{
	//const std::string free_sound = gAudioEngine->TypeToString(SoundType::SUBMERGED_SILENCE_LONG);
	ImGui_ImplGlfwGL3_Shutdown();
	gFakeEngine->GetECSGameManager<ECSManager>().FreeEntityList();
	gFakeEngine->GetECSParticlesManager<ECSManager>().FreeEntityList();
	
	clear(gFakeEngine->GetGraphicManager<Camera2d>());
	clear(gFakeEngine->GetGraphicManager<Input>());
	clear(gFakeEngine->GetGameManager<GlobalVar>());
	clear(gFakeEngine->GetGameManager<Timer>());
	clear(gFakeEngine->GetGameManager<Boolean>());
	clear(gFakeEngine->GetGameManager<SerializationVar>());
	gFakeEngine->GetGraphicManager<StageChanger>().Reset();
	gAudioEngine->UnloadAll();
}

void MainMenu_Unload()
{
	gFakeEngine->RemoveGameManagerList();
}
