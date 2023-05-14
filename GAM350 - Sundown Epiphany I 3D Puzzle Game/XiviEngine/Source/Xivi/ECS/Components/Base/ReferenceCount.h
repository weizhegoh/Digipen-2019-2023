/*****************************************************************
*\file         ReferenceCount.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi::ECS
{
struct ReferenceCount
{
  static constexpr auto info = Component::Type::Data { .m_name = "Reference Count" };
  u32 m_value;
};
}

namespace Xivi::ECS::Components::ReferenceCount::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<ECS::ReferenceCount>( "Reference Count" )
  ( rttr::metadata( "GUID", Component::info<ECS::ReferenceCount>.m_id ) )
  .property( "Count", &ECS::ReferenceCount::m_value );
}
}