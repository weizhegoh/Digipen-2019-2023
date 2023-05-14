/*****************************************************************
*\file		 ParticlesInfo.cpp
*\brief		 This file contains particle helper functions

*\author(s)   Lee Liang Ping	214 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <ParticlesInfo.h>
#include <EntityCreation.h>
extern std::unique_ptr<FakeEngine> gFakeEngine;

void ParticlesInfo::Init(const size_t& amount)
{
	for (unsigned int i = 0; i < amount; ++i)
	{
		EntityCreation::CreateEntity(EntitySystemOption::PARTICLE_ENTITY,
			EntityType{ ObjectType::PARTICLE },
			ParticleType{ ParticleTypesEnum::PARTICLE_NIL },
			ParticleStates{ ParticleState::PARTICLESTATE_NIL },
			Flags{ FLAG_INACTIVE },
			Position(),
			Rigidbody(),
			Transform(),
			Render(),
			RenderTex(),
			Colour(),
			Light(),
			LifeTime(),
			AABB(),
			ParentId(),
			Force(),
			LightFlag(),
			ParticleActions(),
			PhysicFlag()
		);
	}
	totalParticles = (int)amount;
	activeParticles = 0;
}

float ParticlesInfo::SetRandomDirectionAngle(const ObjectDirection direction)
{
	switch (direction)
	{
	case ObjectDirection::CIRCLE:
		return static_cast<float>(mathEngine::RandNumberBetween(0.f, TWO_PI));
		break;
	case ObjectDirection::UP:
		return static_cast<float>(mathEngine::RandNumberBetween(HALF_PI - EIGHTH_PI, HALF_PI + EIGHTH_PI));
		break;
	case ObjectDirection::DOWN:
		return -static_cast<float>(mathEngine::RandNumberBetween(HALF_PI - EIGHTH_PI, HALF_PI + EIGHTH_PI));
		break;
	case ObjectDirection::LEFT:
		return static_cast<float>(mathEngine::RandNumberBetween(PI - EIGHTH_PI, PI + EIGHTH_PI));
		break;
	case ObjectDirection::RIGHT:
		return -static_cast<float>(mathEngine::RandNumberBetween(-EIGHTH_PI, EIGHTH_PI));
		break;
	case ObjectDirection::UPLEFT:
		return static_cast<float>(mathEngine::RandNumberBetween(HALF_PI + EIGHTH_PI, PI - EIGHTH_PI));
		break;
	case ObjectDirection::UPRIGHT:
		return static_cast<float>(mathEngine::RandNumberBetween(EIGHTH_PI, HALF_PI - EIGHTH_PI));
		break;
	case ObjectDirection::DOWNLEFT:
		return static_cast<float>(mathEngine::RandNumberBetween(PI + EIGHTH_PI, PI + QUARTER_PI + EIGHTH_PI));
		break;
	case ObjectDirection::DOWNRIGHT:
		return static_cast<float>(mathEngine::RandNumberBetween(-QUARTER_PI - EIGHTH_PI, -QUARTER_PI + EIGHTH_PI));
		break;
	}
	return 0;
}

Position ParticlesInfo::SetPosition(const ParticleTypesEnum& ptype, const mathEngine::Vector3D& position, const float& dir, float radius)
{
	if (ParticleTypesEnum::PARTICLE_PULSE_INWARDS == ptype || ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS == ptype || ParticleTypesEnum::PARTICLE_PICKUP == ptype)
		return Position{ mathEngine::Vector3D((position.x + (cosf(dir) * radius)),(position.y + (sinf(dir) * radius)), 0.f) };
	else
		return Position{ position };
}

Rigidbody ParticlesInfo::SetRigidbody(const ParticleTypesEnum& ptype, const std::pair<float, float>& velocitybetween, const std::pair<float, float>& accelerationbetween, const float& dir)
{
	Rigidbody rigidBody;
	if (ParticleTypesEnum::PARTICLE_PULSE_INWARDS == ptype)
	{
		if (!(velocitybetween.first == 0.f && velocitybetween.second))
			rigidBody.velocity = { mathEngine::RandNumberBetween(velocitybetween.first, velocitybetween.second) * -cosf(dir),
					mathEngine::RandNumberBetween(velocitybetween.first, velocitybetween.second) * -sinf(dir), 0.f };
		if (!(accelerationbetween.first == 0.f && accelerationbetween.second))
			rigidBody.acceleration = { mathEngine::RandNumberBetween(accelerationbetween.first, accelerationbetween.second) * -cosf(dir),
					mathEngine::RandNumberBetween(accelerationbetween.first, accelerationbetween.second) * -sinf(dir), 0.f };
	}
	else
	{
		if (!(velocitybetween.first == 0.f && velocitybetween.second))
			rigidBody.velocity = { mathEngine::RandNumberBetween(velocitybetween.first, velocitybetween.second) * cosf(dir),
					mathEngine::RandNumberBetween(velocitybetween.first, velocitybetween.second) * sinf(dir), 0.f };
		if (!(accelerationbetween.first == 0.f && accelerationbetween.second))
			rigidBody.acceleration = { mathEngine::RandNumberBetween(accelerationbetween.first, accelerationbetween.second) * cosf(dir),
					mathEngine::RandNumberBetween(accelerationbetween.first, accelerationbetween.second) * sinf(dir), 0.f };
	}
	return rigidBody;
}

Transform ParticlesInfo::SetTransformWorld(const ParticleTypesEnum& ptype, const std::pair<float, float>& between, const mathEngine::Vector3D& position, const Camera2d& camera2d)
{
	(void)ptype;
	Transform transform;
	float scaletemp = static_cast<float>(mathEngine::RandNumberBetween(between.first, between.second));
	transform.scale = { scaletemp, scaletemp, scaletemp };
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 glmposition = glm::vec3(position.x, position.y, position.z);
	model = glm::translate(model, glmposition);
	model = glm::rotate(model, glm::radians(transform.angle + transform.angleoffset), glm::vec3(0.f, 0.f, 1.f));
	model = glm::scale(model, glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));
	transform.model = model;
	transform.view = glm::lookAt(
		glm::vec3(camera2d.pos.x, camera2d.pos.y, 0.f),
		glm::vec3(camera2d.pos.x, camera2d.pos.y, -1.f),
		glm::vec3(0.f, 1.f, 0.f)); // up vector
	transform.proj = glm::ortho(
		float(-camera2d.cam_window.x),
		float(camera2d.cam_window.x),
		float(-camera2d.cam_window.y),
		float(camera2d.cam_window.y));
	return transform;
}

Transform ParticlesInfo::SetTransformScreen(const ParticleTypesEnum& ptype, const std::pair<float, float>& between, const mathEngine::Vector3D& position)
{
	(void)ptype;
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	Transform transform;
	float scaletemp = static_cast<float>(mathEngine::RandNumberBetween(between.first, between.second));
	transform.scale = { scaletemp, scaletemp, scaletemp };
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 glmposition = glm::vec3(position.x, position.y, position.z);
	model = glm::translate(model, glmposition);
	model = glm::rotate(model, glm::radians(transform.angle + transform.angleoffset), glm::vec3(0.f, 0.f, 1.f));
	model = glm::scale(model, glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));
	transform.model = model;
	transform.proj = glm::ortho(
		float(-window.GetHalfWidth()),
		float(window.GetHalfWidth()),
		float(-window.GetHalfHeight()),
		float(window.GetHalfHeight()));
	return transform;
}

void ParticlesInfo::SetDirection(Entity from, Entity to)
{
	auto& mParticleECSmanager = gFakeEngine->GetECSParticlesManager<ECSManager>();
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Position from_pos = mParticleECSmanager.GetComponent<Position>(from);
	Position to_pos = mGameECSmanager.GetComponent<Position>(to);

	Rigidbody& from_ridgidbody = mParticleECSmanager.GetComponent<Rigidbody>(from);

	from_ridgidbody.velocity = mathEngine::Vector3D(0.f, 0.f, 0.f);
	from_ridgidbody.acceleration = to_pos.pos - from_pos.pos;
	from_ridgidbody.acceleration.normalize();
	from_ridgidbody.acceleration *= 50.f;

}

void ParticlesInfo::CreateSmokeParticles(mathEngine::Vector3D position)
{

	TextureType texturesArray[5] =
	{
		TextureType::SMOKE_1,
		TextureType::SMOKE_2,
		TextureType::SMOKE_3,
		TextureType::SMOKE_4,
		TextureType::SMOKE_5
	};

	ObjectDirection directionArray[5] =
	{
		ObjectDirection::UP,
		ObjectDirection::UPRIGHT
	};

	int numberofParticles = mathEngine::RandNumberBetween(5, 10);

	for (int i = 0; i < numberofParticles; ++i)
	{
		int textureRand = mathEngine::RandNumberBetween(1, 5);
		int dirRand = mathEngine::RandNumberBetween(1, 2);

		gFakeEngine->PublishEvent(new CreateParticleEvent(
			position,
			directionArray[dirRand],
			ParticleTypesEnum::PARTICLE_SMOKE,
			std::make_pair<float, float>(0.f, 0.f),
			std::make_pair<float, float>(.02f, .4f),
			std::make_pair<float, float>(80.f, 150.f),
			std::make_pair<float, float>(0.5f, 2.f),
			std::vector<ParticleAction>(),
			{ 1.f, 1.f, 1.f }, { 1.f,1.f,1.f, WorldIntensity }, 0.5f, ShaderOption::FULL_TEXTURE,
			ObjectShapes::SQUARE, texturesArray[textureRand],
			1, PhysicFlag{ P_NORMAL }
		));
	}
}

void ParticlesInfo::TeleportParticlesSTART(mathEngine::Vector3D position, Entity teleportalId)
{

	gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
		position, 0.f, ObjectDirection::CIRCLE,
		ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS,
		std::make_pair<float, float>(100.f, 300.f),
		std::make_pair<float, float>(0.f, 0.f),
		std::make_pair<float, float>(7.f, 9.f),
		std::make_pair<float, float>(1.f, 1.f),
		std::vector<ParticleAction>(),
		{ 1.f, 1.f, 1.f }, { 1.f,1.f,1.f,0.9 }, 1.f, ShaderOption::FULL_TEXTURE,
		ObjectShapes::SQUARE, TextureType::GLOWINGBALL,
		teleportalId, 100, false, PhysicFlag{ P_NORMAL | P_FRICTION }));
}

void ParticlesInfo::TeleportParticlesEND(mathEngine::Vector3D position, Entity teleportalId)
{
	gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
		position, 300.f, ObjectDirection::CIRCLE,
		ParticleTypesEnum::PARTICLE_PULSE_INWARDS,
		std::make_pair<float, float>(100.f, 300.f),
		std::make_pair<float, float>(0.f, 0.f),
		std::make_pair<float, float>(7.f, 9.f),
		std::make_pair<float, float>(1.f, 1.f),
		std::vector<ParticleAction>(),
		{ 1.f, 1.f, 1.f }, { 1.f,1.f,1.f,0.9 }, 1.f, ShaderOption::FULL_TEXTURE,
		ObjectShapes::SQUARE, TextureType::GLOWINGBALL,
		teleportalId, 100, false, PhysicFlag{ P_NORMAL | P_FRICTION }));


}