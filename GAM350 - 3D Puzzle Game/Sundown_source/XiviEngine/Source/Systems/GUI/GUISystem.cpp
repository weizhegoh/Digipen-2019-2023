/*****************************************************************
*\file         GUISystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "GUISystem.h"
#include "Components/GUIComponents.h"

namespace Xivi::GUI
{
void GUISystem::OnCreate() noexcept
{
  m_canvas_query.AllOf<CanvasTag>();
  m_framebuffer = &Service::Locator::Get<Graphics::M_FrameBuffer>();
  m_inputmanager = &Service::Locator::Get<Input::Manager>();
  m_gui_function.AnyOf<UIHover, UITrigger>();
  m_canvas_tag.AllOf<CanvasTag>();
}

void GUISystem::OnPreUpdate() noexcept
{
  ForEach( m_canvas_tag, [&]( Position &pos, AABB &aabb, Transform &transform ) noexcept
  {
    if ( aabb.type == CollisionType::AABB || aabb.type == CollisionType::SPHERE )
    {
      aabb.AABB_mMin = Vec3f { pos.m_position.getX() - transform.scale.getX() * 0.5f, pos.m_position.getY() - transform.scale.getY() * 0.5f, pos.m_position.getZ() - transform.scale.getZ() * 0.5f };
      aabb.AABB_mMax = Vec3f { pos.m_position.getX() + transform.scale.getX() * 0.5f, pos.m_position.getY() + transform.scale.getY() * 0.5f, pos.m_position.getZ() + transform.scale.getZ() * 0.5f };
    }

    if ( aabb.type == CollisionType::SPHERE )
    {
      aabb.Sphere_mCenter = pos.m_position;
      aabb.Sphere_mRadius = ( aabb.AABB_mMax - aabb.AABB_mMin ).length() / 3.f;
    }
  } );
}

void GUISystem::OnUpdate() noexcept
{
  //SendEvent<GameGUIEvent>(this);
  SendEvent<ChildEvent>( this );
}
}