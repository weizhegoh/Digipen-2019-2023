/*****************************************************************
*\file		 FakeEngine.h
*\brief		 Header file declarations for FakeEngine.

*\author(s)   Lee Liang Ping	59 lines x 33% Code contribution
*\author(s)   Kenric Tan Wei Liang	59 lines x 33% Code contribution
*\author(s)   Lee Jun Jie	58 lines x 33% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include <EnginePch.h>
#include <GameTypes.h>

// Managers Controllers
#include <TextureManager.h>
#include <ShapeManager.h>
#include <ShaderManager.h>
#include <AnimationManager.h>
#include <FreeTypeFontManager.h>
#include <../GameState/GameStateManager.h>

// Manager Class
#include <ManagerClassesHeader.h>
class MemoryManager;
class EventBus;
class ShapesManager;
class AnimationManager;
class TextureManager;
class ShaderManager;

// System 

class FakeEngine
{
	std::shared_ptr<MemoryManager> mECSGameManager;
	std::shared_ptr<MemoryManager> mECSParticleManager;
	std::shared_ptr<MemoryManager> mECSLightManager;

	std::shared_ptr<EventBus> mEventBus;

	std::shared_ptr<ShapesManager> mShapesManager;
	std::shared_ptr<AnimationManager> mAnimationManager;
	std::shared_ptr<TextureManager> mTextureManager;
	std::shared_ptr<ShaderManager> mShaderManager;
	std::shared_ptr<Graphics::FreeTypeFontManager> mFreeTypeManager;

	std::shared_ptr<MemoryManager> mGrapicManager;
	std::shared_ptr<MemoryManager> mGameManager;

public:

	void Init()
	{
		mECSGameManager = std::make_shared<MemoryManager>();
		mECSParticleManager = std::make_shared<MemoryManager>();
		mECSLightManager = std::make_shared<MemoryManager>();

		mEventBus = std::make_shared<EventBus>();
		mShapesManager = std::make_shared<ShapesManager>();
		mTextureManager = std::make_shared<TextureManager>();
		mAnimationManager = std::make_shared<AnimationManager>();
		mShaderManager = std::make_shared<ShaderManager>();
		mFreeTypeManager = std::make_shared<Graphics::FreeTypeFontManager>();

		mGrapicManager = std::make_shared<MemoryManager>();
		mGameManager = std::make_shared<MemoryManager>();
	}

	void Exit()
	{
		mECSGameManager.reset();
		mECSParticleManager.reset();
		mECSLightManager.reset();
		mEventBus.reset();

		mShapesManager.reset();
		mTextureManager.reset();
		mShaderManager.reset();
		mFreeTypeManager.reset();
		mAnimationManager.reset();

		mGrapicManager.reset();
		mGameManager.reset();
	}

	template<typename T>
	void RegisterECSGameManager()
	{
		mECSGameManager->RegisterManager<T>();
	}

	template<typename T>
	T& GetECSGameManager()
	{
		return mECSGameManager->GetManager<T>();
	}

	template<typename T>
	void RegisterECSParticlesManager()
	{
		mECSParticleManager->RegisterManager<T>();
	}

	template<typename T>
	T& GetECSParticlesManager()
	{
		return mECSParticleManager->GetManager<T>();
	}

	template<typename T>
	void RegisterECSLightManager()
	{
		mECSLightManager->RegisterManager<T>();
	}

	template<typename T>
	T& GetECSLightManager()
	{
		return mECSLightManager->GetManager<T>();
	}

	template<typename T>
	T& GetECSManager(EntitySystemOption systemoption)
	{
		if (systemoption == EntitySystemOption::GAME_ENTITY)
			return	GetECSGameManager<ECSManager>();
		else if (systemoption == EntitySystemOption::PARTICLE_ENTITY)
			return GetECSParticlesManager<ECSManager>();
		else
			return GetECSLightManager<ECSManager>();
	}

	// ASSETS METHOD
	void PushTextureAssets(TextureType objType, const char* pFileName)
	{
		mTextureManager->PushTextures(objType, pFileName);
	}

	void PushShapeAssets(ObjectShapes objShapes)
	{
		mShapesManager->PushShapes(objShapes);
	}

	Shapes* GetShapes(ObjectShapes objShapes)
	{
		return mShapesManager->GetShapes(objShapes);
	}

	Textures* GetTexture(TextureType objType)
	{
		return mTextureManager->GetTextures(objType);

	}
	void FreeTextures()
	{
		mTextureManager->FreeTextures();
	}

	void RegisterShader(ShaderOption option, std::string vert, std::string frag)
	{
		mShaderManager->AddShader(option, vert, frag);
	}

	GLSLShader& GetShader(const ShaderOption index = ShaderOption::SHAPE)
	{
		return mShaderManager->GetShader(index);
	}

	void RegisterFreeTypeFont(FreeType_Font option, std::string file)
	{
		mFreeTypeManager->AddFont(option, file);
	}

	Graphics::FreeType& GetFreeTypeFont(const FreeType_Font index = FreeType_Font::OpenSansRegular)
	{
		return mFreeTypeManager->GetFont(index);
	}

	template<typename T>
	void RegisterGameManager()
	{
		mGameManager->RegisterManager<T>();
	}

	template<typename T>
	T& GetGameManager()
	{
		return mGameManager->GetManager<T>();
	}

	template<typename T>
	void RemoveGameManager()
	{
		mGameManager->RemoveManager<T>();
	}

	void RemoveGameManagerList()
	{
		mGameManager->RemoveManagerList();
	}

	template<typename T>
	void RegisterGraphicManager()
	{
		mGrapicManager->RegisterManager<T>();
	}

	template<typename T>
	T& GetGraphicManager()
	{
		return mGrapicManager->GetManager<T>();
	}

	// ANIMATION METHOD
	void PushObjectAnimations(TextureType objType, const char* pFileName)
	{
		mAnimationManager->AddAnimation(objType, pFileName);
	}

	Animations* GetObjectAnimations(TextureType objType)
	{
		return mAnimationManager->GetObjectAnimations(objType);
	}

	template<class T, class EventType>
	void SubscribeEvent(T* instance, void (T::* eventFunction)(EventType*), EventID id)
	{
		mEventBus->subscribe(instance, eventFunction, id);
	}

	template<typename EventType>
	void PublishEvent(EventType* event)
	{
		mEventBus->publish(event);
	}

	void ExecuteQueuedEvents()
	{
		mEventBus->executeQueue();
	}

	template<typename EventType>
	void ExecuteEvent(EventType* event)
	{
		mEventBus->execute(event);
	}
};