/*****************************************************************
*\file		 EntityManager.h
*\brief		 ECS Entity Manager that manage all the Entity

*\author(s)   Kenric Tan Wei Liang		15 lines x 33.33% Code contribution
*\author(s)   Lee Jun Jie				16 lines x 33.33% Code contribution
*\author(s)   Lee Liang Ping			16 lines x 33.33% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include <GameTypes.h>
#include <EnginePch.h>

class EntityManager
{

	std::queue<Entity> mEntitiesList {};
	std::set<Entity> mLivingEntitiesList {};
	std::array<BitSignature, MAX_ENTITIES> mEntitiesSignatureList {};
	Size mActiveEntitiesCount {};

public:
	EntityManager()
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
			mEntitiesList.push(entity);
	}

	std::set<Entity> &List()
	{
		return mLivingEntitiesList;
	}

	Entity CreateEntity()
	{
		ARTLESS_ASSERT(mActiveEntitiesCount < MAX_ENTITIES , "Too many entities in existence.");

		Entity id = mEntitiesList.front();
		mEntitiesList.pop();
		++mActiveEntitiesCount;
		mLivingEntitiesList.insert(id);
		return id;
	}

	void DestroyEntity(Entity entity)
	{
		ARTLESS_ASSERT(entity < MAX_ENTITIES , "Entity out of range.");

		mEntitiesSignatureList[entity].reset();
		mEntitiesList.push(entity);
		--mActiveEntitiesCount;
		mLivingEntitiesList.erase( entity );
	}

	void SetBitSignature(Entity entity, BitSignature signature)
	{
		ARTLESS_ASSERT(entity < MAX_ENTITIES , "Entity out of range.");

		mEntitiesSignatureList[entity] = signature;
	}

	BitSignature GetBitSignature(Entity entity)
	{
		ARTLESS_ASSERT(entity < MAX_ENTITIES , "Entity out of range.");

		return mEntitiesSignatureList[entity];
	}
};