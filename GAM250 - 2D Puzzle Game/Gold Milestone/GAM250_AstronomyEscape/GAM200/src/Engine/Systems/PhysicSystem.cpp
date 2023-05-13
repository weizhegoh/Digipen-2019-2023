/*****************************************************************
*\file		 PhysicSystem.cpp
*\brief		 PhysicSystem functions definitions.

*\author(s)   Lee Jun Jie	86 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <PhysicSystem.h>
#include <FakeEngine.h>
#include <Collision.h>
#include "../Engine/Serialization/Serialization.h"


extern std::unique_ptr<AudioEngine> gAudioEngine;
extern std::unique_ptr<FakeEngine> gFakeEngine;
extern float g_dt;
void PhysicSystem::Init(EntitySystemOption systemoption)
{
	auto& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(systemoption);
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	signature.set(mGameECSmanager.GetComponentType<Rigidbody>());
	signature.set(mGameECSmanager.GetComponentType<Position>());
	signature.set(mGameECSmanager.GetComponentType<Force>());
	mGameECSmanager.SetSystemBitSignature<PhysicSystem>(signature);
}

void PhysicSystem::Update(EntitySystemOption systemoption)
{
	auto& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(systemoption);
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	GlobalVar& globalvar = gFakeEngine->GetGameManager<GlobalVar>();
	Boolean& boolean = gFakeEngine->GetGameManager<Boolean>();
	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;

		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		Position& position = mGameECSmanager.GetComponent<Position>(entity);

		Rigidbody& rigidbody = mGameECSmanager.GetComponent<Rigidbody>(entity);
		Force& force = mGameECSmanager.GetComponent<Force>(entity);
		if (objecttype.objType == ObjectType::MAIN_CHARACTER ||
			objecttype.objType == ObjectType::PUSHABLE_BARREL ||
			objecttype.objType == ObjectType::MONSTER || 
			objecttype.objType == ObjectType::MONSTER_PATROL ||
			objecttype.objType == ObjectType::MONSTER_GUIDE)
		{
			if (!boolean.Check(BooleanType::MOVEMENT_LOCK) && objecttype.objType == ObjectType::MAIN_CHARACTER)
				continue;
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			// Apply Friction
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			if (objecttype.objType != ObjectType::PUSHABLE_BARREL &&
				objecttype.objType != ObjectType::PARTICLE)
			{
				State& state = mGameECSmanager.GetComponent<State>(entity);
				if (state.objectState == ObjectState::IDLE)
					rigidbody.velocity = mathEngine::Vector3D(0.f, 0.f, 0.f);
				else
					rigidbody.Integrate(force.mResultantForce, g_dt);
			}

			if (objecttype.objType == ObjectType::MAIN_CHARACTER ||
				objecttype.objType == ObjectType::MONSTER ||
				objecttype.objType == ObjectType::MONSTER_PATROL ||
				objecttype.objType == ObjectType::MONSTER_GUIDE)
			{
				if (globalvar.GetAndConvertCellValue((int)position.pos.x, (int)position.pos.y) != ObjectType::FLOOR)
				{
					if (objecttype.objType == ObjectType::MAIN_CHARACTER)
						if (!globalvar.door.empty())
							globalvar.door.pop_back();

					position.pos = position.starting_pos;
				}
					
			}
			

		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// Apply Epsioln
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		rigidbody.Epsilon();
		position.Update(rigidbody.velocity, g_dt);

		//if (objecttype.objType == ObjectType::MAIN_CHARACTER)
		//{
		//	State& state = mGameECSmanager.GetComponent<State>(entity);
		//	if (state.objectDir == ObjectDirection::RIGHT || state.objectDir == ObjectDirection::LEFT)
		//	{
		//		globalvar.ConvertScreenToBinary(position.pos);
		//		Collision::HotSpot::SnapToCell(position.pos.y);
		//		globalvar.ConvertBinaryToScreen(position.pos);
		//	}
		//	else if (state.objectDir == ObjectDirection::UP || state.objectDir == ObjectDirection::DOWN)
		//	{
		//		globalvar.ConvertScreenToBinary(position.pos);
		//		Collision::HotSpot::SnapToCell(position.pos.x);
		//		globalvar.ConvertBinaryToScreen(position.pos);
		//	}
		//	else
		//	{
		//		globalvar.ConvertScreenToBinary(position.pos);
		//		Collision::HotSpot::SnapToCell(position.pos.x);
		//		Collision::HotSpot::SnapToCell(position.pos.y);
		//		globalvar.ConvertBinaryToScreen(position.pos);
		//	}
		//}

		if (systemoption == EntitySystemOption::GAME_ENTITY)
		{
			Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
			if (input.GetInputBooleen().rotate_button)
			{
				transform.angleoffset += 30.f * g_dt;
				if (transform.angleoffset >= 360.f)
					transform.angleoffset = 0;
			}
			else
				transform.angleoffset = 0.f;
			if (input.GetInputBooleen().scale_button)
			{
				transform.scaleoffset.y = transform.scaleoffset.x += 5.f * g_dt;
				if (transform.scaleoffset.x >= 50.f)
					transform.scaleoffset.y = transform.scaleoffset.x = 0.f;
			}
		}

	}
}

