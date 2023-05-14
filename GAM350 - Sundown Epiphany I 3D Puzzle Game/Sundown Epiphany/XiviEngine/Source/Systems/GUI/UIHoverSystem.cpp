/*****************************************************************
*\file         UIHoverSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "UIHoverSystem.h"
#include "Components/GUIComponents.h"
#include "Xivi/GUI/GUIManager.h"

namespace Xivi::GUI
{
void UIHoverSystem::OnCreate() noexcept
{
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();
  m_interactable_trigger.AllOf<AABB, Parent, UIHover, UIInteractableTag>();
}

void UIHoverSystem::OnPreUpdate() noexcept
{
  if ( m_paused )
    return;

  m_on_top = std::numeric_limits<float>().max();

  ForEach( m_interactable_trigger, [&]( Position &pos, AABB &aabb, Transform &transform ) noexcept
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

void UIHoverSystem::OnUpdate() noexcept
{
  if ( m_paused )
    return;

  Entity ui_on_top;
  static Entity prev_hovered;
  ForEach( m_interactable_trigger, [&]( Entity &entity, Position &pos, AABB &aabb, UIHover &trigger ) noexcept
  {
    GetSystem<GUISystem>().IsItemHover( aabb, [&]
    {
      if ( pos.m_position.getX() < m_on_top )
        prev_hovered = ui_on_top = entity;
    } );
  } );

  if ( prev_hovered.Valid() && prev_hovered != ui_on_top )
  {
    auto &trigger = m_world.GetComponent<UIHover>( prev_hovered );
    trigger.m_callback_unhover( prev_hovered );
    prev_hovered = Entity {};
  }

  if ( ui_on_top.Valid() )
  {
    auto &trigger = m_world.GetComponent<UIHover>( ui_on_top );
    trigger.m_callback( ui_on_top );
  }
}
}