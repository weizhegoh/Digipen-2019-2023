/*****************************************************************
*\file         UIDrag.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi::GUI
{
struct UIDrag
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "UI Drag"
  };

  Vec2f m_min_pt;
  Vec2f m_max_pt;
  Vec2f m_cur_pt;
};
};

namespace Xivi::Components::UIDrag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::UIDrag>( std::string( Xivi::GUI::UIDrag::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::GUI::UIDrag>.m_id ) );
}
}