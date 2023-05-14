/*****************************************************************
*\file         Tags.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "Xivi/ECS/XVECS.h"

namespace Xivi::GUI
{
DefineTagComponent( PauseCanvasTag );
DefineTagComponent( MainMenuCanvasTag );
DefineTagComponent( InstructionCanvasTag );
DefineTagComponent( SettingsCanvasTag );
DefineTagComponent( HowToPlayCanvasTag );
DefineTagComponent( VolumeBarTag );
DefineTagComponent( GammaBarTag );
DefineTagComponent( CreditsCanvasTag );
DefineTagComponent( SummaryCanvasTag );
DefineTagComponent( ExitGamePopupCanvasTag );
DefineTagComponent( WinCanvasTag );
DefineTagComponent( LoseCanvasTag );
DefineTagComponent( GoToMainMenuPopupCanvasTag );
DefineTagComponent( PressEscapeToMainMenuTag );
DefineTagComponent( PressEscapeToWinLoseScreenTag );
DefineTagComponent( GameInteratablePopup );
DefineTagComponent( LevelSelectorCanvasTag );
DefineTagComponent( L1_1Tag );
DefineTagComponent( L1_2Tag );
DefineTagComponent( L1_3Tag );
DefineTagComponent( L1_4Tag );
DefineTagComponent( L2_1Tag );
DefineTagComponent( L2_2Tag );
DefineTagComponent( LevelSelectorImage );
DefineTagComponent( LevelSelectorRat );
DefineTagComponent( MoveLeftTextTag );
DefineTagComponent( GUINumberOnes );
DefineTagComponent( GUINumberTens );
DefineTagComponent( GUINumberHundreds );
DefineTagComponent( GUICheeseGameMenu );
DefineTagComponent( TutorialButtonUITag );
DefineTagComponent( KeyboardButtonUITag );
DefineTagComponent( ControllerButtonUITag );
DefineTagComponent( LevelNumberGameUITag );
}

namespace Xivi::Components::PauseCanvasTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::PauseCanvasTag>( std::string{ ECS::Component::info<Xivi::GUI::PauseCanvasTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::PauseCanvasTag>.m_id ) );
}
}

namespace Xivi::Components::MainMenuCanvasTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::MainMenuCanvasTag>( std::string{ ECS::Component::info<Xivi::GUI::MainMenuCanvasTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::MainMenuCanvasTag>.m_id ) );
}
}

namespace Xivi::Components::InstructionCanvasTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::InstructionCanvasTag>( std::string{ ECS::Component::info<Xivi::GUI::InstructionCanvasTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::InstructionCanvasTag>.m_id ) );
}
}

namespace Xivi::Components::SettingsCanvasTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::SettingsCanvasTag>( std::string{ ECS::Component::info<Xivi::GUI::SettingsCanvasTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::SettingsCanvasTag>.m_id ) );
}
}

namespace Xivi::Components::HowToPlayCanvasTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::HowToPlayCanvasTag>( std::string{ ECS::Component::info<Xivi::GUI::HowToPlayCanvasTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::HowToPlayCanvasTag>.m_id ) );
}
}

namespace Xivi::Components::VolumeBarTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::VolumeBarTag>( std::string{ ECS::Component::info<Xivi::GUI::VolumeBarTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::VolumeBarTag>.m_id ) );
}
}

namespace Xivi::Components::GammaBarTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::GammaBarTag>( std::string{ ECS::Component::info<Xivi::GUI::GammaBarTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::GammaBarTag>.m_id ) );
}
}

namespace Xivi::Components::CreditsCanvasTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::CreditsCanvasTag>( std::string{ ECS::Component::info<Xivi::GUI::CreditsCanvasTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::CreditsCanvasTag>.m_id ) );
}
}

namespace Xivi::Components::SummaryCanvasTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::SummaryCanvasTag>( std::string{ ECS::Component::info<Xivi::GUI::SummaryCanvasTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::SummaryCanvasTag>.m_id ) );
}
}

namespace Xivi::Components::ExitGamePopupCanvasTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::ExitGamePopupCanvasTag>( std::string{ ECS::Component::info<Xivi::GUI::ExitGamePopupCanvasTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::ExitGamePopupCanvasTag>.m_id ) );
}
}

namespace Xivi::Components::WinCanvasTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::WinCanvasTag>( std::string{ ECS::Component::info<Xivi::GUI::WinCanvasTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::WinCanvasTag>.m_id ) );
}
}

namespace Xivi::Components::LoseCanvasTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::LoseCanvasTag>( std::string{ ECS::Component::info<Xivi::GUI::LoseCanvasTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::LoseCanvasTag>.m_id ) );
}
}

namespace Xivi::Components::GoToMainMenuPopupCanvasTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::GoToMainMenuPopupCanvasTag>( std::string{ ECS::Component::info<Xivi::GUI::GoToMainMenuPopupCanvasTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::GoToMainMenuPopupCanvasTag>.m_id ) );
}
}

namespace Xivi::Components::PressEscapeToMainMenuTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::PressEscapeToMainMenuTag>( std::string{ ECS::Component::info<Xivi::GUI::PressEscapeToMainMenuTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::PressEscapeToMainMenuTag>.m_id ) );
}
}

namespace Xivi::Components::PressEscapeToWinLoseScreenTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::PressEscapeToWinLoseScreenTag>( std::string{ ECS::Component::info<Xivi::GUI::PressEscapeToWinLoseScreenTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::PressEscapeToWinLoseScreenTag>.m_id ) );
}
}

namespace Xivi::Components::GameInteratablePopup::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::GameInteratablePopup>( std::string{ ECS::Component::info<Xivi::GUI::GameInteratablePopup>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::GameInteratablePopup>.m_id ) );
}
}

namespace Xivi::Components::LevelSelectorCanvasTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::LevelSelectorCanvasTag>( std::string{ ECS::Component::info<Xivi::GUI::LevelSelectorCanvasTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::LevelSelectorCanvasTag>.m_id ) );
}
}

namespace Xivi::Components::L1_1Tag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::L1_1Tag>( std::string{ ECS::Component::info<Xivi::GUI::L1_1Tag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::L1_1Tag>.m_id ) );
}
}

namespace Xivi::Components::L1_2Tag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::L1_2Tag>( std::string{ ECS::Component::info<Xivi::GUI::L1_2Tag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::L1_2Tag>.m_id ) );
}
}

namespace Xivi::Components::L1_3Tag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::L1_3Tag>( std::string{ ECS::Component::info<Xivi::GUI::L1_3Tag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::L1_3Tag>.m_id ) );
}
}

namespace Xivi::Components::L1_4Tag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::L1_4Tag>( std::string{ ECS::Component::info<Xivi::GUI::L1_4Tag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::L1_4Tag>.m_id ) );
}
}

namespace Xivi::Components::L2_1Tag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::L2_1Tag>( std::string{ ECS::Component::info<Xivi::GUI::L2_1Tag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::L2_1Tag>.m_id ) );
}
}

namespace Xivi::Components::L2_2Tag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::L2_2Tag>( std::string{ ECS::Component::info<Xivi::GUI::L2_2Tag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::L2_2Tag>.m_id ) );
}
}

namespace Xivi::Components::LevelSelectorImage::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::LevelSelectorImage>( std::string{ ECS::Component::info<Xivi::GUI::LevelSelectorImage>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::LevelSelectorImage>.m_id ) );
}
}

namespace Xivi::Components::LevelSelectorRat::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::LevelSelectorRat>( std::string{ ECS::Component::info<Xivi::GUI::LevelSelectorRat>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::LevelSelectorRat>.m_id ) );
}
}

namespace Xivi::Components::MoveLeftTextTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::MoveLeftTextTag>( std::string{ ECS::Component::info<Xivi::GUI::MoveLeftTextTag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::MoveLeftTextTag>.m_id ) );
}
}

namespace Xivi::Components::GUINumberOnes::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::GUINumberOnes>( std::string{ ECS::Component::info<Xivi::GUI::GUINumberOnes>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::GUINumberOnes>.m_id ) );
}
}

namespace Xivi::Components::GUINumberTens::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::GUINumberTens>( std::string{ ECS::Component::info<Xivi::GUI::GUINumberTens>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::GUINumberTens>.m_id ) );
}
}

namespace Xivi::Components::GUINumberHundreds::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::GUINumberHundreds>( std::string{ ECS::Component::info<Xivi::GUI::GUINumberHundreds>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::GUINumberHundreds>.m_id ) );
}
}

namespace Xivi::Components::GUICheeseGameMenu::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::GUICheeseGameMenu>( std::string{ ECS::Component::info<Xivi::GUI::GUICheeseGameMenu>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::GUICheeseGameMenu>.m_id ) );
}
}

namespace Xivi::Components::TutorialButtonUITag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::TutorialButtonUITag>( std::string{ ECS::Component::info<Xivi::GUI::TutorialButtonUITag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::TutorialButtonUITag>.m_id ) );
}
}

namespace Xivi::Components::KeyboardButtonUITag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::KeyboardButtonUITag>( std::string{ ECS::Component::info<Xivi::GUI::KeyboardButtonUITag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::KeyboardButtonUITag>.m_id ) );
}
}

namespace Xivi::Components::ControllerButtonUITag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::ControllerButtonUITag>( std::string{ ECS::Component::info<Xivi::GUI::ControllerButtonUITag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::ControllerButtonUITag>.m_id ) );
}
}

namespace Xivi::Components::LevelNumberGameUITag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::LevelNumberGameUITag>( std::string{ ECS::Component::info<Xivi::GUI::LevelNumberGameUITag>.m_name } )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::LevelNumberGameUITag>.m_id ) );
}
}