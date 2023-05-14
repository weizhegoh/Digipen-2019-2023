/*****************************************************************
*\file         UITrigger.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi::GUI
{
struct UITrigger
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "UI Trigger"
  };

  std::string m_key_to_func;
  std::function<void( Entity )> m_callback;
};
};

namespace Xivi::Components::UITrigger::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::UITrigger>( std::string( Xivi::GUI::UITrigger::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::GUI::UITrigger>.m_id ) )
  .property( "FuncKey", &Xivi::GUI::UITrigger::m_key_to_func )( rttr::policy::prop::as_reference_wrapper );
}
}