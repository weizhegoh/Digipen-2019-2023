/*****************************************************************
*\file		 WindowFPS.h
*\brief		 Header file declarations for HotSpotCollisionSystem.

*\author(s)   Lee Liang Ping	20 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>

class FPS
{
public:
	double fps;
	double userDefinedFPS;

	void Update();
	double GetDeltaTime();
	double update_time(double fps_calc_interval);

	int GetCurrentNumberOfSteps() { return currentNumberOfSteps; }
	double GetAccumulatedTime() { return accumulatedTime; }
	void SetUserDefinedFPS(double set);
	void ResetUserDefinedFPS();

private:
	int currentNumberOfSteps = 0;
	double accumulatedTime = 0.0;
	bool userDefinedFPSBool = false;
	double delta_time;
};