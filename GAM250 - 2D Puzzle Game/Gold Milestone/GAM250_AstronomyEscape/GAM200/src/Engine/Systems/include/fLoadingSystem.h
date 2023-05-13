/*****************************************************************
*\file		 fLoadingSystem.h
*\brief		 Header file declarations for fLoadingSystem.

*\author(s)   Lee Liang Ping	11 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>

class FakeLoadingSystem : public System
{
public:
	void Init();

	void Update();

	void SetLoading(SetLoadingScreen* set);

	void SetWinPage(SetWinScreen* set);

	static void ToggleLOGO();
};