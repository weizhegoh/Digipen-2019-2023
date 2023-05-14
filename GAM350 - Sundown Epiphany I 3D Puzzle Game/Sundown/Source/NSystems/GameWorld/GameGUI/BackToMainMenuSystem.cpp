/*****************************************************************
*\file         BackToMainMenuSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "XiviEngine.h"
#include "BackToMainMenuSystem.h"
#include "HelperFunctions/GameGUIHelperFunction.h"

namespace Xivi
{
void BackToMainMenuSystem::OnCreate() noexcept
{
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();
  m_audio_manager = &Audio::AudioManager::Instance();
  m_canvas_to_exit.AllOf<GUI::CanvasTag, GUI::PressEscapeToMainMenuTag>();
}

void BackToMainMenuSystem::OnEvent() noexcept
{
  bool can_press_esc_to_mainmenu = false;
  ForEach( m_canvas_to_exit, [&]( Entity &entity )
  {
    if ( m_canvas_stack->Exist( entity ) )
    {
      can_press_esc_to_mainmenu = true;
      return true;
    }
    return false;
  } );

  if ( can_press_esc_to_mainmenu )
  {
    if ( !m_canvas_stack->Empty() )
      m_canvas_stack->PopAll();
    Helper::PushMainMenuGUI( m_world, *m_canvas_stack );
    m_audio_manager->StopAllSFX();

    m_audio_manager->playSFX( "SFX_UITRIGGER.ogg", false );
    m_audio_manager->playLoop( "LOOP_MAINMENU.ogg", false );
  }
}
}