/*****************************************************************
*\file         UIBar.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi::GUI
{
DefineDataComponent( UIBar );
i32 m_id = 0;
EndDefine();
};

namespace Xivi::Components::UIBar::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::UIBar>( std::string( Xivi::GUI::UIBar::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::GUI::UIBar>.m_id ) )
  .property( "ID", &Xivi::GUI::UIBar::m_id )( rttr::policy::prop::as_reference_wrapper );
}
}