/*****************************************************************
*\file         DeleteEntityCommand.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "DeleteEntityCommand.h"
#include "DeletePrefabEntityCommand.h"
#include "DeletePrefabInstanceCommand.h"
#include "RegularDeleteCommand.h"

namespace Xivi::Editor
{
Ptr<ICommand> DeleteEntityCommand::Create( ECS::World::Instance &world, Entity &entity )
{
  auto &c_desc_mgr = ECS::Component::DescriptorManager::Instance();
  std::vector<CacheInfo> cache;
  std::function<void( Entity )> recorder = [&]( Entity e )->void
  {
    CacheInfo info;
    info.m_archetype = &world.GetArchetype( e );
    info.m_global_index = e.Index();
    info.m_entry = world.m_entity_manager.Get( e );
    info.m_destroy.m_descriptors = c_desc_mgr.DescriptorVector( info.m_archetype->GetSignature() & ~c_desc_mgr.m_tag_bits );
    // Copy all data
    for ( const auto &[i, desc] : Enumerate( info.m_destroy.m_descriptors ) )
    {
      info.m_destroy.m_components_cache.push_back( MakeUniqueOverwrite<byte[]>( desc->m_size ) );
      if ( desc->m_constructor )
        desc->m_constructor( info.m_destroy.m_components_cache.back().get() );
      auto component = info.m_entry.m_pool->GetComponent( info.m_entry.m_pool_index, *desc );
      if ( desc->m_copy )
        desc->m_copy( info.m_destroy.m_components_cache.back().get(), component );
      else
        std::memcpy( info.m_destroy.m_components_cache.back().get(), component, desc->m_size );
    }

    cache.push_back( std::move( info ) );

    if ( world.HaveComponent<Child>( e ) )
      world.GetEntity( e, [&]( Child &child )
    {
      for ( const auto &c : child.m_entities )
        recorder( c );
    } );
  };

  recorder( entity );
  Ptr<ICommand> command { nullptr };
  if ( world.HaveComponent<Prefab>( entity ) )
    command = DeletePrefabEntityCommand::Create( world, entity, std::move( cache ) );
  else if ( world.HaveComponent<ReferencedPrefab>( entity ) )
    command = DeletePrefabInstanceCommand::Create( world, entity, std::move( cache ) );
  else
    command = RegularDeleteCommand::Create( world, entity, std::move( cache ) );
  return command;
}
}
#endif