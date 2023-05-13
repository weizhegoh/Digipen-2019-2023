/*****************************************************************
*\file		 Victory_screen.h
*\brief		 Header file declarations for Victory Screen System.

*\author(s)   Justin Yip Ling Kit 		10 lines x 100% Code contribution
*
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <EnginePch.h>
#include <FakeEngine.h>
#include <Collision.h>

class Victory_screen : public System {
public:
	void Init();

	void Update();
	
	void Hover(Entity entity);
};

