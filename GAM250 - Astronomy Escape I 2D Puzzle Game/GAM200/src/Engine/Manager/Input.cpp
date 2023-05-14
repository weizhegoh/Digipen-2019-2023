/*****************************************************************
*\file		 Input.cpp
*\brief		 This file contains definition of Inputs function
			 press and triggered

*\author(s)   Lee Liang Ping	141 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <Input.h>
#include <FakeEngine.h>
#include <LifeTime.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;

template<typename T>
void Input::AddKey(const T& key, std::list<T>& vector)
{
	bool sameKey = true;
	for (auto& x : vector)
	{
		if (x == key)
			return;
	}
	if (sameKey)
		vector.push_back(key);
}

template<typename T>
void Input::RemoveKey(const T& key, std::list<T>& vector)
{
	for (auto it = vector.begin(); it != vector.end(); )
	{
		if (*it == key)
		{
			vector.erase(it);
			return;
		}
		else
			it++;
	}
}

template<typename T>
bool Input::CheckKey(const T& key, std::list<T>& vector)
{
	for (auto& x : vector)
	{
		if (x == key)
			return true;
	}
	return false;
}

void Input::AddKeyTriggered(const int& key)
{
	AddKey(key, keyTriggered);
}

void Input::RemoveKeyTriggered(const int& key)
{
	RemoveKey(key, keyTriggered);
}

void Input::ResetKeyTriggered()
{
	keyTriggered.clear();
}

void Input::AddKeyPressed(const int& key)
{
	AddKey(key, keyPressed);
}

void Input::RemoveKeyPressed(const int& key)
{
	RemoveKey(key, keyPressed);
}

void Input::ResetKeyPressed()
{
	keyPressed.clear();
	booleanPressed.clear();
}

bool Input::CheckTriggered(const int& key)
{
	return CheckKey(key, keyTriggered);
}

bool Input::CheckPress(const int& key)
{
	return CheckKey(key, keyPressed);
}

mathEngine::Vector3D  Input::GetMousePos() const
{
	return mousepos;
}

mathEngine::Vector3D  Input::GetMousePosWorld() const
{
	return mouseposWorld;
}

InputBoolean& Input::GetInputBooleen()
{
	return inputBool;
}

void Input::SetMousePos(const double& x, const double& y)
{
	mousepos = mathEngine::Vector3D(static_cast<float>(x), static_cast<float>(y), 0.f);
	Camera2d& camera2d = gFakeEngine->GetGraphicManager<Camera2d>();
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	mathEngine::Vector3D mouseposNdc = mathEngine::Vector3D
	{
		-1.0f + 2.0f * static_cast<float>(x) / window.GetWidth(),
		1.0f - 2.0f * static_cast<float>(y) / window.GetHeight(),
		0.f
	};
	glm::mat4 view = glm::lookAt(
		glm::vec3(camera2d.pos.x, camera2d.pos.y, 0.f),
		glm::vec3(camera2d.pos.x, camera2d.pos.y, -1.0f),
		glm::vec3(0.f, 1.f, 0.f)); // up vector
	glm::mat4 proj = glm::ortho(
		float(-camera2d.cam_window.x),
		float(camera2d.cam_window.x),
		float(-camera2d.cam_window.y),
		float(camera2d.cam_window.y));
	glm::vec3 temp = glm::vec3(glm::inverse(proj * view) * glm::vec4(mouseposNdc.x, mouseposNdc.y, 1.0f, 1.0f));;
	mouseposWorld = mathEngine::Vector3D(temp.x, temp.y, temp.z);
}

void Input::ActivateBoolean(const BooleanType& key)
{
	AddKey(key, booleanPressed);
}
void Input::DeactivateBoolean(const BooleanType& key)
{
	RemoveKey(key, booleanPressed);
}
bool Input::CheckBooleanPress(const BooleanType& key)
{
	return CheckKey(key, booleanPressed);
}

bool Input::ToggleBoolean(bool& boolean)
{
	boolean = !boolean;
	return boolean;
}

void Input::Reset()
{
	ResetKeyTriggered();
	ResetKeyPressed();
	inputBool = InputBoolean();
	mousepos = mouseposWorld = mathEngine::Vector3D();
}

bool Input::CheckTriggered()
{
	return !keyTriggered.empty();
}
bool Input::CheckPress()
{
	return !keyPressed.empty();
}