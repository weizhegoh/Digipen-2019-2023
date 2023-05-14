/*****************************************************************
*\file		 Transform.h
*\brief		 Transform Component

*\author(s)   Lee Liang Ping	15 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <GL/glew.h>
#include <vector3D.h>

struct Transform
{
	mathEngine::Vector3D scale{ 0.0f,0.0f, 0.0f };
	mathEngine::Vector3D scaleoffset{ 0.0f,0.0f, 0.0f };
	float angle{ 0.0f };
	float angleoffset{ 0.0f };
	float anglespeed{ 0.f };
	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 view = glm::mat4(1.0);
	glm::mat4 proj = glm::mat4(1.0);
	FreeRenderType renderType{ FreeRenderType::MODEL_WORLD };
};