/*****************************************************************
*\file		 ShapeManager.cpp
*\brief		 ShapeManager functions definitions.

*\author(s)   Lee Liang Ping	19 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <ShapeManager.h>


ShapesManager::ShapesManager()
{
    // Codes
}

ShapesManager::~ShapesManager()
{
    // Codes
}

void ShapesManager::PushShapes(const ObjectShapes objShapes)
{
    if (!shapeList[objShapes])
        shapeList[objShapes] = std::make_unique<Shapes>(objShapes);
}

Shapes* ShapesManager::GetShapes(const ObjectShapes objShapes)
{
    if (shapeList.count(objShapes) > 0)
        return shapeList[objShapes].get();
    return nullptr;
}