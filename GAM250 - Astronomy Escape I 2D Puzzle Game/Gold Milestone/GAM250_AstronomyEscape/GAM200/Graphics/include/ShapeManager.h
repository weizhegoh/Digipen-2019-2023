/*****************************************************************
*\file		 ShapeManager.h
*\brief		 Header file declarations for ShapeManager.

*\author(s)   Lee Liang Ping	7 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <GameTypes.h>
#include <Shapes.h>
class ShapesManager
{
	std::unordered_map<ObjectShapes, std::unique_ptr<Shapes>> shapeList{};
public:
	ShapesManager();
	~ShapesManager();
	void PushShapes(const ObjectShapes);
	Shapes* GetShapes(const ObjectShapes);
};