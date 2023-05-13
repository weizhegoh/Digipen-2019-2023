/*****************************************************************
*\file         UpdateSummarySystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "UpdateSummarySystem.h"
#include "Xivi/Graphics/Manager/GameStateManager.h"
#include "Components/EngineComponents.h"
#include "Components/GUIComponents.h"
#include "Components/GraphicsComponents.h"
#include "HelperFunctions/GameGUIHelperFunction.h"

namespace Xivi
{
void PrintCheese( ECS::World::Instance &world, const i32 number_of_cheese, const std::string &parent_name ) noexcept
{
  if ( number_of_cheese == 0 )
    return;

  ECS::Query::Instance not_game_menu_cheese;
  not_game_menu_cheese.NoneOf<GUI::GUICheeseGameMenu>();
  world.ForEach( not_game_menu_cheese, [&]( Entity &entity, GUI::UICheese &cheese, RenderSettings &render_settings )
  {
    auto &entry = world.m_entity_manager.Get( world.GetComponent<Parent>( entity ).m_entity );
    if ( entry.m_name == parent_name )
    {
      if ( number_of_cheese >= 1 && cheese.m_id == 1 )
        render_settings.transparent = 1.0f;
      else if ( number_of_cheese >= 2 && cheese.m_id == 2 )
        render_settings.transparent = 1.0f;
      else if ( number_of_cheese >= 3 && cheese.m_id == 3 )
        render_settings.transparent = 1.0f;
    }
  } );
}

void PrintMoveLeft( ECS::World::Instance &world, const i32 number_of_move_left, const Child &childs, const std::string &parent_name ) noexcept
{
  if ( number_of_move_left == 0 )
    return;

  Entity move_count_ones, move_count_tens, move_count_hundreds;
  const i32 ones = number_of_move_left % 10;
  const i32 tens = ( number_of_move_left / 10 ) % 10;
  const i32 hundreds = ( number_of_move_left / 100 ) % 10;

  for ( auto &child : childs.m_entities )
  {
    auto &entry = world.m_entity_manager.Get( child );
    if ( entry.m_name == "Ones" || entry.m_name == "Ones_copy" )
      move_count_ones = child;
    else if ( entry.m_name == "Tens" || entry.m_name == "Tens_copy" )
      move_count_tens = child;
    else if ( entry.m_name == "Hundreds" || entry.m_name == "Hundreds_copy" )
      move_count_hundreds = child;
    if ( move_count_ones.Valid() && move_count_tens.Valid() && move_count_hundreds.Valid() )
      break;
  }
  auto &resource_manager = Service::Locator::Get<RM>();
  // Ones
  {
    std::string str = Helper::GetNumberTextureFont2( ones );

    auto &material = world.GetComponent<Material>( move_count_ones );
    material.diffuse = resource_manager.Get<Graphics::Texture>( str );
    material.specular = resource_manager.Get<Graphics::Texture>( str );
  }
  // Tens
  {
    std::string str = Helper::GetNumberTextureFont2( tens );

    auto &material = world.GetComponent<Material>( move_count_tens );
    material.diffuse = resource_manager.Get<Graphics::Texture>( str );
    material.specular = resource_manager.Get<Graphics::Texture>( str );
  }
  // Hundreds
  {
    std::string str = Helper::GetNumberTextureFont2( hundreds );

    auto &material = world.GetComponent<Material>( move_count_hundreds );
    material.diffuse = resource_manager.Get<Graphics::Texture>( str );
    material.specular = resource_manager.Get<Graphics::Texture>( str );
  }
}

void UpdateSummarySystem::OnEvent() noexcept
{
  #ifndef XV_EDITOR
  auto &gsm = Service::Locator::Get<GSM::GSManager>();
  ECS::Query::Instance summary_canvas;
  summary_canvas.AllOf<GUI::SummaryCanvasTag>();
  ForEach( summary_canvas, [&]( Entity &entity )
  {
    for ( auto &child : GetComponent<Child>( entity ).m_entities )
    {
      auto &entry = m_world.m_entity_manager.Get( child );
      if ( entry.m_name == "1-1" )
      {
        //for (auto& grandchild : GetComponent<Child>(child).m_entities)
        //{
          // PrintCheese(gsm.list_collectable_collected[0]);
          /*auto& entry2 = m_world.m_entity_manager.Get(grandchild);
          if(entry2.m_name == "cheese1")
          gsm.list_collectable_collected*/
        PrintCheese( m_world, gsm.list_collectable_collected[0], "1-1" );
        PrintMoveLeft( m_world, gsm.list_move_left[0], GetComponent<Child>( child ), "1-1" );
      //}
      }
      else if ( entry.m_name == "1-2" )
      {
        PrintCheese( m_world, gsm.list_collectable_collected[1], "1-2" );
        PrintMoveLeft( m_world, gsm.list_move_left[1], GetComponent<Child>( child ), "1-2" );
      }
      else if ( entry.m_name == "1-3" )
      {
        PrintCheese( m_world, gsm.list_collectable_collected[2], "1-3" );
        PrintMoveLeft( m_world, gsm.list_move_left[2], GetComponent<Child>( child ), "1-3" );
      }
      else if ( entry.m_name == "1-4" )
      {
        PrintCheese( m_world, gsm.list_collectable_collected[3], "1-4" );
        PrintMoveLeft( m_world, gsm.list_move_left[3], GetComponent<Child>( child ), "1-4" );
      }
      else if ( entry.m_name == "2-1" )
      {
        PrintCheese( m_world, gsm.list_collectable_collected[4], "2-1" );
        PrintMoveLeft( m_world, gsm.list_move_left[4], GetComponent<Child>( child ), "2-1" );
      }
      else if ( entry.m_name == "2-2" )
      {
        PrintCheese( m_world, gsm.list_collectable_collected[5], "2-2" );
        PrintMoveLeft( m_world, gsm.list_move_left[5], GetComponent<Child>( child ), "2-2" );
      }
    }
  } );
  #endif
}
}