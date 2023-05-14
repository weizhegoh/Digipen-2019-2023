/*****************************************************************
*\file         Parent.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct Parent
{
  static constexpr auto info = ECS::Component::Type::Data { .m_name = "Parent" };
  Entity m_entity;
};
}

namespace Xivi::Components::Parent::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::Parent>( "Parent" )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::Parent>.m_id ) )
  .property( "Parent Entity", &Xivi::Parent::m_entity );
}
}