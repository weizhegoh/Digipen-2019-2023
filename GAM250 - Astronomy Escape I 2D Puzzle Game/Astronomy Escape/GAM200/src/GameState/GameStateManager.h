/*****************************************************************
*\file		 GameStateManager.h
*\brief		 Header file declarations for GameStateManager.

*\author(s)   Kenric Tan Wei Liang	38 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#ifndef GAMESTATE_MANAGER
#define GLAPP_H
#include <vector>
#include "../GameState/GameState.h"
#include <FakeEngine.h>
#include <AudioEngine.h>


typedef void (*fp)();

//function for game state
extern fp GS_Load;
extern fp GS_Init;
extern fp GS_Update;
extern fp GS_Draw;
extern fp GS_Free;
extern fp GS_Unload;

//variables to store game staate
extern unsigned _gameStateInit;
extern unsigned _gameStateCurrent;
extern unsigned _gameStatePrevious;
extern unsigned _gameStateNext;


struct GameState
{

	GameState( unsigned GS_id, fp GS_load, fp GS_init, fp GS_update, fp GS_draw, fp GS_free, fp GS_unload)
	: _GS_id{ GS_id }, _GS_load{ GS_load }, _GS_init{ GS_init }, _GS_update{ GS_update } , 
		_GS_draw{ GS_draw }, _GS_free{ GS_free }, _GS_unload{ GS_unload }
	{}

	unsigned _GS_id;
	fp _GS_load;
	fp _GS_init;
	fp _GS_update;
	fp _GS_draw;
	fp _GS_free;
	fp _GS_unload;
};

static std::vector<GameState> gameStateList;


void GSM_Loop();
void GSM_Initialize(unsigned gameStateInit);
void GSM_ChangeState();
void GSM_AddState(unsigned GS_id, fp GS_load, fp GS_init, fp GS_update, fp GS_draw, fp GS_free, fp GS_unload);


#endif