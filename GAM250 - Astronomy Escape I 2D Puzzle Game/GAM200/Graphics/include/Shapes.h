/*****************************************************************
*\file		 Shapes.h
*\brief		 Header file declarations for Shapes.

*\author(s)   Lee Liang Ping	7 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <GameTypes.h>
#include <Gfx.h>

struct Shapes
{
	// Data Member
	Graphics::Gfx::Model pMesh;
	Graphics::Gfx::Model pHotspot;

	Shapes() = delete;
	Shapes(ObjectShapes);
	~Shapes();
};