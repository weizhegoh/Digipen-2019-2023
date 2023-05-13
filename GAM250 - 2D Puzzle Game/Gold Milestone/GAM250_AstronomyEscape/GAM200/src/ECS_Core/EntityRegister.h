/*****************************************************************
*\file		 EntityRegister.h
*\brief		 Helper Function to Register ECS 
			
*\author(s)   Lee Jun Jie				68 lines x 73.% Code contribution
*\author(s)   Lee Liang Ping			65 lines x 27% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <FakeEngine.h>
#include <SystemList.h>
extern std::unique_ptr<FakeEngine> gFakeEngine;


namespace EntityRegister
{

	void RegisterECSGameComponents()
	{
		auto& ecsEngine = gFakeEngine->GetECSGameManager<ECSManager>();
		ecsEngine.RegisterComponent<EntityType>();
		ecsEngine.RegisterComponent<Render>();
		ecsEngine.RegisterComponent<Position>();
		ecsEngine.RegisterComponent<Transform>();
		ecsEngine.RegisterComponent<Rigidbody>();
		ecsEngine.RegisterComponent<Input>();
		ecsEngine.RegisterComponent<Colour>();
		ecsEngine.RegisterComponent<AABB>();
		ecsEngine.RegisterComponent<State>();
		ecsEngine.RegisterComponent<LineSegment>();
		ecsEngine.RegisterComponent<Flags>();
		ecsEngine.RegisterComponent<CollisionFlag>();
		ecsEngine.RegisterComponent<Inventory>();
		ecsEngine.RegisterComponent<RenderPos>();
		ecsEngine.RegisterComponent<Animation>();
		ecsEngine.RegisterComponent<AI>();
		ecsEngine.RegisterComponent<UI>();
		ecsEngine.RegisterComponent<RenderTex>();
		ecsEngine.RegisterComponent<Light>();
		ecsEngine.RegisterComponent<ParticleType>();
		ecsEngine.RegisterComponent<ParticleStates>();
		ecsEngine.RegisterComponent<LifeTime>();
		ecsEngine.RegisterComponent<Laser>();
		ecsEngine.RegisterComponent<ParentId>();
		ecsEngine.RegisterComponent<Force>();
		ecsEngine.RegisterComponent<MainMenuType>();
		ecsEngine.RegisterComponent<LoadingType>();
		ecsEngine.RegisterComponent<Comet>();
		ecsEngine.RegisterComponent<LightFlag>();
		ecsEngine.RegisterComponent<Lever>();
		ecsEngine.RegisterComponent<Teleporter>();
		ecsEngine.RegisterComponent<Ray>();
		ecsEngine.RegisterComponent<KeyObjective>();
		ecsEngine.RegisterComponent<Mouse>();
	}

	void RegisterECSGameSystem()
	{
		auto& ecsEngine = gFakeEngine->GetECSGameManager<ECSManager>();
		auto& systemList = gFakeEngine->GetECSGameManager<GameSystemsList>();
		// Register Systems
		systemList.renderingSystem = ecsEngine.RegisterSystem<RenderingSystem>();
		systemList.transformSystem = ecsEngine.RegisterSystem<TransformSystem>();
		systemList.inputSystem = ecsEngine.RegisterSystem<InputSystem>();
		systemList.cameraSystem = ecsEngine.RegisterSystem<CameraSystem>();
		systemList.collisionSystem = ecsEngine.RegisterSystem<CollisionSystem>();
		systemList.preCollisionSystem = ecsEngine.RegisterSystem<PreCollisionSystem>();
		systemList.physicSystem = ecsEngine.RegisterSystem<PhysicSystem>();
		systemList.inventorySystem = ecsEngine.RegisterSystem<InventorySystem>();
		systemList.animationSystem = ecsEngine.RegisterSystem<AnimationSystem>();
		systemList.aiSystem = ecsEngine.RegisterSystem<AiSystem>();
		systemList.uiSystem = ecsEngine.RegisterSystem<UiSystem>();
		systemList.keyObjectiveSystem = ecsEngine.RegisterSystem<KeyObjectiveSystem>();
		systemList.textRenderSystem = ecsEngine.RegisterSystem<TextRenderSystem>();
		systemList.imGuiSystem = ecsEngine.RegisterSystem<ImGuiSystem>();
		systemList.stateSystem = ecsEngine.RegisterSystem<StateSystem>();
		systemList.levelEditSystem = ecsEngine.RegisterSystem<LevelEditSystem>();
		systemList.laserSystem = ecsEngine.RegisterSystem<LaserSystem>();
		systemList.laserProjectileSystem = ecsEngine.RegisterSystem<LaserProjectileSystem>();
		systemList.mainMenuInputSystem = ecsEngine.RegisterSystem<MainMenuInputSystem>();
		systemList.hotSpotCollisionSystem = ecsEngine.RegisterSystem<HotSpotCollisionSystem>();
		systemList.doorSystem = ecsEngine.RegisterSystem<DoorSystem>();
		systemList.pauseMenu = ecsEngine.RegisterSystem<PauseMenu>();
		systemList.victory_screen = ecsEngine.RegisterSystem<Victory_screen>();
		systemList.defeat_screen = ecsEngine.RegisterSystem<Defeat_screen>();
		systemList.gameInputSystem = ecsEngine.RegisterSystem<GameInputSystem>();
		systemList.fLoadingSystem = ecsEngine.RegisterSystem<FakeLoadingSystem>();
		systemList.leverSystem = ecsEngine.RegisterSystem<LeverSystem>();
		systemList.teleporterSystem = ecsEngine.RegisterSystem<TeleporterSystem>();
		systemList.tutorialSystem = ecsEngine.RegisterSystem<Tutorial>();

		systemList.scoreSystem = ecsEngine.RegisterSystem<ScoreSystem>();
		systemList.mouseSystem = ecsEngine.RegisterSystem<MouseSystem>();
	}

	void InitECSGameSystem()
	{
		auto& systemList = gFakeEngine->GetECSGameManager<GameSystemsList>();
		systemList.inputSystem->Init();
		systemList.cameraSystem->Init();
		systemList.preCollisionSystem->Init(EntitySystemOption::GAME_ENTITY);
		systemList.collisionSystem->Init();
		systemList.physicSystem->Init(EntitySystemOption::GAME_ENTITY);
		systemList.renderingSystem->Init(EntitySystemOption::GAME_ENTITY);
		systemList.transformSystem->Init(EntitySystemOption::GAME_ENTITY);
		systemList.inventorySystem->Init();
		systemList.animationSystem->Init();
		systemList.aiSystem->Init();
		systemList.uiSystem->Init();
		systemList.keyObjectiveSystem->Init();
		systemList.textRenderSystem->Init();
		systemList.stateSystem->Init();
		systemList.levelEditSystem->Init();
		systemList.laserSystem->Init();
		systemList.laserProjectileSystem->Init();
		systemList.mainMenuInputSystem->Init();
		systemList.hotSpotCollisionSystem->Init();
		systemList.doorSystem->Init();
		systemList.pauseMenu->Init();
		systemList.victory_screen->Init();
		systemList.defeat_screen->Init();
		systemList.gameInputSystem->Init();
		systemList.fLoadingSystem->Init();
		systemList.leverSystem->Init();
		systemList.teleporterSystem->Init();
		systemList.tutorialSystem->Init();
		systemList.scoreSystem->Init();
		systemList.mouseSystem->Init();
	}

	void RegisterECSParticlesComponents()
	{
		auto& ecsEngine = gFakeEngine->GetECSParticlesManager<ECSManager>();
		ecsEngine.RegisterComponent<EntityType>();
		ecsEngine.RegisterComponent<ParticleType>();
		ecsEngine.RegisterComponent<ParticleStates>();
		ecsEngine.RegisterComponent<Flags>();
		ecsEngine.RegisterComponent<Position>();
		ecsEngine.RegisterComponent<Rigidbody>();
		ecsEngine.RegisterComponent<Transform>();
		ecsEngine.RegisterComponent<Render>();
		ecsEngine.RegisterComponent<RenderTex>();
		ecsEngine.RegisterComponent<Colour>();
		ecsEngine.RegisterComponent<Light>();
		ecsEngine.RegisterComponent<LifeTime>();
		ecsEngine.RegisterComponent<CollisionFlag>();
		ecsEngine.RegisterComponent<AABB>();
		ecsEngine.RegisterComponent<ParentId>();
		ecsEngine.RegisterComponent<Force>();
		ecsEngine.RegisterComponent<PointLight>();
		ecsEngine.RegisterComponent<LightFlag>();
		ecsEngine.RegisterComponent<Ray>();
		ecsEngine.RegisterComponent<ParticleActions>();		
		ecsEngine.RegisterComponent<PhysicFlag>();		
	}

	void RegisterECSParticlesSystem()
	{
		auto& ecsEngine = gFakeEngine->GetECSParticlesManager<ECSManager>();
		auto& systemList = gFakeEngine->GetECSParticlesManager<ParticleSystemsList>();
		// Register Systems
		systemList.renderingSystem = ecsEngine.RegisterSystem<RenderingSystem>();
		systemList.transformSystem = ecsEngine.RegisterSystem<TransformSystem>();
		systemList.preCollisionSystem = ecsEngine.RegisterSystem<PreCollisionSystem>();
		//systemList.physicSystem = ecsEngine.RegisterSystem<PhysicSystem>();
		systemList.particleSystem = ecsEngine.RegisterSystem<ParticleSystem>();
		systemList.particleCollisionSystem = ecsEngine.RegisterSystem<ParticleCollisionSystem>();
		systemList.l_p_PhysicSystem = ecsEngine.RegisterSystem<L_P_PhysicSystem>();
	}

	void InitECSParticlesSystem()
	{
		auto& systemListParitcles = gFakeEngine->GetECSParticlesManager<ParticleSystemsList>();
		//systemListParitcles.physicSystem->Init(EntitySystemOption::PARTICLE_ENTITY);
		systemListParitcles.l_p_PhysicSystem->Init(EntitySystemOption::PARTICLE_ENTITY);
		systemListParitcles.preCollisionSystem->Init(EntitySystemOption::PARTICLE_ENTITY);
		systemListParitcles.renderingSystem->Init(EntitySystemOption::PARTICLE_ENTITY);
		systemListParitcles.transformSystem->Init(EntitySystemOption::PARTICLE_ENTITY);
		systemListParitcles.particleSystem->Init();
		systemListParitcles.particleCollisionSystem->Init();
	}

	void RegisterECSLightComponents()
	{
		auto& ecsEngine = gFakeEngine->GetECSLightManager<ECSManager>();
		ecsEngine.RegisterComponent<Flags>();
		ecsEngine.RegisterComponent<Position>();
		ecsEngine.RegisterComponent<Rigidbody>();
		ecsEngine.RegisterComponent<Colour>();
		ecsEngine.RegisterComponent<LifeTime>();
		ecsEngine.RegisterComponent<PointLight>();
		ecsEngine.RegisterComponent<Ray>();
		ecsEngine.RegisterComponent<PhysicFlag>();
		//Light Entities
		ecsEngine.RegisterComponent<ET_LightNormal>();
		ecsEngine.RegisterComponent<ET_Light_MainChar>();
		ecsEngine.RegisterComponent<ET_Light_KeyObjectives>();
		ecsEngine.RegisterComponent<ET_Light_KeyObjectives_Parts>();
		//for debuging purpose.
		ecsEngine.RegisterComponent<Transform>();
		ecsEngine.RegisterComponent<Render>();	
	}

	void RegisterECSLightSystem()
	{
		auto& ecsEngine = gFakeEngine->GetECSLightManager<ECSManager>();
		auto& systemList = gFakeEngine->GetECSLightManager<LightSystemsList>();
		// Register Systems
		systemList.lightSystem = ecsEngine.RegisterSystem<LightSystem>();
		systemList.mainchar_lightSystem = ecsEngine.RegisterSystem<MainChar_LightSystem>();
		systemList.keyObjectives_lightSystem = ecsEngine.RegisterSystem<KeyObjectives_LightSystem>();
		systemList.keyObjectivesParts_lightSystem = ecsEngine.RegisterSystem<KeyObjectivesParts_LightSystem>();
		systemList.l_p_PhysicSystem = ecsEngine.RegisterSystem<L_P_PhysicSystem>();
	}

	void InitECSLightSystem()
	{
		auto& systemList = gFakeEngine->GetECSLightManager<LightSystemsList>();
		systemList.lightSystem->Init();
		systemList.mainchar_lightSystem->Init();
		systemList.keyObjectives_lightSystem->Init();
		systemList.l_p_PhysicSystem->Init(EntitySystemOption::LIGHT_ENTITY);

	}

}