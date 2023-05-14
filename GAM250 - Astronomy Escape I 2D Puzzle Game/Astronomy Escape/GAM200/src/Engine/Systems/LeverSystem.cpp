/*****************************************************************
*\file		 LeverSystem.cpp
*\brief		 LeverSystem functions definitions.

*\author(s)   Kenric Tan		131 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <LeverSystem.h>
#include <FakeEngine.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;
static std::vector<RenderTex> renderVec;

void LeverSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	//signature.set(mGameECSmanager.GetComponentType<Position>());
	signature.set(mGameECSmanager.GetComponentType<Lever>());
	mGameECSmanager.SetSystemBitSignature<LeverSystem>(signature);
}

void LeverSystem::init_map()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();

	for (auto const& f_entity : mEntities)
	{
		EntityType& f_type = mGameECSmanager.GetComponent<EntityType>(f_entity);
		if (f_type.objType == ObjectType::LEVER_SWITCH)
		{
			Lever& f_lever = mGameECSmanager.GetComponent<Lever>(f_entity);

			for (auto const& s_entity : mEntities)
			{
				EntityType& s_type = mGameECSmanager.GetComponent<EntityType>(s_entity);
				if (s_type.objType == ObjectType::LEVER_DOOR)
				{
					Lever& s_lever = mGameECSmanager.GetComponent<Lever>(s_entity);

					if (f_lever.leverID == s_lever.leverID)
					{
						f_lever.leverMap.insert({ f_entity,s_entity });
					}
				}
			}
		}
	}

	StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();

	for (auto const& x : stageChanger.allobjects)
	{
		if (x._object.second.objType == ObjectType::LEVER_SWITCH)
		{
			renderVec = x._texture.second;
		}
	}

}

void LeverSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	for (auto const& entity : mEntities)
	{
		EntityType& type = mGameECSmanager.GetComponent<EntityType>(entity);
		//Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		//State state = mGameECSmanager.GetComponent<State>(entity);

		//State& state = mGameECSmanager.GetComponent<State>(entity);
		Position pos = mGameECSmanager.GetComponent<Position>(entity);
		//Transform& trans = mGameECSmanager.GetComponent<Transform>(entity);
		Position mainChar_Pos = mGameECSmanager.GetComponent<Position>(globalVar.mainCharID);
		State& mainChar_State = mGameECSmanager.GetComponent<State>(globalVar.mainCharID);
		globalVar.ConvertScreenToBinary(pos.pos);
		globalVar.ConvertScreenToBinary(mainChar_Pos.pos);

		if (type.objType == ObjectType::LEVER_SWITCH)
		{
			State state = mGameECSmanager.GetComponent<State>(entity);
			//StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
			RenderTex& render = mGameECSmanager.GetComponent<RenderTex>(entity);


			Lever& lever = mGameECSmanager.GetComponent<Lever>(entity);

			Entity found_door = lever.leverMap.find(entity)->second;
			Lever& door = mGameECSmanager.GetComponent<Lever>(found_door);
			Flags& door_flag = mGameECSmanager.GetComponent<Flags>(found_door);
			State& door_state = mGameECSmanager.GetComponent<State>(found_door);
			Animation& door_ani = mGameECSmanager.GetComponent<Animation>(found_door);





			//if (door_state.objectState == ObjectState::DOOR_OPEN)
			//	std::cout << "Door Open" << std::endl;
			//if (door_state.objectState == ObjectState::DOOR_CLOSE)
			//	std::cout << "Door Close" << std::endl;

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
				//if (input.CheckPress(GLFW_KEY_SPACE))
				//{

				if (input.CheckPress(GLFW_KEY_SPACE))
				{
					if (!door.isOpen)
					{
						door.isOpen = true;
						door_state.objectState = ObjectState::DOOR_OPEN;

						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::LEVER), FMOD_LOOP_OFF, 0.3f);

						if (state.objectDir == ObjectDirection::UP)
							render.texture = renderVec[4].texture;
						else if (state.objectDir == ObjectDirection::DOWN)
							render.texture = renderVec[5].texture;
						else if (state.objectDir == ObjectDirection::LEFT)
							render.texture = renderVec[6].texture;
						else if (state.objectDir == ObjectDirection::RIGHT)
							render.texture = renderVec[7].texture;

					}
					else if (door.isOpen)
					{
						door.isOpen = false;
						door_flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
						door_state.objectState = ObjectState::DOOR_CLOSE;

						gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::LEVER), FMOD_LOOP_OFF, 0.3f);

						if (state.objectDir == ObjectDirection::UP)
							render.texture = renderVec[0].texture;
						else if (state.objectDir == ObjectDirection::DOWN)
							render.texture = renderVec[1].texture;
						else if (state.objectDir == ObjectDirection::LEFT)
							render.texture = renderVec[2].texture;
						else if (state.objectDir == ObjectDirection::RIGHT)
							render.texture = renderVec[3].texture;
					}
					Position& door_position = mGameECSmanager.GetComponent<Position>(found_door);
					ParticlesInfo& particlesList = gFakeEngine->GetGameManager<ParticlesInfo>();
					particlesList.CreateSmokeParticles(door_position.pos);
				}
			}

			if (door_ani.endAni)
			{

				if (door_state.objectState == ObjectState::DOOR_OPEN)
				{
					door_flag.activeFlag = FLAG_INACTIVE;
					//door_state.objectState = ObjectState::IDLE;

					door_ani.endAni = false;
				}
				else if (door_state.objectState == ObjectState::DOOR_CLOSE)
				{
					door_state.objectState = ObjectState::IDLE;

					door_ani.endAni = false;
				}
			}
		}
	}
}