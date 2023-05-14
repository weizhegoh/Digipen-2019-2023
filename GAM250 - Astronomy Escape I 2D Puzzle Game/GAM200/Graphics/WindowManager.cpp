/*****************************************************************
*\file		 WindowManager.cpp
*\brief		 WindowManager functions definitions.

*\author(s)   Lee Jun Jie	37 lines x % Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include"WindowManager.h"
//window class
Window* WindowManager::CreateNewWindow(float width = 800, float height = 600, const char* wName = "NewWindow")
{
	(void)width;
	(void)height;
	(void)wName;
	_windowList.push_front(_currWindow);
	return _currWindow;
}

void WindowManager::ResizeWindow(Window* window, float width, float height)
{
	(void)window;
	(void)width;
	(void)height;

}

Window* WindowManager::GetCurrentWindow()
{
	return _currWindow;
}

void WindowManager::SetCurrentWindow(Window* currWindow)
{
	_currWindow = currWindow;
}

bool WindowManager::InitalizeWindow()
{
	std::cout << "Initializing windows" << std::endl;
	Window* window = CreateNewWindow(800, 600, "test");

	SetCurrentWindow(window);
	return true;
}

void WindowManager::ExitWindow(Window& window)
{
	(void)window;
	for (auto it = _windowList.begin(); it != _windowList.end(); it++)
		delete* it;
}