/*****************************************************************
*\file         EventRefreshPanel.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "Xivi/ECS/XVECS.h"
#include "Components/EngineComponents.h"

namespace Xivi::Editor
{
struct EventRefreshPanelSystem
{
  inline static std::vector<tuplet::tuple<Entity, bool, bool>> m_to_add; // entity, is_prefab, is_ui
  inline static ECS::Signature m_skybox_sig;
  inline static ECS::Signature m_prefab_sig;
  inline static ECS::Signature m_ui_sig;
  inline static std::function<void( ECS::World::Instance &, const Entity, const bool, const bool )> m_recursive_add;

  static void Init() noexcept;

  static void Update() noexcept;

  static void OnCall( ECS::World::Instance &world ) noexcept;
};
}
#endif