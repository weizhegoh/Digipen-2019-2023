/*****************************************************************
*\file		 Window.h
*\brief		 Header file declarations for Window.

*\author(s)   Lee Jun Jie	39 x % Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <FakeEngine.h>
#include <WindowFPS.h>

class Window
{
public:
	bool Init(const int width, const int height, std::string title);
	int GetWidth() { return _width; }
	int GetHeight() { return _height; }

	int GetHalfWidth() { return (int)(_width * 0.5); } const 
	int GetHalfHeight() { return (int)(_height * 0.5); } const
	int GetQuarterWidth() { return (int)(_width * 0.25); } const
	int GetQuarterHeight() { return (int)(_height * 0.25); } const
	int GetEighthWidth() { return (int)(_width * 0.125f); } const
	int GetEighthHeight() { return (int)(_height * 0.125f); } const
	int GetSIXTEENTHWidth() { return (int)(_width * 0.0625f); } const
	int GetSIXTEENTHHeight() { return (int)(_height * 0.0625f); } const

	bool IsFullScreen() { return _fullscreen; }
	std::string GetTitle() { return _title; }
	GLFWwindow* GetWindow() { return ptr_window; }
	bool IsFocus() { return _focus; }
	void PrintWindowTitle();
	void SetWindowResolution(int width, int height);
	void SetWindowFullScreen(bool check);
	void cleanup();

	static void error_cb(int error, char const* description);
	static void fbsize_cb(GLFWwindow* ptr_win, int width, int height);
	static void window_focus_cb(GLFWwindow* ptr_win, int focused);
	void hideMouseCursor(bool);
	void print_specs();

private:
	std::string _title;
	int _width;
	int _height;
	GLFWwindow* ptr_window;
	bool _fullscreen;
	bool _focus = true;
};