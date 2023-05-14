/*****************************************************************
*\file		 SystemList.h
*\brief		 Header file declarations for SystemList.

*\author(s)   Lee Liang Ping	53 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <SystemsHeaders.h>

struct GameSystemsList
{
	AnimationSystem* animationSystem;
	RenderingSystem* renderingSystem;
	TransformSystem* transformSystem;
	KeyObjectiveSystem* keyObjectiveSystem;
	TextRenderSystem* textRenderSystem;

	PreCollisionSystem* preCollisionSystem;
	CollisionSystem* collisionSystem;
	HotSpotCollisionSystem* hotSpotCollisionSystem;

	PhysicSystem* physicSystem;
	InventorySystem* inventorySystem;
	StateSystem* stateSystem;
	AiSystem* aiSystem;
	UiSystem* uiSystem;

	InputSystem* inputSystem;
	GameInputSystem* gameInputSystem;
	MainMenuInputSystem* mainMenuInputSystem;
	CameraSystem* cameraSystem;
	ImGuiSystem* imGuiSystem;
	LevelEditSystem* levelEditSystem;
	
	LaserSystem* laserSystem;
	LaserProjectileSystem* laserProjectileSystem;
	DoorSystem* doorSystem;

	PauseMenu* pauseMenu;
	Victory_screen* victory_screen;
	Defeat_screen* defeat_screen;
	
	FakeLoadingSystem* fLoadingSystem;

	LeverSystem* leverSystem;
	TeleporterSystem* teleporterSystem;
	Tutorial* tutorialSystem;

	ScoreSystem* scoreSystem;
	MouseSystem* mouseSystem;

};

struct ParticleSystemsList
{
	RenderingSystem* renderingSystem;
	TransformSystem* transformSystem;
	PreCollisionSystem* preCollisionSystem;
	//PhysicSystem* physicSystem;
	ParticleSystem* particleSystem;
	ParticleCollisionSystem* particleCollisionSystem;
	L_P_PhysicSystem* l_p_PhysicSystem;
};

struct LightSystemsList
{
	LightSystem* lightSystem;
	MainChar_LightSystem* mainchar_lightSystem;
	KeyObjectives_LightSystem* keyObjectives_lightSystem;
	KeyObjectivesParts_LightSystem* keyObjectivesParts_lightSystem;
	L_P_PhysicSystem* l_p_PhysicSystem;
};