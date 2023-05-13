/*****************************************************************
*\file         WireframeRendererSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "WireframeRendererSystem.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
namespace Xivi::Graphics
{
void WireFrameRendererSystem::OnCreate() noexcept
{
  m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
}

void WireFrameRendererSystem::OnPreUpdate() noexcept
{
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}
void WireFrameRendererSystem::OnUpdate() noexcept
{
  auto &frameBuffer = m_graphic_manager->frameBuffer;
  auto &camera = m_graphic_manager->camera;
  //Animation WireFrame
  {
    auto &shdrpgm = m_graphic_manager->GetShaders( ShaderOption::AnimationWireFrame );
    shdrpgm.Use();
    ForEach( [&](
      Transform &transform,
      Colour &colour,
      AnimationMeshComponent &mesh,
      AnimatorComponent &animator )
    {
      Math::Helper::MathFunction::clamp( colour.colour, 0.f, 1.f );
      shdrpgm.SetUniform( "uColor", colour.colour );
      Gfx::Renderer::BindTransparency( 1.0f, shdrpgm );
      Gfx::Renderer::SetTransform( animator.animator.GetFinalBoneMatrices(), shdrpgm );
      Gfx::Renderer::SetTransform(
        transform.LocalToWorld,
        camera->ViewMat_3D(),
        camera->ProjMat_3D(),
        shdrpgm );
      mesh.model->model.Draw();
      return false;
    } );
    shdrpgm.UnUse();
  }
  //StaticMesh WireFrame
  {
    auto &shdrpgm1 = m_graphic_manager->GetShaders( ShaderOption::NORMAL );
    shdrpgm1.Use();
    ForEach( [&](
      Transform &transform,
      StaticMeshComponent &mesh,
      Colour &colour
    )
    {
      Math::Helper::MathFunction::clamp( colour.colour, 0.f, 1.f );
      shdrpgm1.SetUniform( "uColor", colour.colour );
      Gfx::Renderer::BindTransparency( 1.0f, shdrpgm1 );
      Gfx::Renderer::SetTransform(
        transform.LocalToWorld,
        camera->ViewMat_3D(),
        camera->ProjMat_3D(),
        shdrpgm1 );
      mesh.model->Draw();
      return false;
    } );
    shdrpgm1.UnUse();
  }
}

void WireFrameRendererSystem::OnPostUpdate() noexcept
{
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}
}