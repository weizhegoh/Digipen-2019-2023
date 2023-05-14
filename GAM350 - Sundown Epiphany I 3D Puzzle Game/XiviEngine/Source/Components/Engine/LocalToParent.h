/*****************************************************************
*\file         LocalToParent.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct LocalToParent
{
  static constexpr auto info = ECS::Component::Type::Data { .m_name = "Local To Parent" };

  Vec3f m_position { 0.f,0.f,0.f };
  Vec3f scale { 1.f, 1.f, 1.f };
  Vec3f axis { 0.f, 0.f, 0.f };
  Math::Matrix4 localToParent;
};
}

namespace Xivi::Components::LocalToParent::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::LocalToParent>( "Local To Parent" )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::LocalToParent>.m_id ) )
  .property( "Position", &Xivi::LocalToParent::m_position )( rttr::policy::prop::as_reference_wrapper )
  .property( "scale", &Xivi::LocalToParent::scale )( rttr::policy::prop::as_reference_wrapper )
  .property( "axis", &Xivi::LocalToParent::axis )( rttr::policy::prop::as_reference_wrapper );
}
}