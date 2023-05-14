/*****************************************************************
*\file		 CollisionList.cpp
*\brief		 This file contains Collision List, keep tracks with 
			 colliable list.

*\author(s)   Lee Liang Ping	81 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <CollisionList.h>

/******************************************************************************/
/*!
 \fn std::vector<Entity> CollisionList::GetCollisionParticleList(EntitySystemOption systemOption, CollisionType cType)

 \brief Get a copy of the list of CollisionType's Entity

 \param systemOption
		cType
 \return vector list of entity.
*/
/******************************************************************************/
std::vector<Entity> CollisionList::GetCollisionParticleList(EntitySystemOption systemOption, CollisionType cType)
{
	//Future Use
	//std::vector<Entity> returnList;
	//for (auto& x : collisionList.find(cType)->second)
	//{
	//	if (systemOption == x.first)
	//		returnList.push_back(x.second);
	//}
	//return returnList;

	std::vector<Entity> returnList;
	if (CollisionType::ALL_COLLISIONTYPE == cType)
	{
		for (auto& x : collisionParticleList)
			returnList.push_back(std::get<Entity>(x));
		return returnList;
	}
	for (auto& x : collisionParticleList)
	{
		if (systemOption == std::get<EntitySystemOption>(x)
			&& cType == std::get<CollisionType>(x))
			returnList.push_back(std::get<Entity>(x));
	}
	return returnList;
}

/******************************************************************************/
/*!
 \fn void CollisionList::AddCollisionParticleList(Entity entity, EntitySystemOption systemOption, CollisionType cType)

 \brief Add Entity id to the Collision List

 \param entity
		systemOption
		cType
 \return none
*/
/******************************************************************************/
void CollisionList::AddCollisionParticleList(Entity entity, EntitySystemOption systemOption, CollisionType cType)
{
	//Future Use
	//if (collisionList.find(cType) == collisionList.end())
	//	collisionList.insert({ cType, EntityListW() });
	//collisionList.find(cType)->second.push_back(std::make_pair(systemOption, entity));

	for (auto& x : collisionParticleList)
	{
		if (std::get<Entity>(x) == entity
			&& cType == std::get<CollisionType>(x)
			&& systemOption == std::get<EntitySystemOption>(x))
		{
			return;
		}
	}
	collisionParticleList.push_back(std::make_tuple(entity, systemOption, cType));
}

/******************************************************************************/
/*!
 \fn void CollisionList::ChangeExistingCollisionType(Entity entity, EntitySystemOption systemOption, CollisionType cType)

 \brief Change Collision Type of the selected Entity and Collision Type

 \param entity
		systemOption
		cType
 \return none
*/
/******************************************************************************/
void CollisionList::ChangeExistingCollisionType(Entity entity, EntitySystemOption systemOption, CollisionType cType)
{
	for (auto& x : collisionParticleList)
	{
		if (entity == std::get<Entity>(x) && systemOption == std::get<EntitySystemOption>(x))
			std::get<CollisionType>(x) = cType;
	}
}

/******************************************************************************/
/*!
 \fn void CollisionList::RemoveCollisionList(Entity entity, EntitySystemOption systemOption, CollisionType ctype)

 \brief Change Entity from the Collision List

 \param entity
		systemOption
		cType
 \return none
*/
/******************************************************************************/
void CollisionList::RemoveCollisionList(Entity entity, EntitySystemOption systemOption, CollisionType ctype)
{
	for (auto it = collisionParticleList.begin(); it != collisionParticleList.end();)
	{
		if (std::get<Entity>(*it) == entity
			&& ctype == std::get<CollisionType>(*it)
			&& systemOption == std::get<EntitySystemOption>(*it))
			it = collisionParticleList.erase(it);
		else
			++it;
	}
}

/******************************************************************************/
/*!
 \fn void CollisionList::PrintCollisionList(EntitySystemOption systemOption, CollisionType ctype)

 \brief For debug purposes, to cout the entity id that matches the CollisionType

 \param systemOption
		cType
 \return none
*/
/******************************************************************************/
void CollisionList::PrintCollisionList(EntitySystemOption systemOption, CollisionType ctype)
{
	if (CollisionType::ALL_COLLISIONTYPE == ctype)
	{
		for (auto& x : collisionParticleList)
			std::cout <<"Collision Type: "
			<< EnumToString::Get(std::get<CollisionType>(x))
			<< " Entity Id: "<<(int)std::get<Entity>(x) 
			<< " at " << EnumToString::Get(std::get<EntitySystemOption>(x))
			<< std::endl;
	}
	else
	{
		for (auto& x : collisionParticleList)
		{
			if (systemOption == std::get<EntitySystemOption>(x)
				&& ctype == std::get<CollisionType>(x))
				std::cout << "Collision Type: "
				<< EnumToString::Get(std::get<CollisionType>(x))
				<< " Entity Id: " << (int)std::get<Entity>(x)
				<< " at " << EnumToString::Get(std::get<EntitySystemOption>(x))
				<< std::endl;
		}
	}

}

/******************************************************************************/
/*!
 \fn void CollisionList::Reset()

 \brief Reset Collision List

 \return none
*/
/******************************************************************************/
void CollisionList::Reset()
{
	collisionParticleList.clear();
}