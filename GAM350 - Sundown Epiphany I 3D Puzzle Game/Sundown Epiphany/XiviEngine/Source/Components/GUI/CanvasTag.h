/*****************************************************************
*\file         CanvasTag.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi::GUI
{
struct CanvasTag
{
  constexpr static auto info = Xivi::ECS::Component::Type::Tag
  {
    .m_name = "Canvas Tag"
  };
};
};

namespace Xivi::Components::CanvasTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::CanvasTag>( std::string( Xivi::GUI::CanvasTag::info.m_name ) )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::CanvasTag>.m_id ) );
}
}
