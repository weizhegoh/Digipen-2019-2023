/*****************************************************************
*\file		 MainMenuInputSystem.cpp
*\brief		 Contains definition for all main menu states

*\author(s)   Goh Wei Zhe	927 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <MainMenuInputSystem.h>
#include <FakeEngine.h>
#include <AudioEngine.h>
#include <Window.h>
#include <Collision.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;

void MainMenuInputSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	mGameECSmanager.SetSystemBitSignature<MainMenuInputSystem>(signature);
}

void MainMenuInputSystem::Update()
{
	Input& input = gFakeEngine->GetGraphicManager<Input>();

	//Enter respective menu functions
	if (input.GetInputBooleen().enterMain)
		mainMenu();
	else if (input.GetInputBooleen().enterLevelSelect)
		levelSelectMenu();
	else if (input.GetInputBooleen().enterControls)
		controlMenu();
	else if (input.GetInputBooleen().enterOption)
		optionMenu();
	else if (input.GetInputBooleen().enterCredits)
		creditMenu();
	else if (input.GetInputBooleen().enterQuit)
		quitMenu();
}

void MainMenuInputSystem::mainMenu()
{
	static int selectMenu = PLAY;
	bool click = false;

	Input& input = gFakeEngine->GetGraphicManager<Input>();
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();

	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
		Position& position = mGameECSmanager.GetComponent<Position>(entity);

		//set objects to visible
		if (objecttype.objType == ObjectType::MAIN_MENU_BG ||
			objecttype.objType == ObjectType::PLAY ||
			objecttype.objType == ObjectType::CONTROL ||
			objecttype.objType == ObjectType::OPTION ||
			objecttype.objType == ObjectType::CREDIT ||
			objecttype.objType == ObjectType::QUIT ||
			objecttype.objType == ObjectType::MOUSE_CURSOR)
			flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
		else
			flag.activeFlag = FLAG_INACTIVE;

		if (objecttype.objType == ObjectType::PLAY ||
			objecttype.objType == ObjectType::CONTROL ||
			objecttype.objType == ObjectType::OPTION ||
			objecttype.objType == ObjectType::CREDIT ||
			objecttype.objType == ObjectType::QUIT)
		{
			if (Collision::OBB::StaticPointToStaticRect(input.GetMousePosWorld(), position.pos, transform.scale.x, transform.scale.y))
			{
				//if collide, set curent menu button to collided button
				if (objecttype.objType == ObjectType::PLAY)
				{
					selectMenu = PLAY;

					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (objecttype.objType == ObjectType::CONTROL)
				{
					selectMenu = CONTROLS;

					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (objecttype.objType == ObjectType::OPTION)
				{
					selectMenu = OPTIONS;

					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (objecttype.objType == ObjectType::CREDIT)
				{
					selectMenu = CREDITS;
				
					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (objecttype.objType == ObjectType::QUIT)
				{
					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
					selectMenu = QUIT;
				}
				//if collide, change texture
				SetChangeTexture(false, entity);

				//if mouse click, set mouse click to true
				if (input.CheckPress(GLFW_MOUSE_BUTTON_LEFT))
					click = true;
				else
					click = false;
			}
			else
			{
				//if keyboard current selection, change texture
				if (selectMenu == PLAY && objecttype.objType == ObjectType::PLAY)
				{
					SetChangeTexture(false, entity);
					input.GetInputBooleen().hoverPlayed = FALSE;
				}
				else if (selectMenu == CONTROLS && objecttype.objType == ObjectType::CONTROL)
				{
					SetChangeTexture(false, entity);
					input.GetInputBooleen().hoverPlayed = FALSE;
				}
				else if (selectMenu == OPTIONS && objecttype.objType == ObjectType::OPTION)
				{
					SetChangeTexture(false, entity);
					input.GetInputBooleen().hoverPlayed = FALSE;
				}
				else if (selectMenu == CREDITS && objecttype.objType == ObjectType::CREDIT)
				{
					SetChangeTexture(false, entity);
					input.GetInputBooleen().hoverPlayed = FALSE;
				}
				else if (selectMenu == QUIT && objecttype.objType == ObjectType::QUIT)
				{
					SetChangeTexture(false, entity);
					input.GetInputBooleen().hoverPlayed = FALSE;
				}
				else
					SetChangeTexture(true, entity);
			}
		}
	}

	//Quit game if press escape button at main menu
	if (input.CheckPress(GLFW_KEY_ESCAPE))
		_gameStateNext = GS_STATES::GS_QUIT;

	//Keyboard input, press up or down to select menu
	if (input.CheckPress(GLFW_KEY_DOWN) && selectMenu < QUIT)
	{
		++selectMenu;
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
	}
	else if (input.CheckPress(GLFW_KEY_UP) && selectMenu > PLAY)
	{
		--selectMenu;
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
	}

	//If user presses enter or mouse click
	if (input.CheckPress(GLFW_KEY_SPACE) || click)
	{
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_CLICK), FMOD_LOOP_OFF, gAudioEngine->volume);

		switch (selectMenu)
		{
			//enter level select menu
		case PLAY:
			input.GetInputBooleen().enterMain = false;
			input.GetInputBooleen().enterLevelSelect = true;
			break;

			//enter how to play menu
		case CONTROLS:
			input.GetInputBooleen().enterMain = false;
			input.GetInputBooleen().enterControls = true;
			break;

			//enter option menu
		case OPTIONS:
			input.GetInputBooleen().enterMain = false;
			input.GetInputBooleen().enterOption = true;
			break;

			//enter credit menu
		case CREDITS:
			input.GetInputBooleen().enterMain = false;
			input.GetInputBooleen().enterCredits = true;
			break;

			//enter quit menu
		case QUIT:
			input.GetInputBooleen().enterMain = false;
			input.GetInputBooleen().enterQuit = true;
			break;
		}
	}
}

void MainMenuInputSystem::levelSelectMenu()
{
	static int selectLevel = LEVEL1;
	bool click = false;

	Input& input = gFakeEngine->GetGraphicManager<Input>();
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();

	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
		Position& position = mGameECSmanager.GetComponent<Position>(entity);

		//set objects to visible
		if (objecttype.objType == ObjectType::LEVELSELECT_MENU_BG ||
			objecttype.objType == ObjectType::LEVEL1 ||
			objecttype.objType == ObjectType::LEVEL2 ||
			objecttype.objType == ObjectType::LEVEL3 ||
			objecttype.objType == ObjectType::LEVEL4 ||
			objecttype.objType == ObjectType::LEVEL5 ||
			objecttype.objType == ObjectType::LEVEL_BACK ||
			objecttype.objType == ObjectType::MOUSE_CURSOR)
			flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
		else
			flag.activeFlag = FLAG_INACTIVE;

		if (objecttype.objType == ObjectType::LEVEL1 ||
			objecttype.objType == ObjectType::LEVEL2 ||
			objecttype.objType == ObjectType::LEVEL3 ||
			objecttype.objType == ObjectType::LEVEL4 ||
			objecttype.objType == ObjectType::LEVEL5 ||
			objecttype.objType == ObjectType::LEVEL_BACK)
		{
			if (Collision::OBB::StaticPointToStaticRect(input.GetMousePosWorld(), position.pos, transform.scale.x, transform.scale.y))
			{
				//if collide, set curent menu button to collided button
				if (objecttype.objType == ObjectType::LEVEL1)
				{
					selectLevel = LEVEL1;

					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (objecttype.objType == ObjectType::LEVEL2)
				{
					selectLevel = LEVEL2;
				
					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (objecttype.objType == ObjectType::LEVEL3)
				{
					selectLevel = LEVEL3;
					
					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (objecttype.objType == ObjectType::LEVEL4)
				{
					selectLevel = LEVEL4;

					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (objecttype.objType == ObjectType::LEVEL5)
				{
					selectLevel = LEVEL5;
					
					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (objecttype.objType == ObjectType::LEVEL_BACK)
				{
					selectLevel = LEVEL_BACK;
				
					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				//if collide, change texture
				SetChangeTexture(false, entity);

				//if mouse click, set mouse click to true
				if (input.CheckPress(GLFW_MOUSE_BUTTON_LEFT))
					click = true;
				else
					click = false;
			}
			else
			{
				//if keyboard current selection, change texture
				if (selectLevel == LEVEL1 && objecttype.objType == ObjectType::LEVEL1)
				{
					SetChangeTexture(false, entity);
					input.GetInputBooleen().hoverPlayed = FALSE;
				}
				else if (selectLevel == LEVEL2 && objecttype.objType == ObjectType::LEVEL2)
				{
					SetChangeTexture(false, entity);
					input.GetInputBooleen().hoverPlayed = FALSE;
				}
				else if (selectLevel == LEVEL3 && objecttype.objType == ObjectType::LEVEL3)
				{
					SetChangeTexture(false, entity);
					input.GetInputBooleen().hoverPlayed = FALSE;
				}
				else if (selectLevel == LEVEL4 && objecttype.objType == ObjectType::LEVEL4)
				{
					SetChangeTexture(false, entity);
					input.GetInputBooleen().hoverPlayed = FALSE;
				}
				else if (selectLevel == LEVEL5 && objecttype.objType == ObjectType::LEVEL5)
				{
					SetChangeTexture(false, entity);
					input.GetInputBooleen().hoverPlayed = FALSE;
				}
				else if (selectLevel == LEVEL_BACK && objecttype.objType == ObjectType::LEVEL_BACK)
				{
					SetChangeTexture(false, entity);
					input.GetInputBooleen().hoverPlayed = FALSE;
				}
				else
					SetChangeTexture(true, entity);
			}
		}
	}

	//back to main menu if press escape button
	if (input.CheckPress(GLFW_KEY_ESCAPE))
	{
		selectLevel = LEVEL1;
		input.GetInputBooleen().enterLevelSelect = false;
		input.GetInputBooleen().enterMain = true;
	}

	//Keyboard input, press up or down to select menu
	if (input.CheckPress(GLFW_KEY_DOWN) && selectLevel < LEVEL_BACK)
	{
		++selectLevel;
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
	}
	else if (input.CheckPress(GLFW_KEY_UP) && selectLevel > LEVEL1)
	{
		--selectLevel;
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
	}

	//If user presses enter or mouse click
	if (input.CheckPress(GLFW_KEY_SPACE) || click)
	{
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_CLICK), FMOD_LOOP_OFF, gAudioEngine->volume);

		switch (selectLevel)
		{
			// enter level 1
		case LEVEL1:
			_gameStateNext = GS_STATES::GS_STAGE1;
			gFakeEngine->GetGraphicManager<StageChanger>().InitStage(StagesEnum::ACTUAL_STAGE);
			break;
		case LEVEL2:
			_gameStateNext = GS_STATES::GS_STAGE1;
			gFakeEngine->GetGraphicManager<StageChanger>().InitStage(StagesEnum::LEVEL2);
			break;

		case LEVEL3:
			_gameStateNext = GS_STATES::GS_STAGE1;
			gFakeEngine->GetGraphicManager<StageChanger>().InitStage(StagesEnum::LEVEL3);
			break;

		case LEVEL4:
			_gameStateNext = GS_STATES::GS_STAGE1;
			gFakeEngine->GetGraphicManager<StageChanger>().InitStage(StagesEnum::LEVEL4);
			break;

		case LEVEL5:
			_gameStateNext = GS_STATES::GS_STAGE1;
			gFakeEngine->GetGraphicManager<StageChanger>().InitStage(StagesEnum::LEVEL5);
			break;

			// back to main menu
		case LEVEL_BACK:
			selectLevel = LEVEL1;
			input.GetInputBooleen().enterMain = true;
			input.GetInputBooleen().enterLevelSelect = false;
			break;
		}
	}
}

void MainMenuInputSystem::controlMenu()
{
	static int pageNumber = HOW_TO_PLAY_1;
	static int selectControl = CONTROL_NEXT;
	bool click = false;

	Input& input = gFakeEngine->GetGraphicManager<Input>();
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();

	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
		Position& position = mGameECSmanager.GetComponent<Position>(entity);

		//set objects to visible
		if (pageNumber == HOW_TO_PLAY_1)
		{
			if (objecttype.objType == ObjectType::CONTROL_MENU_BG_1 ||
				objecttype.objType == ObjectType::CONTROL_NEXT ||
				objecttype.objType == ObjectType::CONTROL_BACK ||
				objecttype.objType == ObjectType::MOUSE_CURSOR)
				flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
			else
				flag.activeFlag = FLAG_INACTIVE;
		}
		else if (pageNumber == HOW_TO_PLAY_2)
		{
			selectControl = CONTROL_NEXT;

			if (objecttype.objType == ObjectType::CONTROL_MENU_BG_2 ||
				objecttype.objType == ObjectType::CONTROL_NEXT ||
				objecttype.objType == ObjectType::CONTROL_BACK ||
				objecttype.objType == ObjectType::MOUSE_CURSOR)
				flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
			else
				flag.activeFlag = FLAG_INACTIVE;
		}
		else if (pageNumber == HOW_TO_PLAY_3)
		{
			selectControl = CONTROL_BACK;

			if (objecttype.objType == ObjectType::CONTROL_MENU_BG_3 ||
				objecttype.objType == ObjectType::CONTROL_BACK ||
				objecttype.objType == ObjectType::MOUSE_CURSOR)
				flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
			else
				flag.activeFlag = FLAG_INACTIVE;
		}

		if (objecttype.objType == ObjectType::CONTROL_NEXT ||
			objecttype.objType == ObjectType::CONTROL_BACK)
		{
			if (Collision::OBB::StaticPointToStaticRect(input.GetMousePosWorld(), position.pos, transform.scale.x, transform.scale.y))
			{
				if (pageNumber == HOW_TO_PLAY_3 && objecttype.objType == ObjectType::CONTROL_NEXT)
					continue;

				//if collide, set curent menu button to collided button
				if (objecttype.objType == ObjectType::CONTROL_NEXT)
				{
					selectControl = CONTROL_NEXT;
				
					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (objecttype.objType == ObjectType::CONTROL_BACK)
				{
					selectControl = CONTROL_BACK;
					
					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}

				//if collide, change texture
				SetChangeTexture(false, entity);

				//if mouse click, set mouse click to true
				if (input.CheckPress(GLFW_MOUSE_BUTTON_LEFT))
					click = true;
				else
					click = false;
			}
			else
			{
				//if keyboard current selection, change texture
				if (selectControl == CONTROL_NEXT && objecttype.objType == ObjectType::CONTROL_NEXT)
				{
					SetChangeTexture(false, entity);
					input.GetInputBooleen().hoverPlayed = FALSE;
				}
				else if (selectControl == CONTROL_BACK && objecttype.objType == ObjectType::CONTROL_BACK)
				{
					SetChangeTexture(false, entity);
					input.GetInputBooleen().hoverPlayed = FALSE;
				}
				else
					SetChangeTexture(true, entity);
			}
		}
	}

	//return to previous page if press escape button
	if (input.CheckPress(GLFW_KEY_ESCAPE))
	{
		pageNumber = HOW_TO_PLAY_1;
		selectControl = CONTROL_NEXT;
		input.GetInputBooleen().enterControls = false;
		input.GetInputBooleen().enterMain = true;
	}

	//Keyboard input, press up or down to select menu
	if (input.CheckPress(GLFW_KEY_DOWN) && selectControl < CONTROL_BACK)
	{
		++selectControl;
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
	}
	else if (input.CheckPress(GLFW_KEY_UP) && selectControl > CONTROL_NEXT)
	{
		--selectControl;
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
	}

	//If user presses enter or mouse click
	if (input.CheckPress(GLFW_KEY_SPACE) || click)
	{
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_CLICK), FMOD_LOOP_OFF, gAudioEngine->volume);

		switch (selectControl)
		{
			//Go to next control page
		case CONTROL_NEXT:
			++pageNumber;
			break;

			//return to previous control page
		case CONTROL_BACK:
			pageNumber = HOW_TO_PLAY_1;
			selectControl = CONTROL_NEXT;
			input.GetInputBooleen().enterControls = false;
			input.GetInputBooleen().enterMain = true;
			break;
		}
	}
}

void MainMenuInputSystem::optionMenu()
{
	static int selectOption = OPTION_1920x1080;
	static bool fullscreen = false;
	static bool r1920_1080 = true;
	bool click = false;

	Input& input = gFakeEngine->GetGraphicManager<Input>();
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();

	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
		Position& position = mGameECSmanager.GetComponent<Position>(entity);

		//set objects to visible
		if (objecttype.objType == ObjectType::OPTION_MENU_BG ||
			objecttype.objType == ObjectType::OPTION_1920x1080 ||
			objecttype.objType == ObjectType::FULLSCREEN ||
			objecttype.objType == ObjectType::BGM ||
			objecttype.objType == ObjectType::OPTION_BACK ||
			objecttype.objType == ObjectType::MOUSE_CURSOR)
			flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
		else
			flag.activeFlag = FLAG_INACTIVE;

		if (objecttype.objType == ObjectType::OPTION_1920x1080 ||
			objecttype.objType == ObjectType::FULLSCREEN ||
			objecttype.objType == ObjectType::BGM ||
			objecttype.objType == ObjectType::OPTION_BACK)
		{
			if (Collision::OBB::StaticPointToStaticRect(input.GetMousePosWorld(), position.pos, transform.scale.x, transform.scale.y))
			{
				//if collide, set curent menu button to collided button
				if (objecttype.objType == ObjectType::OPTION_1920x1080)
				{
					selectOption = OPTION_1920x1080;

					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (objecttype.objType == ObjectType::FULLSCREEN)
				{
					selectOption = FULLSCREEN;

					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (objecttype.objType == ObjectType::BGM)
				{
					selectOption = BGM;

					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (objecttype.objType == ObjectType::OPTION_BACK)
				{
					selectOption = OPTION_BACK;

					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}

				//if mouse click, set mouse click to true
				if (input.CheckPress(GLFW_MOUSE_BUTTON_LEFT))
					click = true;
				else
					click = false;

				//if collide, change texture
				if (objecttype.objType == ObjectType::OPTION_BACK)
					SetChangeTexture(false, entity);
			}
			else
			{
				//if keyboard current selection, change texture
				if (selectOption == OPTION_1920x1080 && objecttype.objType == ObjectType::OPTION_1920x1080)
					input.GetInputBooleen().hoverPlayed = FALSE;
				else if (selectOption == FULLSCREEN && objecttype.objType == ObjectType::FULLSCREEN)
					input.GetInputBooleen().hoverPlayed = FALSE;
				else if (selectOption == BGM && objecttype.objType == ObjectType::BGM)
					input.GetInputBooleen().hoverPlayed = FALSE;
				else if (selectOption == OPTION_BACK && objecttype.objType == ObjectType::OPTION_BACK)
				{
					SetChangeTexture(false, entity);
					input.GetInputBooleen().hoverPlayed = FALSE;
				}
				else
				{
					if (objecttype.objType == ObjectType::OPTION_BACK)
						SetChangeTexture(true, entity);
				}
			}
		}
	}

	//back to main menu if press escape button
	if (input.CheckPress(GLFW_KEY_ESCAPE))
	{
		selectOption = FULLSCREEN;
		input.GetInputBooleen().enterOption = false;
		input.GetInputBooleen().enterMain = true;
	}

	//Keyboard input, press up or down to select menu
	if (input.CheckPress(GLFW_KEY_DOWN) && selectOption < OPTION_BACK)
	{
		++selectOption;
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);

	}
	else if (input.CheckPress(GLFW_KEY_UP) && selectOption > OPTION_1920x1080)
	{
		--selectOption;
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
	}

	//If user presses enter or mouse click
	if (input.CheckPress(GLFW_KEY_SPACE) || click)
	{
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_CLICK), FMOD_LOOP_OFF, gAudioEngine->volume);

		Window& window = gFakeEngine->GetGraphicManager<Window>();

		switch (selectOption)
		{
		case OPTION_1920x1080:
			for (auto const& entity : mEntities)
			{
				EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);

				if (objecttype.objType == ObjectType::OPTION_1920x1080)
				{
					if (r1920_1080)
					{
						window.SetWindowResolution(800, 600);
						SetChangeTexture(false, entity);
						r1920_1080 = false;
					}
					else
					{
						window.SetWindowResolution(1920, 1080);
						SetChangeTexture(true, entity);
						r1920_1080 = true;
					}
				}
			}
			break;
	
			//change window mode
		case FULLSCREEN:
			for (auto const& entity : mEntities)
			{
				EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);

				if (objecttype.objType == ObjectType::FULLSCREEN)
				{
					if (window.IsFullScreen())
					{
						window.SetWindowFullScreen(false);
						SetChangeTexture(false, entity);
					}
					else
					{
						window.SetWindowFullScreen(true);
						SetChangeTexture(true, entity);
					}
				}
			}
			break;

			//set audio to mute or unmute
		case BGM:
			for (auto const& entity : mEntities)
			{
				EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);

				if (objecttype.objType == ObjectType::BGM)
				{
					if (gAudioEngine->mute)
						SetChangeTexture(true, entity);
					else
						SetChangeTexture(false, entity);

					gAudioEngine->mute = !gAudioEngine->mute;
					gAudioEngine->setMute(gAudioEngine->mute);
				}
			}
			break;

			//back to main menu if select back button
		case OPTION_BACK:
			input.GetInputBooleen().enterMain = true;
			input.GetInputBooleen().enterOption = false;
			break;
		}
	}
}

void MainMenuInputSystem::creditMenu()
{
	static int pageNumber = CREDIT_1;
	static int selectCredit = CREDIT_NEXT;
	bool click = false;

	Input& input = gFakeEngine->GetGraphicManager<Input>();
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();

	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
		Position& position = mGameECSmanager.GetComponent<Position>(entity);

		//set objects to visible
		if (pageNumber == CREDIT_1)
		{
			if (objecttype.objType == ObjectType::CREDIT_MENU_BG_1 ||
				objecttype.objType == ObjectType::CREDIT_NEXT ||
				objecttype.objType == ObjectType::CREDIT_BACK ||
				objecttype.objType == ObjectType::MOUSE_CURSOR)
				flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
			else
				flag.activeFlag = FLAG_INACTIVE;
		}
		else if (pageNumber == CREDIT_2)
		{
			if (objecttype.objType == ObjectType::CREDIT_MENU_BG_2 ||
				objecttype.objType == ObjectType::CREDIT_NEXT ||
				objecttype.objType == ObjectType::CREDIT_BACK ||
				objecttype.objType == ObjectType::MOUSE_CURSOR)
				flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
			else
				flag.activeFlag = FLAG_INACTIVE;
		}
		else if (pageNumber == CREDIT_3)
		{
			if (objecttype.objType == ObjectType::CREDIT_MENU_BG_3 ||
				objecttype.objType == ObjectType::CREDIT_NEXT ||
				objecttype.objType == ObjectType::CREDIT_BACK ||
				objecttype.objType == ObjectType::MOUSE_CURSOR)
				flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
			else
				flag.activeFlag = FLAG_INACTIVE;
		}
		else if (pageNumber == CREDIT_4)
		{
			if (objecttype.objType == ObjectType::CREDIT_MENU_BG_4 ||
				objecttype.objType == ObjectType::CREDIT_NEXT ||
				objecttype.objType == ObjectType::CREDIT_BACK ||
				objecttype.objType == ObjectType::MOUSE_CURSOR)
				flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
			else
				flag.activeFlag = FLAG_INACTIVE;
		}
		else if (pageNumber == CREDIT_5)
		{
			if (objecttype.objType == ObjectType::CREDIT_MENU_BG_5 ||
				objecttype.objType == ObjectType::CREDIT_NEXT ||
				objecttype.objType == ObjectType::CREDIT_BACK ||
				objecttype.objType == ObjectType::MOUSE_CURSOR)
				flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
			else
				flag.activeFlag = FLAG_INACTIVE;
		}
		else if (pageNumber == CREDIT_6)
		{
			selectCredit = CREDIT_BACK;

			if (objecttype.objType == ObjectType::CREDIT_MENU_BG_6 ||
				objecttype.objType == ObjectType::CREDIT_BACK ||
				objecttype.objType == ObjectType::MOUSE_CURSOR)
				flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
			else
				flag.activeFlag = FLAG_INACTIVE;
		}

		if (objecttype.objType == ObjectType::CREDIT_NEXT ||
			objecttype.objType == ObjectType::CREDIT_BACK)
		{
			if (Collision::OBB::StaticPointToStaticRect(input.GetMousePosWorld(), position.pos, transform.scale.x, transform.scale.y))
			{
				if (pageNumber == CREDIT_6 && objecttype.objType == ObjectType::CREDIT_NEXT)
					continue;

				//if collide, set curent menu button to collided button
				if (objecttype.objType == ObjectType::CREDIT_NEXT)
				{
					selectCredit = CREDIT_NEXT;
				
					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (objecttype.objType == ObjectType::CREDIT_BACK)
				{
					selectCredit = CREDIT_BACK;

					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				//if mouse click, set mouse click to true
				if (input.CheckPress(GLFW_MOUSE_BUTTON_LEFT))
					click = true;
				else
					click = false;

				//if collide, change texture
				SetChangeTexture(false, entity);
			}
			//if keyboard current selection, change texture
			else if (selectCredit == CREDIT_NEXT && objecttype.objType == ObjectType::CREDIT_NEXT)
			{
				SetChangeTexture(false, entity);
				input.GetInputBooleen().hoverPlayed = FALSE;
			}
			else if (selectCredit == CREDIT_BACK && objecttype.objType == ObjectType::CREDIT_BACK)
			{
				SetChangeTexture(false, entity);
				input.GetInputBooleen().hoverPlayed = FALSE;
			}
			else
				SetChangeTexture(true, entity);
		}
	}

	//return to previous page if press escape button
	if (input.CheckPress(GLFW_KEY_ESCAPE))
	{
		pageNumber = CREDIT_1;
		selectCredit = CONTROL_NEXT;
		input.GetInputBooleen().enterCredits = false;
		input.GetInputBooleen().enterMain = true;
	}

	//Keyboard input, press up or down to select menu
	if (input.CheckPress(GLFW_KEY_DOWN) && selectCredit < CREDIT_BACK)
	{
		++selectCredit;
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
	}
	else if (input.CheckPress(GLFW_KEY_UP) && selectCredit > CREDIT_NEXT)
	{
		--selectCredit;
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
	}

	//If user presses enter or mouse click
	if (input.CheckPress(GLFW_KEY_SPACE) || click)
	{
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_CLICK), FMOD_LOOP_OFF, gAudioEngine->volume);

		switch (selectCredit)
		{
			//go to next credit page
		case CREDIT_NEXT:
			++pageNumber;
			break;

			//go to previous credit page
		case CREDIT_BACK:
			pageNumber = CREDIT_1;
			selectCredit = CONTROL_NEXT;

			input.GetInputBooleen().enterCredits = false;
			input.GetInputBooleen().enterMain = true;
			break;
		}
	}
}

void MainMenuInputSystem::quitMenu()
{
	static int selectQuit = YES;
	bool click = false;

	Input& input = gFakeEngine->GetGraphicManager<Input>();
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();

	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
		Position& position = mGameECSmanager.GetComponent<Position>(entity);

		//set objects to visible
		if (objecttype.objType == ObjectType::QUIT_MENU_BG ||
			objecttype.objType == ObjectType::QUIT_YES ||
			objecttype.objType == ObjectType::QUIT_NO ||
			objecttype.objType == ObjectType::MOUSE_CURSOR)
			flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
		else
			flag.activeFlag = FLAG_INACTIVE;

		if (objecttype.objType == ObjectType::QUIT_YES ||
			objecttype.objType == ObjectType::QUIT_NO)
		{
			if (Collision::OBB::StaticPointToStaticRect(input.GetMousePosWorld(), position.pos, transform.scale.x, transform.scale.y))
			{
				//if collide, set curent menu button to collided button
				if (objecttype.objType == ObjectType::QUIT_YES)
				{
					selectQuit = YES;
					
					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (objecttype.objType == ObjectType::QUIT_NO)
				{
					selectQuit = NO;

					if (input.GetInputBooleen().hoverPlayed == FALSE)
					{
						input.GetInputBooleen().hoverPlayed = TRUE;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				//if mouse click, set mouse click to true
				if (input.CheckPress(GLFW_MOUSE_BUTTON_LEFT))
					click = true;
				else
					click = false;

				//if collide, change texture
				SetChangeTexture(false, entity);
			}
			//if keyboard current selection, change texture
			else if (selectQuit == YES && objecttype.objType == ObjectType::QUIT_YES)
			{
				SetChangeTexture(false, entity);
				input.GetInputBooleen().hoverPlayed = FALSE;
			}
			else if (selectQuit == NO && objecttype.objType == ObjectType::QUIT_NO)
			{
				SetChangeTexture(false, entity);
				input.GetInputBooleen().hoverPlayed = FALSE;
			}
			else
				SetChangeTexture(true, entity);
		}
	}

	//back to main menu if press escape button
	if (input.CheckPress(GLFW_KEY_ESCAPE))
	{
		selectQuit = NO;
		input.GetInputBooleen().enterQuit = false;
		input.GetInputBooleen().enterMain = true;
	}

	//Keyboard input, press up or down to select menu
	if (input.CheckPress(GLFW_KEY_DOWN) && selectQuit < NO)
	{
		++selectQuit;
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
	}
	else if (input.CheckPress(GLFW_KEY_UP) && selectQuit > YES)
	{
		--selectQuit;
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_HOVER), FMOD_LOOP_OFF, gAudioEngine->volume);
	}

	//If user presses enter or mouse click
	if (input.CheckPress(GLFW_KEY_SPACE) || click)
	{
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::MOUSE_CLICK), FMOD_LOOP_OFF, gAudioEngine->volume);

		switch (selectQuit)
		{
			//Quit game
		case YES:
			_gameStateNext = GS_STATES::GS_QUIT;
			break;

			//Return to main menu
		case NO:
			selectQuit = YES;
			input.GetInputBooleen().enterQuit = false;
			input.GetInputBooleen().enterMain = true;
			break;
		}
	}
}

void MainMenuInputSystem::SetChangeTexture(bool check, Entity entity)
{
	//GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
	for (auto const& x : stageChanger.allobjects)
	{
		if (x._object.second.objType == objecttype.objType)
		{
			if (check)
				//based on the texture in the object file
				mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[0].texture;
			else
				//change texture to second
				mGameECSmanager.GetComponent<RenderTex>(entity).texture = x._texture.second[1].texture;
		}

	}
}