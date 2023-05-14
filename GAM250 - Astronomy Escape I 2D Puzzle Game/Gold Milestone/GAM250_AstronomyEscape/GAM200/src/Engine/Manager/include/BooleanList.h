/*****************************************************************
*\file		 BooleanList.h
*\brief		 This file contains definiton of Lock functions that
			 acts like a boolean type

*\author(s)   Lee Liang Ping	40 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <GameTypes.h>

/******************************************************************************/
/*!
 \class Lock;

 \brief List of Locks, Act like a list of Boolean 

 \param lockList
*/
/******************************************************************************/
class Boolean
{
	std::vector<BooleanType> lockList;

public:
	void True(BooleanType lockType)
	{
		for (auto x : lockList)
		{
			if (x == lockType)
				return;
		}
		lockList.push_back(lockType);
	}

	bool Check(BooleanType lockType)
	{
		for (auto x : lockList)
		{
			if (x == lockType)
				return false;
		}
		return true;
	}

	void False(BooleanType lockType)
	{
		for (auto it = lockList.begin(); it != lockList.end();)
		{
			if (*it == lockType)
				it = lockList.erase(it);
			else
				++it;
		}
	}

	void Reset()
	{
		lockList.clear();
	}
};