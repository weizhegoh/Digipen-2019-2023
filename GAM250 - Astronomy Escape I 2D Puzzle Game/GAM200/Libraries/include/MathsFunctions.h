/*****************************************************************
*\file		 MathsFunctions.h
*\brief		 Helper Function that perform Maths

*\author(s)   Lee Liang Ping	60 lines x % Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
namespace mathEngine
{
	template<typename T>
	T RandNumberBetween(T lower, T higher)
	{
		if (lower > higher)
			std::swap(lower, higher);
		else if (lower == higher)
			return lower;
		std::random_device rd;
		std::default_random_engine r(rd());
		std::uniform_real_distribution<float> urd (static_cast<float>(lower), static_cast<float>(higher));
		return static_cast<T>(urd(r));
	}

	template<typename T, typename T2>
	T2 RandNumberBetween(T lower, T higher)	
	{
		return static_cast<T2>(RandNumberBetween(lower,higher));
	}

	template <typename T>
	T LinearInterpolate(T point1, T point2, T coefficent)
	{
		return point1 * (1 - coefficent) + point2 * coefficent;
	}
	
	template <typename T>
	bool Equal(T p0, T p1, float epsilon)
	{

		T typeEpsilon = static_cast<T>(epsilon);
		if (p0 == p1)
			return true;
		else if (p0 < p1 + typeEpsilon && p0 > p1 - typeEpsilon)
			return true;
		else
			return false;
	}

	template <typename T>
	bool LessThan(T p0, T p1, float epsilon)
	{

		T typeEpsilon = static_cast<T>(epsilon);
		if (p0 < p1)
			return true;
		else if (p0 - typeEpsilon < p1  ||  p0 + typeEpsilon < p1 )
			return true;
		else
			return false;
	}

	template <typename T>
	bool MoreThan(T p0, T p1, float epsilon)
	{

		T typeEpsilon = static_cast<T>(epsilon);
		if (p0 > p1)
			return true;
		else if (p0 - typeEpsilon > p1 || p0 + typeEpsilon > p1)
			return true;
		else
			return false;
	}

}