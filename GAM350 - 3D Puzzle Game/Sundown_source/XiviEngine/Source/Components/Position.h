/*****************************************************************
*\file         Position.h
*\author(s)    Goh Wei Zhe

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi
{
struct Position
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Position"
  };
  Vec3f m_position { 0.0f, 0.0f, 0.0f };
  Vec3f m_localPosition { 0.0f, 0.0f, 0.0f };
  Vec3f renderPosition { 0.0f, 0.0f, 0.0f };
  float speed = 0.5f;
  bool shaky = false;

  void GetLocalToWorld()
  {}

  glm::vec3 GetGlm()
  {
    return glm::vec3( getX( m_position ), getY( m_position ), getZ( m_position ) );
  }
  void SetGlm( const glm::vec3 position )
  {
    m_position.set( position.x, position.y, position.z );
  }
};
}

namespace Xivi::Components::Position::Reflect
{
RTTR_REGISTRATION
{
// Position
rttr::registration::class_<Xivi::Position>( "Position" )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::Position>.m_id ) )
.property( "Position", &Xivi::Position::m_position )( rttr::policy::prop::as_reference_wrapper )
.property( "Speed", &Xivi::Position::speed )( rttr::policy::prop::as_reference_wrapper )
.property( "shaky", &Xivi::Position::shaky )( rttr::policy::prop::as_reference_wrapper )
;
}
}