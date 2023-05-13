/*****************************************************************
*\file		 LevelEditSystem.h
*\brief		 Header file declarations for LevelEditSystem.

*\author(s)   Kenric Tan Wei Liang	14 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

enum class SaveBoard_MODE
{
	SAVE_ONLY_FLOORS,
	SAVE_MAP_DATA
};
#include <EnginePch.h>
#include <LevelEditHelper.h>

class LevelEditSystem : public System
{
public:
	void Init();
	void Update();
};