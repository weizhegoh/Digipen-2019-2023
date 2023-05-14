/*****************************************************************
*\file         AnimationRendererSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "AnimationRendererSystem.h"
#include <Resource/Manager.h>
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
namespace Xivi::Graphics
{
void AnimationRendererSystem::OnCreate() noexcept
{
  m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
  m_shader_manager = &m_graphic_manager->GetShaders( ShaderOption::ANIMATION );
}

void AnimationRendererSystem::operator()(
  Entity &entity,
  Position &pos,
  Transform &transform,
  AnimationMeshComponent &mesh,
  AnimatorComponent &animator,
  Material &material,
  Colour &colour,
  RenderSettings &renderSettings,
  LightData &lightData ) noexcept
{
  if ( !renderSettings.onRender )
    return;

  auto &frameBuffer = m_graphic_manager->frameBuffer;
  auto &camera = m_graphic_manager->camera;
  m_shader_manager->Use();

  //Push Light Var
  Gfx::LightRenderer::SetViewPos( camera->CameraEye(), *m_shader_manager );
  {
    if ( renderSettings.onLight )
    {
      Gfx::LightRenderer::SetLightOn( renderSettings.onLight, *m_shader_manager );
      Gfx::LightRenderer::SetBlinnPhong( true, *m_shader_manager );
      Gfx::LightRenderer::SetShinniess( material.shinness, *m_shader_manager );
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

      Gfx::LightRenderer::SetDirLights( dirLights, *m_shader_manager );

      PointLights pointLights;
      ForEach( [&]( Position &pos_pl, LightProperty &lightProperty, QuadraticFunction &qFunc )
      {
        if ( HaveComponent<AABB>( entity ) )
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
            pointLights.push_back( pointLight );
          }
        }

        return false;
      } );
      Gfx::LightRenderer::SetPointLights( pointLights, *m_shader_manager );
    }
  }

  m_shader_manager->SetUniform( "shadowRender", m_graphic_manager->shadowRender );
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

      auto &lightPos = lightData.lightData_map.find( x )->second.first;
      auto &lightSpaceMatrix = lightData.lightData_map.find( x )->second.second;
      auto &shadowMapID = lightData.shadowMapID_map.find( x )->second;
      m_shader_manager->SetUniform( "shadowRender", lightData.isValid );
      m_shader_manager->SetUniform( "lightPos", lightPos );
      m_shader_manager->SetUniform( "lightSpaceMatrix", lightSpaceMatrix );

      //Math::Helper::MathFunction::clamp(colour.colour, 0.f, 1.f);
      //m_shader_manager->SetUniform("uColor", colour.colour);
      //m_shader_manager->SetUniform("gamma", m_graphic_manager->gamma);
      //m_shader_manager->SetUniform("ambientColor", GetOrCreateSingletonComponent<GlobalAmbient>().ambient);
      //Gfx::Renderer::BindTransparencyClamp(renderSettings.transparent, *m_shader_manager);
      //Gfx::Renderer::SetTransform(animator.animator.GetFinalBoneMatrices(), *m_shader_manager);
      //Gfx::Renderer::SetTransform(transform.LocalToWorld, camera->ViewMat_3D(), camera->ProjMat_3D(), *m_shader_manager);
      //Gfx::Renderer::BindMaterialTexture(material.diffuse, material.specular, material.shinness, *m_shader_manager);
      m_shader_manager->SetUniform( "shadowMap", 2 );
      glActiveTexture( GL_TEXTURE2 );
      glBindTexture( GL_TEXTURE_2D, shadowMapID );
      //mesh.model->model.Draw();
    }

    //if (b)
    //{
    //	glDisable(GL_BLEND);
    //	glDepthFunc(GL_LEQUAL);
    //}
  }
  //m_shader_manager->SetUniform("shadowRender", m_graphic_manager->shadowRender);
  Math::Helper::MathFunction::clamp( colour.colour, 0.f, 1.f );
  m_shader_manager->SetUniform( "uColor", colour.colour );
  m_shader_manager->SetUniform( "gamma", m_graphic_manager->gamma );
  //m_shader_manager->SetUniform("ambientColor", GetOrCreateSingletonComponent<GlobalAmbient>().ambient);
  Gfx::Renderer::BindTransparencyClamp( renderSettings.transparent, *m_shader_manager );
  Gfx::Renderer::SetTransform( animator.animator.GetFinalBoneMatrices(), *m_shader_manager );
  Gfx::Renderer::SetTransform( transform.LocalToWorld, camera->ViewMat_3D(), camera->ProjMat_3D(), *m_shader_manager );
  Gfx::Renderer::BindMaterialTexture( material.diffuse, material.specular, material.shinness, *m_shader_manager );
  mesh.model->model.Draw();

  m_shader_manager->UnUse();
  Gfx::Renderer::UnBindTexture();
}
}