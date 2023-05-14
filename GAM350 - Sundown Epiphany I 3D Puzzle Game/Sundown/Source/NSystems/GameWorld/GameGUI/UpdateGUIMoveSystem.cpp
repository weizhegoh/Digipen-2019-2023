/*****************************************************************
*\file         UpdateGUIMoveSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "UpdateGUIMoveSystem.h"
#include "NComponents/Entity/Player.h"
#include "Components/GUIComponents.h"
#include "HelperFunctions/GameGUIHelperFunction.h"

namespace Xivi
{
void UpdateGUIMoveSystem::OnCreate() noexcept
{
  m_gui_window.AllOf<Root, GUI::WindowTag>();
  m_resource_manager = &Service::Locator::Get<RM>();
}

void UpdateGUIMoveSystem::OnEvent( const i32 &move_count ) noexcept
{
  Entity move_count_ones, move_count_tens, move_count_hundreds;
  const i32 ones = move_count % 10;
  const i32 tens = ( move_count / 10 ) % 10;
  const i32 hundreds = ( move_count / 100 ) % 10;

  ForEach( m_gui_window, [&]( Entity &entity, Child &childs )
  {
    for ( auto &child : childs.m_entities )
    {
      auto &entry = m_world.m_entity_manager.Get( child );
      if ( entry.m_name == "MoveCountNumberOnes_instance" )
        move_count_ones = child;
      else if ( entry.m_name == "MoveCountNumberTens_instance" )
        move_count_tens = child;
      else if ( entry.m_name == "MoveCountNumberHundreds_instance" )
        move_count_hundreds = child;
      if ( move_count_ones.Valid() && move_count_tens.Valid() && move_count_hundreds.Valid() )
        return true;
    }
    return false;
  } );

//XV_ASSERT_MSG(move_count_ones.Valid() && move_count_tens.Valid() && move_count_hundreds.Valid(), "Canvas not found");
  if ( !( move_count_ones.Valid() && move_count_tens.Valid() && move_count_hundreds.Valid() ) )
    return;
  // Ones
  {
    std::string str = Helper::GetNumberTexture( ones );

    auto &material = GetComponent<Material>( move_count_ones );
    material.diffuse = m_resource_manager->Get<Graphics::Texture>( str );
    material.specular = m_resource_manager->Get<Graphics::Texture>( str );
  }
  // Tens
  {
    std::string str = Helper::GetNumberTexture( tens );

    auto &material = GetComponent<Material>( move_count_tens );
    material.diffuse = m_resource_manager->Get<Graphics::Texture>( str );
    material.specular = m_resource_manager->Get<Graphics::Texture>( str );
  }
  // Hundreds
  {
    std::string str = Helper::GetNumberTexture( hundreds );

    auto &material = GetComponent<Material>( move_count_hundreds );
    material.diffuse = m_resource_manager->Get<Graphics::Texture>( str );
    material.specular = m_resource_manager->Get<Graphics::Texture>( str );
  }
}
}