/*****************************************************************
*\file         CreatePrefabInstanceSystem.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct CreatePrefabInstanceSystem : ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::GlobalEvent<CreatePrefabInstanceInternal>
  {
    .m_name = "Create Prefab Instance System"
  };

  ECS::Signature m_initial;
  ECS::Signature m_update;
  std::function<void( Entity & )> m_recurse_create;

  void OnCreate() noexcept;

  void OnEvent( Entity &entity ) noexcept;
};
}