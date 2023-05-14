/*****************************************************************
*\file		 WindowManager.h
*\brief		 Header file declarations for WindowManager.

*\author(s)   Lee Jun Jie	8 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include <EnginePch.h>
#include <Window.h>

class WindowManager
{
public:
	Window* CreateNewWindow(float width, float height, const char* wName);
	void ResizeWindow(Window* window, float width, float height);
	Window* GetCurrentWindow();
	void SetCurrentWindow(Window* window);
	bool InitalizeWindow();
	void ExitWindow(Window& window);
private:
	Window* _currWindow;
	std::list<Window*> _windowList;
};