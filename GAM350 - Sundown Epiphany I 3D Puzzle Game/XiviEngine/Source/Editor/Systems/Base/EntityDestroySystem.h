/*****************************************************************
*\file         EntityDestroySystem.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "Xivi/ECS/XVECS.h"
#include "Components/EngineComponents.h"

namespace Xivi::Editor
{
struct EntityDestroySystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = Xivi::ECS::System::Type::NotifyDestroy
  {
      .m_id = 14,
      .m_name = "Entity Destroy System"
  };

  std::vector<tuplet::tuple<Entity, bool, bool>> m_to_remove; // entity, is_prefab, is_ui
  ECS::Signature m_prefab_sig;
  ECS::Signature m_ui_sig;

  void OnCreate() noexcept;

  void OnFrameStart() noexcept;

  void OnNotify( Entity &entity ) noexcept;
};
}