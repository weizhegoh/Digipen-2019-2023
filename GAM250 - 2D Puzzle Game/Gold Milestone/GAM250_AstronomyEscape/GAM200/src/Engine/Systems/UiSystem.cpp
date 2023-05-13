/*****************************************************************
*\file		 UiSystem.cpp
*\brief		 UI System Logic

*\author(s)   Justin Yip Ling Kit 		59 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <UiSystem.h>
#include <FakeEngine.h>
extern std::unique_ptr<FakeEngine> gFakeEngine;

void UiSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<UI>());
	mGameECSmanager.SetSystemBitSignature<UiSystem>(signature);
}

void UiSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	for (auto const& entity : mEntities)
	{
		Position& pos = mGameECSmanager.GetComponent<Position>(entity);
		UI& ui = mGameECSmanager.GetComponent<UI>(entity);
		Camera2d& camera2d = gFakeEngine->GetGraphicManager<Camera2d>();
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		SerializationVar& serializationVar = gFakeEngine->GetGameManager<SerializationVar>();

		if (ui.camPos_old.z == 0 && ui.camPos_old.x == 0 && ui.camPos_old.y == 0) {
			ui.camPos_old = pos.pos;
			//ui.camPos_old.z = -0.8f;
		}
		pos.pos = ui.camPos_old + mathEngine::Vector3D(camera2d.pos.x, camera2d.pos.y, camera2d.pos.z);

		//Pop up window system
		if (ui.popup && flag.activeFlag) {
			Input& input = gFakeEngine->GetGraphicManager<Input>();
			if (input.CheckPress(GLFW_KEY_SPACE)) {
				StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
				if (stageChanger.GetCurrentStage() == StagesEnum::ACTUAL_STAGE)
				{
					serializationVar.afterPopup = true;
				}
				mGameECSmanager.GetComponent<Flags>(entity).activeFlag = FLAG_INACTIVE;
			}
		}

		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
		int counter;
		//HP Counter Sysstem
		if (objecttype.objType == ObjectType::UI_HP_COUNTER) {
			for (auto const& x : stageChanger.allobjects) {
				if (x._object.second.objType == objecttype.objType) {
					counter = mGameECSmanager.GetComponent<Inventory> (gFakeEngine->GetGameManager<GlobalVar>().mainCharID)._lifes;
					if (counter < 1)
						counter = 0;
					mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[counter].texture;
				}
			}
		}
		if (objecttype.objType == ObjectType::POPUP) {
			for (auto const& x : stageChanger.allobjects) {
				if (x._object.second.objType == objecttype.objType) {
					counter = (int)stageChanger.GetCurrentStage() - 2;
					mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[counter].texture;
				}
			}
		}
	}
}