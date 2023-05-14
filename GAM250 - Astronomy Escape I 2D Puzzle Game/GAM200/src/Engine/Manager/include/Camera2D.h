/*****************************************************************
*\file		 Camera2d.h
*\brief		 Header file declarations for Camera2d.

*\author(s)   Kenric Tan Wei Liang	23 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <GameTypes.h>
#include <Rigidbody.h>
#include <Ray.h>

struct Camera2d
{
	glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
	glm::vec2 cam_window = glm::vec2({ 0,0 });
	ZoomOption cam_opt = ZoomOption::NORMAL;
	bool zoom_input = false;
	bool zoomIN_input = false;
	bool zoomOUT_input = false;
	
	Rigidbody rigidBody;
	mathEngine::Vector3D targetpos;
	Ray ray;
	float timerPause = 0.f;
	
	bool zoom_to_back = false;

	void Reset();
	void MoveTo(mathEngine::Vector3D from, mathEngine::Vector3D to, float timer);
	void MoveToBack(mathEngine::Vector3D from, mathEngine::Vector3D to, float timer);
	void Update();
private:
};