/*****************************************************************
*\file         Transform.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "Xivi/Core/Utils/Math.h"

namespace Xivi
{
struct Transform
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Transform"
  };
  //allow change
  Vec3f scale { 5.f, 5.f, 5.f };
  Vec3f axis { 0.f, 0.f, 0.f };
  Vec3f axisOffset { 0.f, 0.f, 0.f };
  //dont allow change
  Math::SIMD::Mat4x4 LocalToWorld = Math::SIMD::Mat4x4( 1.0 );
  f32 ratio { 0.f };
};
}

namespace Xivi::Components::Transform::Reflect
{
RTTR_REGISTRATION
{
// Transform
  rttr::registration::class_<Xivi::Transform>( std::string( Xivi::Transform::info.m_name ) )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::Transform>.m_id ) )
.property( "Scale", &Xivi::Transform::scale )( rttr::policy::prop::as_reference_wrapper )
.property( "Axis", &Xivi::Transform::axis )( rttr::policy::prop::as_reference_wrapper )
.property( "LocalToWorld", &Xivi::Transform::LocalToWorld )( rttr::metadata( "NoSerialise", true ), rttr::metadata( "NoEditor", true ) )
;
}
}