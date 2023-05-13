/*****************************************************************
*\file		 L_P_PhysicSystem.h
*\brief		 Header file declarations for PhysicSystem.

*\author(s)   Lee Liang Ping	8 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
class L_P_PhysicSystem : public System
{
public:
	void Init(EntitySystemOption systemoption);

	void Update(EntitySystemOption systemoption);
};