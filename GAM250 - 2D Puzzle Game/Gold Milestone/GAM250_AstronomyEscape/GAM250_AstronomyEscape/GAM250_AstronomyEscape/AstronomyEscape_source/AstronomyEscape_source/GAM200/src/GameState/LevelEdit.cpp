/*****************************************************************
*\file		 LevelEdit.cpp
*\brief		 LevelEdit functions definitions.

*\author(s)   Kenric Tan Wei Liang	 83 lines x 100% Code contribution

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

void LevelEdit_Load()
{
	gFakeEngine->RegisterGameManager<GlobalVar>();
	gFakeEngine->RegisterGameManager<SerializationVar>();
	gFakeEngine->RegisterGameManager<GameCounter>();
	gFakeEngine->RegisterGameManager<LevelEditVar>();
	std::cout << " finish creation " << std::endl;
}

void LevelEdit_Initialize()
{

	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/square.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/wall.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/main_character.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/pushable_barrel.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/monster.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/key_objective.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/turret.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/reddot.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/entrance_door.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/exit_door.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/level_door.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/monster_patrol.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/monster_guide.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/collectibles.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/leverDoor.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/lever.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/teleporter.txt"));
	leveleditVar.allobjects.push_back(Serialization::load_objects("Assets/Serialization/Objects/tutorial_floor.txt"));


	Serialization::load_sound_FromFile();


	//check stuff 
	Serialization::testObjData(leveleditVar.allobjects);

	gFakeEngine->PushShapeAssets(ObjectShapes::CIRCLE);
	gFakeEngine->PushShapeAssets(ObjectShapes::TRIANGLE);
	gFakeEngine->PushShapeAssets(ObjectShapes::SQUARELESS);
	gFakeEngine->PushShapeAssets(ObjectShapes::SQUARE);

	//Serialization::creation_of_gameplay_stuff("Assets/Serialization/level_edit.txt", leveleditVar.allobjects);

	LevelEditHelper::LoadData();
	gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::SUBMERGED_SILENCE_LONG), FMOD_LOOP_NORMAL, gAudioEngine->volume);
	
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	window.hideMouseCursor(false);
}

void LevelEdit_Update()
{
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	window.PrintWindowTitle();

	auto& mGameSystem = gFakeEngine->GetECSGameManager<GameSystemsList>();
	mGameSystem.imGuiSystem->LevelEditUpdate();
	mGameSystem.levelEditSystem->Update();
	//mGameSystem.preCollisionSystem->Update(EntitySystemOption::GAME_ENTITY);


	mGameSystem.cameraSystem->Update();
	mGameSystem.transformSystem->Update(EntitySystemOption::GAME_ENTITY);
	gFakeEngine->ExecuteQueuedEvents();
}

void LevelEdit_Draw()
{
	Graphics::Gfx::RenderFunctions& gRender = gFakeEngine->GetGraphicManager<Graphics::Gfx::RenderFunctions>();
	gRender.ToggleBlender(true);
	gRender.ClearBitBuffer(GL_COLOR_BUFFER_BIT);
	auto& mGameSystem = gFakeEngine->GetECSGameManager<GameSystemsList>();
	mGameSystem.renderingSystem->Update(EntitySystemOption::GAME_ENTITY);

	ImGui::Render();
}

void LevelEdit_Free()
{
	//LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();

	ImGui_ImplGlfwGL3_Shutdown();
	//const std::string free_sound = gAudioEngine->TypeToString(SoundType::SUBMERGED_SILENCE_LONG);

	LevelEditHelper::ResetValues();
	gFakeEngine->GetECSGameManager<ECSManager>().FreeEntityList();
	gFakeEngine->GetGraphicManager<Camera2d>().Reset();
	gFakeEngine->GetGraphicManager<Input>().Reset();
	//gAudioEngine->UnloadSound(free_sound);
	gAudioEngine->UnloadAll();
}

void LevelEdit_Unload()
{
	gFakeEngine->RemoveGameManagerList(); 

	Window& window = gFakeEngine->GetGraphicManager<Window>();
	window.hideMouseCursor(true);
}
