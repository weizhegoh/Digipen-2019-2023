/*****************************************************************
*\file		 CollisionFlag.h
*\brief		 CollisionFlag Component 

*\author(s)   Lee Liang Ping	6 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct CollisionFlag
{
	unsigned int gridCollisionFlag{ FLAG_INACTIVE };
	bool audioActivate{ false };
};