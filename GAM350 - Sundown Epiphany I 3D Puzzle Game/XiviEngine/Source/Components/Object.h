/*****************************************************************
*\file         Object.h
*\author(s)    Goh Wei Zhe

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct Object
{
  Vec3f m_force { 0.0f, 0.0f, 0.0f };
  float m_mass = 1.0f;

  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Object"
  };

  void AddForceX( const float x ) noexcept
  {
    setX( m_force, getX( m_force ) + x );
  }

  void AddForceY( const float y ) noexcept
  {
    setY( m_force, getY( m_force ) + y );
  }

  void AddForceZ( const float z ) noexcept
  {
    setZ( m_force, getZ( m_force ) + z );
  }

  void AddForce( const float x, const float y, const float z ) noexcept
  {
    m_force += Vec3f { x, y, z };
  }

  void AddForce( const Vec3f &force ) noexcept
  {
    m_force += force;
  }
};
}

namespace Xivi::Components::Object::Reflect
{
RTTR_REGISTRATION
{
  // Object
  rttr::registration::class_<Xivi::Object>( "Object" )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::Object>.m_id ) )
  .property( "Force", &Xivi::Object::m_force )( rttr::policy::prop::as_reference_wrapper )
  .property( "Mass", &Xivi::Object::m_mass )( rttr::policy::prop::as_reference_wrapper );
}
}