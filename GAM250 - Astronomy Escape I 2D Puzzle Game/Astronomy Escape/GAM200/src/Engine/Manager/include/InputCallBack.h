/*****************************************************************
*\file		 InputCallBack.h
*\brief		 This file contains definition of Inputs call 
			 backfunctions for glfw

*\author(s)   Lee Liang Ping	10 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <EnginePch.h>
#include <FakeEngine.h>
struct InputCallBack
{
	static void key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	static void mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod);
	static void mousepos_cb(GLFWwindow* pwin, double xpos, double ypos);
	static void mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset);
};



