/*****************************************************************
*\file		 Event.h
*\brief		 Header file declarations for Event.

*\author(s)   Lee Liang Ping	66 lines x 33% Code contribution
*\author(s)   Kenric Tan Wei Liang	66 lines x 33% Code contribution
*\author(s)   Lee Jun Jie	65 lines x 33% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <GameTypes.h>
#include <PhysicFlag.h>

struct Event
{
	EventID id;

	Event(EventID event_id) :
		id{ event_id }
	{}
	virtual ~Event()
	{}
};

struct SetBoulderStatusEvent : public Event
{
	Entity entityA;
	SetBoulderStatusEvent(Entity a) :
		Event(EventID::SetBoulderStatusEvent),
		entityA{ a }
	{}
};

struct PickUpEvent : public Event
{
	Entity main_characterID;
	Entity itemID;

	PickUpEvent(Entity a, Entity b) :
		Event(EventID::PickUpEvent),
		main_characterID{ a },
		itemID{ b }
	{}
};

struct TutorialActivate : public Event
{
	Entity main_characterID;
	Entity tutorialFloorID;

	TutorialActivate(Entity a, Entity b) :
		Event(EventID::ActivateTutorial),
		main_characterID{ a },
		tutorialFloorID{ b }
	{}
};

struct CreateParticleEvent : public Event
{
	mathEngine::Vector3D position;
	ObjectDirection direction;
	ParticleTypesEnum particletype;
	std::pair<float, float> velocityBetween;
	std::pair<float, float> accelationBetween;
	std::pair<float, float> scaleBetwen;
	std::pair<float, float> lifecounter;
	std::vector<ParticleAction> particleActions;
	mathEngine::Vector3D colour;
	glm::vec4 light;
	float transpancy;
	ShaderOption shaderOption;
	ObjectShapes shape;
	TextureType texture;
	int amount;
	PhysicFlag pflag;

	CreateParticleEvent(
		mathEngine::Vector3D position,
		ObjectDirection direction,
		ParticleTypesEnum particletype,
		std::pair<float, float> velocityBetween,
		std::pair<float, float> accelationBetween,
		std::pair<float, float> scaleBetwen,
		std::pair<float, float> lifecounter,
		std::vector<ParticleAction> particleActions,
		mathEngine::Vector3D colour,
		glm::vec4 light,
		float transpancy,
		ShaderOption shaderOption,
		ObjectShapes shape,
		TextureType texture,
		int amount,
		PhysicFlag pflag
	) :
		Event(EventID::CreateParticleEvent),
		position{ position },
		direction{ direction },
		particletype{ particletype },
		velocityBetween{ velocityBetween },
		accelationBetween{ accelationBetween },
		scaleBetwen{ scaleBetwen },
		lifecounter{ lifecounter },
		particleActions{ particleActions },
		colour{ colour },
		light{ light },
		transpancy{ transpancy },
		shaderOption{ shaderOption },
		shape{ shape },
		texture{ texture },
		amount{ amount },
		pflag{ pflag }
	{}


};

struct CreatePulseParticleEvent : public Event
{
	mathEngine::Vector3D position;
	float pulseRadius;
	ObjectDirection direction;
	ParticleTypesEnum particletype;
	std::pair<float, float> velocityBetween;
	std::pair<float, float> accelerateBetween;
	std::pair<float, float> scaleBetwen;
	std::pair<float, float> lifecounter;
	std::vector<ParticleAction> particleActions;
	mathEngine::Vector3D colour;
	glm::vec4 light;
	float transpancy;
	ShaderOption shaderOption;
	ObjectShapes shape;
	TextureType texture;
	Entity parentId;
	int amount;
	bool lightsource;
	Entity lightID;
	PhysicFlag pflag;

	CreatePulseParticleEvent(
		mathEngine::Vector3D position,
		float pulseRadius,
		ObjectDirection direction,
		ParticleTypesEnum particletype,
		std::pair<float, float> velocityBetween,
		std::pair<float, float> accelerateBetween,
		std::pair<float, float> scaleBetwen,
		std::pair<float, float> lifecounter,
		std::vector<ParticleAction> particleActions,
		mathEngine::Vector3D colour,
		glm::vec4 light,
		float transpancy,
		ShaderOption shaderOption,
		ObjectShapes shape,
		TextureType texture,
		Entity parentId,
		int amount,
		bool lightsource,
		PhysicFlag pflag
	) :
		Event(EventID::CreatePulseParticleEvent),
		position{ position },
		pulseRadius{ pulseRadius },
		direction{ direction },
		particletype{ particletype },
		velocityBetween{ velocityBetween },
		accelerateBetween{ accelerateBetween },
		scaleBetwen{ scaleBetwen },
		lifecounter{ lifecounter },
		particleActions{ particleActions },
		colour{ colour },
		light{ light },
		transpancy{ transpancy },
		shaderOption{ shaderOption },
		shape{ shape },
		texture{ texture },
		parentId{ parentId },
		amount{ amount },
		lightsource{ lightsource },
		lightID{ NULL },
		pflag{ pflag }
	{}

	CreatePulseParticleEvent(
		mathEngine::Vector3D position,
		float pulseRadius,
		ObjectDirection direction,
		ParticleTypesEnum particletype,
		std::pair<float, float> velocityBetween,
		std::pair<float, float> accelerateBetween,
		std::pair<float, float> scaleBetwen,
		std::pair<float, float> lifecounter,
		std::vector<ParticleAction> particleActions,
		mathEngine::Vector3D colour,
		glm::vec4 light,
		float transpancy,
		ShaderOption shaderOption,
		ObjectShapes shape,
		TextureType texture,
		Entity parentId,
		int amount,
		bool lightsource,
		Entity lightid,
		PhysicFlag pflag
	) :
	Event(EventID::CreatePulseParticleEvent),
		position{ position },
		pulseRadius{ pulseRadius },
		direction{ direction },
		particletype{ particletype },
		velocityBetween{ velocityBetween },
		accelerateBetween{ accelerateBetween },
		scaleBetwen{ scaleBetwen },
		lifecounter{ lifecounter },
		particleActions{ particleActions },
		colour{ colour },
		light{ light },
		transpancy{ transpancy },
		shaderOption{ shaderOption },
		shape{ shape },
		texture{ texture },
		parentId{ parentId },
		amount{ amount },
		lightsource{ lightsource },
		lightID{ lightid },
		pflag{ pflag }
	{}


};

struct CreateParticleEvents : public Event
{
	mathEngine::Vector3D position;
	float pulseRadius;
	ObjectDirection direction;
	ParticleTypesEnum particletype;
	std::pair<float, float> velocityBetween;
	std::pair<float, float> scaleBetwen;
	std::pair<float, float> lifecounter;
	std::vector<ParticleAction> particleActions;
	mathEngine::Vector3D colour;
	glm::vec4 light;
	float transpancy;
	ShaderOption shaderOption;
	ObjectShapes shape;
	TextureType texture;
	Entity parentId;
	int amount;
	ParticleTypesEnum ptype;
	PhysicFlag pflag;

	CreateParticleEvents(
		ParticleTypesEnum ptype,
		mathEngine::Vector3D position,
		float pulseRadius,
		ObjectDirection direction,
		ParticleTypesEnum particletype,
		std::pair<float, float> velocityBetween,
		std::pair<float, float> scaleBetwen,
		std::pair<float, float> lifecounter,
		std::vector<ParticleAction> particleActions,
		mathEngine::Vector3D colour,
		glm::vec4 light,
		float transpancy,
		ShaderOption shaderOption,
		ObjectShapes shape,
		TextureType texture,
		Entity parentId,
		int amount,
		PhysicFlag pflag
	) :
		Event(EventID::CreatePulseParticleEvent),
		ptype{ ptype },
		position{ position },
		pulseRadius{ pulseRadius },
		direction{ direction },
		particletype{ particletype },
		velocityBetween{ velocityBetween },
		scaleBetwen{ scaleBetwen },
		lifecounter{ lifecounter },
		particleActions{ particleActions },
		colour{ colour },
		light{ light },
		transpancy{ transpancy },
		shaderOption{ shaderOption },
		shape{ shape },
		texture{ texture },
		parentId{ parentId },
		amount{ amount },
		pflag{ pflag }
	{}
};




struct SetLoadingScreen : public Event
{
	bool set;
	SetLoadingScreen(
		bool set
	) :
		Event(EventID::SetLoadingScreenEvent),
		set{ set }
	{}
};


struct SetWinScreen : public Event
{
	bool set;
	int pageNum;
	SetWinScreen(
		bool set,
		int pageNum
	) :
		Event(EventID::SetWincreenEvent),
		set{ set },
		pageNum{ pageNum }
	{}
};

