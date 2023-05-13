/*****************************************************************
*\file		 ECSManager.h
*\brief		 ECS WRAPPER CLASS

*\author(s)   Kenric Tan Wei Liang		26 lines x 33.33% Code contribution
*\author(s)   Lee Jun Jie				26 lines x 33.33% Code contribution
*\author(s)   Lee Liang Ping			26 lines x 33.33% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>

class ECSManager
{
	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<SystemManager> mSystemManager;

public:
	ECSManager()
	{
		mEntityManager = std::make_unique<EntityManager>();
		mComponentManager = std::make_unique<ComponentManager>();
		mSystemManager = std::make_unique<SystemManager>();
	}

	// ENTITIES METHOD
	Entity CreateEntity()
	{
		return mEntityManager->CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		mEntityManager->DestroyEntity(entity);
		mComponentManager->EntityDestroyed(entity);
		mSystemManager->EntityDestroyed(entity);
	}

	void FreeEntityList()
	{
		for (auto entity = mEntityManager->List().begin(); entity != mEntityManager->List().end(); )
			DestroyEntity(*entity++);
	}

	template<typename T>
	void RegisterComponent()
	{
		mComponentManager->RegisterComponent<T>();
	}

	// COMPONENTS METHOD
	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		mComponentManager->AddComponent<T>(entity, component);

		auto bitSignature = mEntityManager->GetBitSignature(entity);
		bitSignature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetBitSignature(entity, bitSignature);

		mSystemManager->EntitySignatureChanged(entity, bitSignature);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		mComponentManager->RemoveComponent<T>(entity);

		auto bitSignature = mEntityManager->GetBitSignature(entity);
		bitSignature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetBitSignature(entity, bitSignature);

		mSystemManager->EntitySignatureChanged(entity, bitSignature);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return mComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}

	template<typename T>
	bool CheckComponent(Entity entity)
	{
		return mComponentManager->CheckData<T>(entity);
	}

	// SYSTEM METHOD
	template<typename T>
	T* RegisterSystem()
	{
		return mSystemManager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemBitSignature(BitSignature signature)
	{
		mSystemManager->SetBitSignature<T>(signature);
	}
};