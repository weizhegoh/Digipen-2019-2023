/*****************************************************************
*\file		 System.h
*\brief		 system declaration

*\author(s)   Lee Jun Jie		8 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <GameTypes.h>

class System
{
public:
	std::set<Entity> mEntities;
};