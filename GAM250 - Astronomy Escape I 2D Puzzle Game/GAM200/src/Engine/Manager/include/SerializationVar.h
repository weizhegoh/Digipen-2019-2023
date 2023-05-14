/*****************************************************************
*\file		 SerializationVar.h
*\brief		 Header file declarations for SerializationVar.

*\author(s)   Kenric Tan Wei Liang	49 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <GameTypes.h>
#include <Object.h>

struct SerializationVar
{
	std::map<int, std::pair<ObjectType, std::string>> objData =
	{
		{0, {ObjectType::FLOOR , "FLOOR"}},
		{1, {ObjectType::MAIN_CHARACTER	, "MAIN_CHARACTER"}},
		{2, {ObjectType::WALL , "WALL"}},
		{3, {ObjectType::PUSHABLE_BARREL , "PUSHABLE_BARREL"}},
		{4, {ObjectType::KEY_OBJECTIVE , "KEY_OBJECTIVE"}},
		{5, {ObjectType::MONSTER , "MONSTER"}},
		{6, {ObjectType::TURRET, "TURRET"}},
		{7, {ObjectType::ENTRANCE_DOOR, "ENTRANCE_DOOR"}},
		{8, {ObjectType::EXIT_DOOR, "EXIT_DOOR"}},
		{9, {ObjectType::LEVEL_DOOR, "LEVEL_DOOR"}},
		{10, {ObjectType::MONSTER_PATROL , "MONSTER_PATROL"}},
		{11, {ObjectType::MONSTER_GUIDE , "MONSTER_GUIDE"}},
		{12, {ObjectType::COLLECTIBLES, "COLLECTIBLES"}},
		{13, {ObjectType::LEVER_DOOR,"LEVER_DOOR"}},
		{14, {ObjectType::LEVER_SWITCH, "LEVER_SWITCH"}},
		{15, {ObjectType::TELEPORTER, "TELEPORTER"}},
		{16, {ObjectType::TUTORIAL_FLOOR, "TUTORIAL_FLOOR"}}
	};

	std::map<int, std::pair<ObjectDirection, std::string>> dirData =
	{
		{0, {ObjectDirection::NO_DIR , "NO_DIRECTION"}},
		{1, {ObjectDirection::LEFT	, "LEFT"}},
		{2, {ObjectDirection::RIGHT , "RIGHT"}},
		{3, {ObjectDirection::UP , "UP"}},
		{4, {ObjectDirection::DOWN , "DOWN"}}
	};


	int teleporterCount = 0;

	//tutorial funcs
	std::map<Entity, RenderTex> tutorialMap;
	bool activateFlag;
	float tutorialTime;
	bool afterPopup;
	bool duringTutorial = false;
	
	bool press;
	int caseId = 0;

	Entity tutorialEntity;
	Entity boxEntity;
	Object instructObject;
	Entity instructEntity;

	float speed;
	bool audiocheck{ false };
};
