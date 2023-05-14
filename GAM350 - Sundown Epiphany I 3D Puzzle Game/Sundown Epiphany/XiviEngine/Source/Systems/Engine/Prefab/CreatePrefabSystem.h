/*****************************************************************
*\file         CreatePrefabInstance.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct CreatePrefabSystem : ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::GlobalEvent<CreatePrefab>
  {
    .m_name = "Create Prefab System"
  };

  ECS::Signature m_initial;
  ECS::Signature m_update;
  std::function<void( Entity & )> m_recurse_create;

  void OnCreate() noexcept;

  void OnEvent( Entity &entity, const std::string &filename );
};
}