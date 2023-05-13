/*****************************************************************
*\file		 Circle.h
*\brief		 Circle Component

*\author(s)   Lee Liang Ping	8 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct Circle
{
	glm::vec2 center;
	float radius;

	Circle(mathEngine::Vector3D center_, float radius_) : center{ center_.x, center_.y }, radius{ radius_ }
	{}
};