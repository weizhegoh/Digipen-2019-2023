/*****************************************************************
*\file		 Timer.cpp
*\brief		 This file contains Timer List that keep tracks on 
			 individual timing

*\author(s)   Lee Liang Ping	92 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include <EnginePch.h>
#include <Timer.h>
#include <FakeEngine.h>
extern std::unique_ptr<FakeEngine> gFakeEngine;

/******************************************************************************/
/*!
 \fn void Timer::SetSingletonTimer(TimerMap object, float time)

 \brief Add Timer and time to the Timer List

 \param object
		time

 \return none
*/
/******************************************************************************/
void Timer::SetSingletonTimer(TimerMap object, float time)
{
	auto it = singletonTimerList.find(object);
	if (it != singletonTimerList.end())
	{
		it->second += time;
		return;
	}
	singletonTimerList.insert(std::make_pair(object, time));
}

void Timer::SetTimer(TimerMap object, float time)
{
	auto it = singletonTimerList.find(object);
	if (it != singletonTimerList.end())
		return;
	singletonTimerList.insert(std::make_pair(object, time));
}

/******************************************************************************/
/*!
 \fn void Timer::SetSingletonTimer(TimerMap object, float time)

 \brief check if the timer exsist if the timer still running

 \param object
		time

 \return boolean
		 false when the timer is still running in the timerList
		 true when there is no timer in the timerList
*/
/******************************************************************************/
bool Timer::CheckSingletonTimer(TimerMap object, float time)
{
	auto it = singletonTimerList.find(object);
	if (it != singletonTimerList.end())
		return false; //return false when the timer is still running in the timerList
	else
	{
		if (time > 0)
			SetSingletonTimer(object, time);
		return true; //return true when there is no timer in the timerList
	}
}

/******************************************************************************/
/*!
 \fn float Timer::GetSingletonTimerValue(TimerMap object)

 \brief Get the Existing time from the Timer List.

 \param object

 \return float (time of the list)
*/
/******************************************************************************/
float Timer::GetSingletonTimerValue(TimerMap object)
{
	auto it = singletonTimerList.find(object);
	if (it != singletonTimerList.end())
		return it->second; //return false when the timer is still running in the timerList
	else
		return 0; //return true when there is no timer in the timerList
}

/******************************************************************************/
/*!
 \fn void Timer::UpdateGlobalTime()

 \brief Update Global Time

 \return none
*/
/******************************************************************************/
void Timer::UpdateGlobalTime()
{
	globalTime += g_dt;
}

/******************************************************************************/
/*!
 \fn void Timer::UpdateTimer()

 \brief Update the game time and all the timer in the list

 \return none
*/
/******************************************************************************/
void Timer::UpdateTimer()
{
	gameTime += g_dt;
	for (auto it = singletonTimerList.begin(); it != singletonTimerList.end();)
	{
		it->second -= static_cast<float>(g_dt);
		if (it->second <= 0.f)
			it = singletonTimerList.erase(it);
		else
			++it;
	}

	for (auto it = frameList.begin(); it != frameList.end();)
	{
		--it->second;
		if (it->second <= 0)
			it = frameList.erase(it);
		else
			++it;
	}
}

/******************************************************************************/
/*!
 \fn void Timer::Remove()

 \brief Remove the timer from the list

 \param object

 \return none
*/
/******************************************************************************/
void Timer::Remove(TimerMap object)
{
	auto it = singletonTimerList.find(object);
	if (it != singletonTimerList.end())
		singletonTimerList.erase(it);
}

/******************************************************************************/
/*!
 \fn void Timer::SetTimerLoop(TimerMap object, int loopframe)

 \brief Add the amount of loop that the timer you want to lock

 \param object
		loopframe

 \return none
*/
/******************************************************************************/
void Timer::SetTimerLoop(TimerMap object, int loopframe)
{
	auto it = frameList.find(object);
	if (it != frameList.end())
		return;
	frameList.insert(std::make_pair(object, loopframe));
}

/******************************************************************************/
/*!
 \fn void Timer::SetTimerLoop(TimerMap object, int loopframe)

 \brief Check if the Timer is exist or not

 \param object

 \return boolean
		 return false when the timer is still running in the timerList
		 return true when there is no timer in the timerList
*/
/******************************************************************************/
bool Timer::CheckTimerLoop(TimerMap object)
{
	auto it = frameList.find(object);
	if (it != frameList.end())
		return false; //return false when the timer is still running in the timerList
	else
		return true; //return true when there is no timer in the timerList
}

/******************************************************************************/
/*!
 \fn void Timer::Reset()

 \brief Reset Timer List

 \return none
*/
/******************************************************************************/
void Timer::Reset()
{
	singletonTimerList.clear();
	frameList.clear();
	globalTime = gameTime = 0;
}