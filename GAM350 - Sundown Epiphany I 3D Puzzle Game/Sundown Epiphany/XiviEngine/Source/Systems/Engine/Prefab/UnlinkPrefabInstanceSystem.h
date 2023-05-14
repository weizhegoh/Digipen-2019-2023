/*****************************************************************
*\file         UnlinkPrefabInstanceSystem.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct UnlinkPrefabInstanceSystem : ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::SystemEvent<PrefabCleanupSystem, PrefabCleanupSystem::UnlinkPrefabInstance>
  {
    .m_name = "Unlink Prefab Instance System"
  };

  void OnEvent( Entity &entity, ReferencedPrefab &ref_prefab );
};
}