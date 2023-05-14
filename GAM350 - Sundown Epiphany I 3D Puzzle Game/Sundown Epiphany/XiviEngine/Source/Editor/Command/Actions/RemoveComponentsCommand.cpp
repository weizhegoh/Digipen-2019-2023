/*****************************************************************
*\file         RemoveComponentsCommand.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "RemoveComponentsCommand.h"

namespace Xivi::Editor
{
RemoveComponentsCommand::RemoveComponentsCommand( ECS::World::Instance &world,
                                                  Entity &entity,
                                                  DestroyInfo &&destroy_info )
  : m_world( world ),
  m_entity( entity ),
  m_destroy_info( std::move( destroy_info ) )
{}

Ptr<ICommand> RemoveComponentsCommand::Create( ECS::World::Instance &world, Entity &entity, ECS::DescriptorSpan remove )
{
  DestroyInfo destroy_info;
  auto components = world.GetComponents( entity, remove );
  for ( const auto &[i, desc] : Enumerate( remove ) )
  {
    destroy_info.m_descriptors.push_back( desc );
    if ( desc->m_type == ECS::Component::Type::ID::Data )
      if ( desc->m_copy )
      {
        destroy_info.m_components_cache.push_back( MakeUnique<byte[]>( desc->m_size ) );
        desc->m_copy( destroy_info.m_components_cache.back().get(), components[i] );
      }
      else
      {
        destroy_info.m_components_cache.push_back( MakeUniqueOverwrite<byte[]>( desc->m_size ) );
        std::memcpy( destroy_info.m_components_cache.back().get(), components[i], desc->m_size );
      }
  }
  auto command = new RemoveComponentsCommand( world, entity, std::move( destroy_info ) );
  command->Execute();
  return command;
}

void RemoveComponentsCommand::Execute()
{
  m_entity = m_world.AddOrRemoveComponents( m_entity, {}, m_destroy_info.m_descriptors );
  m_world.m_archetype_manager.UpdateStructuralChanges();
}

void RemoveComponentsCommand::Undo()
{
  m_entity = m_world.AddOrRemoveComponents( m_entity, m_destroy_info.m_descriptors );
  m_world.m_archetype_manager.UpdateStructuralChanges();
  auto components = m_world.GetComponents( m_entity, m_destroy_info.m_descriptors );
  for ( const auto &[i, desc] : Enumerate( m_destroy_info.m_descriptors ) )
    if ( desc->m_type == ECS::Component::Type::ID::Data )
    {
      if ( desc->m_copy )
        desc->m_copy( components[i], m_destroy_info.m_components_cache[i].get() );
      else
        std::memcpy( components[i], m_destroy_info.m_components_cache[i].get(), desc->m_size );
    }
}
}
#endif