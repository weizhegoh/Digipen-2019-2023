/*****************************************************************
*\file		 CollisionSystem.cpp
*\brief		 CollisionSystem functions definitions.

*\author(s)   Lee Liang Ping	306 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <CollisionSystem.h>
#include <FakeEngine.h>
#include <Collision.h>
#include <EntityCreation.h>
#include <AiSystem.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;

void CollisionSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	signature.set(mGameECSmanager.GetComponentType<AABB>());
	signature.set(mGameECSmanager.GetComponentType<Flags>());
	signature.set(mGameECSmanager.GetComponentType<CollisionFlag>());
	signature.set(mGameECSmanager.GetComponentType<Rigidbody>());
	mGameECSmanager.SetSystemBitSignature<CollisionSystem>(signature);
}

void CollisionSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	Boolean& boolean = gFakeEngine->GetGameManager<Boolean>();
	ForceManager& forceManager = gFakeEngine->GetGameManager<ForceManager>();

	for (std::set<Entity>::const_reverse_iterator f_entity = mEntities.rbegin(); f_entity != mEntities.rend(); ++f_entity)
	{
		Flags& f_flag = mGameECSmanager.GetComponent<Flags>(*f_entity);
		if (0 == (f_flag.activeFlag & FLAG_ACTIVE))
			continue;
		Rigidbody& f_rigidbody = mGameECSmanager.GetComponent<Rigidbody>(*f_entity);
		AABB& f_aabb = mGameECSmanager.GetComponent<AABB>(*f_entity);
		Position& f_position = mGameECSmanager.GetComponent<Position>(*f_entity);
		Transform& f_transform = mGameECSmanager.GetComponent<Transform>(*f_entity);

		if (input.CheckPress(GLFW_MOUSE_BUTTON_LEFT))
		{
			if (Collision::OBB::StaticPointToStaticRect(input.GetMousePosWorld(), f_position.pos, f_transform.scale.x, f_transform.scale.y))
				globalVar.selected_EntityID = *f_entity;
			else
				globalVar.selected_EntityID = NULL;
		}


		EntityType& f_objecttype = mGameECSmanager.GetComponent<EntityType>(*f_entity);
		if (f_objecttype.objType != ObjectType::MAIN_CHARACTER && f_objecttype.objType != ObjectType::MONSTER
			&& f_objecttype.objType != ObjectType::MONSTER_PATROL && f_objecttype.objType != ObjectType::MONSTER_GUIDE)
			continue;

		if (f_objecttype.objType == ObjectType::MAIN_CHARACTER)
		{
			State& f_state = mGameECSmanager.GetComponent<State>(*f_entity);
			Inventory& f_inventory = mGameECSmanager.GetComponent<Inventory>(*f_entity);

			mathEngine::Vector3D check_leftpoint = mathEngine::Vector3D(f_position.pos.x - f_transform.scale.x, f_position.pos.y, 0.f);
			mathEngine::Vector3D check_rightpoint = mathEngine::Vector3D(f_position.pos.x + f_transform.scale.x, f_position.pos.y, 0.f);
			mathEngine::Vector3D check_toppoint = mathEngine::Vector3D(f_position.pos.x, f_position.pos.y + f_transform.scale.y, 0.f);
			mathEngine::Vector3D check_bottompoint = mathEngine::Vector3D(f_position.pos.x, f_position.pos.y - f_transform.scale.y, 0.f);


			for (std::set<Entity>::const_reverse_iterator s_entity = mEntities.rbegin(); s_entity != mEntities.rend(); ++s_entity)
			{
				Flags& s_flag = mGameECSmanager.GetComponent<Flags>(*s_entity);
				if (0 == (s_flag.activeFlag & FLAG_ACTIVE))
					continue;

				EntityType& s_objecttype = mGameECSmanager.GetComponent<EntityType>(*s_entity);
				if (s_objecttype.objType == ObjectType::MAIN_CHARACTER)
					continue;

				Rigidbody& s_rigidbody = mGameECSmanager.GetComponent<Rigidbody>(*s_entity);
				AABB& s_aabb = mGameECSmanager.GetComponent<AABB>(*s_entity);

				if (s_objecttype.objType == ObjectType::KEY_OBJECTIVE)
				{
					//check for key objectives
					if (Collision::Aabb::Intersection_RectRect(f_aabb, f_rigidbody.velocity, s_aabb, s_rigidbody.velocity))
					{
						std::cout << " collied " << (int)f_objecttype.objType << " with " << (int)s_objecttype.objType << std::endl;
						gFakeEngine->ExecuteEvent(new PickUpEvent(*f_entity, *s_entity));
						std::cout << " current inventory size : " << f_inventory._currObjective << std::endl;
						Inventory& inventory = mGameECSmanager.GetComponent<Inventory>(*f_entity);
						GlobalVar& globalvar = gFakeEngine->GetGameManager<GlobalVar>();
						Camera2d& camera2d = gFakeEngine->GetGraphicManager<Camera2d>();
						
						if (inventory._currObjective == globalvar.max_objective)
						{
							Position& level_position = mGameECSmanager.GetComponent<Position>(globalVar.level_doorID);
							camera2d.MoveToBack(f_position.pos, level_position.pos, 2.f);
							globalvar.level_door_opened = true;
							gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DOOM), FMOD_LOOP_OFF, 0.5f);
						}

						mGameECSmanager.DestroyEntity(*s_entity);
					}
				}
				else if (s_objecttype.objType == ObjectType::COLLECTIBLES)
				{
					//check for key objectives
					if (Collision::Aabb::Intersection_RectRect(f_aabb, f_rigidbody.velocity, s_aabb, s_rigidbody.velocity))
					{
						std::cout << " collied " << (int)f_objecttype.objType << " with " << (int)s_objecttype.objType << std::endl;
						gFakeEngine->ExecuteEvent(new PickUpEvent(*f_entity, *s_entity));
						std::cout << " current inventory size : " << f_inventory._currObjective << std::endl;
						mGameECSmanager.DestroyEntity(*s_entity);
					}
				}
				else if (s_objecttype.objType == ObjectType::TUTORIAL_FLOOR)
				{
					//check for key objectives
					if (Collision::Aabb::Intersection_RectRect(f_aabb, f_rigidbody.velocity, s_aabb, s_rigidbody.velocity))
					{
						//std::cout << " collied " << (int)f_objecttype.objType << " with " << (int)s_objecttype.objType << std::endl;
						gFakeEngine->ExecuteEvent(new TutorialActivate(*f_entity, *s_entity));
						//std::cout << " current inventory size : " << f_inventory._currObjective << std::endl;
					}
				}
				else if (s_objecttype.objType == ObjectType::PUSHABLE_BARREL && input.GetInputBooleen().space_key)
				{
					if (input.GetInputBooleen().boulder_collied_hack)
						continue;
					//check for pushable barrel
					State& s_state = mGameECSmanager.GetComponent<State>(*s_entity);
					Position& s_position = mGameECSmanager.GetComponent<Position>(*s_entity);
					Position boulderbinarypos = s_position;
					globalVar.ConvertScreenToBinary(boulderbinarypos.pos.x, boulderbinarypos.pos.y);

					Transform& s_transform = mGameECSmanager.GetComponent<Transform>(*s_entity);

					if (!f_inventory._currBoulder && (Collision::Aabb::Intersection_RectRect(f_aabb, f_rigidbody.velocity, s_aabb, s_rigidbody.velocity)))
					{
						f_inventory._currBoulder = *s_entity;
						if (Collision::OBB::StaticPointToStaticRect(check_rightpoint, s_position.pos, s_transform.scale.x, s_transform.scale.y)
							&& f_state.objectDir == ObjectDirection::RIGHT)
						{
							s_state.objectDir = ObjectDirection::RIGHT;
							if (globalVar.GetCellValue((int)(boulderbinarypos.pos.x + 1), (int)boulderbinarypos.pos.y) == ObjectType::FLOOR)
								globalVar.SetCellValue(boulderbinarypos.pos, ObjectType::FLOOR);
						}
						else if (Collision::OBB::StaticPointToStaticRect(check_leftpoint, s_position.pos, s_transform.scale.x, s_transform.scale.y)
							&& f_state.objectDir == ObjectDirection::LEFT)
						{
							s_state.objectDir = ObjectDirection::LEFT;
							if (globalVar.GetCellValue((int)(boulderbinarypos.pos.x - 1), (int)(boulderbinarypos.pos.y)) == ObjectType::FLOOR)
								globalVar.SetCellValue(boulderbinarypos.pos, ObjectType::FLOOR);
						}
						else if (Collision::OBB::StaticPointToStaticRect(check_bottompoint, s_position.pos, s_transform.scale.x, s_transform.scale.y)
							&& f_state.objectDir == ObjectDirection::DOWN)
						{
							s_state.objectDir = ObjectDirection::DOWN;
							if (globalVar.GetCellValue((int)boulderbinarypos.pos.x, (int)(boulderbinarypos.pos.y - 1)) == ObjectType::FLOOR)
								globalVar.SetCellValue(boulderbinarypos.pos, ObjectType::FLOOR);
						}
						else if (Collision::OBB::StaticPointToStaticRect(check_toppoint, s_position.pos, s_transform.scale.x, s_transform.scale.y)
							&& f_state.objectDir == ObjectDirection::UP)
						{
							s_state.objectDir = ObjectDirection::UP;
							if (globalVar.GetCellValue((int)boulderbinarypos.pos.x, (int)(boulderbinarypos.pos.y + 1)) == ObjectType::FLOOR)
								globalVar.SetCellValue(boulderbinarypos.pos, ObjectType::FLOOR);
						}
						else
						{
							//won't hit/call but just in case
							//std::cout << "SETBOULDER  : " << std::endl;
							forceManager.removeforce(*f_entity);
							s_state.objectDir = ObjectDirection::NO_DIR;
							s_state.objectState = ObjectState::IDLE;
							f_inventory._currBoulder = NULL;
							globalVar.SetCellValue(boulderbinarypos.pos, ObjectType::PUSHABLE_BARREL);
						}
					}
					if (f_inventory._currBoulder == *s_entity)
					{
						if (s_state.objectDir == ObjectDirection::RIGHT)
						{
							if (globalVar.GetCellValue((int)(boulderbinarypos.pos.x + 1), (int)boulderbinarypos.pos.y) == ObjectType::FLOOR)
							{
								s_state.objectState = ObjectState::PUSHING_BARREL;
								s_rigidbody.velocity = mathEngine::Vector3D(f_rigidbody.velocity.x, 0.f, 0.f);
							}
							else
							{
								globalVar.SetCellValue(boulderbinarypos.pos, ObjectType::PUSHABLE_BARREL);
								forceManager.removeforce(*f_entity);
								f_rigidbody.velocity = s_rigidbody.velocity = mathEngine::Vector3D(0.f, 0.f, 0.f);
								f_position.pos = globalVar.GetConvertBinaryToScreen(f_position.prev_binarypos);
							}
						}
						if (s_state.objectDir == ObjectDirection::LEFT)
						{
							if (globalVar.GetCellValue((int)(boulderbinarypos.pos.x - 1), (int)boulderbinarypos.pos.y) == ObjectType::FLOOR)
							{
								s_state.objectState = ObjectState::PUSHING_BARREL;
								s_rigidbody.velocity = mathEngine::Vector3D(f_rigidbody.velocity.x, 0.f, 0.f);
							}
							else
							{
								globalVar.SetCellValue(boulderbinarypos.pos, ObjectType::PUSHABLE_BARREL);
								forceManager.removeforce(*f_entity);
								f_rigidbody.velocity = s_rigidbody.velocity = mathEngine::Vector3D(0.f, 0.f, 0.f);
								f_position.pos = globalVar.GetConvertBinaryToScreen(f_position.prev_binarypos);
							}
						}
						else if (s_state.objectDir == ObjectDirection::DOWN)
						{
							if (globalVar.GetCellValue((int)(boulderbinarypos.pos.x), (int)(boulderbinarypos.pos.y + 1)) == ObjectType::FLOOR)
							{
								s_state.objectState = ObjectState::PUSHING_BARREL;
								s_rigidbody.velocity = mathEngine::Vector3D(0.f, f_rigidbody.velocity.y, 0.f);
							}
							else
							{
								globalVar.SetCellValue(boulderbinarypos.pos, ObjectType::PUSHABLE_BARREL);
								forceManager.removeforce(*f_entity);
								f_rigidbody.velocity = s_rigidbody.velocity = mathEngine::Vector3D(0.f, 0.f, 0.f);
								f_position.pos = globalVar.GetConvertBinaryToScreen(f_position.prev_binarypos);
							}
						}
						else if (s_state.objectDir == ObjectDirection::UP)
						{
							if (globalVar.GetCellValue((int)boulderbinarypos.pos.x, (int)(boulderbinarypos.pos.y - 1)) == ObjectType::FLOOR)
							{
								s_state.objectState = ObjectState::PUSHING_BARREL;
								s_rigidbody.velocity = mathEngine::Vector3D(0.f, f_rigidbody.velocity.y, 0.f);
							}
							else
							{
								globalVar.SetCellValue(boulderbinarypos.pos, ObjectType::PUSHABLE_BARREL);
								forceManager.removeforce(*f_entity);
								f_rigidbody.velocity = s_rigidbody.velocity = mathEngine::Vector3D(0.f, 0.f, 0.f);
								f_position.pos = globalVar.GetConvertBinaryToScreen(f_position.prev_binarypos);
								std::cout << f_position.pos.x << " " << f_position.pos.y << std::endl;
							}
						}

						if (!Collision::OBB::StaticPointToStaticRect(check_rightpoint, s_position.pos, s_transform.scale.x * 0.5f, s_transform.scale.y * 0.5f)
							&& f_state.objectDir == ObjectDirection::RIGHT)
						{
							forceManager.removeforce(*f_entity);
							s_state.objectDir = ObjectDirection::NO_DIR;
							s_state.objectState = ObjectState::IDLE;
							f_inventory._currBoulder = NULL;
							globalVar.SetCellValue(boulderbinarypos.pos, ObjectType::PUSHABLE_BARREL);
							f_position.pos = globalVar.GetConvertBinaryToScreen(f_position.prev_binarypos);
						}
						else if (!Collision::OBB::StaticPointToStaticRect(check_leftpoint, s_position.pos, s_transform.scale.x * 0.5f, s_transform.scale.y * 0.5f)
							&& f_state.objectDir == ObjectDirection::LEFT)
						{
							forceManager.removeforce(*f_entity);
							s_state.objectDir = ObjectDirection::NO_DIR;
							s_state.objectState = ObjectState::IDLE;
							f_inventory._currBoulder = NULL;
							globalVar.SetCellValue(boulderbinarypos.pos, ObjectType::PUSHABLE_BARREL);
							f_position.pos = globalVar.GetConvertBinaryToScreen(f_position.prev_binarypos);
						}
						else if (!Collision::OBB::StaticPointToStaticRect(check_bottompoint, s_position.pos, s_transform.scale.x * 0.5f, s_transform.scale.y * 0.5f)
							&& f_state.objectDir == ObjectDirection::DOWN)
						{
							forceManager.removeforce(*f_entity);
							s_state.objectDir = ObjectDirection::NO_DIR;
							s_state.objectState = ObjectState::IDLE;
							f_inventory._currBoulder = NULL;
							globalVar.SetCellValue(boulderbinarypos.pos, ObjectType::PUSHABLE_BARREL);
							f_position.pos = globalVar.GetConvertBinaryToScreen(f_position.prev_binarypos);
						}
						else if (!Collision::OBB::StaticPointToStaticRect(check_toppoint, s_position.pos, s_transform.scale.x * 0.5f, s_transform.scale.y * 0.5f)
							&& f_state.objectDir == ObjectDirection::UP)
						{
							forceManager.removeforce(*f_entity);
							s_state.objectDir = ObjectDirection::NO_DIR;
							s_state.objectState = ObjectState::IDLE;
							f_inventory._currBoulder = NULL;
							globalVar.SetCellValue(boulderbinarypos.pos, ObjectType::PUSHABLE_BARREL);
							f_position.pos = globalVar.GetConvertBinaryToScreen(f_position.prev_binarypos);
						}
					}

				}
				else if (s_objecttype.objType == ObjectType::MONSTER || s_objecttype.objType == ObjectType::MONSTER_PATROL || s_objecttype.objType == ObjectType::MONSTER_GUIDE)
				{
					if (Collision::Aabb::Intersection_RectRect(f_aabb, f_rigidbody.velocity, s_aabb, s_rigidbody.velocity) && boolean.Check(BooleanType::MOVEMENT_LOCK))
					{
						gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
							f_position.pos, 10.f, ObjectDirection::CIRCLE,
							ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS,
							std::make_pair<float, float>(60.f, 80.f),
							std::make_pair<float, float>(0.f, 0.f),
							std::make_pair<float, float>(8.f, 10.f),
							std::make_pair<float, float>(0.5f, 0.6f),
							std::vector<ParticleAction>(),
							{ 1.f, 1.f, 0.f }, { 0.f,1.f,1.f,0.9f }, 1.f, ShaderOption::FULL_TEXTURE,
							ObjectShapes::SQUARE, TextureType::GLOWINGBALL, NULL, mathEngine::RandNumberBetween(10, 30), false, PhysicFlag{ P_NORMAL }));

						if (!input.GetInputBooleen().god_mode || !boolean.Check(BooleanType::MOVEMENT_LOCK))
						{
							EntityCreation::AssignEntity(
								EntitySystemOption::GAME_ENTITY, *f_entity,
								State{ ObjectState::DEATH, ObjectDirection::NO_DIR });
							gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DEATH), FMOD_LOOP_OFF, gAudioEngine->volume);
							boolean.True(BooleanType::MOVEMENT_LOCK);
						}
					}
				}
				//else if (s_objecttype.objType == ObjectType::LEVEL_DOOR)
				//{
				//	if (globalVar.level_door_opened)
				//	{
				//		Light& light = mGameECSmanager.GetComponent<Light>(*s_entity);
				//		light.highlight = glm::vec4{ 1.0f,1.0f,1.0f,1.0f };
				//		if (Collision::Aabb::Intersection_RectRect(f_aabb, f_rigidbody.velocity, s_aabb, s_rigidbody.velocity))
				//		{
				//			StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
				//			stageChanger.NextState();
				//		}
				//	}
				//}

				if (Collision::Aabb::Intersection_RectRect(f_inventory.collisionArea, f_rigidbody.velocity, s_aabb, s_rigidbody.velocity))
					mGameECSmanager.GetComponent<CollisionFlag>(*s_entity).audioActivate = true;
				else
					mGameECSmanager.GetComponent<CollisionFlag>(*s_entity).audioActivate = false;
			}
		}
		else if (f_objecttype.objType == ObjectType::MONSTER || f_objecttype.objType == ObjectType::MONSTER_PATROL || f_objecttype.objType == ObjectType::MONSTER_GUIDE)
		{
			Inventory& mainCharinventory = mGameECSmanager.GetComponent<Inventory>(globalVar.mainCharID);
			if (mainCharinventory._currBoulder)
			{
				Rigidbody& s_rigidbody = mGameECSmanager.GetComponent<Rigidbody>(mainCharinventory._currBoulder);
				AABB& s_aabb = mGameECSmanager.GetComponent<AABB>(mainCharinventory._currBoulder);
				Position& s_position = mGameECSmanager.GetComponent<Position>(mainCharinventory._currBoulder);

				//AI Collied with current pushing barrel.
				if (Collision::Aabb::Intersection_RectRect(f_aabb, f_rigidbody.velocity, s_aabb, s_rigidbody.velocity))
				{
					globalVar.SetCellValue(globalVar.GetConvertScreenToBinary(s_position.pos), ObjectType::PUSHABLE_BARREL);
					forceManager.removeforce(*f_entity);
					f_rigidbody.velocity = s_rigidbody.velocity = mathEngine::Vector3D(0.f, 0.f, 0.f);
					f_position.pos = globalVar.GetConvertBinaryToScreen(f_position.prev_binarypos);
					//Set boulder. 
					mathEngine::Vector3D currBinaryPos = globalVar.GetConvertScreenToBinary(f_position.pos);
					Collision::HotSpot::SnapToCell(currBinaryPos.x);
				}
			}
		}
	}
}