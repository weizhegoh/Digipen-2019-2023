/*****************************************************************
*\file		 Teleporter.h
*\brief		 Teleporter component

*\author(s)  Kenric Tan Wei Liang		6 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct Teleporter
{
	int teleporterID;
	std::map<Entity, Entity> teleporterMap;
};