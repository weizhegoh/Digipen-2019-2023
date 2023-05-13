/*****************************************************************
*\file		 HotSpotCollisionSystem.cpp
*\brief		 HotSpotCollisionSystem functions definitions.

*\author(s)   Lee Liang Ping	118 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <HotSpotCollisionSystem.h>
#include <FakeEngine.h>
#include <Collision.h>
#include <AiSystem.h>


extern std::unique_ptr<FakeEngine> gFakeEngine;

void HotSpotCollisionSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	signature.set(mGameECSmanager.GetComponentType<AABB>());
	signature.set(mGameECSmanager.GetComponentType<Flags>());
	signature.set(mGameECSmanager.GetComponentType<CollisionFlag>());
	mGameECSmanager.SetSystemBitSignature<HotSpotCollisionSystem>(signature);
}

void HotSpotCollisionSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;
		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);

		if (objecttype.objType == ObjectType::MAIN_CHARACTER
			|| objecttype.objType == ObjectType::PUSHABLE_BARREL
			|| objecttype.objType == ObjectType::MONSTER
			|| objecttype.objType == ObjectType::MONSTER_PATROL
			|| objecttype.objType == ObjectType::MONSTER_GUIDE)
		{
			CollisionFlag& cflag = mGameECSmanager.GetComponent<CollisionFlag>(entity);
			Rigidbody& rigidbody = mGameECSmanager.GetComponent<Rigidbody>(entity);
			Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
			Position& position = mGameECSmanager.GetComponent<Position>(entity);
			//State& state = mGameECSmanager.GetComponent<State>(entity);
			bool check = false;

			cflag.gridCollisionFlag =
				Collision::HotSpot::CheckInstanceBinaryMapCollision(
					position.pos.x, position.pos.y,
					transform.scale.x, transform.scale.y);


			if (cflag.gridCollisionFlag != 0)
			{
				Position binarypos = position;

				globalVar.ConvertScreenToBinary(binarypos.pos.x, binarypos.pos.y);
				if (cflag.gridCollisionFlag & COLLISION_BOTTOM)
				{
					rigidbody.velocity.y = 0.f;
					Collision::HotSpot::SnapToCell(binarypos.pos.y);

					if (objecttype.objType == ObjectType::MONSTER
						&& mGameECSmanager.GetComponent<State>(entity).objectDir == ObjectDirection::DOWN)
						AiSystem::AITurn(entity);

					//if (objecttype.objType == ObjectType::MONSTER_PATROL)
					//{
					//	AI& ai = mGameECSmanager.GetComponent<AI>(entity);
					//	ai.time = 0;
					//}
						
					check = true;
				}
				//check for collision top sides 
				if (cflag.gridCollisionFlag & COLLISION_TOP)
				{
					rigidbody.velocity.y = 0.f;
					Collision::HotSpot::SnapToCell(binarypos.pos.y);

					if (objecttype.objType == ObjectType::MONSTER
						&& mGameECSmanager.GetComponent<State>(entity).objectDir == ObjectDirection::UP)
						AiSystem::AITurn(entity);

					//if (objecttype.objType == ObjectType::MONSTER_PATROL)
					//{
					//	AI& ai = mGameECSmanager.GetComponent<AI>(entity);
					//	ai.time = 0;
					//}

					check = true;
				}
				//check for collision left sides 
				if (cflag.gridCollisionFlag & COLLISION_LEFT)
				{
					rigidbody.velocity.x = 0.f;
					Collision::HotSpot::SnapToCell(binarypos.pos.x);

					if (objecttype.objType == ObjectType::MONSTER
						&& mGameECSmanager.GetComponent<State>(entity).objectDir == ObjectDirection::LEFT)
						AiSystem::AITurn(entity);

					//if (objecttype.objType == ObjectType::MONSTER_PATROL)
					//{
					//	AI& ai = mGameECSmanager.GetComponent<AI>(entity);
					//	ai.time = 0;
					//}

					check = true;
				}
				//check for collision right sides 
				if (cflag.gridCollisionFlag & COLLISION_RIGHT)
				{
					rigidbody.velocity.x = 0.f;
					Collision::HotSpot::SnapToCell(binarypos.pos.x);

					if (objecttype.objType == ObjectType::MONSTER
						&& mGameECSmanager.GetComponent<State>(entity).objectDir == ObjectDirection::RIGHT)
						AiSystem::AITurn(entity);

					//if (objecttype.objType == ObjectType::MONSTER_PATROL)
					//{
					//	AI& ai = mGameECSmanager.GetComponent<AI>(entity);

					//	ai.time = 0;
					//}
					check = true;
				}
				globalVar.ConvertBinaryToScreen(binarypos.pos.x, binarypos.pos.y);
				position = binarypos;


			}
		}
	}
}


void HotSpotCollisionSystem::ResetPosition()
{
	GlobalVar& globalvar = gFakeEngine->GetGameManager<GlobalVar>();
	globalvar.SetZeroData();

	auto& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(EntitySystemOption::GAME_ENTITY);
	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;
		Position& position = mGameECSmanager.GetComponent<Position>(entity);
		position.pos = position.starting_pos;

		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);

		Position binposition;
		binposition.pos = globalvar.GetConvertScreenToBinary(position.pos);

		if (objecttype.objType == ObjectType::PUSHABLE_BARREL)
			globalvar.SetCellValue(binposition.pos, ObjectType::PUSHABLE_BARREL);
		else if (objecttype.objType == ObjectType::WALL)
			globalvar.SetCellValue(binposition.pos, ObjectType::WALL);
		else if (objecttype.objType == ObjectType::TURRET)
			globalvar.SetCellValue(binposition.pos, ObjectType::TURRET);
		else if (objecttype.objType == ObjectType::LEVEL_DOOR)
			globalvar.SetCellValue(binposition.pos, ObjectType::LEVEL_DOOR);
		else if (objecttype.objType == ObjectType::TELEPORTER)
			globalvar.SetCellValue(binposition.pos, ObjectType::TELEPORTER);
		else if (objecttype.objType == ObjectType::LEVER_DOOR)
			globalvar.SetCellValue(binposition.pos, ObjectType::LEVER_DOOR);
		else if (objecttype.objType == ObjectType::LEVER_SWITCH)
			globalvar.SetCellValue(binposition.pos, ObjectType::LEVER_SWITCH);
	}

}