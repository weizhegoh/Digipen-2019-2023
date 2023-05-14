/*****************************************************************
*\file         Colour.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "Xivi/Core/Utils/Math.h"

namespace Xivi
{
struct Colour
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Colour"
  };
  //allow change
  Vec3f colour = Vec3f( 1.f, 1.f, 1.f );
  Vec3f init_colour = Vec3f( 1.f, 1.f, 1.f );
};
}

namespace Xivi::Components::Colour::Reflect
{
RTTR_REGISTRATION
{
// Colour
rttr::registration::class_<Xivi::Colour>( std::string( Xivi::Colour::info.m_name ) )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::Colour>.m_id ) )
.property( "Colour", &Xivi::Colour::colour )( rttr::policy::prop::as_reference_wrapper )
.property( "Initial Colour", &Xivi::Colour::init_colour )( rttr::policy::prop::as_reference_wrapper );
}
}