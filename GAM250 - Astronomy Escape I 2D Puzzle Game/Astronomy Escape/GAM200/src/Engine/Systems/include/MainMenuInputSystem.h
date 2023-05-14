/*****************************************************************
*\file		 MainMenuInputSystem.h
*\brief		 Contains declaration for all main menu states

*\author(s)   Goh Wei Zhe	72 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#ifndef MAIN_MENU_INPUT_SYSTEM_H
#define MAIN_MENU_INPUT_SYSTEM_H

#include <EnginePch.h>

class MainMenuInputSystem : public System
{
public:
	void Init();
	void Update();

	void mainMenu();
	void levelSelectMenu();
	void controlMenu();
	void optionMenu();
	void creditMenu();
	void quitMenu();

	void SetChangeTexture(bool check, Entity entity);
};

enum MAIN_MENU
{
	PLAY = 0,
	CONTROLS,
	OPTIONS,
	CREDITS,
	QUIT
};

enum LEVELSELECT_MENU
{
	LEVEL1 = 0,
	LEVEL2,
	LEVEL3,
	LEVEL4,
	LEVEL5,
	LEVEL_BACK
};

enum OPTION_MENU
{
	OPTION_1920x1080 = 0,
	OPTION_800x600,
	FULLSCREEN,
	BGM,
	OPTION_BACK
};

enum CONTROL_MENU
{
	CONTROL_NEXT = 0,
	CONTROL_BACK
};

enum CONTROL_MENU_BG
{
	HOW_TO_PLAY_1 = 0,
	HOW_TO_PLAY_2,
	HOW_TO_PLAY_3,
};

enum CREDIT_MENU
{
	CREDIT_NEXT = 0,
	CREDIT_BACK
};

enum CREDIT_MENU_BG
{
	CREDIT_1 = 0,
	CREDIT_2,
	CREDIT_3,
	CREDIT_4,
	CREDIT_5,
	CREDIT_6
};

enum QUIT_MENU
{
	YES = 0,
	NO
};

#endif // MAIN_MENU_INPUT_SYSTEM_H
