/*****************************************************************
*\file		 MouseSystem.cpp
*\brief		 MouseSystem functions definitions.

*\author(s)   Lee Liang Ping	38 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <FakeEngine.h>
#include <MouseSystem.h>
#include <SystemList.h>
extern std::unique_ptr<FakeEngine> gFakeEngine;

void MouseSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<Mouse>());
	mGameECSmanager.SetSystemBitSignature<MouseSystem>(signature);
}

void MouseSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	auto& systemlist = gFakeEngine->GetECSGameManager<GameSystemsList>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	for (auto const& entity : mEntities)
	{
		systemlist.transformSystem->transform(entity, EntitySystemOption::GAME_ENTITY);
		Position& position = mGameECSmanager.GetComponent<Position>(entity);
		LifeTime& lifetime = mGameECSmanager.GetComponent<LifeTime>(entity);
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);

		if (position.pos.x == input.GetMousePosWorld().x && position.pos.y + 25.f == input.GetMousePosWorld().y)
		{
			lifetime.timeCounter += g_dt;
			if (lifetime.timeCounter >= 5.f)
				flag.activeFlag = FLAG_INACTIVE;
		}
		else
		{
			flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
			lifetime.timeCounter = 0;
			position.pos = input.GetMousePosWorld();
			position.pos.y -= 25.f;
		}
	}
}