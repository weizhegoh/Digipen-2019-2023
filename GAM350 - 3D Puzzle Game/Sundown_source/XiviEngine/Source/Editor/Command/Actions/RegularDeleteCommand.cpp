/*****************************************************************
*\file         RegularDeleteCommand.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "RegularDeleteCommand.h"

namespace Xivi::Editor
{
RegularDeleteCommand::RegularDeleteCommand( ECS::World::Instance &world,
                                            Entity &entity,
                                            std::vector<CacheInfo> &&cached )
  :m_world( world ),
  m_entity( entity ),
  m_cached( std::move( cached ) )
{}

Ptr<ICommand> RegularDeleteCommand::Create( ECS::World::Instance &world, Entity &entity, std::vector<CacheInfo> &&cache )
{
  auto command = new RegularDeleteCommand( world, entity, std::move( cache ) );
  command->Execute();
  return command;
}

void RegularDeleteCommand::Execute()
{
  Entity tmp = m_entity;
  m_world.DestroyEntity( tmp );
  m_world.m_archetype_manager.UpdateStructuralChanges();
}

void RegularDeleteCommand::Undo()
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
  m_world.m_archetype_manager.UpdateStructuralChanges();
}
}
#endif