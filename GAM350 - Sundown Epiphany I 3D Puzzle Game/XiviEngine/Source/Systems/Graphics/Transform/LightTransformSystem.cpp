/*****************************************************************
*\file         LightTransformSystem.cpp
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "LightTransformSystem.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
namespace Xivi::Graphics
{
void LightTransformSystem::OnCreate() noexcept
{
  m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
  m_framebuffer = &Service::Locator::Get<Graphics::M_FrameBuffer>();
}
void LightTransformSystem::OnUpdate() noexcept
{
  auto &m_debugDrawer = m_graphic_manager->GetDebugDraw();
  bool debugRender = ( m_graphic_manager->debugRender );
  ForEach( [&]( Position &pos, LightProperty &lightProperty, Direction &dir )
  {
    dir.m_dir = ( dir.endPoint - pos.m_position ).normalize();
    if ( debugRender )
      m_debugDrawer.DrawRay(
      Graphics::Shapes::Ray( pos.m_position, dir.m_dir ), 10000.f ).Color( Math::Vector3( 1.f, 0.f, 0.f ) );
  } );

  ForEach( [&]( Position &pos, LightProperty &lightProperty, QuadraticFunction &qFunc )
  {
    if ( debugRender )
      m_debugDrawer.DrawSphere(
      Graphics::Shapes::Sphere( pos.m_position, lightProperty.radius ) ).Color( Math::Vector3( 1.f, 0.f, 0.f ) );
  } );
}
}