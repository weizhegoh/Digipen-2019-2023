/*****************************************************************
*\file         InstructionPageSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "XiviEngine.h"
#include "InstructionPageSystem.h"
#include "HelperFunctions/GameGUIHelperFunction.h"

namespace Xivi
{
void InstructionPageSystem::OnCreate() noexcept
{
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();
  m_audio_manager = &Audio::AudioManager::Instance();
  m_resource_manager = &Service::Locator::Get<RM>();
  m_instruction.AllOf<GUI::InstructionCanvasTag>();
}

void InstructionPageSystem::OnEvent( const bool &left_triggered, const bool &right_triggered ) noexcept
{
  if ( m_paused )
    return;

  bool no_instruction = true;

  for ( const auto &ui : *m_canvas_stack->Get() )
  {
    if ( HaveComponent<GUI::InstructionCanvasTag>( ui ) )
      no_instruction = false;
  }

  // Early exit if not instruction page
  if ( no_instruction )
    return;

  // Current have 2 page, keyboard and controller
  const static i32 number_of_pg = 2;

  // Change the page to show
  if ( left_triggered )
  {
    --m_cur_pg;
    if ( m_cur_pg < 0 )
      m_cur_pg = number_of_pg - 1;
  }
  if ( right_triggered )
  {
    ++m_cur_pg;
    if ( m_cur_pg >= number_of_pg )
      m_cur_pg = 0;
  }

  // Show the page according to page number
  std::string texture = m_cur_pg == 0 ?
    "Assets/Textures/Instruction.dds" :
    "Assets/Textures/Instruction_Controller.dds";

  // Set the texture
  ForEach( m_instruction, [&]( Material &material )
  {
    material.diffuse = m_resource_manager->Get<Graphics::Texture>( texture );
    material.specular = m_resource_manager->Get<Graphics::Texture>( texture );
  } );

// Play sfx
  m_audio_manager->playSFX( "SFX_UIHOVER.ogg", false );
}
}