/*****************************************************************
*\file		 GameStateManager.cpp
*\brief		 GameStateManager functions definitions.

*\author(s)   Kenric Tan Wei Liang	85 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include "GameStateManager.h"


fp GS_Load;
fp GS_Init;
fp GS_Update;
fp GS_Draw;
fp GS_Free;
fp GS_Unload;


unsigned _gameStateInit;
unsigned _gameStateCurrent;
unsigned _gameStatePrevious;
unsigned _gameStateNext;

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;

void GSM_Initialize(unsigned gameStateInit)
{
	_gameStateInit = _gameStateCurrent = _gameStatePrevious = _gameStateNext = gameStateInit;

}

void GSM_Loop()
{
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	FPS& fps = gFakeEngine->GetGraphicManager<FPS>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	while (_gameStateCurrent != GS_QUIT )
	{
		if (_gameStateCurrent == GS_RESTART)
		{
			_gameStateCurrent = _gameStatePrevious;
			_gameStateNext = _gameStatePrevious;
		}
		else
		{
			//load new game state
			GSM_ChangeState();
			GS_Load();
		}

		GS_Init();
		while (_gameStateCurrent == _gameStateNext)
		{
			if (glfwWindowShouldClose(window.GetWindow()))
				_gameStateNext = GS_QUIT;

			input.ResetKeyPressed();
			glfwPollEvents();
			fps.Update();
			//check for input , update and render
			GS_Update();
			GS_Draw();
			gAudioEngine->Update();
			glfwSwapBuffers(window.GetWindow());
		}
		GS_Free();

		if (_gameStateNext != GS_RESTART)
			GS_Unload();

		_gameStatePrevious = _gameStateCurrent;
		_gameStateCurrent = _gameStateNext;
		std::cout << _gameStateCurrent << std::endl;
	}
	std::cout << _gameStateCurrent << std::endl;
}


void GSM_ChangeState()
{
	for (auto const& x : gameStateList)
	{
		if (_gameStateCurrent == x._GS_id)
		{
			
			GS_Load = x._GS_load;
			GS_Init = x._GS_init;
			GS_Update = x._GS_update;
			GS_Draw = x._GS_draw;
			GS_Free = x._GS_free;
			GS_Unload = x._GS_unload;
			
			break;
		}
	}
}

void GSM_AddState(unsigned GS_id, fp GS_load, fp GS_init, fp GS_update, fp GS_draw, fp GS_free, fp GS_unload)
{
	//can add check for max game state

	//check whether the gamestate exsist
	for (auto const& x : gameStateList)
	{
		if (GS_id == x._GS_id)
		{
			return;
		}
	}
	
	GameState gamestate(GS_id, GS_load, GS_init, GS_update, GS_draw, GS_free, GS_unload);
	gameStateList.push_back(gamestate);

}