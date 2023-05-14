/*****************************************************************
*\file		 Light.h
*\brief		 Light Component 

*\author(s)   Lee Liang Ping	7 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct Light
{
    glm::vec4 highlight{ 1.f, 1.f, 1.f, WorldIntensity };
    float transparncy{ 1.f };
    bool interp{ false };
};