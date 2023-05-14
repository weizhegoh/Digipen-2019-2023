/*****************************************************************
*\file		 Colour.h
*\brief		 Colour Component 

*\author(s)   Lee Liang Ping	11 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct Colour
{
    glm::vec3 colour;

    void Random() 
    {
        colour.r = mathEngine::RandNumberBetween(0.f, 1.f);
        colour.b = mathEngine::RandNumberBetween(0.f, 1.f);
        colour.g = mathEngine::RandNumberBetween(0.f, 1.f);

    }
};