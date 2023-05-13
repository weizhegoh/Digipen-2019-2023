/*****************************************************************
*\file		 AiSystem.cpp
*\brief		 AI System Logic

*\author(s)   Kenric Tan Wei Liang		122 lines x 50% Code contribution
*\author(s)   Justin Yip Ling Kit 		122 lines x 50% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <AiSystem.h>
#include <Collision.h>
#include <EnumToString.h>
#include <FakeEngine.h>
extern std::unique_ptr<FakeEngine> gFakeEngine;

ObjectDirection switch_turn(TURN_TYPE& turn_type, ObjectDirection& objectDir) {
	switch (turn_type)
	{
	case TURN_TYPE::TURN_BACK:
		switch (objectDir)
		{
		case ObjectDirection::UP:
			return ObjectDirection::DOWN;
			break;
		case ObjectDirection::DOWN:
			return ObjectDirection::UP;
			break;
		case ObjectDirection::LEFT:
			return ObjectDirection::RIGHT;
			break;
		default: //case ObjectDirection::RIGHT:
			return ObjectDirection::LEFT;
			break;
		}
		break;
	case TURN_TYPE::TURN_LEFT:
		switch (objectDir)
		{
		case ObjectDirection::UP:
			return ObjectDirection::LEFT;
			break;
		case ObjectDirection::DOWN:
			return ObjectDirection::RIGHT;
			break;
		case ObjectDirection::LEFT:
			return ObjectDirection::DOWN;
			break;
		default: //case ObjectDirection::RIGHT:
			return ObjectDirection::UP;
			break;
		}
		break;
	case TURN_TYPE::TURN_RIGHT:
		switch (objectDir)
		{
		case ObjectDirection::UP:
			return ObjectDirection::RIGHT;
			break;
		case ObjectDirection::DOWN:
			return ObjectDirection::LEFT;
			break;
		case ObjectDirection::LEFT:
			return ObjectDirection::UP;
			break;
		default: //case ObjectDirection::RIGHT:
			return ObjectDirection::DOWN;
			break;
		}
		break;
	default:
		return ObjectDirection::NO_DIR;
		break;
	}
}

void AiSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	//signature.set(mGameECSmanager.GetComponentType<EntityType>());
	signature.set(mGameECSmanager.GetComponentType<AI>());
	mGameECSmanager.SetSystemBitSignature<AiSystem>(signature);
}

void AiSystem::AILogic(Entity entity)
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	State& state = mGameECSmanager.GetComponent<State>(entity);
	AI& ai = mGameECSmanager.GetComponent<AI>(entity);
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	Position pos = mGameECSmanager.GetComponent<Position>(entity);
	Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);

	std::random_device rd; //creating a random device to be stored in the default random engine
	std::default_random_engine dre(rd());
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> urd_dir(0, 4);
	std::uniform_real_distribution<float> urd_time(2, 4);

	mathEngine::Vector3D check_leftpoint = mathEngine::Vector3D(pos.pos.x - transform.scale.x * 0.49f, pos.pos.y, 0.f);
	mathEngine::Vector3D check_rightpoint = mathEngine::Vector3D(pos.pos.x + transform.scale.x * 0.49f, pos.pos.y, 0.f);
	mathEngine::Vector3D check_toppoint = mathEngine::Vector3D(pos.pos.x, pos.pos.y + transform.scale.y * 0.49f, 0.f);
	mathEngine::Vector3D check_bottompoint = mathEngine::Vector3D(pos.pos.x, pos.pos.y - transform.scale.y * 0.49f, 0.f);
	globalVar.ConvertScreenToBinary(check_leftpoint);
	globalVar.ConvertScreenToBinary(check_rightpoint);
	globalVar.ConvertScreenToBinary(check_toppoint);
	globalVar.ConvertScreenToBinary(check_bottompoint);

	if (ai.time <= 0)
	{
		ai.time = urd_time(dre);
		state.objectDir = static_cast<ObjectDirection>(urd_dir(dre));
	}
	ai.time -= g_dt;
}

void AiSystem::ChangeState(ObjectDirection objdir, Entity entity)
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	State& state = mGameECSmanager.GetComponent<State>(entity);
	AI& ai = mGameECSmanager.GetComponent<AI>(entity);
	ForceManager& forceManager = gFakeEngine->GetGameManager<ForceManager>();

	switch (objdir)
	{
	/*case ObjectDirection::NO_DIR:
		forceManager.removeforce(entity);
		break;*/
	case ObjectDirection::UP:
		//rigidbody.velocity = glm::vec3(0.f, rigidbody.acceleration.y, 0.f);
		forceManager.addforce(entity, mathEngine::Vector3D(0.f, 1500.f, 0.f), 5.f);
		break;
	case ObjectDirection::DOWN:
		//rigidbody.velocity = glm::vec3(0.f, -rigidbody.acceleration.y, 0.f);
		forceManager.addforce(entity, mathEngine::Vector3D(0.f, -1500.f, 0.f), 5.f);
		break;
	case ObjectDirection::LEFT:
		//rigidbody.velocity = glm::vec3(-rigidbody.acceleration.x, 0.f, 0.f);
		forceManager.addforce(entity, mathEngine::Vector3D(-1500.f, 0.f, 0.f), 5.f);
		break;
	case ObjectDirection::RIGHT:
		//rigidbody.velocity = glm::vec3(rigidbody.acceleration.x, 0.f, 0.f);
		forceManager.addforce(entity, mathEngine::Vector3D(1500.f, 0.f, 0.f), 5.f);
		break;
	default: //case ObjectDirection::NO_DIR;
		if (mGameECSmanager.GetComponent<EntityType>(entity).objType == ObjectType::MONSTER)
		//rigidbody.velocity = glm::vec3(0.f, 0.f, 0.f);
			state.objectDir = ai.start_dir;
		else
			forceManager.removeforce(entity);

		break;
	}
}

void AiSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;
		EntityType entity_type = mGameECSmanager.GetComponent<EntityType>(entity);
		switch (entity_type.objType) {
		//Patrolling Monster Type
		case ObjectType::MONSTER_PATROL: {
			State& state = mGameECSmanager.GetComponent<State>(entity);
			state.objectState = ObjectState::MONSTER_MOVING;
			AILogic(entity);
			ChangeState(state.objectDir, entity);
			if (state.objectDir == ObjectDirection::NO_DIR)
				state.objectState = ObjectState::MONSTER_IDLE;
		}
			break;

		//Guiding Monster
		case ObjectType::MONSTER_GUIDE: {
			GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
			Position pos = mGameECSmanager.GetComponent<Position>(entity);
			State& state = mGameECSmanager.GetComponent<State>(entity);
			Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
			state.objectState = ObjectState::MONSTER_MOVING;
			ChangeState(state.objectDir, entity);
			if (state.objectDir == ObjectDirection::NO_DIR)
				state.objectDir = ObjectDirection::RIGHT;

			mathEngine::Vector3D check_leftpoint = mathEngine::Vector3D(pos.pos.x - transform.scale.x * 0.49f, pos.pos.y, 0.f);
			mathEngine::Vector3D check_rightpoint = mathEngine::Vector3D(pos.pos.x + transform.scale.x * 0.49f, pos.pos.y, 0.f);
			mathEngine::Vector3D check_toppoint = mathEngine::Vector3D(pos.pos.x, pos.pos.y + transform.scale.y * 0.49f, 0.f);
			mathEngine::Vector3D check_bottompoint = mathEngine::Vector3D(pos.pos.x, pos.pos.y - transform.scale.y * 0.49f, 0.f);
			globalVar.ConvertScreenToBinary(check_leftpoint);
			globalVar.ConvertScreenToBinary(check_rightpoint);
			globalVar.ConvertScreenToBinary(check_toppoint);
			globalVar.ConvertScreenToBinary(check_bottompoint);
			if (state.objectDir == ObjectDirection::RIGHT)
			{
				if (globalVar.GetCellValue((int)(check_leftpoint.x + 1), (int)check_leftpoint.y) != ObjectType::FLOOR)
				{
					if (globalVar.GetCellValue((int)(check_bottompoint.x), (int)check_bottompoint.y - 1) == ObjectType::FLOOR)
						state.objectDir = ObjectDirection::UP;
					else
						state.objectDir = ObjectDirection::DOWN;
				}
			}
			else if (state.objectDir == ObjectDirection::DOWN)
			{
				if (globalVar.GetCellValue((int)(check_toppoint.x), (int)check_toppoint.y + 1) != ObjectType::FLOOR)
				{
					if (globalVar.GetCellValue((int)(check_leftpoint.x + 1), (int)check_leftpoint.y) == ObjectType::FLOOR)
						state.objectDir = ObjectDirection::RIGHT;
					else
						state.objectDir = ObjectDirection::LEFT;
				}
			}
			else if (state.objectDir == ObjectDirection::LEFT)
			{
				if (globalVar.GetCellValue((int)(check_rightpoint.x - 1), (int)check_rightpoint.y) != ObjectType::FLOOR)
				{
					if (globalVar.GetCellValue((int)(check_toppoint.x), (int)check_toppoint.y + 1) == ObjectType::FLOOR)
						state.objectDir = ObjectDirection::DOWN;
					else
						state.objectDir = ObjectDirection::UP;
				}
			}
			else if (state.objectDir == ObjectDirection::UP)
			{
				if (globalVar.GetCellValue((int)(check_bottompoint.x), (int)check_bottompoint.y - 1) != ObjectType::FLOOR)
				{
					if (globalVar.GetCellValue((int)(check_rightpoint.x - 1), (int)check_rightpoint.y) == ObjectType::FLOOR)
						state.objectDir = ObjectDirection::LEFT;
					else
						state.objectDir = ObjectDirection::RIGHT;
				}
			}
		}
			break;
		case ObjectType::MONSTER: {
			///***************************************************************************
			//	This is probably not how you want it implemented, but I'm doing it
			//	this way so that i can see the code working. Lots of it are going
			//	to be lifted from other code
			//***************************************************************************/
			State& state = mGameECSmanager.GetComponent<State>(entity);
			Rigidbody& rigidbody = mGameECSmanager.GetComponent<Rigidbody>(entity);
			AI& ai = mGameECSmanager.GetComponent<AI>(entity);
			//ForceManager& forceManager = gFakeEngine->GetGameManager<ForceManager>();
			//Update Positioning
			state.objectState = ObjectState::MONSTER_MOVING;
			ChangeState(state.objectDir, entity);
			//Update delay
			if (ai.turn_delay > 0.0f)
				ai.turn_delay -= g_dt;

			//Check if there is a corner to turn
			if (ai.corner_turn != TURN_TYPE::TURN_NULL && ai.turn_delay <= 0.0f) {
				Position& position = mGameECSmanager.GetComponent<Position>(entity);
				GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();

				Force& force = mGameECSmanager.GetComponent<Force>(entity);
				rigidbody.Integrate(force.mResultantForce, g_dt);

				mathEngine::Vector3D direction_check(rigidbody.velocity.x, rigidbody.velocity.y, rigidbody.velocity.z);
				if (ai.corner_turn == TURN_TYPE::TURN_BACK)
					direction_check *= -1;
				else {//TURN_LEFT OR TURN_RIGHT

					float rotation = PI / 2 * (ai.corner_turn == TURN_TYPE::TURN_RIGHT ? -1 : 1);

					direction_check.normalize();
					direction_check.x = direction_check.x * cos(rotation) - direction_check.y * sin(rotation);
					direction_check.y = direction_check.x * sin(rotation) + direction_check.y * cos(rotation);
				}

				//direction_check += position.pos;
				direction_check += position.pos;
				//globalVar.ConvertScreenToBinary(direction_check);

				if (globalVar.GetAndConvertCellValue(direction_check) == ObjectType::FLOOR) {
					Collision::HotSpot::SnapToCell(position.pos.x);
					Collision::HotSpot::SnapToCell(position.pos.y);
					state.objectDir = switch_turn(ai.corner_turn, state.objectDir);
					//rigidbody.velocity = mathEngine::Vector3D(0, 0, 0);
					ai.turn_delay = ai.turn_delay_max;
				}
				else if (globalVar.GetAndConvertCellValue(direction_check) == ObjectType::OUTOFBOUND)
				{
				}
			}
		}
			break;
		default: //Has nothing to do with this system
			break;
		}
		//your function code  etc
	}
}

void AiSystem::AITurn(Entity entity)
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	State& state = mGameECSmanager.GetComponent<State>(entity);
	AI& ai = mGameECSmanager.GetComponent<AI>(entity);
	//ai.turn_delay = ai.turn_delay_max;

	state.objectDir = switch_turn(ai.turn_type, state.objectDir);
}