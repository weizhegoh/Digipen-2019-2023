/*****************************************************************
*\file         DebugDrawRendererSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "DebugDrawRendererSystem.h"
#include <Resource/Manager.h>
#include "Xivi/Graphics/Manager/GraphicManagers.h"
namespace Xivi::Graphics
{
void DebugDrawRendererSystem::OnCreate() noexcept
{
  m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
}

void  DebugDrawRendererSystem::OnPreUpdate() noexcept
{
}
void  DebugDrawRendererSystem::OnUpdate() noexcept
{
  if ( !m_graphic_manager->debugRender )
    return;
  auto &frameBuffer = m_graphic_manager->frameBuffer;
  auto &camera = m_graphic_manager->camera;
  auto &debugDrawer = m_graphic_manager->GetDebugDraw();
  auto &shdrpgm1 = m_graphic_manager->GetShaders( ShaderOption::LINE );
  shdrpgm1.Use();

  auto &shapesList = debugDrawer.ShapeList();
  for ( size_t i = 0; i < shapesList.size(); ++i )
  {
    DebugShape &shape = shapesList[i];
    if ( ( shape.mMask & debugDrawer.isActive() ) == 0 )
      continue;
    if ( shape.mOnTop )
      glDisable( GL_DEPTH_TEST );

    for ( size_t j = 0; j < shape.mSegments.size(); ++j )
    {
      Shapes::LineSegment &segment = shape.mSegments[j];
      Math::Helper::MathFunction::clamp( shape.mColor, 0.f, 1.f );
      shape.mTransform = Math::Mat4x4().identity();
      Gfx::Renderer::SetTransformLine(
        camera->ViewMat_3D(),
        camera->ProjMat_3D(),
        shdrpgm1 );
      Graphics::Gfx::Renderer::DrawLine( segment.mStart, segment.mEnd, shape.mColor, debugDrawer.VAO, debugDrawer.VBO );
    }

    if ( shape.mOnTop )
      glEnable( GL_DEPTH_TEST );
  }
  shdrpgm1.UnUse();
  auto &timer = Service::Locator::Get<TimerEngine>();
  debugDrawer.Update( timer.GetDeltaTime(), camera->CameraEye(), camera->CameraDirection() );
}

void  DebugDrawRendererSystem::OnPostUpdate() noexcept
{}
}