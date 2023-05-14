/*****************************************************************
*\file		 ComponentList.h
*\brief		 Data Stroage of all comoponent List

*\author(s)   Kenric Tan Wei Liang		19 lines x 33.33% Code contribution
*\author(s)   Lee Jun Jie				19 lines x 33.33% Code contribution
*\author(s)   Lee Liang Ping			19 lines x 33.33% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <GameTypes.h>



class IComponentList
{
public:
	virtual ~IComponentList() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};


template<typename T>
class ComponentList : public IComponentList
{
	std::array<T, MAX_ENTITIES> mEntitiesComponentList{};
	std::unordered_map<Entity, Index> mEntityToIndexMap{};
	std::unordered_map<Index, Entity> mIndexToEntityMap{};
	Size mComponentsCount{};

public:
	void InsertData(Entity entity, T component)
	{
		ARTLESS_ASSERT(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end(), "Component added to same entity more than once.");

		// Put new entry at end
		Index newIndex = mComponentsCount;
		mEntityToIndexMap[entity] = newIndex;
		mIndexToEntityMap[newIndex] = entity;
		mEntitiesComponentList[newIndex] = component;
		++mComponentsCount;
	}

	void RemoveData(Entity entity)
	{
		ARTLESS_ASSERT(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end(), "Removing non-existent component.");

		// Copy element at end into deleted element's place to maintain density
		Index indexOfRemovedEntity = mEntityToIndexMap[entity];
		Index indexOfLastElement = mComponentsCount - 1;
		mEntitiesComponentList[indexOfRemovedEntity] = mEntitiesComponentList[indexOfLastElement];

		// Update map to point to moved spot
		Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
		mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		mEntityToIndexMap.erase(entity);
		mIndexToEntityMap.erase(indexOfLastElement);

		--mComponentsCount;
	}

	T& GetData(Entity entity)
	{
		ARTLESS_ASSERT(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end(), "Retrieving non-existent component.");

		return mEntitiesComponentList[mEntityToIndexMap[entity]];
	}

	bool CheckData(Entity entity)
	{
		if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
			return false;
		else
			return true;
	}

	void EntityDestroyed(Entity entity) override
	{
		if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
			RemoveData(entity);
	}
};