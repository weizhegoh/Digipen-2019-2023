/*****************************************************************
*\file		 AABB.h
*\brief		 AABB Component

*\author(s)   Lee Liang Ping	12 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct AABB
{
	glm::vec2 min;
	glm::vec2 max;
	glm::vec2 size{ 0.5f, 0.5f };


	void Update(mathEngine::Vector3D& position, mathEngine::Vector3D& scale)
	{
		min = { -size.x * scale.x + position.x, -size.y * scale.y + position.y };
		max = { size.x * scale.x + position.x, size.y * scale.y + position.y };
	}
};