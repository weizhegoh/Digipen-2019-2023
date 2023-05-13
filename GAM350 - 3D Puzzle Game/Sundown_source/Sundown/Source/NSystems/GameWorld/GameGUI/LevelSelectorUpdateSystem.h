/*****************************************************************
*\file         LevelSelectorUpdateSystem.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "UIButtonMovementSystem.h"
#include "Xivi/Graphics/Manager/GameStateManager.h"
#include "HelperFunctions/GameGUIHelperFunction.h"
#include "Components/GUIComponents.h"

namespace Xivi
{
DefineSystemEventSystem( LevelSelectorUpdateSystem, UIButtonMovementSystem, LevelSelectorEvent );

Ptr<GUI::Manager> m_canvas_stack { nullptr };
Ptr<RM> m_resource_manager { nullptr };
Ptr<Audio::AudioManager> m_audio_manager { nullptr };
void OnCreate() noexcept;
void OnEvent( Entity &hovered_entity ) noexcept;

template <ECS::Component::Concepts::IsComponent C>
void UpdateScoreAndRat( Entity &hovered_entity, const GSM::GS_STATES state_to_view ) noexcept
{
  bool found = false;
  if ( m_world.HaveComponent<GUI::L1_1Tag>( hovered_entity ) ||
       m_world.HaveComponent<GUI::L1_2Tag>( hovered_entity ) ||
       m_world.HaveComponent<GUI::L1_3Tag>( hovered_entity ) ||
       m_world.HaveComponent<GUI::L1_4Tag>( hovered_entity ) ||
       m_world.HaveComponent<GUI::L2_1Tag>( hovered_entity ) ||
       m_world.HaveComponent<GUI::L2_2Tag>( hovered_entity ) )
    found = true;
  if ( !m_world.HaveComponent<C>( hovered_entity ) && found )
    return;

  #ifndef XV_EDITOR
  auto &gsm = Service::Locator::Get<GSM::GSManager>();
  #endif
  i32 index = -1;
  std::string texture_rat = "Assets/Textures/", texture_img = "Assets/Textures/";
  if ( found )
  {
    switch ( state_to_view )
    {
      case GSM::LEVEL1_STAGE1:
      index = 0;
      texture_rat += "TomRat.dds";
      texture_img += "Level_1-1.dds";
      break;
      case GSM::LEVEL1_STAGE2:
      index = 1;
      texture_rat += "TomRat.dds";
      texture_img += "Level_1-2.dds";
      break;
      case GSM::LEVEL1_STAGE3:
      index = 2;
      texture_rat += "TomRat.dds";
      texture_img += "Level_1-3.dds";
      break;
      case GSM::LEVEL1_STAGE4:
      index = 3;
      texture_rat += "TomRat.dds";
      texture_img += "Level_1-4.dds";
      break;
      case GSM::LEVEL2_STAGE1:
      index = 4;
      texture_rat += "JohnRat.dds";
      texture_img += "Level_2-1.dds";
      break;
      case GSM::LEVEL2_STAGE2:
      index = 5;
      texture_rat += "JohnRat.dds";
      texture_img += "Level_2-2.dds";
      break;
      default:
      break;
    }
  }
  else
  {
    texture_rat += "default.dds";
    texture_img += "default.dds";
  }

  // Update rat image (John/Tom)
  {
    ECS::Query::Instance img_rat;
    img_rat.AllOf<GUI::LevelSelectorRat>();
    ForEach( img_rat, [&]( Material &material, RenderSettings &render_settings )
    {
      render_settings.transparent = index < 0 ?
        0.0f :
        1.0f;

      material.diffuse = m_resource_manager->Get<Graphics::Texture>( texture_rat );
      material.specular = m_resource_manager->Get<Graphics::Texture>( texture_rat );
    } );
  }

  // Update level image
  {
    ECS::Query::Instance img_level;
    img_level.AllOf<GUI::LevelSelectorImage>();
    ForEach( img_level, [&]( Material &material, RenderSettings &render_settings )
    {
      render_settings.transparent = index < 0 ?
        0.0f :
        1.0f;

      material.diffuse = m_resource_manager->Get<Graphics::Texture>( texture_img );
      material.specular = m_resource_manager->Get<Graphics::Texture>( texture_img );
    } );
  }

  #ifndef XV_EDITOR
      // Update number of cheese
  {
    i32 number_of_cheese = index < 0 ?
      0 :
      gsm.list_collectable_highscore[index];
    ECS::Query::Instance not_game_menu_cheese;
    not_game_menu_cheese.NoneOf<GUI::GUICheeseGameMenu>();
    ForEach( not_game_menu_cheese, [&]( GUI::UICheese &cheese, RenderSettings &render_settings )
    {
      render_settings.transparent = cheese.m_id <= number_of_cheese ?
        1.0f :
        0.0f;
    } );
  }

  // Update Move Left Text
  {
    ECS::Query::Instance move_left_text;
    move_left_text.AllOf<GUI::MoveLeftTextTag>();
    ForEach( move_left_text, [&]( RenderSettings &render_settings )
    {
      render_settings.transparent = index < 0 ?
        0.0f :
        1.0f;
    } );
  }

  // Update move left
  {
    i32 number_of_move_left = index < 0 ?
      0 :
      gsm.list_move_highscore[index];
    const i32 ones = number_of_move_left % 10;
    const i32 tens = ( number_of_move_left / 10 ) % 10;
    const i32 hundreds = ( number_of_move_left / 100 ) % 10;

    // Ones
    {
      ECS::Query::Instance ones_query;
      ones_query.AllOf<GUI::GUINumberOnes>();
      ForEach( ones_query, [&]( Entity &entity, Material &material, RenderSettings &render_settings )
      {
        render_settings.transparent = index < 0 ?
          0.0f :
          1.0f;

        std::string tex = Helper::GetNumberTextureFont2( ones );
        material.diffuse = m_resource_manager->Get<Graphics::Texture>( tex );
        material.specular = m_resource_manager->Get<Graphics::Texture>( tex );
      } );
    }

    // Tens
    {
      ECS::Query::Instance tens_query;
      tens_query.AllOf<GUI::GUINumberTens>();
      ForEach( tens_query, [&]( Entity &entity, Material &material, RenderSettings &render_settings )
      {
        render_settings.transparent = index < 0 ?
          0.0f :
          1.0f;

        std::string tex = Helper::GetNumberTextureFont2( tens );
        material.diffuse = m_resource_manager->Get<Graphics::Texture>( tex );
        material.specular = m_resource_manager->Get<Graphics::Texture>( tex );
      } );
    }

    // Hundreds
    {
      ECS::Query::Instance hundreds_query;
      hundreds_query.AllOf<GUI::GUINumberHundreds>();
      ForEach( hundreds_query, [&]( Entity &entity, Material &material, RenderSettings &render_settings )
      {
        render_settings.transparent = index < 0 ?
          0.0f :
          1.0f;

        std::string tex = Helper::GetNumberTextureFont2( hundreds );
        material.diffuse = m_resource_manager->Get<Graphics::Texture>( tex );
        material.specular = m_resource_manager->Get<Graphics::Texture>( tex );
      } );
    }
  }
  #endif
}

EndDefine();
}