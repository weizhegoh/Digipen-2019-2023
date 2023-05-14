/*****************************************************************
*\file         Velocity.h
*\author(s)    Goh Wei Zhe

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
namespace Xivi
{
struct Velocity
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Velocity"
  };
  Vec3f m_velocity { 0.0f, 0.0f, 0.0f };
};
}

namespace Xivi::Components::Velocity::Reflect
{
RTTR_REGISTRATION
{
  // Velocity
  rttr::registration::class_<Xivi::Velocity>( "Velocity" )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::Velocity>.m_id ) )
  .property( "Velocity", &Xivi::Velocity::m_velocity )( rttr::policy::prop::as_reference_wrapper );
}
}