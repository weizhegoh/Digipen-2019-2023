/*****************************************************************
*\file		 SystemManager.h
*\brief		 ECS System Manager that manage all the system

*\author(s)   Kenric Tan Wei Liang		15 lines x 33.33% Code contribution
*\author(s)   Lee Jun Jie				16 lines x 33.33% Code contribution
*\author(s)   Lee Liang Ping			16 lines x 33.33% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include "System.h"
#include <cassert>
#include <memory>
#include <unordered_map>


class SystemManager
{

	std::unordered_map<std::string, BitSignature> mSystemSignatureList{};
	std::unordered_map<std::string, std::unique_ptr<System>> mSystemsList{};

public:
	template<typename T>
	T* RegisterSystem()
	{
		std::string typeName = typeid(T).name();

		ARTLESS_ASSERT(mSystemsList.find(typeName) == mSystemsList.end() , "System already registered.");

		mSystemsList.insert({typeName, std::make_unique<T>()});
		return static_cast<T*>(mSystemsList[typeName].get());
	}

	template<typename T>
	void SetBitSignature(BitSignature signature)
	{
		std::string typeName = typeid(T).name();

		ARTLESS_ASSERT(mSystemsList.find(typeName) != mSystemsList.end() , "System not registered.");

		mSystemSignatureList.insert({typeName, signature});
	}

	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : mSystemsList)
		{
			auto const& system = pair.second;
			system->mEntities.erase(entity); // Remove entity from system
		}
	}

	void EntitySignatureChanged(Entity entity, BitSignature entitySignature)
	{
		for (auto const& pair : mSystemsList)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSystemSignatureList[type];

			if ((entitySignature & systemSignature) == systemSignature)
				system->mEntities.insert(entity); // Insert entity into system when bit match
			else
				system->mEntities.erase(entity); // Remove entity from system when bit does not match
		}
	}
};
