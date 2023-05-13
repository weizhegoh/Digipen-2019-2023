/*****************************************************************
*\file         PrefabTag.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct PrefabTag
{
  static constexpr auto info = ECS::Component::Type::Exclusive { .m_name = "Prefab Tag" };
};
}

namespace Xivi::Components::PrefabTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::PrefabTag>( "Prefab Tag" )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::PrefabTag>.m_id ) );
}
}