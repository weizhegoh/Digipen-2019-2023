/*****************************************************************
*\file		 ParticlesInfo.h
*\brief		 Header file declarations for ParticlesInfo.

*\author(s)   Lee Liang Ping	25 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <FakeEngine.h> 
#include <ComponentsHeader.h>
#include <Camera2D.h>
#include <Window.h>

struct ParticleNum
{
	int totalParticles;
	int activeParticles;
};

class ParticlesInfo : public ParticleNum
{
public:

	void Init(const size_t& amount = 100);

	float SetRandomDirectionAngle(const ObjectDirection direction);
	Position SetPosition(const ParticleTypesEnum& ptype, const mathEngine::Vector3D& position, const float& dir, float radius = 1.f);
	Rigidbody SetRigidbody(const ParticleTypesEnum& ptype, const std::pair<float, float>& velocitybetween, const std::pair<float, float>& accelerationbetween, const float& dir);
	Transform SetTransformWorld(const ParticleTypesEnum& ptype, const std::pair<float, float>& between, const mathEngine::Vector3D& position, const Camera2d& camera2d);
	Transform SetTransformScreen(const ParticleTypesEnum& ptype, const std::pair<float, float>& between, const mathEngine::Vector3D& position);

	void SetDirection(Entity from, Entity to);

	void CreateSmokeParticles(mathEngine::Vector3D position);
	void TeleportParticlesSTART(mathEngine::Vector3D position, Entity teleportalId);
	void TeleportParticlesEND(mathEngine::Vector3D position, Entity teleportalId);
};