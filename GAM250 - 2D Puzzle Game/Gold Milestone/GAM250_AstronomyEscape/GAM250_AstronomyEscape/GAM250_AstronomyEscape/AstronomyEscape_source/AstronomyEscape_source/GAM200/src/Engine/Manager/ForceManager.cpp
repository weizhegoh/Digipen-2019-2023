/*****************************************************************
*\file		 ForceManager.cpp
*\brief		 ForceManager functions definitions.

*\author(s)   Lee Jun Jie	109 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include <EnginePch.h>
#include <ForceManager.h>
#include <FakeEngine.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern float g_dt;

ForceClass::ForceClass(ForceType type, mathEngine::Vector3D force, float lifetime) : mLifeTime{ lifetime }
{
	if (type == ForceType::FORCE)
		AddForce(force);
	else if (type == ForceType::FRICTION)
		AddFriction(force);
	else if (type == ForceType::GRAVITY)
		AddGravity(force);
	else
		SummedForce();
}

void ForceClass::AddForce(mathEngine::Vector3D force)
{
	mForce.x = force.x;
	mForce.y = force.y;
}

void ForceClass::AddFriction(mathEngine::Vector3D friction)
{
	mFriction.x = friction.x;
	mFriction.y = friction.y;
}

void ForceClass::AddGravity(mathEngine::Vector3D gravity)
{
	mGravity.x = gravity.x;
	mGravity.y = gravity.y;
}

void ForceClass::SummedForce()
{
	// update value through component list? removing the getter function
	mSummedForce = mForce + mFriction + mGravity;
}

void ForceClass::SetLifeTime(float time)
{
	mLifeTime = time;
}

mathEngine::Vector3D ForceClass::GetForce()
{
	return mForce;
}

mathEngine::Vector3D ForceClass::GetFriction()
{
	return mFriction;
}

mathEngine::Vector3D ForceClass::GetGravity()
{
	return mGravity;
}

mathEngine::Vector3D ForceClass::GetSummedForce()
{
	return mSummedForce;
}

void ForceManager::addforce(Entity entity, mathEngine::Vector3D force, float lifetime)
{
	/*ForceList::iterator x;
	if (x->first == entity && x->second.first == forceType)
		x->second.second.mLifeTime = lifetime;*/
		// check entities and the forcetype if is the same, if same, add/ replace the lifetime.
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Force& forces = mGameECSmanager.GetComponent<Force>(entity);
	forces.lifeTime = lifetime;
	forces.mResultantForce = force;
}

void ForceManager::addforce(Entity entity, mathEngine::Vector3D force, float lifetime, ForceType forceType)
{
	//auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	//Force& forces = mGameECSmanager.GetComponent<Force>(entity);
	//ForceList::iterator x;
	//if (x->first == entity && x->second.first == forceType)
	//	forces.lifeTime = lifetime;
		// check entities and the forcetype if is the same, if same, add/ replace the lifetime.
	forceList.insert(std::make_pair(entity, std::make_pair(forceType, ForceClass(forceType, force, lifetime))));
}

void ForceManager::removeforce(Entity entity)
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Force& forces = mGameECSmanager.GetComponent<Force>(entity);
	forces.mResultantForce = { 0.f, 0.f, 0.f };
}

void ForceManager::UpdateForce()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	std::vector <ForceList::iterator> removeList;
	for (ForceList::iterator it = forceList.begin(); it != forceList.end(); ++it)
	{
		Force& forces = mGameECSmanager.GetComponent<Force>(it->first);
		switch (it->second.first)
		{
		case ForceType::FORCE:
			addforce(it->first, it->second.second.GetForce(), forces.lifeTime, it->second.first);
			break;
		case ForceType::FRICTION:
			addforce(it->first, it->second.second.GetFriction(), forces.lifeTime, it->second.first);
			break;
		case ForceType::GRAVITY:
			addforce(it->first, it->second.second.GetGravity(), forces.lifeTime, it->second.first);
			break;
		case ForceType::SUMMEDFORCES:
			addforce(it->first, forces.mResultantForce, forces.lifeTime, it->second.first);
		default:
			removeforce(it->first);
			break;
		}

		FPS& fps = gFakeEngine->GetGraphicManager<FPS>();
		for (int step = 0; step < fps.GetCurrentNumberOfSteps(); ++step)
			forces.lifeTime -= static_cast<float>(g_dt);
		if (forces.lifeTime <= 0.f)
			removeList.push_back(it);
	}

	for (const auto& x : removeList)
		forceList.erase(x);
}

void ForceManager::Reset()
{
	forceList.clear();
}