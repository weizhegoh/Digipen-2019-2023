/*****************************************************************
*\file         RenderSetting.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
namespace Xivi
{
struct RenderSettings
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Render Settings"
  };
  bool onLight;
  bool onRender;
  float transparent = 1.f;
  void Defualt()
  {
    onLight = onRender = true;
  }
  void ColliderBox()
  {
    onLight = onRender = false;
  }
};
}

namespace Xivi::Components::RenderSettings::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::RenderSettings>( std::string( Xivi::RenderSettings::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::RenderSettings>.m_id ) )
  .property( "transparent", &Xivi::RenderSettings::transparent )( rttr::policy::prop::as_reference_wrapper )
  .property( "onLight", &Xivi::RenderSettings::onLight )( rttr::policy::prop::as_reference_wrapper )
  .property( "onRender", &Xivi::RenderSettings::onRender )( rttr::policy::prop::as_reference_wrapper );
}
}