/*****************************************************************
*\file		 Victory_screen.cpp
*\brief		 Victory Screen Logic

*\author(s)   Justin Yip Ling Kit 	84 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <Victory_screen.h>
extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;

void Victory_screen::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<UI>());
	mGameECSmanager.SetSystemBitSignature<Victory_screen>(signature);
}

void Victory_screen::Update() {
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();

	for (auto const& entity : mEntities) {
		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (objecttype.objType == ObjectType::VICTORY_BG ||
			objecttype.objType == ObjectType::VICTORY_CONTINUE ||
			objecttype.objType == ObjectType::VICTORY_MAINMENU ||
			objecttype.objType == ObjectType::VICTORY_RESTART) {

			if (!input.GetInputBooleen().win_state)
				flag.activeFlag = FLAG_INACTIVE;
			else {
				flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;

				//if (objecttype.objType == ObjectType::VICTORY_CONTINUE /*&&
				//	gFakeEngine->GetGraphicManager<StageChanger>().GetCurrentStage() == StagesEnum::LEVEL5*/)
				//	flag.activeFlag = FLAG_INACTIVE;
				
				//check if it collides with the screen
				Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
				Position& position = mGameECSmanager.GetComponent<Position>(entity);
				if (Collision::OBB::StaticPointToStaticRect(input.GetMousePosWorld(), position.pos, transform.scale.x, transform.scale.y)) {
					if (objecttype.objType == ObjectType::VICTORY_CONTINUE ||
						objecttype.objType == ObjectType::VICTORY_MAINMENU ||
						objecttype.objType == ObjectType::VICTORY_RESTART)
						Hover(entity);

					if (input.CheckPress(GLFW_MOUSE_BUTTON_1)) {
						input.GetInputBooleen().hoverEntity = 0;
						switch (objecttype.objType) {
						case ObjectType::VICTORY_CONTINUE:
							//***************************************************************************************************************************************************
							//Insert Continue Function Here!!
							//***************************************************************************************************************************************************					
							//if(gFakeEngine->GetGraphicManager<StageChanger>().GetCurrentStage() == StagesEnum::LEVEL5)

							gFakeEngine->GetGraphicManager<StageChanger>().NextState();
							input.GetInputBooleen().pause_screen = true;

							break;
						case ObjectType::VICTORY_MAINMENU:
							_gameStateNext = GS_MAINMENU;
							break;

						case ObjectType::VICTORY_RESTART:
							_gameStateNext = GS_RESTART;
							break;

						default: //Should not reach here
							break;
						}
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_CLICK), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
			}
		}
	}
}

void Victory_screen::Hover(Entity entity) {
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();

	if (input.GetInputBooleen().hoverEntity != entity)
	{
		//GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
		StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
		EntityType& objecttype_1 = mGameECSmanager.GetComponent<EntityType>(entity);

		for (auto const& x : stageChanger.allobjects)
		{
			if (x._object.second.objType == objecttype_1.objType)
			{
				mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[1].texture;
			}
			if (input.GetInputBooleen().hoverEntity)
			{
				EntityType& objecttype_2 = mGameECSmanager.GetComponent<EntityType>(input.GetInputBooleen().hoverEntity);

				if (x._object.second.objType == objecttype_2.objType)
				{
					mGameECSmanager.GetComponent<RenderTex>(input.GetInputBooleen().hoverEntity).texture = x._texture.second[0].texture;
				}
			}
		}
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
	}
	input.GetInputBooleen().hoverEntity = entity;
}