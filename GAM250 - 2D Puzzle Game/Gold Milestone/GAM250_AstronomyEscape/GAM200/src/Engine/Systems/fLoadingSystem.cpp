/*****************************************************************
*\file		 fLoadingSystem.cpp
*\brief		 fLoadingSystem functions definitions.

*\author(s)   Lee Liang Ping	210 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <fLoadingSystem.h>
#include <RenderingSystem.h>
#include <FakeEngine.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;
void FakeLoadingSystem::Init()
{
	gFakeEngine->SubscribeEvent(this, &FakeLoadingSystem::SetWinPage, EventID::SetWincreenEvent);
	gFakeEngine->SubscribeEvent(this, &FakeLoadingSystem::SetLoading, EventID::SetLoadingScreenEvent);
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	signature.set(mGameECSmanager.GetComponentType<Transform>());
	signature.set(mGameECSmanager.GetComponentType<Render>());
	signature.set(mGameECSmanager.GetComponentType<LoadingType>());
	mGameECSmanager.SetSystemBitSignature<FakeLoadingSystem>(signature);
}


void FakeLoadingSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Window& window = gFakeEngine->GetGraphicManager<Window>();

	for (auto const& entity : mEntities)
	{
		Position& position = mGameECSmanager.GetComponent<Position>(entity);
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;
		Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
		transform.scale = mathEngine::Vector3D((float)window.GetWidth(), (float)window.GetHeight(), 0.f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 glmposition = glm::vec3(position.pos.x, position.pos.y, position.pos.z);
		model = glm::translate(model, glmposition);
		model = glm::rotate(model, glm::radians(transform.angle + transform.angleoffset), glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, glm::vec3(transform.scale.x + transform.scaleoffset.x, transform.scale.y + transform.scaleoffset.y, transform.scale.z));
		transform.model = model;
		transform.view = glm::mat4(1.0f);
		transform.proj = glm::ortho(
			float(-window.GetHalfWidth()),
			float(window.GetHalfWidth()),
			float(-window.GetHalfHeight()),
			float(window.GetHalfHeight()));
	}

	RenderingSystem::RenderObjects(mEntities, EntitySystemOption::GAME_ENTITY);
	if (!(_gameStateCurrent == GS_MAINMENU))
	{
		Input& input = gFakeEngine->GetGraphicManager<Input>();
		Graphics::FreeType& freetype = gFakeEngine->GetFreeTypeFont(FreeType_Font::OCR_A_EXTENDED);
		if (!input.GetInputBooleen().win_screen)
		{
			freetype.RenderText(
				"Loading...",
				(float)window.GetQuarterWidth(), -(float)window.GetHalfHeight() * 0.9f,
				1.5,
				glm::vec3(1.f, 1.f, 1.f),
				FreeRenderType::SCREEN);
		}
	}

	Input& input = gFakeEngine->GetGraphicManager<Input>();
	if (input.CheckPress(GLFW_KEY_SPACE))
		if (input.GetInputBooleen().win_screen)
			_gameStateNext = GS_MAINMENU;



}

void FakeLoadingSystem::SetLoading(SetLoadingScreen* set)
{
	for (auto const& entity : mEntities)
	{
		auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		if (objecttype.objType == ObjectType::LOADING_LOGO || objecttype.objType == ObjectType::LOADING_SCREENS)
		{
			Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
			if (set->set)
				flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
			else
				flag.activeFlag = FLAG_INACTIVE;
		}
	}
}

void FakeLoadingSystem::SetWinPage(SetWinScreen* set)
{
	//GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	for (auto const& entity : mEntities)
	{
		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (objecttype.objType == ObjectType::LOADING_LOGO || objecttype.objType == ObjectType::LOADING_SCREENS)
		{
			if (objecttype.objType == ObjectType::LOADING_SCREENS)
				input.GetInputBooleen().win_screen = true;
			if (set->set)
			{
				flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
				for (auto const& x : stageChanger.allobjects)
				{
					if (x._object.second.objType == objecttype.objType)
					{
						mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[set->pageNum].texture;
					}
				}
			}
			else
				flag.activeFlag = FLAG_INACTIVE;
		}
	}

}

void FakeLoadingSystem::ToggleLOGO()
{
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	InputBoolean& inputBoolean = input.GetInputBooleen();
	auto& globalbool = gFakeEngine->GetGraphicManager<Graphics::Storage>();
	switch (inputBoolean.logostate)
	{
	case LogoState::DIGIPENLOGO:
		gFakeEngine->ExecuteEvent(new SetWinScreen(true, 1));
		inputBoolean.logostate = LogoState::FMODLOGO;
		break;
	case LogoState::FMODLOGO:
		gFakeEngine->ExecuteEvent(new SetWinScreen(true, 2));
		inputBoolean.logostate = LogoState::TEAMLOGO;
		break;
	case LogoState::TEAMLOGO:
		gFakeEngine->ExecuteEvent(new SetWinScreen(true, 3));
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DIALOG_1), FMOD_LOOP_OFF, 1.f);
		inputBoolean.logostate = LogoState::CINEMATIC_1;
		break;
	case LogoState::CINEMATIC_1:
		gFakeEngine->ExecuteEvent(new SetWinScreen(true, 4));
		gAudioEngine->UnloadSound(gAudioEngine->TypeToString(SoundType::DIALOG_1));
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DIALOG_2), FMOD_LOOP_OFF, 1.f);
		inputBoolean.logostate = LogoState::CINEMATIC_2;
		break;
	case LogoState::CINEMATIC_2:
		gFakeEngine->ExecuteEvent(new SetWinScreen(true, 5));
		gAudioEngine->UnloadSound(gAudioEngine->TypeToString(SoundType::DIALOG_2));
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DIALOG_3), FMOD_LOOP_OFF, 1.f);
		inputBoolean.logostate = LogoState::CINEMATIC_3;
		break;
	case LogoState::CINEMATIC_3:
		gFakeEngine->ExecuteEvent(new SetWinScreen(true, 6));
		gAudioEngine->UnloadSound(gAudioEngine->TypeToString(SoundType::DIALOG_3));
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DIALOG_4), FMOD_LOOP_OFF, 1.f);
		inputBoolean.logostate = LogoState::CINEMATIC_4;
		break;
	case LogoState::CINEMATIC_4:
		gFakeEngine->ExecuteEvent(new SetWinScreen(true, 7));
		gAudioEngine->UnloadSound(gAudioEngine->TypeToString(SoundType::DIALOG_4));
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DIALOG_5), FMOD_LOOP_OFF, 1.f);
		inputBoolean.logostate = LogoState::CINEMATIC_5;
		break;
	case LogoState::CINEMATIC_5:
		gFakeEngine->ExecuteEvent(new SetWinScreen(true, 8));
		gAudioEngine->UnloadSound(gAudioEngine->TypeToString(SoundType::DIALOG_5));
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DIALOG_6), FMOD_LOOP_OFF, 1.f);
		inputBoolean.logostate = LogoState::CINEMATIC_6;
		break;
	case LogoState::CINEMATIC_6:
		gFakeEngine->ExecuteEvent(new SetWinScreen(true, 9));
		gAudioEngine->UnloadSound(gAudioEngine->TypeToString(SoundType::DIALOG_6));
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DIALOG_7), FMOD_LOOP_OFF, 1.f);
		inputBoolean.logostate = LogoState::CINEMATIC_7;
		break;
	case LogoState::CINEMATIC_7:
		gFakeEngine->ExecuteEvent(new SetWinScreen(true, 10));
		gAudioEngine->UnloadSound(gAudioEngine->TypeToString(SoundType::DIALOG_7));
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DIALOG_8), FMOD_LOOP_OFF, 1.f);
		inputBoolean.logostate = LogoState::CINEMATIC_8;
		break;
	case LogoState::CINEMATIC_8:
		gFakeEngine->ExecuteEvent(new SetWinScreen(true, 11));
		gAudioEngine->UnloadSound(gAudioEngine->TypeToString(SoundType::DIALOG_8));
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DIALOG_9), FMOD_LOOP_OFF, 1.f);
		inputBoolean.logostate = LogoState::CINEMATIC_9;
		break;
	case LogoState::CINEMATIC_9:
		gFakeEngine->ExecuteEvent(new SetWinScreen(true, 12));
		gAudioEngine->UnloadSound(gAudioEngine->TypeToString(SoundType::DIALOG_9));
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DIALOG_10), FMOD_LOOP_OFF, 1.f);
		inputBoolean.logostate = LogoState::CINEMATIC_10;
		break;
	case LogoState::CINEMATIC_10:
		gFakeEngine->ExecuteEvent(new SetWinScreen(true, 13));
		gAudioEngine->UnloadSound(gAudioEngine->TypeToString(SoundType::DIALOG_10));
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DIALOG_11), FMOD_LOOP_OFF, 1.f);
		inputBoolean.logostate = LogoState::CINEMATIC_11;
		break;
	case LogoState::CINEMATIC_11:
		gFakeEngine->ExecuteEvent(new SetWinScreen(true, 14));
		gAudioEngine->UnloadAll();
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::SUBMERGED_SILENCE_LONG), FMOD_LOOP_NORMAL, 0.6f);
		inputBoolean.logostate = LogoState::CINEMATIC_12;
		break;
	case LogoState::CINEMATIC_12:
		inputBoolean.logostate = LogoState::GOTOMAINMENU;

		gFakeEngine->ExecuteEvent(new SetLoadingScreen(false));
		globalbool.globalLogo = false;
		break;
	default:
		break;
	}
}