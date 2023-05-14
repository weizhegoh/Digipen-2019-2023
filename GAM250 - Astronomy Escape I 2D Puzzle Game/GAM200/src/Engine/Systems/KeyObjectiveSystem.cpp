/*****************************************************************
*\file		 KeyObjectiveSystem.cpp
*\brief		 LightSystem functions definitions.

*\author(s)   Lee Liang Ping	39 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <FakeEngine.h>
#include <KeyobjectiveSystem.h>
#include <SystemList.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;

void KeyObjectiveSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<KeyObjective>());
	mGameECSmanager.SetSystemBitSignature<KeyObjectiveSystem>(signature);
}

void KeyObjectiveSystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	//auto& mLightECSmanager = gFakeEngine->GetECSManager<ECSManager>(EntitySystemOption::LIGHT_ENTITY);
	//auto& mLightSystem = gFakeEngine->GetECSLightManager<LightSystemsList>();
	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;

		EntityType& objecttype = mGameECSmanager.GetComponent<EntityType>(entity);
		if (objecttype.objType == ObjectType::KEY_OBJECTIVE)
		{
			Light& light = mGameECSmanager.GetComponent<Light>(entity);
			if (light.interp)
			{
				light.highlight.a = mathEngine::LinearInterpolate(light.highlight.a, 1.f, g_dt);
				if (light.highlight.a >= .9f)
					light.interp = false;
			}
			else
			{
				light.highlight.a = mathEngine::LinearInterpolate(light.highlight.a, 0.f, g_dt);
				if (light.highlight.a <= 0.3f)
					light.interp = true;
			}
		}




	}
	
	//for (const auto& light_entity : mLightSystem.keyObjectives_lightSystem->mEntities)
	//{
	//	ET_Light_KeyObjectives& et_Light_ko = mLightECSmanager.GetComponent<ET_Light_KeyObjectives>(light_entity);
	//}




}