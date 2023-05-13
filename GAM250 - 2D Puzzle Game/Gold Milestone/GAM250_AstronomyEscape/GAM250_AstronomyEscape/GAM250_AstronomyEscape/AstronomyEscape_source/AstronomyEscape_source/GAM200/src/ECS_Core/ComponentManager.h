/*****************************************************************
*\file		 ComponentManager.h
*\brief		 ECS Component Manager that manage all the  Component

*\author(s)   Kenric Tan Wei Liang		20 lines x 33.33% Code contribution
*\author(s)   Lee Jun Jie				20 lines x 33.33% Code contribution
*\author(s)   Lee Liang Ping			20 lines x 33.33% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <GameTypes.h>


class ComponentManager
{
	std::unordered_map<std::string, ComponentType> mComponentTypeList{};
	std::unordered_map<std::string, std::unique_ptr<IComponentList>> mComponentList{};
	ComponentType mComponentSignature{};


	template<typename T>
	ComponentList<T>* GetComponentList()
	{
		std::string typeName = typeid(T).name();
		//assert(mComponentTypeList.find(typeName) != mComponentTypeList.end() && "Component not registered before use.");
		ARTLESS_ASSERT(mComponentTypeList.find(typeName) != mComponentTypeList.end() , "Component not registered before use.");
		return static_cast<ComponentList<T>*>(mComponentList[typeName].get());
	}

public:
	template<typename T>
	void RegisterComponent()
	{
		std::string typeName = typeid(T).name();
		//assert(mComponentTypeList.find(typeName) == mComponentTypeList.end() && "Registering component type more than once.");
		ARTLESS_ASSERT(mComponentTypeList.find(typeName) == mComponentTypeList.end() , "Registering component type more than once.");
		mComponentTypeList.insert({typeName, mComponentSignature});
		mComponentList.insert({typeName, std::make_unique<ComponentList<T>>()});

		++mComponentSignature;
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		std::string typeName = typeid(T).name();

		ARTLESS_ASSERT(mComponentTypeList.find(typeName) != mComponentTypeList.end() , "Component not registered before use.");
		//assert(mComponentTypeList.find(typeName) != mComponentTypeList.end() && "Component not registered before use.");
		return mComponentTypeList[typeName];
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		GetComponentList<T>()->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		GetComponentList<T>()->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentList<T>()->GetData(entity);
	}

	template<typename T>
	bool CheckData(Entity entity)
	{
		return GetComponentList<T>()->CheckData(entity);
	}

	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : mComponentList)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
		}
	}
};