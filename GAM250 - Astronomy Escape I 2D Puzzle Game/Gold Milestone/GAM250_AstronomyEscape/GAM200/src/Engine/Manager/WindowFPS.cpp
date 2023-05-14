/*****************************************************************
*\file		 WindowFPS.cpp
*\brief		 WindowFPS functions definitions.

*\author(s)   Lee Liang Ping	50 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <WindowFPS.h>
extern float g_dt;

void FPS::Update()
{
	currentNumberOfSteps = 0; 
	delta_time = static_cast<float>(FPS::update_time(1.0));
	accumulatedTime += delta_time;
	
	g_dt = static_cast<float>(FPS::GetDeltaTime());

	while (accumulatedTime >= g_dt)
	{
		accumulatedTime -= g_dt;
		currentNumberOfSteps++;	
	}
}

void FPS::SetUserDefinedFPS(double set)
{
	userDefinedFPS = set;
	userDefinedFPSBool = true;
}

void FPS::ResetUserDefinedFPS()
{
	userDefinedFPSBool = false;
	userDefinedFPS = 0;
}

double FPS::GetDeltaTime()
{
	if (userDefinedFPSBool && userDefinedFPS > 0)
		return 1.f / userDefinedFPS;
	else
		return 1.f / fps;
}

double FPS::update_time(double fps_calc_interval) {

	static double prev_time = glfwGetTime();
	double curr_time = glfwGetTime();
	double _delta_time = curr_time - prev_time;
	prev_time = curr_time;
	// fps calculations
	static double count = 0.0;
	static double start_time = glfwGetTime();
	double elapsed_time = curr_time - start_time;
	++count;
	fps_calc_interval = (fps_calc_interval < 0.0) ? 0.0 : fps_calc_interval;
	fps_calc_interval = (fps_calc_interval > 10.0) ? 10.0 : fps_calc_interval;
	if (elapsed_time > fps_calc_interval) {
		FPS::fps = count / elapsed_time;
		start_time = curr_time;
		count = 0.0;
	}
	return _delta_time;
}