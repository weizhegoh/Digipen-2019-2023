/*****************************************************************
*\file		 StageChanger.h
*\brief		 This file contains particle helper functions

*\author(s)   Lee Liang Ping	23 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <Object.h>

class StageChanger
{
public:
	using LevelFileLocations = std::pair<std::string, std::string>;
	void LoadStage(StagesEnum stage);
	void LoadStage();
	void Reset();
	
	void StageUpdate();
	void ChangeState(StagesEnum stage);
	void InitStage(StagesEnum stage);
	void NextState();

	std::vector<Object> allobjects;
	LevelFileLocations GetFileLocation(StagesEnum& stage);
	void Print();
	StagesEnum& GetCurrentStage(){ return currStage; }
	StagesEnum& GetNextStage(){ return nextStage; }


private:
	StagesEnum currStage;
	StagesEnum nextStage;
	



};