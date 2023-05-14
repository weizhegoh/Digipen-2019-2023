/*****************************************************************
*\file         StaticRendererSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "StaticRendererSystem.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
namespace Xivi::Graphics
{
void StaticRendererSystem::OnCreate() noexcept
{
  m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
  m_framebuffer = &Service::Locator::Get<Graphics::M_FrameBuffer>();
}

void StaticRendererSystem::operator()(
  Entity &entity,
  Position &pos,
  Transform &transform,
  StaticMeshComponent &mesh,
  Material &material,
  Colour &colour,
  RenderSettings &renderSettings,
  LightData &lightData ) noexcept
{
  //XV_CORE_WARN(m_world.m_entity_manager.m_entities[entity.Index()].m_name);
  // Possible optimisation by removing aabb checks
  if ( !renderSettings.onRender )
    return;
  auto &camera = m_graphic_manager->camera;
  if ( HaveComponent<AABB>( entity ) )
  {
    auto &aabb = GetComponent<AABB>( entity );
    if ( !aabb.isOnFrustum( camera->GetFrustum(), transform ) )
      return;
  }

  //auto& shadowParameter = m_graphic_manager->shadowParameter;
  auto &frameBuffer = m_graphic_manager->frameBuffer;
  //auto& camera = m_graphic_manager->camera;
  auto &shdrpgm = m_graphic_manager->GetShaders( ShaderOption::TEXTURE );

  shdrpgm.Use();
  //shdrpgm.SetUniform("shadowRender", m_graphic_manager->shadowRender);
  //Push Light Var
  {
  // Possible optimisation by removing aabb checks
    if ( HaveComponent<AABB>( entity ) )
    {
      {
        if ( renderSettings.onLight )
        {
          Vec3f campos = camera->CameraEye();
          Gfx::LightRenderer::SetViewPos( campos, shdrpgm );
          Gfx::LightRenderer::SetLightOn( renderSettings.onLight, shdrpgm );
          Gfx::LightRenderer::SetBlinnPhong( true, shdrpgm );
          Gfx::LightRenderer::SetShinniess( material.shinness, shdrpgm );
          DirLights dirLights;
          ForEach( [&]( Position &pos_dl, LightProperty &lightProperty, Direction &dir )
          {
            Graphics::DirLight dirLight;
            dirLight.ambient = lightProperty.ambient;
            dirLight.diffuse = lightProperty.diffuse;
            dirLight.specular = lightProperty.specular;
            dirLight.direction = dir.m_dir;
            dirLight.position = pos_dl.m_position;
            dirLights.push_back( std::move( dirLight ) );
            return false;
          } );

          Gfx::LightRenderer::SetDirLights( dirLights, shdrpgm );

          // Possible optimisation through caching
          PointLights pointLights;
          ForEach( [&]( Position &pos_pl, LightProperty &lightProperty, QuadraticFunction &qFunc )
          {
            auto &checkAabb = GetComponent<AABB>( entity );
            if ( Math::Helper::MathFunction::SphereAABB( checkAabb.AABB_mMin,
                 checkAabb.AABB_mMax, pos_pl.m_position, lightProperty.radius ) )
            {
              PointLight pointLight;
              pointLight.position = pos_pl.m_position;
              pointLight.ambient = lightProperty.ambient;
              pointLight.diffuse = lightProperty.diffuse;
              pointLight.specular = lightProperty.specular;
              pointLight.constant = qFunc.constant;
              pointLight.linear = qFunc.linear;
              pointLight.quadratic = qFunc.quadratic;
              pointLights.push_back( std::move( pointLight ) );
            }

            return false;
          } );

          Gfx::LightRenderer::SetPointLights( pointLights, shdrpgm );
        }
      }
    }
  }

  shdrpgm.SetUniform( "shadowRender", m_graphic_manager->shadowRender );
  auto &global_ambient = GetOrCreateSingletonComponent<GlobalAmbient>().ambient;
  if ( m_graphic_manager->shadowRender && lightData.isValid && !( renderSettings.transparent < 1.f ) )
  {
    int b = 0;
    for ( auto &x : lightData.lightEntitiesList )
    {
      //b++;
      //if (b > 1)
      //{
      //	glEnable(GL_BLEND);
      //	glBlendFunc(GL_ONE, GL_ONE);
      //	glDepthFunc(GL_EQUAL);
      //}

      shdrpgm.SetUniform( "shadowRender", lightData.isValid );
      auto light_data_iter = lightData.lightData_map.find( x );
      auto &lightPos = light_data_iter->second.first;
      auto &lightSpaceMatrix = light_data_iter->second.second;
      auto &shadowMapID = lightData.shadowMapID_map.find( x )->second;
      shdrpgm.SetUniform( "lightPos", lightPos );
      shdrpgm.SetUniform( "lightSpaceMatrix", lightSpaceMatrix );
      shdrpgm.SetUniform( "shadowMap", 4 );
      glActiveTexture( GL_TEXTURE4 );
      glBindTexture( GL_TEXTURE_2D, shadowMapID );
      //shdrpgm.SetUniform("parallaxMappingOn", material.parallaxMappingOn);

      //shdrpgm.SetUniform("gamma", m_graphic_manager->gamma);
      ////shdrpgm.SetUniform("viewPos", camera->CameraEye());
      //Math::Helper::MathFunction::clamp(colour.colour, 0.f, 1.f);
      //shdrpgm.SetUniform("uColor", colour.colour);
      //shdrpgm.SetUniform("ambientColor", global_ambient);
      //Gfx::Renderer::BindMaterialTexture(material.diffuse, material.specular, material.shinness, shdrpgm);
      //Gfx::Renderer::BindTransparencyClamp(renderSettings.transparent, shdrpgm);
      //Gfx::Renderer::SetTransform(
      //	transform.LocalToWorld,
      //	camera->ViewMat_3D(),
      //	camera->ProjMat_3D(),
      //	shdrpgm);
      //mesh.model->Draw();
    }

    //if (b)
    //{
    //	glDisable(GL_BLEND);
    //	glDepthFunc(GL_LEQUAL);
    //}
  }
  //else
  //{
  //	shdrpgm.SetUniform("shadowRender", m_graphic_manager->shadowRender);
  //	shdrpgm.SetUniform("gamma", m_graphic_manager->gamma);
  //	//shdrpgm.SetUniform("viewPos", camera->CameraEye());
  //	Math::Helper::MathFunction::clamp(colour.colour, 0.f, 1.f);
  //	shdrpgm.SetUniform("uColor", colour.colour);
  //	shdrpgm.SetUniform("ambientColor", global_ambient);
  //	Gfx::Renderer::BindMaterialTexture(material.diffuse, material.specular, material.shinness, shdrpgm);
  //
  //	auto& m_resource_manager = Service::Locator::Get<RM>();

  //	//fix scene file error, going to remove after etc...
  //	//if (!material.normal)
  //	//	material.normal = m_resource_manager.Get<Graphics::Texture>("Assets/Textures/default.dds");
  //	//if (!material.bump)
  //	//	material.bump = m_resource_manager.Get<Graphics::Texture>("Assets/Textures/default.dds");

  //	shdrpgm.SetUniform("parallaxMappingOn", material.parallaxMappingOn);
  //	if (material.parallaxMappingOn)
  //	{
  //		shdrpgm.SetUniform("material.normalTex", 2);
  //		shdrpgm.SetUniform("material.height_scale", material.height_scale);
  //		Gfx::Renderer::OpenGLBindTexture(GL_TEXTURE2, material.normal->pTex);
  //		Gfx::Renderer::BindTexture(material.normal->pTex);
  //		shdrpgm.SetUniform("material.bumpTex", 3);
  //		Gfx::Renderer::OpenGLBindTexture(GL_TEXTURE3, material.bump->pTex);
  //		Gfx::Renderer::BindTexture(material.bump->pTex);
  //	}
  //	Gfx::Renderer::BindTransparencyClamp(renderSettings.transparent, shdrpgm);
  //	Gfx::Renderer::SetTransform(
  //		transform.LocalToWorld,
  //		camera->ViewMat_3D(),
  //		camera->ProjMat_3D(),
  //		shdrpgm);
  //	mesh.model->Draw();
  //}

  shdrpgm.SetUniform( "gamma", m_graphic_manager->gamma );
  //shdrpgm.SetUniform("viewPos", camera->CameraEye());
  Math::Helper::MathFunction::clamp( colour.colour, 0.f, 1.f );
  shdrpgm.SetUniform( "uColor", colour.colour );
  shdrpgm.SetUniform( "ambientColor", global_ambient );
  Gfx::Renderer::BindMaterialTexture( material.diffuse, material.specular, material.shinness, shdrpgm );
  shdrpgm.SetUniform( "parallaxMappingOn", material.parallaxMappingOn );
  if ( material.parallaxMappingOn )
  {
    shdrpgm.SetUniform( "material.normalTex", 2 );
    shdrpgm.SetUniform( "material.height_scale", material.height_scale );
    Gfx::Renderer::OpenGLBindTexture( GL_TEXTURE2, material.normal->pTex );
    Gfx::Renderer::BindTexture( material.normal->pTex );
    shdrpgm.SetUniform( "material.bumpTex", 3 );
    Gfx::Renderer::OpenGLBindTexture( GL_TEXTURE3, material.bump->pTex );
    Gfx::Renderer::BindTexture( material.bump->pTex );
  }
  Gfx::Renderer::BindTransparencyClamp( renderSettings.transparent, shdrpgm );
  Gfx::Renderer::SetTransform(
    transform.LocalToWorld,
    camera->ViewMat_3D(),
    camera->ProjMat_3D(),
    shdrpgm );
  mesh.model->Draw();
  shdrpgm.UnUse();
  Gfx::Renderer::UnBindTexture();
}
}