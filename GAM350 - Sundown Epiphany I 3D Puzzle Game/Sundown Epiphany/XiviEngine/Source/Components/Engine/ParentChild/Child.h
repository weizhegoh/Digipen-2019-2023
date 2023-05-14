/*****************************************************************
*\file         Child.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct Child
{
  static constexpr auto info = ECS::Component::Type::Data { .m_name = "Child" };
  std::vector<Entity> m_entities;
};
}

namespace Xivi::Components::Child::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::Child>( "Child" )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::Child>.m_id ) )
  .property( "Child Entities", &Xivi::Child::m_entities );
}
}