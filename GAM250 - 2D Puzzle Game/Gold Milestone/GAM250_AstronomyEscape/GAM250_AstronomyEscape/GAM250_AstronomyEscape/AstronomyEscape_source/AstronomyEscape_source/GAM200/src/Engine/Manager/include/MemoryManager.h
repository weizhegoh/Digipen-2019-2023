/*****************************************************************
*\file		 MemoryManager.h
*\brief		 Memory Manager to create 

*\author(s)   Lee Liang Ping	43 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <ManagerList.h>
#include <GameTypes.h>


class MemoryManager
{
	std::unordered_map<std::string, std::unique_ptr<IManager>> mManagerList{};

	template<typename T>
	Manager<T>* GetManagerList()
	{
		std::string typeName = typeid(T).name();
		ARTLESS_ASSERT(mManagerList.find(typeName) != mManagerList.end(), "Manager not registered before use.");
		return static_cast<Manager<T>*>(mManagerList[typeName].get());
	}

public:
	template<typename T>
	void RegisterManager()
	{
		std::string typeName = typeid(T).name();
		ARTLESS_ASSERT(mManagerList.find(typeName) == mManagerList.end(), "Registering Manager type more than once.");
		mManagerList.insert({ typeName, std::make_unique<Manager<T>>() });
	}

	template<typename T>
	T& GetManager()
	{
		return GetManagerList<T>()->GetData();
	}

	template<typename T>
	void RemoveManager()
	{
		std::string typeName = typeid(T).name();
		ARTLESS_ASSERT(mManagerList.find(typeName) != mManagerList.end(), "Manager not registered before use.");
		auto it = mManagerList.find(typeName);
		it->second.reset(nullptr);
		mManagerList.erase(typeName);
	}

	void RemoveManagerList()
	{
		for (auto& x : mManagerList)
			x.second.reset(nullptr);
		mManagerList.clear();
	}
};