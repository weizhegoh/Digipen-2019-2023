/*****************************************************************
*\file		 Inventory.h
*\brief		 Inventory Component

*\author(s)  Kenric Tan Wei Liang		14 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <AABB.h>

struct Inventory
{
	int _currObjective{ 0 };
	int _lifes{ 3 };
	std::list<int> _inventory;
	Entity _currBoulder{ NULL };
	AABB collisionArea{ {0.f,0.f}, {0.f,0.f}, { 10.5f, 2.0f } };
	ObjectDirection inputDir{ ObjectDirection::NO_DIR };
	float timer;
	int audioPickupIndex{ 0 };
};