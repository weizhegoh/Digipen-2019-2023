/*****************************************************************
*\file		 GameInputSystem.h
*\brief		 Header file declarations for GameInputSystem.

*\author(s)    Lee Liang Ping	11 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>

class GameInputSystem : public System
{
public:
	void Init();

	void Update();

	void SetBoulderStatus(SetBoulderStatusEvent* rEvent);

	void InputVelocity(Entity entity, ObjectDirection dir);

	void MovementUpdate(ObjectDirection dir);
};