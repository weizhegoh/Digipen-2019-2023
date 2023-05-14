/*****************************************************************
*\file         BackToWinLoseScreenSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "XiviEngine.h"
#include "Xivi/Graphics/Manager/GameStateManager.h"
#include "BackToWinLoseScreenSystem.h"
#include "HelperFunctions/GameGUIHelperFunction.h"

namespace Xivi
{
void BackToWinLoseScreenSystem::OnCreate() noexcept
{
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();
  m_audio_manager = &Audio::AudioManager::Instance();
  #ifndef XV_EDITOR
  m_gsm = &Service::Locator::Get<GSM::GSManager>();
  #endif
  m_canvas_can_go_back_to_winlose_sreen.AllOf<GUI::CanvasTag, GUI::PressEscapeToWinLoseScreenTag>();
  m_resource_manager = &Service::Locator::Get<RM>();
}

void BackToWinLoseScreenSystem::OnEvent() noexcept
{
  bool can_go_back_to_winlose_sreen = false;
  ForEach( m_canvas_can_go_back_to_winlose_sreen, [&]( Entity &entity )
  {
    if ( m_canvas_stack->Exist( entity ) )
    {
      can_go_back_to_winlose_sreen = true;
      return true;
    }
    return false;
  } );

  if ( !can_go_back_to_winlose_sreen )
    return;

  if ( !m_canvas_stack->Empty() )
    m_canvas_stack->PopAll();

  if ( !m_gsm )
    return;
  #ifndef XV_EDITOR
  switch ( static_cast<GSM::GS_STATES>( m_gsm->mGS_current ) )
  {
    case GSM::WIN:
    {
      std::string str = "Assets/Textures/";
      ECS::Query::Instance canvas;
      canvas.AllOf<GUI::CanvasTag, GUI::WinCanvasTag>();
      switch ( m_gsm->collectables_collected )
      {
        case 0:
        str += "0star_goal.dds";
        break;
        case 1:
        str += "1star_goal.dds";
        break;
        case 2:
        str += "2star_goal.dds";
        break;
        case 3:
        str += "3star_goal.dds";
        break;
        default:
        break;
      }
      ForEach( canvas, [&]( Entity &entity, Material &material )
      {
        material.diffuse = m_resource_manager->Get<Graphics::Texture>( str );
        material.specular = m_resource_manager->Get<Graphics::Texture>( str );
        Helper::RecursivePushGUI( m_world, entity );
      } );

      m_audio_manager->playSFX( "SFX_UITRIGGER.ogg", false );
      break;
    }
    case GSM::WIN2:
    {
      if ( m_gsm->collectables_collected == 0 )
        break;

      ECS::Query::Instance not_game_menu_cheese;
      not_game_menu_cheese.NoneOf<GUI::GUICheeseGameMenu>();
      ForEach( not_game_menu_cheese, [&]( Entity &entity, GUI::UICheese &cheese, RenderSettings &render_settings )
      {
        //auto& entry = world->m_entity_manager.Get(world->GetComponent<Parent>(entity).m_entity);
        //if (entry.m_name == parent_name)
        {
          if ( m_gsm->collectables_collected >= 1 && cheese.m_id == 1 )
            render_settings.transparent = 1.0f;
          else if ( m_gsm->collectables_collected >= 2 && cheese.m_id == 2 )
            render_settings.transparent = 1.0f;
          else if ( m_gsm->collectables_collected >= 3 && cheese.m_id == 3 )
            render_settings.transparent = 1.0f;
        }
      } );
      ECS::Query::Instance canvas;
      canvas.AllOf<GUI::CanvasTag, GUI::WinCanvasTag>();
      ForEach( canvas, [&]( Entity &entity )
      {
        Helper::RecursivePushGUI( m_world, entity );
      } );

      m_audio_manager->playSFX( "SFX_UITRIGGER.ogg", false );
      break;
    }
    case GSM::GAMEOVER: [[fallthrough]];
    case GSM::GAMEOVER2:
    {
      ECS::Query::Instance canvas;
      canvas.AllOf<GUI::CanvasTag, GUI::LoseCanvasTag>();
      ForEach( canvas, [&]( Entity &entity )
      {
        Helper::RecursivePushGUI( m_world, entity );
      } );

      m_audio_manager->playSFX( "SFX_UITRIGGER.ogg", false );
      break;
    }
  }
  #endif
}
}