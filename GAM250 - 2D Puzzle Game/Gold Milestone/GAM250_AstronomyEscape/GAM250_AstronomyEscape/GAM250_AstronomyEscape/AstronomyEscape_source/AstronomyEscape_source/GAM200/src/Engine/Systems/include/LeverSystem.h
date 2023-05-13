/*****************************************************************
*\file		 DoorSystem.h
*\brief		 Header file declarations for DoorSystem.

*\author(s)   Kenric Tan		9 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>

class LeverSystem : public System
{
public:
	void Init();
	void init_map();
	void Update();
};