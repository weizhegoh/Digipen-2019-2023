/*****************************************************************
*\file         DeserialisePrefabSystem.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct DeserialisePrefabSystem : ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::GlobalEvent<DeserialisePrefab>
  {
    .m_name = "Serialise Prefab System"
  };

  void OnEvent( Entity &entity, const std::string &filename );
};
}