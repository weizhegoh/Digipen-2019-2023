/*****************************************************************
*\file         UIDragSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "UIDragSystem.h"
#include "Components/GUIComponents.h"
#include "Xivi/GUI/GUIManager.h"

namespace Xivi::GUI
{
void UIDragSystem::OnCreate() noexcept
{
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();
  m_interactable_drag.AllOf<AABB, Parent, UIDrag, UIInteractableTag>();
}

void UIDragSystem::OnPreUpdate() noexcept
{
  if ( m_paused )
    return;

  m_on_top = std::numeric_limits<float>().max();

  ForEach( m_interactable_drag, [&]( Position &pos, AABB &aabb, Transform &transform ) noexcept
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

void UIDragSystem::OnUpdate() noexcept
{
  if ( m_paused )
    return;

  Entity ui_on_top;
  auto mouse_pos = Service::Locator::Get<Input::Manager>().Mouse()->Position();
  ForEach( m_interactable_drag, [&]( Entity &entity, Position &pos, AABB &aabb, UIDrag &drag ) noexcept
  {
    GetSystem<GUISystem>().IsItemHold( aabb, [&]
    {
      if ( pos.m_position.getX() < m_on_top )
        ui_on_top = entity;
    } );
  } );

  if ( ui_on_top.Valid() )
  {
    auto &drag = m_world.GetComponent<UIDrag>( ui_on_top );
    drag.m_cur_pt = Math::CMath::clamp( mouse_pos.Vector(), drag.m_min_pt, drag.m_max_pt );
  }
}
}