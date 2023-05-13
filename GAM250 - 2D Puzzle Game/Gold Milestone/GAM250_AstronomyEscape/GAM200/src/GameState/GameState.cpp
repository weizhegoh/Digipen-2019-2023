/*****************************************************************
*\file		 GameState.cpp
*\brief		 GameState functions definitions.

*\author(s)   Kenric Tan Wei Liang	9 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include "../GameState/GameState.h"


void GameStatesInitialize()
{
	GSM_AddState(GS_MAINMENU, MainMenu_Load, MainMenu_Initialize, MainMenu_Update, MainMenu_Draw, MainMenu_Free, MainMenu_Unload);
	GSM_AddState(GS_STAGE1, Stage1_Load, Stage1_Initialize, Stage1_Update, Stage1_Draw, Stage1_Free, Stage1_Unload );
	GSM_AddState(GS_LEVELEDIT, LevelEdit_Load, LevelEdit_Initialize, LevelEdit_Update, LevelEdit_Draw, LevelEdit_Free, LevelEdit_Unload);
}