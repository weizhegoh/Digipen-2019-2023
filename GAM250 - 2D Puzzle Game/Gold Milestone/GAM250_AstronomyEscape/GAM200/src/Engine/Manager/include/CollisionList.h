/*****************************************************************
*\file		 CollisionList.h
*\brief		 This file contains Collision List, keep tracks with
			 colliable list.

*\author(s)   Lee Liang Ping	15 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <EnginePch.h>
#include <GameTypes.h>

/******************************************************************************/
/*!
 \class CollisionList;

 \brief List of Collision store the enitiy with the set of CollisionType 
 Note* Potentially reduce the amount of collisions checks.

 \param collisionParticleList
*/
/******************************************************************************/
class CollisionList
{
	using TupleCollisionType = std::tuple<Entity, EntitySystemOption, CollisionType>;
	std::vector<TupleCollisionType> collisionParticleList;

	//Future Use
	//using EntityListW = std::vector<std::pair<EntitySystemOption, Entity>>;
	//using CollisionMap= std::unordered_map<CollisionType, EntityListW>;
	//using CollisionIterator = CollisionMap::iterator;
	//CollisionMap collisionList;

public:
	// Return copy of filtered Entity
	std::vector<Entity> GetCollisionParticleList(EntitySystemOption systemOption, CollisionType cType);
	void AddCollisionParticleList(Entity entity, EntitySystemOption systemOption, CollisionType cType);
	void ChangeExistingCollisionType(Entity entity, EntitySystemOption systemOption, CollisionType cType);
	void RemoveCollisionList(Entity entity, EntitySystemOption systemOption, CollisionType ctype);
	void PrintCollisionList(EntitySystemOption systemOption, CollisionType ctype);
	void Reset();
};
