/*****************************************************************
*\file		 DoorSystem.cpp
*\brief		 DoorSystem functions definitions.

*\author(s)   Lee Jun Jie		178 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <DoorSystem.h>
#include <FakeEngine.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;

void DoorSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	signature.set(mGameECSmanager.GetComponentType<Flags>());
	signature.set(mGameECSmanager.GetComponentType<Transform>());
	signature.set(mGameECSmanager.GetComponentType<AABB>());
	signature.set(mGameECSmanager.GetComponentType<State>());
	mGameECSmanager.SetSystemBitSignature<DoorSystem>(signature);
}

void DoorSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	for (auto const& entity : mEntities)
	{
		EntityType& type = mGameECSmanager.GetComponent<EntityType>(entity);
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);

		State& state = mGameECSmanager.GetComponent<State>(entity);
		Position pos = mGameECSmanager.GetComponent<Position>(entity);
		Transform& trans = mGameECSmanager.GetComponent<Transform>(entity);
		Position mainChar_Pos = mGameECSmanager.GetComponent<Position>(globalVar.mainCharID);
		globalVar.ConvertScreenToBinary(pos.pos);
		globalVar.ConvertScreenToBinary(mainChar_Pos.pos);


		auto door_check = std::find(globalVar.door.begin(), globalVar.door.end(), entity) != globalVar.door.end();
		if (type.objType == ObjectType::ENTRANCE_DOOR)
		{

			for (auto const& s_entity : mEntities)
			{
				EntityType& s_type = mGameECSmanager.GetComponent<EntityType>(s_entity);
				if (s_type.objType == ObjectType::MONSTER || s_type.objType == ObjectType::MONSTER_PATROL || s_type.objType == ObjectType::MONSTER_GUIDE)
				{
					Position s_pos = mGameECSmanager.GetComponent<Position>(s_entity);
					State& s_state = mGameECSmanager.GetComponent<State>(s_entity);

					globalVar.ConvertScreenToBinary(s_pos.pos);
					if (((((int)pos.pos.x == (int)s_pos.pos.x) && (((int)pos.pos.y - 1) == (int)s_pos.pos.y)) && s_state.objectDir == ObjectDirection::DOWN) ||
						((((int)pos.pos.x == (int)s_pos.pos.x) && (((int)pos.pos.y + 1) == (int)s_pos.pos.y)) && s_state.objectDir == ObjectDirection::UP) ||
						((((int)(pos.pos.x - 1) == (int)s_pos.pos.x) && ((int)pos.pos.y == (int)s_pos.pos.y)) && s_state.objectDir == ObjectDirection::RIGHT) ||
						((((int)(pos.pos.x + 1) == (int)s_pos.pos.x) && ((int)pos.pos.y == (int)s_pos.pos.y)) && s_state.objectDir == ObjectDirection::LEFT)
						)
					{
						AI& ai = mGameECSmanager.GetComponent<AI>(s_entity);
						ai.time = 0;

					}

				}

			}

			if (globalVar.entrance_door_opened)
			{
				if (!door_check)
					flag.activeFlag = FLAG_INACTIVE;
				else
				{
					trans.angle += 30.f * g_dt * 10.f;
					if (trans.angle >= 360.f)
						trans.angle = 0;
				}

				if (state.objectDir == ObjectDirection::UP)
				{
					if (((int)pos.pos.x == (int)mainChar_Pos.pos.x) && (((int)pos.pos.y - 1) == (int)mainChar_Pos.pos.y) && (!door_check))
					{
						globalVar.door.push_back(entity);
						globalVar.entrance_door_opened = false;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DOOR_CLOSE), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (state.objectDir == ObjectDirection::DOWN)
				{
					if (((int)pos.pos.x == (int)mainChar_Pos.pos.x) && (((int)pos.pos.y + 1) == (int)mainChar_Pos.pos.y) && (!door_check))
					{
						globalVar.door.push_back(entity);
						globalVar.entrance_door_opened = false;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DOOR_CLOSE), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (state.objectDir == ObjectDirection::LEFT)
				{
					if (((int)(pos.pos.x - 1) == (int)mainChar_Pos.pos.x) && ((int)pos.pos.y == (int)mainChar_Pos.pos.y) && (!door_check))
					{
						globalVar.door.push_back(entity);
						globalVar.entrance_door_opened = false;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DOOR_CLOSE), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
				else if (state.objectDir == ObjectDirection::RIGHT)
				{
					if (((int)(pos.pos.x + 1) == (int)mainChar_Pos.pos.x) && ((int)pos.pos.y == (int)mainChar_Pos.pos.y) && (!door_check))
					{
						globalVar.door.push_back(entity);
						globalVar.entrance_door_opened = false;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DOOR_CLOSE), FMOD_LOOP_OFF, gAudioEngine->volume);
					}
				}
			}
			else
			{
				trans.angle += 30.f * g_dt * 10.f;
				if (trans.angle >= 360.f)
					trans.angle = 0;

				flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
			}
		}

		else if (type.objType == ObjectType::EXIT_DOOR)
		{
			if (globalVar.exit_door_opened)
			{
				flag.activeFlag = FLAG_INACTIVE;
				if (state.objectDir == ObjectDirection::UP)
				{
					if (((int)pos.pos.x == (int)mainChar_Pos.pos.x) && (((int)pos.pos.y - 1) == (int)mainChar_Pos.pos.y))

					{
						globalVar.entrance_door_opened = true;
						globalVar.exit_door_opened = false;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DOOR_CLOSE), FMOD_LOOP_OFF, 0.5f);
					}
				}
				else if (state.objectDir == ObjectDirection::DOWN)
				{
					if (((int)pos.pos.x == (int)mainChar_Pos.pos.x) && (((int)pos.pos.y + 1) == (int)mainChar_Pos.pos.y))
					{
						globalVar.entrance_door_opened = true;
						globalVar.exit_door_opened = false;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DOOR_CLOSE), FMOD_LOOP_OFF, 0.5f);
					}
				}
				else if (state.objectDir == ObjectDirection::LEFT)
				{
					if (((int)(pos.pos.x - 1) == (int)mainChar_Pos.pos.x) && ((int)pos.pos.y == (int)mainChar_Pos.pos.y))
					{
						globalVar.entrance_door_opened = true;
						globalVar.exit_door_opened = false;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DOOR_CLOSE), FMOD_LOOP_OFF, 0.5f);
					}
				}
				else if (state.objectDir == ObjectDirection::RIGHT)
				{
					if (((int)(pos.pos.x + 1) == (int)mainChar_Pos.pos.x) && ((int)pos.pos.y == (int)mainChar_Pos.pos.y))
					{
						globalVar.entrance_door_opened = true;
						globalVar.exit_door_opened = false;
						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::DOOR_CLOSE), FMOD_LOOP_OFF, 0.5f);
					}
				}
			}
			else
			{
				trans.angle += 30.f * g_dt * 10.f;
				if (trans.angle >= 360.f)
					trans.angle = 0;

				flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
			}
		}
		else if (type.objType == ObjectType::LEVEL_DOOR)
		{
			if (globalVar.level_door_opened)
			{
				//Light& light = mGameECSmanager.GetComponent<Light>(entity);
				//light.highlight = glm::vec4{ 1.0f,1.0f,1.0f,1.0f };

				if (((int)pos.pos.x == (int)mainChar_Pos.pos.x) && (((int)pos.pos.y - 1) == (int)mainChar_Pos.pos.y)
					|| ((int)pos.pos.x == (int)mainChar_Pos.pos.x) && (((int)pos.pos.y + 1) == (int)mainChar_Pos.pos.y)
					|| ((int)(pos.pos.x - 1) == (int)mainChar_Pos.pos.x) && ((int)pos.pos.y == (int)mainChar_Pos.pos.y)
					|| ((int)(pos.pos.x + 1) == (int)mainChar_Pos.pos.x) && ((int)pos.pos.y == (int)mainChar_Pos.pos.y))
				{
					if (input.CheckPress(GLFW_KEY_SPACE))
					{
						input.GetInputBooleen().win_state = true;
						gFakeEngine->GetGameManager<Boolean>().True(BooleanType::MOVEMENT_LOCK);
						globalVar.level_door_opened = false;
					}

				}
			}
		}

	}
}