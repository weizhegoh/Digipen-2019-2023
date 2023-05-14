/*****************************************************************
*\file		 Force.h
*\brief		 Force Component 

*\author(s)   Lee Jun Jie	8 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <vector3D.h>

struct Force
{
	//mathEngine::Vector3D mForce;
	mathEngine::Vector3D mResultantForce{ 0.f, 0.f, 0.f };
	float lifeTime{ 0.f };
	float invMass{ 1.f };
};