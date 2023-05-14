/*****************************************************************
*\file		 TransformSystem.cpp
*\brief		 TransformSystem functions definitions.

*\author(s)   Lee Liang Ping	54 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <TransformSystem.h>
#include <FakeEngine.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;

void TransformSystem::Init(EntitySystemOption systemoption)
{
	auto& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(systemoption);
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<Transform>());
	mGameECSmanager.SetSystemBitSignature<TransformSystem>(signature);
}


void TransformSystem::Update(EntitySystemOption systemoption)
{
	for (auto const& entity : mEntities)
		transform(entity, systemoption);
}

void TransformSystem::transform(Entity entity, EntitySystemOption systemoption)
{
	ECSManager& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(systemoption);
	Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
	if (0 == (flag.activeFlag & FLAG_ACTIVE))
		return;
	Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
	Position& position = mGameECSmanager.GetComponent<Position>(entity);
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 glmposition = glm::vec3(position.pos.x, position.pos.y, position.pos.z);
	model = glm::translate(model, glmposition);
	model = glm::rotate(model, glm::radians(transform.angle + transform.angleoffset), glm::vec3(0.f, 0.f, 1.f));
	model = glm::scale(model, glm::vec3(transform.scale.x + transform.scaleoffset.x, transform.scale.y + transform.scaleoffset.y, transform.scale.z));
	transform.model = model;

	if (transform.renderType == FreeRenderType::MODEL_WORLD)
	{
		Camera2d& camera2d = gFakeEngine->GetGraphicManager<Camera2d>();
		transform.view = glm::lookAt(
			glm::vec3(camera2d.pos.x, camera2d.pos.y, 0.f),
			glm::vec3(camera2d.pos.x, camera2d.pos.y, -1.f),
			glm::vec3(0.f, 1.f, 0.f)); // up vector
		transform.proj = glm::ortho(
			float(-camera2d.cam_window.x),
			float(camera2d.cam_window.x),
			float(-camera2d.cam_window.y),
			float(camera2d.cam_window.y));
	}
	else
	{
		Window& window = gFakeEngine->GetGraphicManager<Window>();
		transform.view = glm::mat4(1.0f);
		transform.proj = glm::ortho(
			float(-window.GetHalfWidth()),
			float(window.GetHalfWidth()),
			float(-window.GetHalfHeight()),
			float(window.GetHalfHeight()));
	}
}