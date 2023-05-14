/*****************************************************************
*\file         ParticleRendererSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "ParticleRendererSystem.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Components/ParticleComponents.h"
namespace Xivi::Graphics
{
void ParticleRendererSystem::OnCreate() noexcept
{
  m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
  m_framebuffer = &Service::Locator::Get<Graphics::M_FrameBuffer>();
}

void ParticleRendererSystem::operator()( ParticleEmitter &emitter, ParticleSettings &settings ) noexcept
{
  if ( !emitter.isActive )
    return;
  auto &camera = m_graphic_manager->camera;
  auto &shdrpgm = m_graphic_manager->GetShaders( ShaderOption::TEXTURE );
  shdrpgm.Use();
  auto &model = emitter.model;
  auto &diffuse = emitter.diffuse;
  auto &specular = emitter.specular;
  auto &shinness = emitter.shinness;
  for ( size_t i = 0; i < settings.count; ++i )
  {
    auto &particle = emitter.particles[i];
    if ( !particle.isActive )
      continue;
    auto &renderSettings = particle.renderSettings;
    if ( !renderSettings.onRender )
      continue;

    auto &pos = particle.position;
    auto &colour = particle.colour;
    auto &transform = particle.transform;
    auto &unitScale = emitter.unitScales;

    if ( renderSettings.onLight )
    {
      Vec3f campos = camera->CameraEye();
      Gfx::LightRenderer::SetViewPos( campos, shdrpgm );
      Gfx::LightRenderer::SetLightOn( renderSettings.onLight, shdrpgm );
      Gfx::LightRenderer::SetBlinnPhong( true, shdrpgm );
      Gfx::LightRenderer::SetShinniess( shinness, shdrpgm );
      DirLights dirLights;
      ForEach( [&]( LightProperty &lightProperty, Direction &dir )
      {
        Graphics::DirLight dirLight;
        dirLight.ambient = lightProperty.ambient;
        dirLight.diffuse = lightProperty.diffuse;
        dirLight.specular = lightProperty.specular;
        dirLight.direction = dir.m_dir;
        dirLights.push_back( std::move( dirLight ) );
        return false;
      } );

      //Gfx::LightRenderer::SetDirLights(dirLights, shdrpgm);

      PointLights pointLights;
      ForEach( [&]( Position &pos_pl, LightProperty &lightProperty, QuadraticFunction &qFunc )
      {
        Math::Vector3 scaleTemp = transform.scale;

        if ( Math::Helper::MathFunction::SphereSphere( pos.m_position, scaleTemp.getX(),
             pos_pl.m_position, lightProperty.radius ) )
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

        return false;
      } );
      Gfx::LightRenderer::SetPointLights( pointLights, shdrpgm );
    }
    shdrpgm.SetUniform( "shadowRender", false );
    Math::Helper::MathFunction::clamp( colour.colour, 0.f, 1.f );
    shdrpgm.SetUniform( "uColor", colour.colour );
    Gfx::Renderer::BindMaterialTexture( diffuse, specular, shinness, shdrpgm );
    Gfx::Renderer::BindTransparencyClamp( renderSettings.transparent, shdrpgm );
    Gfx::Renderer::SetTransform(
      transform.LocalToWorld,
      camera->ViewMat_3D(),
      camera->ProjMat_3D(),
      shdrpgm );
    model->Draw();
    Gfx::Renderer::UnBindTexture();
  }
  shdrpgm.UnUse();
}
}