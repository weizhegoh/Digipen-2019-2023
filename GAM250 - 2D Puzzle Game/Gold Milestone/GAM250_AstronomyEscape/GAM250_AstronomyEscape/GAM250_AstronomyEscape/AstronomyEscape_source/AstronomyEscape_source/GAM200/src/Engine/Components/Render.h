/*****************************************************************
*\file		 Render.h
*\brief		 Render Component 

*\author(s)   Lee Liang Ping	11 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include <Gfx.h>
#include <GameTypes.h>
#include <Shapes.h>
#include <Textures.h>
#include <map>

struct Render
{
	Shapes* shape{ nullptr };
	ShaderOption shaderOption{ ShaderOption::SHAPE };
}; 