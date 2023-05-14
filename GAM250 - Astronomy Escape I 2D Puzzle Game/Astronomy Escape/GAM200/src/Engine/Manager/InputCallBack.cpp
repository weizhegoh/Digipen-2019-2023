/*****************************************************************
*\file		 InputCallBack.cpp
*\brief		 This file contains definition of Inputs call 
			 backfunctions for glfw

*\author(s)   Lee Liang Ping	77 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include <EnginePch.h>
#include <InputCallBack.h>
extern std::unique_ptr<FakeEngine> gFakeEngine;

void InputCallBack::key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod) {
	(void)mod;
	(void)scancode;
	(void)key;
	(void)pwin;

	Input& input = gFakeEngine->GetGraphicManager<Input>();
	if (GLFW_PRESS == action) {
	#ifdef _DEBUG
		std::cout << "Key pressed" << std::endl;
	#endif
		input.AddKeyTriggered(key);
		input.AddKeyPressed(key);
	}
	else if (GLFW_REPEAT == action) {
	#ifdef _DEBUG
		std::cout << "Key repeatedly pressed" << std::endl;
	#endif
	}
	else if (GLFW_RELEASE == action) {
	#ifdef _DEBUG
		std::cout << "Key released" << std::endl;
	#endif
		input.RemoveKeyTriggered(key);
	}
}

void InputCallBack::mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod) {
	(void)mod;
	(void)pwin;

	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
	#ifdef _DEBUG
		std::cout << "Left mouse button ";
	#endif
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
	#ifdef _DEBUG
		std::cout << "Right mouse button ";
	#endif
		break;
	}
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	switch (action) {
	case GLFW_PRESS:
	#ifdef _DEBUG
		std::cout << "pressed!!!" << std::endl;
	#endif
		input.AddKeyTriggered(button);
		input.AddKeyPressed(button);
		break;
	case GLFW_RELEASE:
	#ifdef _DEBUG
		std::cout << "released!!!" << std::endl;
	#endif
		input.RemoveKeyTriggered(button);
		break;
	}

	
}

void InputCallBack::mousepos_cb(GLFWwindow* pwin, double xpos, double ypos) {
	#ifdef _DEBUG
	std::cout << "Mouse cursor position: (" << xpos << ", " << ypos << ")" << std::endl;
	#endif
	(void)pwin;
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	input.SetMousePos(xpos, ypos);
}

void InputCallBack::mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset) {
	#ifdef _DEBUG
	std::cout << "Mouse scroll wheel offset: ("
		<< xoffset << ", " << yoffset << ")" << std::endl;
	#endif
	(void)xoffset;
	(void)yoffset;
	(void)pwin;
}