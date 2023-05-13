/*****************************************************************
*\file		 Textures.h
*\brief		 Header file declarations for Textures.

*\author(s)   Lee Liang Ping	6 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <GameTypes.h>
#include <Gfx.h>

struct Textures
{
	GLuint pTex;

	Textures() = delete;
	Textures(const std::string pFileName);
	~Textures();
};

