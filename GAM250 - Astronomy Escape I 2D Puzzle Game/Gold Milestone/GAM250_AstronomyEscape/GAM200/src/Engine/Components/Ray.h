/*****************************************************************
*\file		 Ray.h
*\brief		 Ray component

*\author(s)   Lee Liang Ping	7 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <vector3D.h>

struct Ray
{
	mathEngine::Vector3D mSr;
	mathEngine::Vector3D mDr;
};