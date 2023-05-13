/*****************************************************************
*\file		 ForceManager.h
*\brief		 Header file declarations for ForceManager.

*\author(s)   Lee Jun Jie	35 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <EnginePch.h>
#include <GameTypes.h>

class ForceClass
{
	mathEngine::Vector3D mForce{ 0.f, 0.f, 0.f };
	mathEngine::Vector3D mFriction{ 0.f, 0.f, 0.f };
	mathEngine::Vector3D mGravity{ 0.f, 0.f, 0.f };
	mathEngine::Vector3D mSummedForce{ 0.f, 0.f, 0.f };
	float mLifeTime{ 0.f };

public:
	ForceClass(ForceType type, mathEngine::Vector3D force, float lifetime);
	~ForceClass() = default;

	//void Integrate(float dt);
	void AddForce(mathEngine::Vector3D force);
	void AddFriction(mathEngine::Vector3D force);
	void AddGravity(mathEngine::Vector3D force);
	void SummedForce();
	void SetLifeTime(float time);

	// Getter function
	mathEngine::Vector3D GetForce();
	mathEngine::Vector3D GetFriction();
	mathEngine::Vector3D GetGravity();
	mathEngine::Vector3D GetSummedForce();
};

class ForceManager
{
	using ForceList = std::map<Entity, std::pair<ForceType, ForceClass>>;
	ForceList forceList;

public:
	ForceManager() = default;
	void addforce(Entity entity, mathEngine::Vector3D force, float lifetime);
	void addforce(Entity entity, mathEngine::Vector3D force, float lifetime, ForceType forceType);
	void removeforce(Entity entity);
	void UpdateForce();
	void Reset();
};