/*****************************************************************
*\file		 Defeat_screen.cpp
*\brief		 Defeat Screen Logic

*\author(s)   Justin Yip Ling Kit 	76 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <Defeat_screen.h>
extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;

void Defeat_screen::Init() {
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<UI>());
	mGameECSmanager.SetSystemBitSignature<Defeat_screen>(signature);
}

void Defeat_screen::Update() {
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();

	for (auto const& entity : mEntities) {
		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (objecttype.objType == ObjectType::DEFEAT_BG ||
			objecttype.objType == ObjectType::DEFEAT_MAINMENU ||
			objecttype.objType == ObjectType::DEFEAT_RESTART) {

			if (!input.GetInputBooleen().defeat_state)
				flag.activeFlag = FLAG_INACTIVE;
			else {
				flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;

				//check if it collides with the screen
				Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
				Position& position = mGameECSmanager.GetComponent<Position>(entity);
				if (Collision::OBB::StaticPointToStaticRect(input.GetMousePosWorld(), position.pos, transform.scale.x, transform.scale.y)) {
					if (objecttype.objType == ObjectType::DEFEAT_MAINMENU ||
						objecttype.objType == ObjectType::DEFEAT_RESTART)
					{
						Hover(entity);
					}
					if (input.CheckPress(GLFW_MOUSE_BUTTON_1)) {
						switch (objecttype.objType) {
						case ObjectType::DEFEAT_MAINMENU:
							input.GetInputBooleen().hoverEntity = 0;
							_gameStateNext = GS_MAINMENU;
							break;

						case ObjectType::DEFEAT_RESTART:
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

void Defeat_screen::Hover(Entity entity) {
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();

	if (input.GetInputBooleen().hoverEntity != entity)
	{
		StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
		//GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
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