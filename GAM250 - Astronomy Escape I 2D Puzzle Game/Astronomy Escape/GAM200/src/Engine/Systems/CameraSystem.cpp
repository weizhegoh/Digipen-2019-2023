/*****************************************************************
*\file		 CameraSystem.cpp
*\brief		 CameraSystem functions definitions.

*\author(s)   Kenric Tan Wei Liang		77 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <CameraSystem.h>
#include <FakeEngine.h>
#include <SystemList.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern glm::vec2 gamesize;
extern float g_dt;

void CameraSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	mGameECSmanager.SetSystemBitSignature<CameraSystem>(signature);
}

void CameraSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Camera2d& camera2d = gFakeEngine->GetGraphicManager<Camera2d>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	if (_gameStateCurrent == GS_LEVELEDIT)
	{
		if (camera2d.zoomOUT_input == true)
		{
			camera2d.cam_window.x += (globalVar.GAME_SIZE.x * g_dt);
			camera2d.cam_window.y += (globalVar.GAME_SIZE.y * g_dt);
		}
		if (camera2d.zoomIN_input == true)
		{
			camera2d.cam_window.x -= (globalVar.GAME_SIZE.x * g_dt);
			camera2d.cam_window.y -= (globalVar.GAME_SIZE.y * g_dt);
		}
	}
	else if (_gameStateCurrent == GS_MAINMENU)
	{
		return;
	}
	else
	{

		for (auto const& entity : mEntities)
		{
			EntityType& type = mGameECSmanager.GetComponent<EntityType>(entity);
			if (type.objType == ObjectType::MAIN_CHARACTER)
			{
				Position& position = mGameECSmanager.GetComponent<Position>(entity);
				FPS& fps = gFakeEngine->GetGraphicManager<FPS>();
				for (int step = 0; step < fps.GetCurrentNumberOfSteps(); ++step)
				{
					if (camera2d.zoom_input)
					{
						Position level_position;
						GlobalVar& globalvar = gFakeEngine->GetGameManager<GlobalVar>();
						if (globalvar.level_door_opened)
							level_position = mGameECSmanager.GetComponent<Position>(globalVar.level_doorID);
						else
						{
							auto& mGameSystem = gFakeEngine->GetECSGameManager<GameSystemsList>();
							for (const auto& entity_ko : mGameSystem.keyObjectiveSystem->mEntities)
							{
								level_position = mGameECSmanager.GetComponent<Position>(entity_ko);
							}
						}
							
						camera2d.MoveToBack(position.pos , level_position.pos, 4.f);
						camera2d.zoom_input = false;
					}

					if (camera2d.cam_opt == ZoomOption::NORMAL)
					{
						camera2d.pos = { position.pos.x,position.pos.y,position.pos.z };
					}
					else
					{
						camera2d.Update();
					}

				}
			}
		}
	}
}