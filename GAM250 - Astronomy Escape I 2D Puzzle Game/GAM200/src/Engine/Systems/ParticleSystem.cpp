/*****************************************************************
*\file		 ParticleSystem.cpp
*\brief		 Particle system functions definitions.

*\author(s)   Lee Liang Ping	172 lines x % Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <ParticleSystem.h>
#include <FakeEngine.h>
#include <EntityCreation.h>
#include <SystemList.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern float g_dt;

void ParticleSystem::Init()
{
	gFakeEngine->SubscribeEvent(this, &ParticleSystem::CreatePulseParticles, EventID::CreatePulseParticleEvent);
	gFakeEngine->SubscribeEvent(this, &ParticleSystem::CreateParticles, EventID::CreateParticleEvent);
	auto& mParticleECSmanager = gFakeEngine->GetECSParticlesManager<ECSManager>();
	BitSignature signature;
	signature.set(mParticleECSmanager.GetComponentType<ParticleType>());
	signature.set(mParticleECSmanager.GetComponentType<ParticleStates>());
	mParticleECSmanager.SetSystemBitSignature<ParticleSystem>(signature);
}

void ParticleSystem::Update()
{
	auto& mParticleECSmanager = gFakeEngine->GetECSParticlesManager<ECSManager>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	//FPS& fps = gFakeEngine->GetGraphicManager<FPS>();
	for (auto const& entity : mEntities)
	{
		Flags& flag = mParticleECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;

		LifeTime& lifetime = mParticleECSmanager.GetComponent<LifeTime>(entity);

		ParticleActions& actions = mParticleECSmanager.GetComponent<ParticleActions>(entity);
		//for (int step = 0; step < fps.GetCurrentNumberOfSteps(); ++step)
		lifetime.timeCounter -= g_dt;
		if (lifetime.timeCounter < 0)
		{
			if (actions.pActions.empty() && lifetime.isTherelifeTime)
			{
				mParticleECSmanager.GetComponent<Flags>(entity).activeFlag = FLAG_INACTIVE;
				continue;
			}
			actions.UpdateAction();

			ParticlesInfo& particlesList = gFakeEngine->GetGameManager<ParticlesInfo>();


			if (actions.current_action == ParticleAction::TOWARDS_LEVEL_DOOR)
			{
				particlesList.SetDirection(entity, globalVar.level_doorID);
				lifetime.timeCounter = lifetime.lifetime;
				lifetime.isTherelifeTime = false;
			}
			else if (actions.current_action == ParticleAction::TOWARDS_MC)
			{
				particlesList.SetDirection(entity, globalVar.mainCharID);
				lifetime.timeCounter = lifetime.lifetime;
			}

			if (actions.current_action == ParticleAction::END && lifetime.isTherelifeTime)
			{
				clear(actions.pActions);
				mParticleECSmanager.GetComponent<Flags>(entity).activeFlag = FLAG_INACTIVE;
				continue;
			}

		}
	}
}

void ParticleSystem::CreatePulseParticles(CreatePulseParticleEvent* s)
{
	int amount = 0;
	auto& mGameECSmanager = gFakeEngine->GetECSParticlesManager<ECSManager>();
	Camera2d& camera2d = gFakeEngine->GetGraphicManager<Camera2d>();
	ParticlesInfo& particlesList = gFakeEngine->GetGameManager<ParticlesInfo>();


	for (auto entity = mEntities.begin(), end = mEntities.end(); entity != end; ++entity)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(*entity);
		if (1 == (flag.activeFlag & FLAG_ACTIVE))
			continue;
		float  dir = 0.f;
		dir = particlesList.SetRandomDirectionAngle(s->direction);

		EntityCreation::AssignEntity(EntitySystemOption::PARTICLE_ENTITY,
			*entity,
			particlesList.SetPosition(s->particletype, s->position, dir, s->pulseRadius),
			particlesList.SetTransformWorld(s->particletype, s->scaleBetwen, s->position, camera2d),
			particlesList.SetRigidbody(s->particletype, s->velocityBetween, s->accelerateBetween, dir),
			Colour{ glm::vec3(s->colour.x, s->colour.y, s->colour.z) },
			Light{ glm::vec4(s->light.x, s->light.y, s->light.z, s->light.w), s->transpancy },
			LifeTime{ mathEngine::RandNumberBetween(s->lifecounter.first, s->lifecounter.second) },
			Render{ gFakeEngine->GetShapes(s->shape), s->shaderOption },
			RenderTex{ gFakeEngine->GetTexture(s->texture) },
			ParticleType{ s->particletype },
			Flags{ FLAG_ACTIVE | FLAG_VISIBLE },
			ParentId{ s->parentId },
			Force{},
			LightFlag{ LIGHT_NIL },
			PhysicFlag{ s->pflag })
			;

		mGameECSmanager.GetComponent<ParticleActions>(*entity).Add(s->particleActions);

		if (++amount == s->amount)
		{

			if (s->lightsource)
			{

				EntityCreation::AssignEntity(EntitySystemOption::PARTICLE_ENTITY,
					*entity,
					particlesList.SetPosition(s->particletype, s->position, dir, 0),
					particlesList.SetTransformWorld(s->particletype, s->scaleBetwen, s->position, camera2d),
					Rigidbody(),
					Colour{ glm::vec3(s->colour.x, s->colour.y, s->colour.z) },
					Light{ glm::vec4(s->light.x, s->light.y, s->light.z, s->light.w), s->transpancy },
					LifeTime{ mathEngine::RandNumberBetween(s->lifecounter.first, s->lifecounter.second) },
					Render{ gFakeEngine->GetShapes(s->shape), s->shaderOption },
					RenderTex{ gFakeEngine->GetTexture(s->texture) },
					ParticleType{ s->particletype },
					Flags{ FLAG_ACTIVE | FLAG_VISIBLE },
					ParentId{ s->parentId },
					Force{},
					LightFlag{ LIGHT_NIL },
					PhysicFlag{ s->pflag })
					;
				auto& mLightECSmanager = gFakeEngine->GetECSManager<ECSManager>(EntitySystemOption::LIGHT_ENTITY);
				//auto& mLightSystem = gFakeEngine->GetECSLightManager<LightSystemsList>();
				mLightECSmanager.GetComponent<ET_Light_KeyObjectives>(s->lightID).followingId = *entity;
			}

			break;
		}
	}
}

void  ParticleSystem::CreateParticles(CreateParticleEvent* s)
{
	int amount = 0;
	auto& mGameECSmanager = gFakeEngine->GetECSParticlesManager<ECSManager>();
	Camera2d& camera2d = gFakeEngine->GetGraphicManager<Camera2d>();
	ParticlesInfo& particlesList = gFakeEngine->GetGameManager<ParticlesInfo>();

	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (1 == (flag.activeFlag & FLAG_ACTIVE))
			continue;
		float  dir = 0.f;
		dir = particlesList.SetRandomDirectionAngle(s->direction);
		EntityCreation::AssignEntity(EntitySystemOption::PARTICLE_ENTITY,
			entity,
			Position{ s->position },
			particlesList.SetTransformWorld(s->particletype, s->scaleBetwen, s->position, camera2d),
			particlesList.SetRigidbody(s->particletype, s->velocityBetween, s->accelationBetween, dir),
			Colour{ glm::vec3(s->colour.x, s->colour.y, s->colour.z) },
			Light{ glm::vec4(s->light.x, s->light.y, s->light.z, s->light.w), s->transpancy },
			LifeTime{ mathEngine::RandNumberBetween(s->lifecounter.first, s->lifecounter.second) },
			Render{ gFakeEngine->GetShapes(s->shape), s->shaderOption },
			RenderTex{ gFakeEngine->GetTexture(s->texture) },
			ParticleType{ s->particletype },
			Flags{ FLAG_ACTIVE | FLAG_VISIBLE },
			Force{},
			LightFlag{ LIGHT_ALL },
			PhysicFlag{s->pflag}
		);

		mGameECSmanager.GetComponent<ParticleActions>(entity).Add(s->particleActions);

		if (++amount == s->amount)
			break;
	}
}

void ParticleSystem::GenerationUpdate()
{
	Timer& timer = gFakeEngine->GetGameManager<Timer>();
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	static float testtime = 0.f;
	if (timer.CheckSingletonTimer(TimerMap::PARTICLE_GENERATIONN, 10.5f))
	{
		gFakeEngine->PublishEvent(new CreateParticleEvent(
			{ (float)window.GetWidth(), (float)window.GetHeight(),0 },
			ObjectDirection::DOWNLEFT,
			ParticleTypesEnum::PARTICLE_NIL,
			std::make_pair<float, float>(0.f, 0.f),
			std::make_pair<float, float>(2.f, 10.f),
			std::make_pair<float, float>(4.f, 15.f),
			std::make_pair<float, float>(10.1f, 20.2f),
			std::vector<ParticleAction>(),
			{ 0.5f, 0.5f, 0.f }, { 1.f,1.f,1.f, WorldIntensity }, 1.f, ShaderOption::FULL_TEXTURE,
			ObjectShapes::SQUARE, TextureType::GLOWINGBALL,
			mathEngine::RandNumberBetween(1, 3), PhysicFlag{P_NORMAL}));
	}

}