/*****************************************************************
*\file         HowToPlaySystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "XiviEngine.h"
#include "HowToPlaySystem.h"
#include "HelperFunctions/GameGUIHelperFunction.h"

namespace Xivi
{
void HowToPlaySystem::OnCreate() noexcept
{
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();
  m_audio_manager = &Audio::AudioManager::Instance();
  m_resource_manager = &Service::Locator::Get<RM>();
  m_how_to_play.AllOf<GUI::HowToPlayCanvasTag>();
}

void HowToPlaySystem::OnEvent( const bool &left_triggered, const bool &right_triggered ) noexcept
{
  if ( m_paused )
    return;

  bool no_how_to_play_pg = true;

  for ( const auto &ui : *m_canvas_stack->Get() )
  {
    if ( HaveComponent<GUI::HowToPlayCanvasTag>( ui ) )
      no_how_to_play_pg = false;
  }

  // Early exit if not how to play page
  if ( no_how_to_play_pg )
    return;

  // Change the page to show
  if ( left_triggered )
    --m_cur_pg;
  if ( right_triggered )
    ++m_cur_pg;
  m_cur_pg = Math::clamp( m_cur_pg, 0, s_number_of_pg - 1 );

  // Show the page according to page number
  std::string texture = m_list_of_how_to__play[m_cur_pg];

  // Set the texture
  ForEach( m_how_to_play, [&]( Material &material )
  {
    material.diffuse = m_resource_manager->Get<Graphics::Texture>( texture );
    material.specular = m_resource_manager->Get<Graphics::Texture>( texture );
  } );

// Play sfx
  m_audio_manager->playSFX( "SFX_UIHOVER.ogg", false );
}
}