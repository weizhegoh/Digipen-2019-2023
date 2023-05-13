/*****************************************************************
*\file         UIDragSystem.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "GUISystem.h"

namespace Xivi::GUI
{
class Manager;
struct UIDragSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = Xivi::ECS::System::Type::ChildUpdate<GUISystem, GUISystem::ChildEvent>
  {
    .m_id = 503,
    .m_name = "UI Drag System"
  };

  Ptr<GUI::Manager> m_canvas_stack;
  ECS::Query::Instance m_interactable_drag;
  float m_on_top;

  void OnCreate() noexcept;

  void OnPreUpdate() noexcept;

  void OnUpdate() noexcept;
};
}