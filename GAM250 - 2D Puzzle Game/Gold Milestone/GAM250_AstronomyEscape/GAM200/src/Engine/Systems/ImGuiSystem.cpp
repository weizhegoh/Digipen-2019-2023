/*****************************************************************
*\file		 ImGuiSystem.cpp
*\brief		 ImGuiSystem functions definitions.

*\author(s)   Lee Jun Jie	109 lines x 37% Code contribution
*\author(s)   Kenric Tan Wei Liang	185 lines x 63% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <FakeEngine.h>
#include <ImGuiSystem.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;

void ImGuiSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	mGameECSmanager.SetSystemBitSignature<ImGuiSystem>(signature);
}

void ImGuiSystem::Update()
{
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	GlobalVar& globalvar = gFakeEngine->GetGameManager<GlobalVar>();
	GameCounter& gameCounter = gFakeEngine->GetGameManager<GameCounter>();
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	FPS& fps = gFakeEngine->GetGraphicManager<FPS>();


	ImGui_ImplGlfwGL3_NewFrame();

	ImGui::Begin("Modifiers");
	if (ImGui::CollapsingHeader("Window Status"))
	{
		ImGui::Text("Window Width : %d\nWindow Height : %d", window.GetWidth(), window.GetHeight());
		if (ImGui::Button("Set Screen 800 x 600"))
			window.SetWindowResolution(800, 600);
		ImGui::SameLine();
		if (ImGui::Button("Set Screen 1920 x 1080"))
			window.SetWindowResolution(1920, 1080);

		if (ImGui::Button("Set FullScreen"))
			window.SetWindowFullScreen(true);
		ImGui::SameLine();
		if (ImGui::Button("Off FullScreen"))
			window.SetWindowFullScreen(false);
	}
	if (ImGui::CollapsingHeader("FPS"))
	{
		ImGui::Text("Real Time FPS : %.2f", fps.fps);
		ImGui::Text("User Defined FPS : %.2f", fps.userDefinedFPS);
		ImGui::Text("Current Number of Steps : %d", fps.GetCurrentNumberOfSteps());
		ImGui::Text("Accumulate Time : %.2f", fps.GetAccumulatedTime());
		ImGui::Text("Delta Time : %.5f", g_dt);
		if (ImGui::Button(" 120 "))
			fps.SetUserDefinedFPS(120);
		ImGui::SameLine();
		if (ImGui::Button(" 60 "))
			fps.SetUserDefinedFPS(60);
		ImGui::SameLine();
		if (ImGui::Button(" 40 "))
			fps.SetUserDefinedFPS(40);
		ImGui::SameLine();
		if (ImGui::Button(" 20 "))
			fps.SetUserDefinedFPS(20);
		ImGui::SameLine();
		if (ImGui::Button(" 10 "))
			fps.SetUserDefinedFPS(10);
		ImGui::SameLine();
		if (ImGui::Button(" 1 "))
			fps.SetUserDefinedFPS(1);
		ImGui::SameLine();
		if (ImGui::Button(" Reset FPS "))
			fps.ResetUserDefinedFPS();
	}
	if (ImGui::CollapsingHeader("Graphic Debug Settings"))
	{
		ImGui::Checkbox("Draw AABB Box", &input.GetInputBooleen().line_key);
		ImGui::Checkbox("Draw Hot Spot", &input.GetInputBooleen().hotspot_key);

		ImGuiVar& imGuiVar = gFakeEngine->GetGameManager<ImGuiVar>();
		ImGui::SliderFloat("Ambient: ", &imGuiVar.lightambient, 0, 100);
		ImGui::SliderFloat("Colour R: ", &imGuiVar.lightColour.r, 0, 1);
		ImGui::SliderFloat(" B: ", &imGuiVar.lightColour.b, 0, 1);
		ImGui::SliderFloat(" G: ", &imGuiVar.lightColour.g, 0, 1);

		ImGui::SliderFloat(" c: ", &imGuiVar.lightconstant, -100, 100);
		ImGui::SliderFloat(" x: ", &imGuiVar.lightlinear, -1, 1);
		ImGui::SliderFloat(" x^2: ", &imGuiVar.lightquadratic, -10, 10);
		ImGui::SliderFloat(" X: ", &imGuiVar.lightpos.x, -3000, 3000);
		ImGui::SliderFloat(" Y: ", &imGuiVar.lightpos.y, -3000, 3000);
		ImGui::SliderFloat(" Z: ", &imGuiVar.lightpos.z, -100, 100);

	}
	if (ImGui::CollapsingHeader("Game Info"))
	{
		auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
		if (ImGui::CollapsingHeader("Entity Info"))
		{
			State& state = mGameECSmanager.GetComponent<State>(globalvar.mainCharID);
			Position& position = mGameECSmanager.GetComponent<Position>(globalvar.mainCharID);
			ImGui::Text("***********************\n Charater Status \n***********************\n");
			ImGui::Text("State : %s\n", EnumToString::Get(state.objectState).c_str());
			ImGui::Text("Dir : %s\n", EnumToString::Get(state.objectDir).c_str());
			ImGui::Text("Position : X: %.2f Y: %.2f Z: %.2f\n", position.pos.x, position.pos.y, position.pos.z);
		}
		if (ImGui::CollapsingHeader("Mouse Position"))
		{
			mathEngine::Vector3D mousPos = input.GetMousePos();
			ImGui::Text("Mouse Position : X: %.2f Y: %.2f Z: %.2f\n", mousPos.x, mousPos.y, mousPos.z);
			mousPos = input.GetMousePosWorld();
			ImGui::Text("Mouse Position in World Space: X: %.2f Y: %.2f Z: %.2f\n", mousPos.x, mousPos.y, mousPos.z);
			ImGui::Checkbox("Activate Mouse", &input.GetInputBooleen().activateMouseDraw);
		}
		if (ImGui::CollapsingHeader("Entity Count"))
			ImGui::Text("Entity Total Amount : %d\nEntity Active Amount : %d", gameCounter.totalEntityCount, gameCounter.totalActiveCount);
		if (ImGui::CollapsingHeader("Helper"))
		{
			Inventory& inventory = mGameECSmanager.GetComponent<Inventory>(globalvar.mainCharID);

			ImGui::Text("Current inventory : %d\n", inventory._currObjective);
			ImGui::Text("Total inventory : %d\n", globalvar.max_objective);
			if (ImGui::Button("Add one keycard"))
			{
				inventory._currObjective++;
			}

			ImGui::Checkbox("Activate Hack", &input.GetInputBooleen().activate_hack);
			if (input.GetInputBooleen().activate_hack)
			{
				ImGui::Checkbox("Activate God Mode", &input.GetInputBooleen().god_mode);
				ImGui::Checkbox("Activate Speed Hack", &input.GetInputBooleen().speed_hack);
			}

			ImGui::Text("\nDebug Testing");
			ImGui::Checkbox("Rotate Object", &input.GetInputBooleen().rotate_button);
			ImGui::SameLine();
			ImGui::Checkbox("Scale Object", &input.GetInputBooleen().scale_button);
			if (ImGui::Button("Print Map Data"))
				globalvar.printMapData();
		}
	}
	ImGui::End();
}

void ImGuiSystem::LevelEditUpdate()
{
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();
	SerializationVar& serializationVar = gFakeEngine->GetGameManager<SerializationVar>();
	GameCounter& gameCounter = gFakeEngine->GetGameManager<GameCounter>();
	static const ImVec4 pressColor{ 0.784f, 0.784f, 0.784f, 0.784f };
	static const ImVec4 DefaultColor{ 1.0f, 0.0f, 0.0f, 0.15f };
	leveleditVar.prev_createMode = leveleditVar.curr_createMode;
	ImGui_ImplGlfwGL3_NewFrame();

	ImGui::Begin("Modifiers");
	if (ImGui::CollapsingHeader("Window Status"))
	{
		ImGui::Text("Window Width : %d\nWindow Height : %d", window.GetWidth(), window.GetHeight());
		ImGui::Text("FPS : %.2f", gFakeEngine->GetGraphicManager<FPS>().fps);
		ImGui::Text("Entity Total Amount : %d\nEntity Active Amount : %d", gameCounter.totalEntityCount, gameCounter.totalActiveCount);
	}
	if (ImGui::CollapsingHeader("Select Mode"))
	{
		ImGui::CheckboxTick("Create New Board", &leveleditVar.new_board);
		if (leveleditVar.new_board)
		{
			leveleditVar.curr_createMode = LevelEditVar::CreationMode::NEW;
			leveleditVar.load_board = false;
		}
		ImGui::CheckboxTick("Load Existing Board", &leveleditVar.load_board);
		if (leveleditVar.load_board)
		{
			leveleditVar.curr_createMode = LevelEditVar::CreationMode::LOAD;
			leveleditVar.new_board = false;
		}

		if (leveleditVar.curr_createMode != leveleditVar.prev_createMode)
		{
			//do clear reset logic here
			LevelEditHelper::ResetValues();
			LevelEditHelper::getFileName();
		}

	}

	if (leveleditVar.new_board)
	{

		if (ImGui::CollapsingHeader("Generate Sample Board"))
		{
			ImGui::Text(leveleditVar.filename.c_str());
			//ImGui::Text("Board"); ImGui::SameLine(50.0f); ImGui::Combo("Number", &leveleditVar.currentLevel, leveleditVar.level, IM_ARRAYSIZE(leveleditVar.level));
			ImGui::SliderInt("Stack", &leveleditVar.stack, 0, 20);
			ImGui::SliderInt("Slice", &leveleditVar.slice, 0, 30);
			ImGui::SliderInt("TileSize", &leveleditVar.tilesize, 100, 100);
			if (leveleditVar.generationCheck)
			{
				if (ImGui::Button("Generate"))
				{
					LevelEditHelper::CreateBoard();
					LevelEditHelper::GenerateBoard();
					leveleditVar.generationTrue = true;
					leveleditVar.generationCheck = false;
					leveleditVar.message = leveleditVar.filename + " created";
				}
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, pressColor);
				ImGui::Button("Generate");
			}
		}
		ImGui::PushStyleColor(ImGuiCol_Button, DefaultColor);
	}
	else if (leveleditVar.load_board)
	{
		if (ImGui::CollapsingHeader("Load Exsisting"))
		{
			ImGui::Combo("FileName", &leveleditVar.selectedFileName,
				[](void* vec, int idx, const char** out_text) {
					std::vector<std::string>* vector = reinterpret_cast<std::vector<std::string>*>(vec);
					if (idx < 0 || idx >= vector->size())return false;
					*out_text = vector->at(idx).c_str();
					return true;
				}, reinterpret_cast<void*>(&leveleditVar.currentDir), static_cast<int>(leveleditVar.currentDir.size()));

			leveleditVar.filename = leveleditVar.currentDir[leveleditVar.selectedFileName];

			if (ImGui::Button("Load"))
			{
				LevelEditHelper::ClearBoard();
				leveleditVar.message = "LOAD SUCCEEDED";
				LevelEditHelper::LoadBoard(leveleditVar.filename);
				LevelEditHelper::GenerateBoard();
				leveleditVar.generationTrue = true;
			}
		}


	}
	if (ImGui::CollapsingHeader("Object Data") && leveleditVar.generationTrue)
	{

		ImGui::Combo("Object", &leveleditVar.selectedObject,
			[](void* vec, int idx, const char** out_text) {
				std::vector<std::string>* vector = reinterpret_cast<std::vector<std::string>*>(vec);
				if (idx < 0 || idx >= vector->size())return false;
				*out_text = vector->at(idx).c_str();
				return true;
			}, reinterpret_cast<void*>(&leveleditVar.str_vectorObj), static_cast<int>(leveleditVar.str_vectorObj.size()));


		for (const auto x : leveleditVar.allobjects)
		{
			if (leveleditVar.str_vectorObj.at(leveleditVar.selectedObject) == x._name.second)
			{
				leveleditVar.selectedObject_data = x;
			}
		}

		//if changed 
		if (leveleditVar.selectedObject != leveleditVar.prev_selectedObject)
		{

			leveleditVar.str_vectorTex.clear();
			leveleditVar.str_vectorLeverID.clear();
			leveleditVar.str_vectorTeleporterID.clear();
			for (unsigned int i = 0; i < leveleditVar.selectedObject_data._texture.second.size(); i++)
			{
				std::string str = "texture" + std::to_string(i);
				leveleditVar.str_vectorTex.push_back(str);
			}
			
			if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::LEVER_DOOR)
			{
				for (unsigned int i = 0; i < leveleditVar.leverDoorCount; i++)
				{
					std::string str = "LeverID" + std::to_string(i);
					leveleditVar.str_vectorLeverID.push_back(str);
				}
			}
			else if(leveleditVar.selectedObject_data._object.second.objType == ObjectType::LEVER_SWITCH)
			{
				for (unsigned int i = 0; i < leveleditVar.leverSwitchCount; i++)
				{
					std::string str = "LeverID" + std::to_string(i);
					leveleditVar.str_vectorLeverID.push_back(str);
				}
			}
			
			if (leveleditVar.teleporterCount == 1)
			{
				for (unsigned int i = 0; i < leveleditVar.teleporterCount; i++)
				{
					std::string str = "TeleporterID" + std::to_string(i);
					leveleditVar.str_vectorTeleporterID.push_back(str);
				}
			}
			else if (leveleditVar.teleporterCount % 2 == 0)
			{
				for (unsigned int i = 0; i < (leveleditVar.teleporterCount / 2)+1; i++)
				{
					std::string str = "TeleporterID" + std::to_string(i);
					leveleditVar.str_vectorTeleporterID.push_back(str);
				}
			}
			else
			{
				for (unsigned int i = 0; i < ((leveleditVar.teleporterCount - 1) / 2)+1; i++)
				{
					std::string str = "TeleporterID" + std::to_string(i);
					leveleditVar.str_vectorTeleporterID.push_back(str);
				}
			}


			leveleditVar.prev_selectedObject = leveleditVar.selectedObject;

		}

		if (ImGui::CollapsingHeader("Texture Data"))
		{
			ImGui::Combo("Texture", &leveleditVar.selectedTexture,
				[](void* vec, int idx, const char** out_text) {
					std::vector<std::string>* vector = reinterpret_cast<std::vector<std::string>*>(vec);
					if (idx < 0 || idx >= vector->size())return false;
					*out_text = vector->at(idx).c_str();
					return true;
				}, reinterpret_cast<void*>(&leveleditVar.str_vectorTex), static_cast<int>(leveleditVar.str_vectorTex.size()));
			//leveleditVar.selectedTex = leveleditVar.selectedObject_data._texture.second[leveleditVar.selectedTexture];
		}

		if (ImGui::CollapsingHeader("Direction Data"))
		{
			ImGui::Combo("Direction", &leveleditVar.selectedDir,
				[](void* vec, int idx, const char** out_text) {
					std::vector<std::string>* vector = reinterpret_cast<std::vector<std::string>*>(vec);
					if (idx < 0 || idx >= vector->size())return false;
					*out_text = vector->at(idx).c_str();
					return true;
				}, reinterpret_cast<void*>(&leveleditVar.str_vectorDir), static_cast<int>(leveleditVar.str_vectorDir.size()));
		}


		for (const auto x : serializationVar.dirData)
		{
			if (leveleditVar.str_vectorDir.at(leveleditVar.selectedDir) == x.second.second)
			{
				leveleditVar.selectedObject_data._state.second.objectDir = x.second.first;
			}
		}

		if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::LEVER_DOOR
			|| leveleditVar.selectedObject_data._object.second.objType == ObjectType::LEVER_SWITCH)
		{

			if (ImGui::CollapsingHeader("Lever ID"))
			{
				ImGui::Combo("Lever ID", &leveleditVar.selectedLeverID,
					[](void* vec, int idx, const char** out_text) {
						std::vector<std::string>* vector = reinterpret_cast<std::vector<std::string>*>(vec);
						if (idx < 0 || idx >= vector->size())return false;
						*out_text = vector->at(idx).c_str();
						return true;
					}, reinterpret_cast<void*>(&leveleditVar.str_vectorLeverID), static_cast<int>(leveleditVar.str_vectorLeverID.size()));
			}
		}
		else if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::TELEPORTER)
		{
			if (ImGui::CollapsingHeader("Teleporter ID"))
			{
				ImGui::Combo("Teleporter ID", &leveleditVar.selectedTeleporterID,
					[](void* vec, int idx, const char** out_text) {
						std::vector<std::string>* vector = reinterpret_cast<std::vector<std::string>*>(vec);
						if (idx < 0 || idx >= vector->size())return false;
						*out_text = vector->at(idx).c_str();
						return true;
					}, reinterpret_cast<void*>(&leveleditVar.str_vectorTeleporterID), static_cast<int>(leveleditVar.str_vectorTeleporterID.size()));
			}
		}
		//else if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::TUTORIAL_FLOOR)
		//{


		//}

		ImGui::Text("Image Preview");
		if (leveleditVar.selectedObject_data._state.second.objectDir == ObjectDirection::UP)
			ImGui::Image((void*)(intptr_t)leveleditVar.selectedObject_data._texture.second[leveleditVar.selectedTexture].texture->pTex, ImVec2(128, 128), ImVec2(1, 0), ImVec2(0, 1));
		else if (leveleditVar.selectedObject_data._state.second.objectDir == ObjectDirection::RIGHT)
			ImGui::Image((void*)(intptr_t)leveleditVar.selectedObject_data._texture.second[leveleditVar.selectedTexture].texture->pTex, ImVec2(128, 128), ImVec2(1, 1), ImVec2(0, 0));
		else if (leveleditVar.selectedObject_data._state.second.objectDir == ObjectDirection::DOWN)
			ImGui::Image((void*)(intptr_t)leveleditVar.selectedObject_data._texture.second[leveleditVar.selectedTexture].texture->pTex, ImVec2(128, 128), ImVec2(0, 0), ImVec2(1, 1));
		else
			ImGui::Image((void*)(intptr_t)leveleditVar.selectedObject_data._texture.second[leveleditVar.selectedTexture].texture->pTex, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Text("");

		if (ImGui::CollapsingHeader("Save Objects"))
		{
			ImGui::CheckboxTick("Add Object", &leveleditVar.addObject_Check);
			if (leveleditVar.addObject_Check)
				leveleditVar.deleteObject_Check = false;

			ImGui::CheckboxTick("Delete Object", &leveleditVar.deleteObject_Check);
			if (leveleditVar.deleteObject_Check)
				leveleditVar.addObject_Check = false;

			ImGui::Text("");
			if (ImGui::Button("Clear"))
			{
				LevelEditHelper::ClearBoard();
			}
			ImGui::Text("");
			if (ImGui::Button("Save"))
			{
				if (!leveleditVar.isMainCharacterAdded)
					leveleditVar.message = "Remember to Include MainCharacter";
				else if (leveleditVar.leverDoorCount != leveleditVar.leverSwitchCount)
					leveleditVar.message = "One to One releationship for lever and switch";
				else
				{
					LevelEditHelper::setBoardData();
					LevelEditHelper::SaveBoard(SaveBoard_MODE::SAVE_MAP_DATA, leveleditVar.data);
					leveleditVar.message = "Saved successfully";
				}
			}
		}
	}
	if (ImGui::CollapsingHeader("MESSAGE BOX"))
		ImGui::Text("***********************\n MESSAGE BOX \n***********************\n");
	ImGui::Text(leveleditVar.message.c_str());

	ImGui::End();
}