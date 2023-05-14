/*****************************************************************
*\file		 PointLight.h
*\brief		 PointLight Component

*\author(s)   Lee Liang Ping	8 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct PointLight
{
    mathEngine::Vector3D attenuationEq{ 1.f, 1.f, 1.f }; //aX^2 + bX + C
    float ambient;
    float ambientDir;
    float maxAmbient;
};