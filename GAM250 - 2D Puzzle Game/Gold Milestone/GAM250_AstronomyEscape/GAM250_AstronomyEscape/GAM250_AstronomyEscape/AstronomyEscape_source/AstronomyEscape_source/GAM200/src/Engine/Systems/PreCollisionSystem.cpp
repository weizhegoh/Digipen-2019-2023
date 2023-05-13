/*****************************************************************
*\file		 PreCollisionSystem.cpp
*\brief		 PreCollisionSystem functions definitions.

*\author(s)  Lee Liang Ping		44 lines x 50% Code contribution
*\author(s)   Lee Jun Jie		44 lines x 50% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <PreCollisionSystem.h>
#include <FakeEngine.h>


extern std::unique_ptr<FakeEngine> gFakeEngine;

void PreCollisionSystem::Init(EntitySystemOption systemOption)
{
	auto& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(systemOption);
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<AABB>());
	signature.set(mGameECSmanager.GetComponentType<Flags>());
	mGameECSmanager.SetSystemBitSignature<PreCollisionSystem>(signature);
}

void PreCollisionSystem::Update(EntitySystemOption systemOption)
{	
	auto& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(systemOption);
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	int totalEntityCount = 0;
	int totalActiveCount = 0;

	for (auto const& entity : mEntities)
	{
		totalEntityCount++;

		EntityType& entityType = mGameECSmanager.GetComponent<EntityType>(entity);
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		Position position = mGameECSmanager.GetComponent<Position>(entity);

		if (entityType.objType == ObjectType::ENTRANCE_DOOR && (0 == (flag.activeFlag & FLAG_ACTIVE)))
		{
			globalVar.ConvertScreenToBinary(position.pos);
			globalVar.SetCellValue(position.pos, ObjectType::FLOOR);
		}
		else if (entityType.objType == ObjectType::ENTRANCE_DOOR && (0 == (flag.activeFlag & FLAG_INACTIVE)))
		{
			globalVar.ConvertScreenToBinary(position.pos);
			globalVar.SetCellValue(position.pos, ObjectType::ENTRANCE_DOOR);
		}

		if (entityType.objType == ObjectType::EXIT_DOOR && (0 == (flag.activeFlag & FLAG_ACTIVE)))
		{
			globalVar.ConvertScreenToBinary(position.pos);
			globalVar.SetCellValue(position.pos, ObjectType::FLOOR);
		}
		else if (entityType.objType == ObjectType::EXIT_DOOR && (0 == (flag.activeFlag & FLAG_INACTIVE)))
		{
			globalVar.ConvertScreenToBinary(position.pos);
			globalVar.SetCellValue(position.pos, ObjectType::EXIT_DOOR);
		}


		if (entityType.objType == ObjectType::LEVER_DOOR && (0 == (flag.activeFlag & FLAG_ACTIVE)))
		{
			globalVar.ConvertScreenToBinary(position.pos);
			globalVar.SetCellValue(position.pos, ObjectType::FLOOR);
		}
		else if (entityType.objType == ObjectType::LEVER_DOOR && (0 == (flag.activeFlag & FLAG_INACTIVE)))
		{
			globalVar.ConvertScreenToBinary(position.pos);
			globalVar.SetCellValue(position.pos, ObjectType::LEVER_DOOR);
		}

		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;
		totalActiveCount++;

		Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
		AABB& aabb = mGameECSmanager.GetComponent<AABB>(entity);
		//Setup for AABB bounding box

		aabb.Update(position.pos, transform.scale);


		if (entityType.objType == ObjectType::MAIN_CHARACTER
			|| entityType.objType == ObjectType::MONSTER 
			|| entityType.objType == ObjectType::MONSTER_PATROL
			|| entityType.objType == ObjectType::MONSTER_GUIDE)
		{
			Position& pos = mGameECSmanager.GetComponent<Position>(entity);
			pos.prev_pos = pos.pos;
			pos.prev_binarypos = globalVar.GetConvertScreenToBinary(pos.pos);

			if (entityType.objType == ObjectType::MAIN_CHARACTER)
			{
				Inventory& inventory = mGameECSmanager.GetComponent<Inventory>(entity);
				inventory.collisionArea.min = {
					-inventory.collisionArea.size.x * transform.scale.x + position.pos.x,
					-inventory.collisionArea.size.y * transform.scale.y + position.pos.y };
				inventory.collisionArea.max = {
					inventory.collisionArea.size.x * transform.scale.x + position.pos.x,
					inventory.collisionArea.size.y * transform.scale.y + position.pos.y };
			}
		}
	}

	if (systemOption == EntitySystemOption::GAME_ENTITY)
	{
		GameCounter& gameCounter = gFakeEngine->GetGameManager<GameCounter>();
		gameCounter.totalEntityCount = totalEntityCount;
		gameCounter.totalActiveCount = totalActiveCount;
	}
	else
	{
		ParticlesInfo& particlesinfo = gFakeEngine->GetGameManager<ParticlesInfo>();
		particlesinfo.totalParticles = totalEntityCount;
		particlesinfo.activeParticles = totalActiveCount;
	}
}