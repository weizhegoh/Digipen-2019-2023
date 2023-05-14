/*****************************************************************
*\file         Direction.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
namespace Xivi
{
struct Direction
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Direction"
  };
  //allow change
  Vec3f m_dir { 0.0f, 0.0f, 0.0f };
  Vec3f offset { 0.0f, 0.0f, 0.0f };
  Vec3f endPoint { 0.0f, 0.0f, 0.0f };
  float speed = 1.f;

  //glm::vec3 GetGlm() { return glm::vec3(getX(m_dir), getY(m_dir), getZ(m_dir)); }
  //void SetGlm(const glm::vec3 position) { m_dir.set(position.x, position.y, position.z); }
};
}

namespace Xivi::Components::Direction::Reflect
{
RTTR_REGISTRATION
{
    // Direction
    rttr::registration::class_<Xivi::Direction>( std::string( Xivi::Direction::info.m_name ) )
    ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::Direction>.m_id ) )
    .property( "Direction", &Xivi::Direction::m_dir )( rttr::policy::prop::as_reference_wrapper )
    .property( "Offset", &Xivi::Direction::offset )( rttr::policy::prop::as_reference_wrapper )
    .property( "Speed", &Xivi::Direction::speed )( rttr::policy::prop::as_reference_wrapper )
    .property( "EndPoint", &Xivi::Direction::endPoint )( rttr::policy::prop::as_reference_wrapper );
}
}