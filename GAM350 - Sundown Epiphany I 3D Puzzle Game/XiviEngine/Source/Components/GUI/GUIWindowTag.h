/*****************************************************************
*\file         GUIWindowTag.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi::GUI
{
struct WindowTag
{
  constexpr static auto info = Xivi::ECS::Component::Type::Tag
  {
    .m_name = "Window Tag"
  };
};
};

namespace Xivi::Components::WindowTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::WindowTag>( std::string( Xivi::GUI::WindowTag::info.m_name ) )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::WindowTag>.m_id ) );
}
}