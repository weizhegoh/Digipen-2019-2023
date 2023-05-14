/*****************************************************************
*\file         UICheckBox.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi::GUI
{
struct UICheckBox
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "UI Check Box"
  };

  bool m_is_checked { false };
};
};

namespace Xivi::Components::UICheckBox::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::UICheckBox>( std::string( Xivi::GUI::UICheckBox::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::GUI::UICheckBox>.m_id ) )
  .property( "Default", &Xivi::GUI::UICheckBox::m_is_checked )( rttr::policy::prop::as_reference_wrapper );
}
}