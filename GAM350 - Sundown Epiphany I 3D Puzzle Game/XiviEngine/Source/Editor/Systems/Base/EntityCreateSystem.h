/*****************************************************************
*\file         EntityCreateSystem.h
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
struct EntityCreateSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = Xivi::ECS::System::Type::NotifyCreate
  {
    .m_id = 13,
    .m_name = "Entity Create System"
  };

  //std::vector<tuplet::tuple<Entity, bool, bool>> m_to_add; // entity, is_prefab, is_ui
  ECS::Signature m_prefab_sig;
  ECS::Signature m_ui_sig;
  ECS::Signature m_skybox_sig;
  bool m_need_refresh_panel { false };
  vector<Entity> m_to_select_entity;

  void OnCreate() noexcept;

  void OnFrameStart() noexcept;

  void OnNotify( Entity &entity ) noexcept;
};
}