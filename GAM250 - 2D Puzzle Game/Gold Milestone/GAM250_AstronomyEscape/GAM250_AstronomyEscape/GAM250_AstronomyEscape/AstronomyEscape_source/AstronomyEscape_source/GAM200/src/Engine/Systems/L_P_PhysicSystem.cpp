/*****************************************************************
*\file		 L_P_PhysicSystem.cpp
*\brief		 PhysicSystem functions definitions.

*\author(s)   Lee Liang Ping	42 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <L_P_PhysicSystem.h>
#include <FakeEngine.h>
#include <Collision.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern float g_dt;

void L_P_PhysicSystem::Init(EntitySystemOption systemoption)
{
	auto& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(systemoption);
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<Rigidbody>());
	signature.set(mGameECSmanager.GetComponentType<Position>());
	mGameECSmanager.SetSystemBitSignature<L_P_PhysicSystem>(signature);
}

void L_P_PhysicSystem::Update(EntitySystemOption systemoption)
{
	auto& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(systemoption);
	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;

		PhysicFlag& p_flag = mGameECSmanager.GetComponent<PhysicFlag>(entity);
		if (0 == (p_flag.flag & P_NORMAL))
			continue;

		Position& position = mGameECSmanager.GetComponent<Position>(entity);
		Rigidbody& rigidbody = mGameECSmanager.GetComponent<Rigidbody>(entity);

		rigidbody.velocity.x += rigidbody.acceleration.x;
		rigidbody.velocity.y += rigidbody.acceleration.y;

		if (p_flag.flag & P_FRICTION)
		{
			rigidbody.acceleration.x *= 0.99f;
			rigidbody.acceleration.y *= 0.99f;
		}

		if (p_flag.flag & P_GRAVITY)
		{
			rigidbody.velocity.y -= (21.0f / 0.22209f) * g_dt;
		}
		rigidbody.Epsilon();
		position.Update(rigidbody.velocity, g_dt);

	}


}