/*****************************************************************
*\file         PopupDialogueComponents.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi::GUI
{
DefineDataComponent( SwitchPopupDialogue );
bool m_activated = false;
bool m_closed = false;
EndDefine();

DefineDataComponent( PressurePlatePopupDialogue );
bool m_activated = false;
bool m_closed = false;
EndDefine();

DefineDataComponent( CheesePopupDialogue );
bool m_activated = false;
bool m_closed = false;
EndDefine();

DefineDataComponent( CartPopupDialogue );
bool m_activated = false;
bool m_closed = false;
EndDefine();

DefineDataComponent( HolePopupDialogue );
bool m_activated = false;
bool m_closed = false;
EndDefine();

DefineDataComponent( ObstaclePopupDialogue );
bool m_activated = false;
bool m_closed = false;
EndDefine();
};

namespace Xivi
{
using UIDialogueComponents = CList<
  GUI::SwitchPopupDialogue,
  GUI::PressurePlatePopupDialogue,
  GUI::CheesePopupDialogue,
  GUI::CartPopupDialogue,
  GUI::HolePopupDialogue,
  GUI::ObstaclePopupDialogue
>;
}

namespace Xivi::Components::SwitchPopupDialogue::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::SwitchPopupDialogue>( std::string( Xivi::GUI::SwitchPopupDialogue::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::GUI::SwitchPopupDialogue>.m_id ) )
  .property( "ID", &Xivi::GUI::SwitchPopupDialogue::m_activated )( rttr::metadata( "NoEditor", true ) )
  .property( "ID", &Xivi::GUI::SwitchPopupDialogue::m_closed )( rttr::metadata( "NoEditor", true ) );
}
}

namespace Xivi::Components::PressurePlatePopupDialogue::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::PressurePlatePopupDialogue>( std::string( Xivi::GUI::PressurePlatePopupDialogue::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::GUI::PressurePlatePopupDialogue>.m_id ) )
  .property( "ID", &Xivi::GUI::PressurePlatePopupDialogue::m_activated )( rttr::metadata( "NoEditor", true ) )
  .property( "ID", &Xivi::GUI::PressurePlatePopupDialogue::m_closed )( rttr::metadata( "NoEditor", true ) );
}
}

namespace Xivi::Components::CheesePopupDialogue::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::CheesePopupDialogue>( std::string( Xivi::GUI::CheesePopupDialogue::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::GUI::CheesePopupDialogue>.m_id ) )
  .property( "ID", &Xivi::GUI::CheesePopupDialogue::m_activated )( rttr::metadata( "NoEditor", true ) )
  .property( "ID", &Xivi::GUI::CheesePopupDialogue::m_closed )( rttr::metadata( "NoEditor", true ) );
}
}

namespace Xivi::Components::CartPopupDialogue::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::CartPopupDialogue>( std::string( Xivi::GUI::CartPopupDialogue::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::GUI::CartPopupDialogue>.m_id ) )
  .property( "ID", &Xivi::GUI::CartPopupDialogue::m_activated )( rttr::metadata( "NoEditor", true ) )
  .property( "ID", &Xivi::GUI::CartPopupDialogue::m_closed )( rttr::metadata( "NoEditor", true ) );
}
}

namespace Xivi::Components::HolePopupDialogue::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::HolePopupDialogue>( std::string( Xivi::GUI::HolePopupDialogue::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::GUI::HolePopupDialogue>.m_id ) )
  .property( "ID", &Xivi::GUI::HolePopupDialogue::m_activated )( rttr::metadata( "NoEditor", true ) )
  .property( "ID", &Xivi::GUI::HolePopupDialogue::m_closed )( rttr::metadata( "NoEditor", true ) );
}
}

namespace Xivi::Components::ObstaclePopupDialogue::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::ObstaclePopupDialogue>( std::string( Xivi::GUI::ObstaclePopupDialogue::info.m_name ) )
  ( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::GUI::ObstaclePopupDialogue>.m_id ) )
  .property( "ID", &Xivi::GUI::ObstaclePopupDialogue::m_activated )( rttr::metadata( "NoEditor", true ) )
  .property( "ID", &Xivi::GUI::ObstaclePopupDialogue::m_closed )( rttr::metadata( "NoEditor", true ) );
}
}