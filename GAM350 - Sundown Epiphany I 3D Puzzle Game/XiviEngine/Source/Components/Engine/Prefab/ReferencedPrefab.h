/*****************************************************************
*\file         ReferencedPrefab.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct ReferencedPrefab
{
  static constexpr auto info = ECS::Component::Type::Data { .m_name = "Referenced Prefab" };
  Entity m_entity;
  std::vector<ECS::Component::ID> m_overrides;
};
}

namespace Xivi::Components::ReferencedPrefab::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::ReferencedPrefab>( "Referenced Prefab" )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::ReferencedPrefab>.m_id ) )
  .property( "Prefab Entity", &Xivi::ReferencedPrefab::m_entity )
  .property( "Prefab Overrides", &Xivi::ReferencedPrefab::m_overrides );
}
}