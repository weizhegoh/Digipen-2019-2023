/*****************************************************************
*\file         Prefab.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct Prefab
{
  static constexpr auto info = ECS::Component::Type::Data { .m_name = "Prefab" };
  std::vector<Entity> referenced_entities;
};
}

namespace Xivi::Components::Prefab::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::Prefab>( "Prefab" )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::Prefab>.m_id ) )
  .property( "Referenced Entities", &Xivi::Prefab::referenced_entities );
}
}