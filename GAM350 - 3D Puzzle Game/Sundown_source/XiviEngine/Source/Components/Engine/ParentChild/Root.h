/*****************************************************************
*\file         Root.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct Root
{
  static constexpr auto info = ECS::Component::Type::Tag { .m_name = "Root" };
};
}

namespace Xivi::Components::Root::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::Root>( "Root" )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::Root>.m_id ) );
}
}