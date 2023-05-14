/*****************************************************************
*\file		 LightType.h
*\brief		 Light Component

*\author(s)   Lee Liang Ping	21 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct ET_LightNormal
{
};

struct ET_Light_MainChar
{
};

struct ET_Light_KeyObjectives
{
	ParentId id;
	TypeState state { TypeState::Active };
	Entity followingId;
};

struct ET_Light_KeyObjectives_Parts
{
	ParentId id;
};

struct ET_Light_LevelDoor
{
	ParentId id;
};
