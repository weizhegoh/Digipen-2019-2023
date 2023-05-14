/*****************************************************************
*\file         UI.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi
{
struct UI
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "UI"
  };
  //allow change
  float transparency;
};
}

namespace Xivi::Components::UI::Reflect
{
RTTR_REGISTRATION
{
  // Transform
    rttr::registration::class_<Xivi::UI>( std::string( Xivi::UI::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::UI>.m_id ) )
  .property( "transparency", &Xivi::UI::transparency )( rttr::policy::prop::as_reference_wrapper )
  ;
}
}