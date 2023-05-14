/*****************************************************************
*\file         EventRefreshPanel.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "EventRefreshPanel.h"
#include "Editor/System.h"
#include "Components/GraphicsComponents.h"
#include "../Sundown/Source/NComponents.h"
namespace Xivi::Editor
{
void EventRefreshPanelSystem::Init() noexcept
{
  m_skybox_sig.set( ECS::Component::info<SkyBoxMaterial>.m_bit_index );
  m_prefab_sig.set( ECS::Component::info<PrefabTag>.m_bit_index );
  m_ui_sig.set( ECS::Component::info<UI>.m_bit_index );

  m_recursive_add = [=]( ECS::World::Instance &world, const Entity entity, const bool is_prefab, const bool is_ui ) noexcept -> void
  {
    if ( world.HaveComponent<Child>( entity ) )
    {
      auto &children = world.GetComponent<Child>( entity ).m_entities;
      for ( auto &child : children )
      {
        if ( child.Zombie() )
          continue;
        m_to_add.push_back( { child, is_prefab, is_ui } );
        m_recursive_add( world, child, is_prefab, is_ui );
      }
    }
  };
}

void EventRefreshPanelSystem::Update() noexcept
{
  for ( const auto &[entity, is_prefab, is_ui] : m_to_add )
    Editor::System::Instance().AddEntity( entity, is_prefab, is_ui );
  m_to_add.clear();
}

void EventRefreshPanelSystem::OnCall( ECS::World::Instance &world ) noexcept
{
  m_to_add.clear();
  auto &editor_sys = Editor::System::Instance();
  auto &entity_context = editor_sys.m_entity_context;
  entity_context.world_entity.clear();
  entity_context.prefab_entity.clear();
  entity_context.ui_entity.clear();
  entity_context.ClearSelections();
  entity_context.copies.clear();

  for ( const auto &[i, entry] : Enumerate( world.m_entity_manager.m_entities ) )
  {
    if ( entry.Zombie() )
      continue;

    auto &entity = entry.m_pool->GetComponent<ECS::Entity>( entry.m_pool_index );

    if ( world.HaveComponent<Parent>( entity ) )
      continue;

    auto &archetype = world.GetArchetype( entity );

    bool is_skybox = ( archetype.GetSignature() & m_skybox_sig ).any();
    if ( is_skybox )
      continue;

    bool is_prefab = ( archetype.GetSignature() & m_prefab_sig ).any();
    bool is_ui = ( archetype.GetSignature() & m_ui_sig ).any();
    m_to_add.push_back( { entity, is_prefab, is_ui } );

    m_recursive_add( world, entity, is_prefab, is_ui );
  }

  std::sort( m_to_add.begin(), m_to_add.end(), [&]( tuplet::tuple<Entity, bool, bool> &lhs, tuplet::tuple<Entity, bool, bool> &rhs )
  {
    auto lhs_entity = tuplet::get<0>( lhs );
    auto rhs_entity = tuplet::get<0>( rhs );

    auto sort_by_name = [&]()
    {
      //XV_CORE_INFO( "Sort By Name" );
        // Sort by name
      auto name = world.m_entity_manager.Get( lhs_entity ).m_name;
      while ( std::isdigit( name.back() ) )
        name.pop_back();

      if ( auto rhs_name_pos = world.m_entity_manager.Get( rhs_entity ).m_name.find( name ); rhs_name_pos != std::string::npos )
      {
        if ( auto lhs_num = world.m_entity_manager.Get( lhs_entity ).m_name.substr( name.size() ); lhs_num.empty() ||
             !std::ranges::all_of( lhs_num.begin(), lhs_num.end(), []( char c )
        {
          return std::isdigit( c );
        } ) )
        {
          //XV_CORE_INFO( "Sort By Name False" );
          return false;
        }
        else if ( auto rhs_num = world.m_entity_manager.Get( rhs_entity ).m_name.substr( rhs_name_pos + name.size() ); rhs_num.empty() ||
                  !std::ranges::all_of( rhs_num.begin(), rhs_num.end(), []( char c )
        {
          return std::isdigit( c );
        } ) )
        {
          //XV_CORE_INFO( "Sort By Name True" );
          return false;
        }
        else
        {
          //XV_CORE_INFO( "Sort By Name Integer" );

          auto ln = std::stoi( lhs_num );
          auto rn = std::stoi( rhs_num );
          return ln < rn;
        }
      }
      else
      {
        //XV_CORE_INFO( "Sort By Name-Name" );
        return world.m_entity_manager.Get( lhs_entity ).m_name < world.m_entity_manager.Get( rhs_entity ).m_name;
      }
    };

    if ( !world.HaveComponent<EntityType>( lhs_entity ) && !world.HaveComponent<EntityType>( rhs_entity ) )
      return sort_by_name();

    if ( !world.HaveComponent<EntityType>( lhs_entity ) && world.HaveComponent<EntityType>( rhs_entity ) )
    {
        //XV_CORE_INFO( "Sort By Type False" );
      return false;
    }

    if ( world.HaveComponent<EntityType>( lhs_entity ) && !world.HaveComponent<EntityType>( rhs_entity ) )
    {
      //XV_CORE_INFO( "Sort By Type True" );
      return true;
    }
    auto &lhs_type = world.GetComponent<EntityType>( lhs_entity );
    auto &rhs_type = world.GetComponent<EntityType>( rhs_entity );

    if ( lhs_type.value == rhs_type.value )
    {
      if ( world.HaveComponent<ReferencedPrefab>( lhs_entity ) && world.HaveComponent<ReferencedPrefab>( rhs_entity ) )
      {
        //XV_CORE_INFO( "Sort By Prefab Entity" );
        return std::bit_cast<u64>( lhs_entity ) < std::bit_cast<u64>( rhs_entity );
      }

      if ( !world.HaveComponent<ReferencedPrefab>( lhs_entity ) && world.HaveComponent<ReferencedPrefab>( rhs_entity ) )
      {
        //XV_CORE_INFO( "Sort By Prefab False" );
        return false;
      }
      if ( world.HaveComponent<ReferencedPrefab>( lhs_entity ) && !world.HaveComponent<ReferencedPrefab>( rhs_entity ) )
      {
        //XV_CORE_INFO( "Sort By Prefab True" );
        return true;
      }
      return sort_by_name();
    }

    //XV_CORE_INFO( "Sort By type" );
    return lhs_type.value < rhs_type.value;
  } );
}
}
#endif