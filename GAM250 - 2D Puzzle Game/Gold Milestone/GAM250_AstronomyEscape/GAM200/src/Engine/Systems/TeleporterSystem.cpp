/*****************************************************************
*\file		 TeleporterSystem.cpp
*\brief		 TeleporterSystem functions definitions.

*\author(s)   Kenric Tan		120 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <TeleporterSystem.h>
#include <FakeEngine.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;

static std::vector<std::pair<Entity, Entity>> vec;


void TeleporterSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	signature.set(mGameECSmanager.GetComponentType<Position>());
	signature.set(mGameECSmanager.GetComponentType<State>());
	signature.set(mGameECSmanager.GetComponentType<Teleporter>());
	mGameECSmanager.SetSystemBitSignature<TeleporterSystem>(signature);
}

void TeleporterSystem::init_map()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	SerializationVar& serializationVar = gFakeEngine->GetGameManager<SerializationVar>();

	vec.resize(serializationVar.teleporterCount / 2);

	for (int i = 0; i < serializationVar.teleporterCount / 2; i++)
	{
		bool check = true;
		Entity store = 0;
		for (auto const& f_entity : mEntities)
		{
			EntityType& f_type = mGameECSmanager.GetComponent<EntityType>(f_entity);
			if (f_type.objType == ObjectType::TELEPORTER)
			{
				Teleporter& f_teleporter = mGameECSmanager.GetComponent<Teleporter>(f_entity);

				if (f_teleporter.teleporterID == i)
				{
					if (check)
					{
						store = f_entity;
						check = false;
					}
					else
					{
						vec[i].first = store;
						vec[i].second = f_entity;
					}
				}
			}
		}
	}
}

void TeleporterSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	for (auto const& entity : mEntities)
	{
		EntityType& type = mGameECSmanager.GetComponent<EntityType>(entity);
		//Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);

		State& state = mGameECSmanager.GetComponent<State>(entity);
		Position pos = mGameECSmanager.GetComponent<Position>(entity);
		//Transform& trans = mGameECSmanager.GetComponent<Transform>(entity);
		Position mainChar_Pos = mGameECSmanager.GetComponent<Position>(globalVar.mainCharID);
		State& mainChar_State = mGameECSmanager.GetComponent<State>(globalVar.mainCharID);
		globalVar.ConvertScreenToBinary(mainChar_Pos.pos);
		globalVar.ConvertScreenToBinary(pos.pos);

		if (type.objType == ObjectType::TELEPORTER)
		{


			if (((int)mainChar_Pos.pos.x == (int)(pos.pos.x - 1) && (int)mainChar_Pos.pos.y == (int)pos.pos.y &&
				mainChar_State.objectDir == ObjectDirection::RIGHT && state.objectDir == ObjectDirection::LEFT)
				|| ((int)mainChar_Pos.pos.x == (int)(pos.pos.x + 1) && (int)mainChar_Pos.pos.y == (int)pos.pos.y
					&& mainChar_State.objectDir == ObjectDirection::LEFT && state.objectDir == ObjectDirection::RIGHT)
				|| ((int)mainChar_Pos.pos.x == (int)pos.pos.x && (int)mainChar_Pos.pos.y == (int)(pos.pos.y - 1)
					&& mainChar_State.objectDir == ObjectDirection::DOWN && state.objectDir == ObjectDirection::UP)
				|| ((int)mainChar_Pos.pos.x == (int)pos.pos.x && (int)mainChar_Pos.pos.y == (int)(pos.pos.y + 1)
					&& mainChar_State.objectDir == ObjectDirection::UP && state.objectDir == ObjectDirection::DOWN)
				) // main char left of the lever
			{
				if (input.CheckPress(GLFW_KEY_SPACE))
				{

					Teleporter& teleporter = mGameECSmanager.GetComponent<Teleporter>(entity);

					Entity first = vec[teleporter.teleporterID].first;
					Entity second = vec[teleporter.teleporterID].second;
					Entity found = first;

					if (entity != first)
					{
						found = first;
					}
					else if (entity != second)
					{
						found = second;
					}

					Position& main = mGameECSmanager.GetComponent<Position>(globalVar.mainCharID);
					Position& tele = mGameECSmanager.GetComponent<Position>(found);

					if (state.objectDir == ObjectDirection::LEFT)
					{
						main.pos.x = tele.pos.x + globalVar.objectSize;
						main.pos.y = tele.pos.y;
					}
					else if (state.objectDir == ObjectDirection::RIGHT)
					{
						main.pos.x = tele.pos.x - globalVar.objectSize;
						main.pos.y = tele.pos.y;

					}
					else if (state.objectDir == ObjectDirection::UP)
					{
						main.pos.x = tele.pos.x;
						main.pos.y = tele.pos.y - globalVar.objectSize;
					}
					else if (state.objectDir == ObjectDirection::DOWN)
					{
						
						main.pos.x = tele.pos.x;
						main.pos.y = tele.pos.y + globalVar.objectSize;
					}
					gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::WARP), FMOD_LOOP_OFF, 0.5f);


					Position& teleporter_position = mGameECSmanager.GetComponent<Position>(entity);
					ParticlesInfo& particlesList = gFakeEngine->GetGameManager<ParticlesInfo>();

					particlesList.TeleportParticlesSTART(teleporter_position.pos, entity);
					particlesList.TeleportParticlesEND(tele.pos, found);
					
				}
			}
		}
	}
}