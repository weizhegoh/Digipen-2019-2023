/*****************************************************************
*\file         PauseUnpauseSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "PauseUnpauseSystem.h"
#include "XiviEngine.h"
#include "HelperFunctions/GameGUIHelperFunction.h"

namespace Xivi
{
void PauseUnpauseSystem::OnCreate() noexcept
{
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();
  m_audio_manager = &Audio::AudioManager::Instance();
  m_gui_window.AllOf<Root, GUI::WindowTag>();
}

void PauseUnpauseSystem::OnEvent() noexcept
{
  Entity pause_canvas;

  ForEach( m_gui_window, [&]( Entity &entity, Child &childs )
  {
    for ( auto &child : childs.m_entities )
    {
      if ( m_world.HaveComponents<GUI::PauseCanvasTag, UI>( child ) )
      {
        pause_canvas = child;
        return true;
      }
    }
    return false;
  } );

// Don't do anything if there is no pause canvas
  if ( !pause_canvas.Valid() )
    return;

  // Pause actived, play sfx
  m_audio_manager->playSFX( "SFX_UITRIGGER.ogg", false );

  if ( !m_paused )
  {
    //pause game
    m_paused = true;
    // Pop all canvas if any
    if ( !m_canvas_stack->Empty() )
      m_canvas_stack->PopAll();
    // Push pause uis
    if ( pause_canvas.Valid() )
      Helper::RecursivePushGUI( m_world, pause_canvas );
    m_canvas_stack->Sort();
  }
  else
  {
    //unpause
    m_paused = false;

    // Pop all canvas if any
    if ( !m_canvas_stack->Empty() )
      m_canvas_stack->PopAll();
    // Push back game ui
    Helper::PushGameGUI( m_world, *m_canvas_stack );
  }
}
}