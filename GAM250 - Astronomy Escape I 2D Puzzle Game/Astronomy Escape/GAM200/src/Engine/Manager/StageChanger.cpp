/*****************************************************************
*\file		 ParticlesInfo.cpp
*\brief		 This file contains particle helper functions

*\author(s)   Lee Liang Ping	123 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/


#include <EnginePch.h>
#include  <FakeEngine.h>
#include "../GameState/GameState.h"
#include "../Engine/Serialization/Serialization.h"

extern std::unique_ptr<FakeEngine> gFakeEngine;

void StageChanger::LoadStage(StagesEnum stage)
{
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	Serialization::testObjData(allobjects);
	LevelFileLocations floc = GetFileLocation(stage);
	if (!floc.first.empty())
	{
		Serialization::LoadBoard(floc.first);
		Serialization::AssignBoard(globalVar.data, allobjects);
	}
	
	if (!floc.second.empty())
 		Serialization::creation_of_gameplay_stuff(floc.second, allobjects);
	
	globalVar.printMapData();
	Print();
}

void StageChanger::LoadStage()
{
	LoadStage(currStage);
}


void StageChanger::Reset()
{
	allobjects.clear();
}

StageChanger::LevelFileLocations StageChanger::GetFileLocation(StagesEnum& stage)
{
	//first = board , second = level objects
	LevelFileLocations ret;
	switch (stage)
	{
	case StagesEnum::TUTORIAL_1:
		ret.first = "Assets/Serialization/Board/board1.txt";
		ret.second = "Assets/Serialization/level1.txt";
		break;
	case StagesEnum::TUTORIAL_2:
		ret.first = "Assets/Serialization/Board/board1.txt";
		ret.second = "Assets/Serialization/level1.txt";
		break;
	case StagesEnum::ACTUAL_STAGE:
		ret.first = "Assets/Serialization/Board/board1.txt";
		ret.second = "Assets/Serialization/level1.txt";
		break;
	case StagesEnum::LEVEL2:
		ret.first = "Assets/Serialization/Board/board2.txt";
		ret.second = "Assets/Serialization/level1.txt";
		break;

	case StagesEnum::LEVEL3:
		ret.first = "Assets/Serialization/Board/board3.txt";
		ret.second = "Assets/Serialization/level1.txt";
		break;

	case StagesEnum::LEVEL4:
		ret.first = "Assets/Serialization/Board/board4.txt";
		ret.second = "Assets/Serialization/level1.txt";
		break;

	case StagesEnum::LEVEL5:
		ret.first = "Assets/Serialization/Board/board5.txt";
		ret.second = "Assets/Serialization/level1.txt";
		break;

	case StagesEnum::MAIN_MENU:
		ret.first = "";
		ret.second = "Assets/Serialization/main_menu.txt";
		break;
	default:
		break;
	}
	return ret;
}

void StageChanger::StageUpdate()
{
	if (currStage != nextStage)
	{
		currStage = nextStage;
		_gameStateNext = GS_RESTART;
	}

}

void StageChanger::ChangeState(StagesEnum stage)
{
	nextStage = stage;
}

void StageChanger::NextState()
{
	switch (currStage)
	{
	case StagesEnum::TUTORIAL_1:
		nextStage = StagesEnum::TUTORIAL_2;
		break;
	case StagesEnum::TUTORIAL_2:
		nextStage = StagesEnum::ACTUAL_STAGE;
		break;
	case StagesEnum::ACTUAL_STAGE:
		nextStage = StagesEnum::LEVEL2;
		break;
	case StagesEnum::LEVEL2:
		nextStage = StagesEnum::LEVEL3;
		break;
	case StagesEnum::LEVEL3:
		nextStage = StagesEnum::LEVEL4;
		break;
		
	case StagesEnum::LEVEL4:
		nextStage = StagesEnum::LEVEL5;
		break;
	case StagesEnum::LEVEL5:
		gFakeEngine->PublishEvent(new SetWinScreen(true, 1));
		break;
	case StagesEnum::WIN:
		gFakeEngine->GetGraphicManager<Input>().GetInputBooleen().win_state = true;
		break;
	default:
		break;
	}
}



void StageChanger::InitStage(StagesEnum stage)
{
	nextStage = currStage = stage;
}

void  StageChanger::Print()
{
	std::cout << "currStage: " << EnumToString::Get(currStage) << std::endl;
	std::cout << "nextStage: " <<EnumToString::Get(nextStage) << std::endl;

}