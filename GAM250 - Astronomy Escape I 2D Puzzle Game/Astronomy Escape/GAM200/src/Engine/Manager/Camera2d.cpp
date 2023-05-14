/*****************************************************************
*\file		 Camera2d.cpp
*\brief		 This file contains definiton of Lock functions that
			 acts like a boolean type

*\author(s)   Lee Liang Ping	104 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <Camera2D.h>
#include <FakeEngine.h>

constexpr float epsilon = 0.00002f;
constexpr float accelerateSp = 55.f;
extern float g_dt;
extern std::unique_ptr<FakeEngine> gFakeEngine;

void Camera2d::MoveTo(mathEngine::Vector3D from, mathEngine::Vector3D to, float timer)
{
	if (cam_opt != ZoomOption::NORMAL)
		return;
	targetpos = mathEngine::Vector3D(to.x, to.y, 0);
	ray.mSr = from;

	mathEngine::Vector3D camPosition = mathEngine::Vector3D(pos.x, pos.y, 0);
	mathEngine::Vector3D dir = (targetpos - camPosition);
	dir.normalize();
	ray.mDr = dir;
	ray.mDr.print();
	rigidBody.acceleration.x = rigidBody.acceleration.y = accelerateSp;
	rigidBody.acceleration.x *= (dir.x);
	rigidBody.acceleration.y *= (dir.y);


	timerPause = timer;

	cam_opt = ZoomOption::ZOOM_TO_OBJECT;
}

void Camera2d::MoveToBack(mathEngine::Vector3D from, mathEngine::Vector3D to, float timer)
{
	Boolean& boolean = gFakeEngine->GetGameManager<Boolean>();
	boolean.True(BooleanType::MOVEMENT_LOCK);
	MoveTo(from, to, timer);
	zoom_to_back = true;
}


void Camera2d::Update()
{
	if (cam_opt == ZoomOption::NORMAL)
		return;


	mathEngine::Vector3D camPosition = mathEngine::Vector3D(pos.x, pos.y, pos.z);
	
	if (cam_opt == ZoomOption::ZOOM_TO_OBJECT)
	{
		mathEngine::Vector3D campos_vector = camPosition - ray.mSr;
		mathEngine::Vector3D tagetvector_norm = targetpos - ray.mSr;

		float t1 = campos_vector.dot(ray.mDr);
		float t2 = tagetvector_norm.dot(ray.mDr);
		if (t1 >= t2)
		{

			cam_opt = ZoomOption::ZOOM_PAUSE;

			if (!zoom_to_back)
			{
				clear(rigidBody);
				clear(ray.mDr);
			}
			else
			{
				clear(rigidBody.velocity);
			}

			pos.x = targetpos.x;
			pos.y = targetpos.y;
			return;
		}

		rigidBody.velocity.x += rigidBody.acceleration.x;
		rigidBody.velocity.y += rigidBody.acceleration.y;
		pos.x += rigidBody.velocity.x * g_dt;
		pos.y += rigidBody.velocity.y * g_dt;
		rigidBody.velocity *= 0.9f;
	}

	if (cam_opt == ZoomOption::ZOOM_PAUSE)
	{
		if (timerPause == -1)
		{
			cam_opt = ZoomOption::NORMAL;
			return;
		}

		timerPause -= g_dt;
		if (timerPause <= 0.f)
		{
			
			if (zoom_to_back)
			{
				cam_opt = ZoomOption::ZOOM_TO_OBJECT;

				rigidBody.acceleration.x *= (-1);
				rigidBody.acceleration.y *= (-1);
				ray.mDr.x *= -1;
				ray.mDr.y *= -1;
				targetpos = ray.mSr;
				ray.mSr = camPosition;
				Boolean& boolean = gFakeEngine->GetGameManager<Boolean>();
				boolean.False(BooleanType::MOVEMENT_LOCK);
				zoom_to_back = false;
			}
			else
			{
				cam_opt = ZoomOption::NORMAL;
			}
		}
		return;
	}


}

void Camera2d::Reset()
{
	pos = { 0.0f, 0.0f, 0.0f };
	cam_window = glm::vec2({ 0,0 });
	cam_opt = ZoomOption::NORMAL;
	zoom_input = false;
	zoomIN_input = false;
	zoomOUT_input = false;
}