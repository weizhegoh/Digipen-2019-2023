/*****************************************************************
*\file         DeletePrefabEntityCommand.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "DeletePrefabEntityCommand.h"

namespace Xivi::Editor
{
DeletePrefabEntityCommand::DeletePrefabEntityCommand( ECS::World::Instance &world,
                                                      Entity &entity,
                                                      std::vector<CacheInfo> &&cached,
                                                      std::unordered_map<Entity, ReferencedPrefab> &&references )
  :m_world( world ),
  m_entity( entity ),
  m_cached( std::move( cached ) ),
  m_references( std::move( references ) )
{}

Ptr<ICommand> DeletePrefabEntityCommand::Create( ECS::World::Instance &world, Entity &entity, std::vector<CacheInfo> &&cache )
{
  std::unordered_map<Entity, ReferencedPrefab> references;

  std::function<void( Entity )> recorder = [&]( Entity e )
  {
    world.GetEntity( e, [&]( Prefab &prefab )
    {
      for ( auto &ref_e : prefab.referenced_entities )
        world.GetEntity( ref_e, [&]( ReferencedPrefab &ref_prefab )
      {
        references[ref_e] = ref_prefab;
      } );
    } );

    if ( world.HaveComponent<Child>( e ) )
      world.GetEntity( e, [&]( Child &child )
    {
      for ( auto &c : child.m_entities )
        recorder( c );
    } );
  };

  recorder( entity );
  auto command = new DeletePrefabEntityCommand( world, entity, std::move( cache ), std::move( references ) );
  command->Execute();
  return command;
}

void DeletePrefabEntityCommand::Execute()
{
  Entity tmp = m_entity;
  m_world.DestroyEntity( tmp );
  m_world.m_archetype_manager.UpdateStructuralChanges();
}

void DeletePrefabEntityCommand::Undo()
{
  // Rebuild Object
  for ( auto &cached_info : m_cached )
  {
    auto entity = cached_info.m_archetype->CreateEntity();
    auto &entry = m_world.m_entity_manager.Get( entity );

    // Update original cache with new pool & pool index
    cached_info.m_entry.m_pool = entry.m_pool;
    cached_info.m_entry.m_pool_index = entry.m_pool_index;

    // Copy back all data
    for ( const auto &[i, desc] : Enumerate( cached_info.m_destroy.m_descriptors ) )
    {
      // Used reference to update even if pool entity is referencing different global
      auto component = entry.m_pool->GetComponent( entry.m_pool_index, *desc );
      if ( desc->m_copy )
        desc->m_copy( component, cached_info.m_destroy.m_components_cache[i].get() );
      else
        std::memcpy( component, cached_info.m_destroy.m_components_cache[i].get(), desc->m_size );
    }
  }

  // Relink Global
  for ( const auto &cached_info : m_cached )
  {
    auto &entry = m_world.m_entity_manager.m_entities[cached_info.m_global_index];
    entry = cached_info.m_entry;
  }

  // Relink Prefab Instances
  std::function<void( Entity )> linker = [&]( Entity entity )
  {
    m_world.GetEntity( entity, [&]( Prefab &prefab )
    {
      for ( auto &e : prefab.referenced_entities )
        m_world.AddOrRemoveComponents<CList<ReferencedPrefab>>( e, [&]( ReferencedPrefab &ref_prefab )
      {
        ref_prefab = m_references[e];
      } );
    } );

    if ( m_world.HaveComponent<Child>( entity ) )
      m_world.GetEntity( entity, [&]( Child &child )
    {
      for ( auto &c : child.m_entities )
        linker( c );
    } );
  };

  linker( m_entity );
  m_world.m_archetype_manager.UpdateStructuralChanges();
}
}
#endif