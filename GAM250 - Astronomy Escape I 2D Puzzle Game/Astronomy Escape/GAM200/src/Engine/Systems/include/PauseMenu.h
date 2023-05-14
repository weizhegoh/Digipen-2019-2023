/*****************************************************************
*\file		 PauseMenu.h
*\brief		 xxxxxxxxxxx

*\author(s)   Yip Ling Kit Justin	15 lines x 100% Code contribution 100%
			  Goh Wei Zhe			3 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <Collision.h>

class PauseMenu : public System
{
public:

	void Init();

	void Update();
	void Hover(Entity entity);
	void Pause_SetChangeTexture(Entity entity);
	void SetChangeTexture(bool check, Entity entity);
};

enum PAUSE_CONTROL_MENU_BG
{
	PAUSE_HOW_TO_PLAY_1 = 0,
	PAUSE_HOW_TO_PLAY_2,
	PAUSE_HOW_TO_PLAY_3,
};
