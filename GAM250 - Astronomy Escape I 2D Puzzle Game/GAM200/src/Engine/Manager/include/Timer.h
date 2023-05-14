/*****************************************************************
*\file		 Timer.h
*\brief		 This file contains Timer List that keep tracks on
			 individual timing

*\author(s)   Lee Liang Ping	22 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <EnginePch.h>
#include <GameTypes.h>

extern float g_dt;

/******************************************************************************/
/*!
 \class Timer;

 \brief List of Timer store the enitiy with the set of TimerList

 \param singletonTimerList
		frameList
		globalTime
		gameTime
*/
/******************************************************************************/
class Timer
{
	std::map<TimerMap, float> singletonTimerList;
	std::map<TimerMap, int> frameList;
public:
	double globalTime;
	double gameTime;
	//for time lock
	void SetSingletonTimer(TimerMap object, float time);
	void SetTimer(TimerMap object, float time);
	//check if the timer exsist if the timer still running
	bool CheckSingletonTimer(TimerMap object, float time = 0);
	float GetSingletonTimerValue(TimerMap object);

	//for loop lock
	void SetTimerLoop(TimerMap object, int loopframe = 0);
	bool CheckTimerLoop(TimerMap object);

	void UpdateGlobalTime();
	void UpdateTimer();
	void Remove(TimerMap object);

	void Reset();
};