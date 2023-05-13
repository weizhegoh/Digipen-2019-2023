/*****************************************************************
*\file         PrefabCleanupSystem.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct PrefabCleanupSystem : ECS::System::Instance
{
  static constexpr auto info = ECS::System::Type::NotifyDestroy
  {
    .m_id = 103,
    .m_name = "Entity Cleanup System"
  };

  using UnlinkPrefab = ECS::Event::Instance<Prefab &>;
  using UnlinkPrefabInstance = ECS::Event::Instance<Entity &, ReferencedPrefab &>;
  using Events = tuplet::tuple<UnlinkPrefab, UnlinkPrefabInstance>;

  void operator()( Entity &entity, Prefab *prefab, ReferencedPrefab *ref_prefab );
};
}