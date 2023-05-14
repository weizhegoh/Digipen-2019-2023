/*****************************************************************
*\file         UIHover.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi::GUI
{
struct UIHover
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "UI Hover"
  };

  std::string m_key_to_func;
  std::function<void( Entity )> m_callback;
  std::string m_key_to_func_unhover;
  std::function<void( Entity )> m_callback_unhover;
};
};

namespace Xivi::Components::UIHover::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::UIHover>( std::string( Xivi::GUI::UIHover::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::GUI::UIHover>.m_id ) )
  .property( "FuncKey", &Xivi::GUI::UIHover::m_key_to_func )( rttr::policy::prop::as_reference_wrapper )
  .property( "FuncKeyUnhover", &Xivi::GUI::UIHover::m_key_to_func_unhover )( rttr::policy::prop::as_reference_wrapper );
}
}