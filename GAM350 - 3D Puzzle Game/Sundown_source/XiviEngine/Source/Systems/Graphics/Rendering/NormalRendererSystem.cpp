/*****************************************************************
*\file         NormalRendererSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "NormalRendererSystem.h"
namespace Xivi::Graphics
{
void NormalRendererSystem::OnCreate() noexcept
{
  m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
  m_shader = &m_graphic_manager->GetShaders( ShaderOption::NORMAL );
}
void NormalRendererSystem::operator()(
  Position &pos,
  Transform &transform,
  StaticMeshComponent &mesh,
  Colour &colour,
  DebugDrawComponent &debugDraw,
  RenderSettings &renderSettings ) noexcept
{
  if ( !m_graphic_manager->debugRender || !renderSettings.onRender )
    return;

  auto &frameBuffer = m_graphic_manager->frameBuffer;
  auto &camera = m_graphic_manager->camera;
  m_shader->Use();

  Math::Helper::MathFunction::clamp( colour.colour, 0.f, 1.f );
  m_shader->SetUniform( "uColor", colour.colour );
  Gfx::Renderer::BindTransparencyClamp( renderSettings.transparent, *m_shader );
  Gfx::Renderer::SetTransform( transform.LocalToWorld, camera->ViewMat_3D(), camera->ProjMat_3D(), *m_shader );
  mesh.model->Draw();
  m_shader->UnUse();
}
}