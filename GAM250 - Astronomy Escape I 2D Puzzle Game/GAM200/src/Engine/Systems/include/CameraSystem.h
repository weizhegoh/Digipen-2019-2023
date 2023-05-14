/*****************************************************************
*\file		 CameraSystem.h
*\brief		 Header file declarations for CameraSystem.

*\author(s)   Kenric Tan Wei Liang		8 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>

class CameraSystem : public System
{
public:
	void Init();

	void Update();
};