/*****************************************************************
*\file		 AiSystem.h
*\brief		 Header file declarations for AiSystem.

*\author(s)   Justin Yip Ling Kit 		10 lines x 100% Code contribution
* 
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

class AiSystem : public System
{

public:
	void Init();

	void Update();
	
	static void AILogic(Entity entity);
	void ChangeState(ObjectDirection objdir, Entity entity);
	static void AITurn(Entity);
};