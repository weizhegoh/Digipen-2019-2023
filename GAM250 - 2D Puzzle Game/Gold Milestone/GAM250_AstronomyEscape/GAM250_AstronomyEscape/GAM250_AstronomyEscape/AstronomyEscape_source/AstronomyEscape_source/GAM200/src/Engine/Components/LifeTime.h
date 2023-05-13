/*****************************************************************
*\file		 LifeTime.h
*\brief		 LifeTime Component

*\author(s)   Lee Liang Ping	16 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct LifeTime
{
	float timeCounter{ 0.f };
	float lifetime{ 0.f };
	bool isTherelifeTime{ true };

	LifeTime() = default;
	LifeTime(float lifetime) : lifetime{ lifetime }, timeCounter{ lifetime }
	{
		isTherelifeTime = (lifetime > 0.f);
	}

	void Random(float low, float high)
	{
		*this = LifeTime(mathEngine::RandNumberBetween(low, high));
	}
};