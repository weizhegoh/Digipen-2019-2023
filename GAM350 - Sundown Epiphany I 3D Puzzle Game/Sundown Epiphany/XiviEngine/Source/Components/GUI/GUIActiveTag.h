/*****************************************************************
*\file         GUIActiveTag.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi::GUI
{
struct GUIActiveTag
{
  constexpr static auto info = Xivi::ECS::Component::Type::Tag
  {
    .m_name = "GUI Active Tag"
  };
};
};

namespace Xivi::Components::GUIActiveTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::GUIActiveTag>( std::string( Xivi::GUI::GUIActiveTag::info.m_name ) )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::GUIActiveTag>.m_id ) );
}
}
