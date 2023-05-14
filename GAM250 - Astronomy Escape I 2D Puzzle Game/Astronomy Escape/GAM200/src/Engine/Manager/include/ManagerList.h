/*****************************************************************
*\file		 ManagerList.h
*\brief		 Declaration of Manager

*\author(s)   Lee Liang Ping  18 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include <GameTypes.h>

class IManager
{
public:
	virtual ~IManager() = default;
};

template<typename T>
class Manager : public IManager
{
	T mManagerList{};
	Size mManagerCount{};
public:

	T& GetData()
	{
		return mManagerList;
	}
};