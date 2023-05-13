/*****************************************************************
*\file		 LevelEditSystem.cpp
*\brief		 LevelEditSystem functions definitions.

*\author(s)   Kenric Tan Wei Liang	139 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <FakeEngine.h>
#include <LevelEditSystem.h>
#include "../Serialization/Serialization.h"

extern std::unique_ptr<FakeEngine> gFakeEngine;


void LevelEditSystem::Init()
{

}

void LevelEditSystem::Update()
{
	LevelEditHelper::GetPos();
	LevelEditHelper::CheckInput();

	LevelEditVar& leveleditVar = gFakeEngine->GetGameManager<LevelEditVar>();
	ImGuiIO& io = ImGui::GetIO();

	mathEngine::Vector3D tmp_obj_pos;
	LevelEditHelper::calculateGamePos(tmp_obj_pos, leveleditVar.mousepos_y, leveleditVar.mousepos_x);
	Position obj_pos;
	obj_pos.pos = tmp_obj_pos;

	if (leveleditVar.addObject_Check)
	{
		if (leveleditVar.button_click && !io.WantCaptureMouse )
		{			
			//do a boolean check eg map data to check if the current selected map is true or false
			if (!leveleditVar.checkMapData[leveleditVar.mousepos_y][leveleditVar.mousepos_x])
			{
				if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::MAIN_CHARACTER && leveleditVar.isMainCharacterAdded)
				{
					leveleditVar.message = "Main Character can only be added once";
				}
				else
				{
					Entity entityID;
					LevelEditHelper::createEntity(leveleditVar.selectedObject_data._object.second.objType, leveleditVar.selectedTexture, leveleditVar.selectedObject_data._state.second.objectDir, 
						obj_pos, leveleditVar.selectedLeverID, leveleditVar.selectedTeleporterID ,entityID);

					if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::MAIN_CHARACTER)
						leveleditVar.isMainCharacterAdded = true;

					leveleditVar.data._mapdata[leveleditVar.mousepos_y][leveleditVar.mousepos_x] = leveleditVar.selectedObject_data._object.second.objType;
					leveleditVar.data._texdata[leveleditVar.mousepos_y][leveleditVar.mousepos_x] = leveleditVar.selectedTexture;
					leveleditVar.data._dirdata[leveleditVar.mousepos_y][leveleditVar.mousepos_x] = leveleditVar.selectedObject_data._state.second.objectDir;

					if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::LEVER_DOOR
						|| leveleditVar.selectedObject_data._object.second.objType == ObjectType::LEVER_SWITCH)
					{
						leveleditVar.data._leverID[leveleditVar.mousepos_y][leveleditVar.mousepos_x] = leveleditVar.selectedLeverID;
					}

					if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::TELEPORTER)
					{
						leveleditVar.data._teleporterID[leveleditVar.mousepos_y][leveleditVar.mousepos_x] = leveleditVar.selectedTeleporterID;
						leveleditVar.teleporterCount++;
					}

					leveleditVar.entityData[leveleditVar.mousepos_y][leveleditVar.mousepos_x] = entityID;
					leveleditVar.checkMapData[leveleditVar.mousepos_y][leveleditVar.mousepos_x] = true;

					if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::LEVER_DOOR)
					{
						
						leveleditVar.leverDoorCount++;

					}
					else if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::LEVER_SWITCH)
					{
						leveleditVar.leverSwitchCount++;
					}
				}
			}
			else
			{
				leveleditVar.message = "Map Data Already Added / Adding to places outside of game space";
			}
		}
	}
	else if (leveleditVar.deleteObject_Check)
	{
		if (leveleditVar.button_click && !io.WantCaptureMouse)
		{
			if (leveleditVar.checkMapData[leveleditVar.mousepos_y][leveleditVar.mousepos_x])
			{
					auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();

					if (leveleditVar.data._mapdata[leveleditVar.mousepos_y][leveleditVar.mousepos_x] == ObjectType::MAIN_CHARACTER)
						leveleditVar.isMainCharacterAdded = false;

					mGameECSmanager.DestroyEntity(leveleditVar.entityData[leveleditVar.mousepos_y][leveleditVar.mousepos_x]);
					leveleditVar.data._mapdata[leveleditVar.mousepos_y][leveleditVar.mousepos_x] = ObjectType::FLOOR;
					leveleditVar.data._texdata[leveleditVar.mousepos_y][leveleditVar.mousepos_x] = 0;
					leveleditVar.data._dirdata[leveleditVar.mousepos_y][leveleditVar.mousepos_x] = ObjectDirection::NO_DIR;
					leveleditVar.data._leverID[leveleditVar.mousepos_y][leveleditVar.mousepos_x] = 0;
					leveleditVar.checkMapData[leveleditVar.mousepos_y][leveleditVar.mousepos_x] = false;
					leveleditVar.data._teleporterID[leveleditVar.mousepos_y][leveleditVar.mousepos_x] = 0;

					/*if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::LEVER_DOOR)
					{
						leveleditVar.leverDoorCount--;

					}

					else if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::LEVER_SWITCH)
					{
						leveleditVar.leverSwitchCount--;
					}
					else if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::TELEPORTER)
						leveleditVar.teleporterCount--;*/

			}
			else
			{
				leveleditVar.message = "Map Data Already Deleted~";
			}
		}
	}

	if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::LEVER_DOOR
		|| leveleditVar.selectedObject_data._object.second.objType == ObjectType::LEVER_SWITCH)

	{
		leveleditVar.str_vectorLeverID.clear();
		if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::LEVER_DOOR)
		{
			for (unsigned int i = 0; i < leveleditVar.leverDoorCount; i++)
			{
				std::string str = "LeverID" + std::to_string(i);
				leveleditVar.str_vectorLeverID.push_back(str);
			}
		}
		else if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::LEVER_SWITCH)
		{
			for (unsigned int i = 0; i < leveleditVar.leverSwitchCount; i++)
			{
				std::string str = "LeverID" + std::to_string(i);
				leveleditVar.str_vectorLeverID.push_back(str);

			}
		}
	}
	else if (leveleditVar.selectedObject_data._object.second.objType == ObjectType::TELEPORTER)
	{
		leveleditVar.str_vectorTeleporterID.clear();

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
			for (unsigned int i = 0; i < (leveleditVar.teleporterCount / 2) + 1; i++)
			{
				std::string str = "TeleporterID" + std::to_string(i);
				leveleditVar.str_vectorTeleporterID.push_back(str);
			}
		}
		else
		{
			for (unsigned int i = 0; i < ((leveleditVar.teleporterCount - 1) / 2) + 1; i++)
			{
				std::string str = "TeleporterID" + std::to_string(i);
				leveleditVar.str_vectorTeleporterID.push_back(str);
			}
		}
	}
}
