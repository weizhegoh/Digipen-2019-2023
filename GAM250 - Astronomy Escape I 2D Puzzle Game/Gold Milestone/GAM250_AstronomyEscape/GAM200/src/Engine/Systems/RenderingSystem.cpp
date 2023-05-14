/*****************************************************************
*\file		 RenderingSystem.cpp
*\brief		 RenderingSystem functions definitions.

*\author(s)   Lee Liang Ping	183 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <FakeEngine.h>
#include <RenderingSystem.h>
#include <ComponentsHeader.h>
#include <Collision.h>
#include <Gfx.h>
#include <SystemList.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;

void RenderingSystem::Init(EntitySystemOption systemoption)
{
	auto& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(systemoption);
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	signature.set(mGameECSmanager.GetComponentType<Transform>());
	signature.set(mGameECSmanager.GetComponentType<Render>());
	mGameECSmanager.SetSystemBitSignature<RenderingSystem>(signature);
}

void RenderingSystem::Update(EntitySystemOption systemoption)
{
	auto& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(systemoption);
	std::vector<Entity> zTotalground;
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	if (input.GetInputBooleen().win_screen)
	{
		std::copy_if(mEntities.begin(), mEntities.end(), std::back_inserter(zTotalground),
			[&](Entity a)
			{
				EntityType& entityType = mGameECSmanager.GetComponent<EntityType>(a);
				if (entityType.objType == ObjectType::PAUSE_MENU_BG ||
					entityType.objType == ObjectType::PAUSE_RESUME ||
					entityType.objType == ObjectType::PAUSE_HOWTOPLAY ||
					entityType.objType == ObjectType::PAUSE_MAINMENU ||
					entityType.objType == ObjectType::PAUSE_QUIT ||
					entityType.objType == ObjectType::PAUSE_HOWTOPLAY_BG ||

					entityType.objType == ObjectType::PAUSE_OPTIONS ||
					entityType.objType == ObjectType::QUIT_MENU_BG ||
					entityType.objType == ObjectType::QUIT_YES ||
					entityType.objType == ObjectType::QUIT_NO ||
					entityType.objType == ObjectType::OPTION_MENU_BG ||
					entityType.objType == ObjectType::OPTION_BACK ||
					entityType.objType == ObjectType::FULLSCREEN ||
					entityType.objType == ObjectType::BGM ||
					entityType.objType == ObjectType::RES_1920_1080 ||
					entityType.objType == ObjectType::RES_800_600
					)
					return true;
				else
					return false;
			});
	}
	else
	std::copy(mEntities.begin(), mEntities.end(), std::back_inserter(zTotalground));

	std::sort(std::begin(zTotalground), std::end(zTotalground),
		[&](Entity a, Entity b)
		{
			Position& position_a = mGameECSmanager.GetComponent<Position>(a);
			Position& position_b = mGameECSmanager.GetComponent<Position>(b);
			return position_a.pos.z > position_b.pos.z;
		});

	RenderObjects(zTotalground, systemoption);
}

void  RenderingSystem::RenderObjects(const std::set<Entity>& renderList, EntitySystemOption systemoption)
{
	std::vector<Entity> zTotalground;
	std::copy(renderList.begin(), renderList.end(), std::back_inserter(zTotalground));
	RenderObjects(zTotalground, systemoption);
}


void  RenderingSystem::RenderObjects(const std::vector<Entity>& renderList, EntitySystemOption systemoption)
{
	Graphics::Gfx::RenderFunctions& gRender = gFakeEngine->GetGraphicManager<Graphics::Gfx::RenderFunctions>();
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	AABB aabb;
	aabb.min = { -0.5f * window.GetWidth(), -0.5f * window.GetHeight() };
	aabb.max = { 0.5f * window.GetWidth(), 0.5f * window.GetHeight() };

	auto& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(systemoption);

	for (auto const& entity : renderList)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_VISIBLE))
			continue;
		if (!Collision::Aabb::Intersection_RectRect(aabb, mathEngine::Vector3D(0, 0, 0), aabb, mathEngine::Vector3D(0, 0, 0)))
			continue;

		Render& render = mGameECSmanager.GetComponent<Render>(entity);
		Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
		GLSLShader& shdrpgm = gFakeEngine->GetShader(render.shaderOption);

		Light& light = mGameECSmanager.GetComponent<Light>(entity);
		shdrpgm.Use();
		gRender.BindVertexArray(render.shape->pMesh.vaoid);
		if (render.shaderOption != ShaderOption::SHAPE)
		{
			RenderTex& rendertex = mGameECSmanager.GetComponent<RenderTex>(entity);
			gRender.BindTexture(rendertex.texture->pTex);
			shdrpgm.SetUniform("uTex2d", 6);
			//light.highlight.w = 0.1;
			shdrpgm.SetUniform("uAmbientColor", light.highlight);
		}
		else
		{
			Colour& colour = mGameECSmanager.GetComponent<Colour>(entity);
			shdrpgm.SetUniform("uColor", colour.colour);
		}

		if (render.shaderOption == ShaderOption::ANIMATION_TEXTURE)
		{
			RenderPos& renderPos = mGameECSmanager.GetComponent<RenderPos>(entity);
			Animation& animation = mGameECSmanager.GetComponent<Animation>(entity);
			State& state = mGameECSmanager.GetComponent<State>(entity);
			shdrpgm.SetUniform("uFrameX", (int)renderPos.offset.x);
			shdrpgm.SetUniform("uFrameY", (int)renderPos.offset.y);
			shdrpgm.SetUniform("uFrameIndex", (int)animation.currentFrameIndex + (int)animation.animationsList->frameOffset.find(state.objectState)->second);
		}

		if (render.shaderOption == ShaderOption::TEXTURE_LIGHT || render.shaderOption == ShaderOption::FULL_TEXTURE || render.shaderOption == ShaderOption::ANIMATION_TEXTURE)
		{
			RenderLight(entity, systemoption, shdrpgm);
		}

		gRender.SetTransparency(light.transparncy, shdrpgm);
		gRender.SetTransform(transform.model, transform.view, transform.proj, shdrpgm);
		//gRender.SetRenderScreen(transform.renderType, shdrpgm);
		gRender.DrawElements(render.shape->pMesh, DrawOption::DEFAULT);
		gRender.BindVertexArray(0);
		shdrpgm.UnUse();

		RenderDebug(entity, systemoption);
	}
}

void RenderingSystem::RenderDebug(Entity entity, EntitySystemOption& systemoption)
{
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	if (input.GetInputBooleen().line_key || input.GetInputBooleen().hotspot_key)
	{
		Graphics::Gfx::RenderFunctions& gRender = gFakeEngine->GetGraphicManager<Graphics::Gfx::RenderFunctions>();
		auto& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(systemoption);
		Colour& colour = mGameECSmanager.GetComponent<Colour>(entity);
		GLSLShader& shdpgm = gFakeEngine->GetShader(ShaderOption::SHAPE);
		Render& render = mGameECSmanager.GetComponent<Render>(entity);
		Transform& transform = mGameECSmanager.GetComponent<Transform>(entity);
		shdpgm.Use();
		gRender.SetTransparency(1.f, shdpgm);
		if (_gameStateCurrent == GS_LEVELEDIT)
		{
			Colour col;
			col.colour = { 1.0,0.0,0.0 };
			shdpgm.SetUniform("uColor", col.colour);
		}
		else
			shdpgm.SetUniform("uColor", colour.colour);
		gRender.SetTransform(transform.model, transform.view, transform.proj, shdpgm);
		if (input.GetInputBooleen().line_key)
		{
			gRender.BindVertexArray(render.shape->pMesh.vaoid);
			glLineWidth(3.f);
			gRender.DrawElements(render.shape->pMesh, DrawOption::LINE_STRIP);
		}
		if (input.GetInputBooleen().hotspot_key && render.shape->pHotspot.vaoid)
		{
			gRender.BindVertexArray(render.shape->pHotspot.vaoid);
			glPointSize(5.f);
			gRender.DrawArray(render.shape->pHotspot, DrawOption::POINTS);
		}
		gRender.BindVertexArray(0);
		shdpgm.UnUse();
	}
}

void RenderingSystem::RenderLight(Entity entity, EntitySystemOption& systemoption, GLSLShader& shdrpgm)
{
	Graphics::Gfx::RenderFunctions& gRender = gFakeEngine->GetGraphicManager<Graphics::Gfx::RenderFunctions>();
	auto& mGameECSmanager = gFakeEngine->GetECSManager<ECSManager>(systemoption);
	LightFlag& lightFlag = mGameECSmanager.GetComponent<LightFlag>(entity);
	AABB& obj_aabb = mGameECSmanager.GetComponent<AABB>(entity);


	auto& mLightSystem = gFakeEngine->GetECSLightManager<LightSystemsList>();
	auto& mLightECSmanager = gFakeEngine->GetECSManager<ECSManager>(EntitySystemOption::LIGHT_ENTITY);
	int i = 0;
	for (auto x : LIGHT_CHECK)
	{
		if ((lightFlag.flag & x))
		{
			std::set<Entity> lightSets;
			switch (x)
			{
			case LIGHT_NORMAL:
				lightSets = mLightSystem.lightSystem->mEntities;
				break;
			case LIGHT_MC:
				lightSets = mLightSystem.mainchar_lightSystem->mEntities;
				break;
			case LIGHT_KO:
				lightSets = mLightSystem.keyObjectives_lightSystem->mEntities;
				break;
			case LIGHT_KO_PARTS:
				lightSets = mLightSystem.keyObjectivesParts_lightSystem->mEntities;
				break;
			}

			for (const auto& light_entity : lightSets)
			{
				Flags& et_lightflag = mLightECSmanager.GetComponent<Flags>(light_entity);
				if (0 == (et_lightflag.activeFlag & FLAG_ACTIVE))
					continue;


				Position& position = mLightECSmanager.GetComponent<Position>(light_entity);
				PointLight& pointLight = mLightECSmanager.GetComponent<PointLight>(light_entity);


				if (!Collision::OBB::CircleToRect(Circle(position.pos, pointLight.maxAmbient), obj_aabb))
					continue;

				Colour& colour = mLightECSmanager.GetComponent<Colour>(light_entity);
				gRender.SetLight(pointLight, position.pos, colour.colour, i, shdrpgm);
			}
		}

	}

	if (i >= 0)
	{
		shdrpgm.SetUniform("isLight", true);
		shdrpgm.SetUniform("activeLights", i);
	}
	else
	{
		shdrpgm.SetUniform("isLight", false);
		shdrpgm.SetUniform("activeLights", 0);
	}
}
