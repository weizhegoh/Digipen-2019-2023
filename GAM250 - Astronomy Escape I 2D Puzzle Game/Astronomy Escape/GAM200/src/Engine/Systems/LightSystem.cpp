/*****************************************************************
*\file		 LightSystem.cpp
*\brief		 LightSystem functions definitions.

*\author(s)   Lee Liang Ping	125 lines x % Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <LightSystem.h>
#include <FakeEngine.h>
#include <EntityCreation.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern float g_dt;

void LightSystem::Init()
{
	auto& mLightECSmanager = gFakeEngine->GetECSLightManager<ECSManager>();
	BitSignature signature;
	signature.set(mLightECSmanager.GetComponentType<ET_LightNormal>());
	mLightECSmanager.SetSystemBitSignature<LightSystem>(signature);
}

void LightSystem::Update()
{
	auto& mLightECSmanager = gFakeEngine->GetECSLightManager<ECSManager>();
	for (auto const& entity : mEntities)
	{
		Flags& flag = mLightECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;
		PointLight& pointlight = mLightECSmanager.GetComponent<PointLight>(entity);
		Rigidbody& rigidbody = mLightECSmanager.GetComponent<Rigidbody>(entity);
		pointlight.ambient += rigidbody.velocity.z * g_dt * pointlight.ambientDir;

		if (pointlight.ambient >= pointlight.maxAmbient)
			pointlight.ambientDir = -1.f;

		if (pointlight.ambient <= 0.f)
		{
			pointlight.ambientDir = 1.f;
			LightEffects& lightEffects = gFakeEngine->GetGameManager<LightEffects>();
			lightEffects.RandomGenerate_NormalLight(entity);
			Position& position = mLightECSmanager.GetComponent<Position>(entity);
			Colour& colour = mLightECSmanager.GetComponent<Colour>(entity);

			gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
				position.pos, 35.f, ObjectDirection::CIRCLE,
				ParticleTypesEnum::PARTICLE_PULSE_INWARDS,
				std::make_pair<float, float>(50.f, 50.f),
				std::make_pair<float, float>(0.f, 0.f),
				std::make_pair<float, float>(7.f, 9.f),
				std::make_pair<float, float>(0.8f, 0.8f),
				std::vector<ParticleAction>(),
				 mathEngine::Vector3D(colour.colour.x, colour.colour.y, colour.colour.z), 
				{ colour.colour.x,colour.colour.y,colour.colour.z,0.9 }, 0.7f, ShaderOption::FULL_TEXTURE,
				ObjectShapes::SQUARE, TextureType::GLOWINGBALL,
				entity, mathEngine::RandNumberBetween(10, 50), false, PhysicFlag{ P_NORMAL })
			);
		}
	}

	/*auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	SerializationVar& serializationVar = gFakeEngine->GetGameManager<SerializationVar>();
	for (const auto& x : serializationVar.tutorialMap)
	{
		Position position = mGameECSmanager.GetComponent<Position>(x.first);
		position.pos.y = mGameECSmanager.GetComponent<AABB>(x.first).min.y;

		LifeTime& lifetime = mGameECSmanager.GetComponent<LifeTime>(x.first);
		lifetime.timeCounter += g_dt;

		if (lifetime.timeCounter > lifetime.lifetime)
		{
			gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
				position.pos, 50.f, ObjectDirection::CIRCLE,
				ParticleTypesEnum::PARTICLE_PULSE_INWARDS,
				std::make_pair<float, float>(10.f, 10.f),
				std::make_pair<float, float>(0.f, 0.f),
				std::make_pair<float, float>(7.f, 9.f),
				std::make_pair<float, float>(0.6f, 0.6f),
				std::vector<ParticleAction>(),
				{ 1.f, 1.f, 0.f }, { 1.f,1.f,0.f,0.9 }, 0.7f, ShaderOption::FULL_TEXTURE,
				ObjectShapes::SQUARE, TextureType::GLOWINGBALL,
				x.first, mathEngine::RandNumberBetween(1, 10), false, PhysicFlag{ P_NORMAL })
			);
			lifetime.timeCounter = 0;
		}
	}*/


}

void MainChar_LightSystem::Init()
{
	auto& mLightECSmanager = gFakeEngine->GetECSLightManager<ECSManager>();
	BitSignature signature;
	signature.set(mLightECSmanager.GetComponentType<ET_Light_MainChar>());
	mLightECSmanager.SetSystemBitSignature<MainChar_LightSystem>(signature);
}

void MainChar_LightSystem::Update()
{
	auto& mLightECSmanager = gFakeEngine->GetECSLightManager<ECSManager>();
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	for (auto const& entity : mEntities)
	{
		Flags& flag = mLightECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;
		Position& position = mLightECSmanager.GetComponent<Position>(entity);
		Position& mc_position = mGameECSmanager.GetComponent<Position>(globalVar.mainCharID);
		position.pos = mc_position.pos;

		State& mc_state = mGameECSmanager.GetComponent<State>(globalVar.mainCharID);
		PointLight& pointlight = mLightECSmanager.GetComponent<PointLight>(entity);
		if (mc_state.objectState == ObjectState::DEATH)
		{
			Rigidbody& rigidbody = mLightECSmanager.GetComponent<Rigidbody>(entity);
			pointlight.ambient += rigidbody.velocity.z * g_dt;
		}
		else
			pointlight.ambient = pointlight.maxAmbient;

	}
}

void MainChar_LightSystem::ResetAmbient()
{
	auto& mLightECSmanager = gFakeEngine->GetECSLightManager<ECSManager>();
	for (auto const& entity : mEntities)
	{
		Flags& flag = mLightECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;
		PointLight& pointlight = mLightECSmanager.GetComponent<PointLight>(entity);
		pointlight.ambient = pointlight.maxAmbient;
	}
}

void KeyObjectives_LightSystem::Init()
{
	auto& mLightECSmanager = gFakeEngine->GetECSLightManager<ECSManager>();
	BitSignature signature;
	signature.set(mLightECSmanager.GetComponentType<ET_Light_KeyObjectives>());
	mLightECSmanager.SetSystemBitSignature<KeyObjectives_LightSystem>(signature);
}

void KeyObjectives_LightSystem::Update()
{
	auto& mLightECSmanager = gFakeEngine->GetECSLightManager<ECSManager>();
	auto& mParticleECSmanager = gFakeEngine->GetECSParticlesManager<ECSManager>();
	//GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	for (auto const& entity : mEntities)
	{
		Flags& flag = mLightECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;

		ET_Light_KeyObjectives& et_Light_ko = mLightECSmanager.GetComponent<ET_Light_KeyObjectives>(entity);

		if (et_Light_ko.state == TypeState::Removing)
		{
			mLightECSmanager.GetComponent<Position>(entity) = mParticleECSmanager.GetComponent<Position>(et_Light_ko.followingId);
		}
	}
}

void KeyObjectivesParts_LightSystem::Init()
{
	auto& mLightECSmanager = gFakeEngine->GetECSLightManager<ECSManager>();
	BitSignature signature;
	signature.set(mLightECSmanager.GetComponentType<ET_Light_KeyObjectives_Parts>());
	mLightECSmanager.SetSystemBitSignature<KeyObjectivesParts_LightSystem>(signature);
}

void KeyObjectivesParts_LightSystem::Update()
{
	//auto& mLightECSmanager = gFakeEngine->GetECSLightManager<ECSManager>();
	////GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	//for (auto const& entity : mEntities)
	//{
	//}
}

