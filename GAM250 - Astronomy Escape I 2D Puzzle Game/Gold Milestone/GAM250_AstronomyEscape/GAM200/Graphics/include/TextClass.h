/*****************************************************************
*\file		 TextClass.h
*\brief		 Header file declarations for TextClass.

*\author(s)   Lee Liang Ping	15 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>


struct TextClass
{
	using Duration = std::pair<DurationType, float>;
	mathEngine::Vector3D position;
	float scale;
	glm::vec3 color;
	bool activte;
	Duration lifetime;
	FreeRenderType type;
	FreeType_Font fontType;
	std::string text;
	bool onremove;
};
