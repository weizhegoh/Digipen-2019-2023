/*****************************************************************
*\file		 Lever.h
*\brief		 Lever component

*\author(s)  Kenric Tan Wei Liang		9 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct Lever
{
	int leverID;
	bool isOpen;
	float dt;
	bool lock{true};
	std::map<Entity, Entity> leverMap;
};