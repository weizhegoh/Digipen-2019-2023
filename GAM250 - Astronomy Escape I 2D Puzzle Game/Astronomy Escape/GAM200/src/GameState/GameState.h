/*****************************************************************
*\file		 GameState.h
*\brief		 Header file declarations for HotSpotCollisionSystem.

*\author(s)   Kenric Tan Wei Liang	 34 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include "../GameState/GameStateManager.h"


//list of states
enum GS_STATES
{
	GS_MAINMENU = 1,
	GS_STAGE1,
	GS_STAGE2,
	//
	GS_STAGE3,
	GS_STAGE4,
	GS_STAGE5,
	//GS_TUTORIAL,
	//
	GS_LEVELEDIT,
	GS_RESTART,
	GS_QUIT,
	GS_TESTSTATE
};

void MainMenu_Load();
void MainMenu_Initialize();
void MainMenu_Update();
void MainMenu_Draw();
void MainMenu_Free();
void MainMenu_Unload();

void GameStatesInitialize();

void Stage1_Load();
void Stage1_Initialize();
void Stage1_Update();
void Stage1_Draw();
void Stage1_Free();
void Stage1_Unload();

void LevelEdit_Load();
void LevelEdit_Initialize();
void LevelEdit_Update();
void LevelEdit_Draw();
void LevelEdit_Free();
void LevelEdit_Unload();
