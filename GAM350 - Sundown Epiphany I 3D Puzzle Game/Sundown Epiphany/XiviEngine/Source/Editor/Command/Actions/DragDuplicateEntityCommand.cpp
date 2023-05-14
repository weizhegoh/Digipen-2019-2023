/*****************************************************************
*\file         DragDuplicateEntityCommand.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "DragDuplicateEntityCommand.h"
#include "Events/EngineEvents.h"

namespace Xivi::Editor
{
DragDuplicateEntityCommand::DragDuplicateEntityCommand( ECS::World::Instance &world,
                                                        const Entity &clone_from,
                                                        const ECS::Component::Descriptor &descriptor,
                                                        UniquePtr<byte[]> &&data,
                                                        Entity &return_ref )
  : m_world( world ),
  m_clone_from( clone_from ),
  m_descriptor( descriptor ),
  m_data( std::move( data ) ),
  m_return_ref( return_ref ),
  m_created( return_ref )
{
  // Get all entries for undoing later
  m_recorder = [&]( Entity entity )
  {
    m_entries.push_back( &m_world.m_entity_manager.Get( entity ) );

    if ( m_world.HaveComponent<Child>( entity ) )
      m_world.GetEntity( entity, [&]( Child &child )
    {
      for ( auto &c : child.m_entities )
        m_recorder( c );
    } );
  };

  m_recorder( m_created );
}

DragDuplicateEntityCommand::~DragDuplicateEntityCommand()
{
  if ( m_descriptor.m_destructor )
    m_descriptor.m_destructor( m_data.get() );
}

Ptr<ICommand> DragDuplicateEntityCommand::Create( ECS::World::Instance &world,
                                                  const Entity &duplicate_from, const ECS::Component::Descriptor &descriptor, byte &data, Entity &return_ref )
{
  UniquePtr<byte[]> data_buffer = MakeUnique<byte[]>( descriptor.m_size );
  if ( descriptor.m_copy )
    descriptor.m_copy( data_buffer.get(), &data );
  else
    std::memcpy( data_buffer.get(), &data, descriptor.m_size );

  return new DragDuplicateEntityCommand( world, duplicate_from, descriptor, std::move( data_buffer ), return_ref );
}

void DragDuplicateEntityCommand::Execute()
{
  m_created = m_clone_from;
  m_world.SendGlobalEvent<CloneEntity>( m_created );
  m_world.m_archetype_manager.UpdateStructuralChanges();
  auto component = m_world.GetComponent( m_created, m_descriptor );
  if ( m_descriptor.m_copy )
    m_descriptor.m_copy( component, m_data.get() );
  else
    std::memcpy( component, m_data.get(), m_descriptor.m_size );
  m_return_ref = m_created;
  m_recorder( m_created );
}

void DragDuplicateEntityCommand::Undo()
{
  m_world.DestroyEntity( m_created );
  m_world.m_archetype_manager.UpdateStructuralChanges();
  for ( auto &entry : m_entries )
    --entry->m_validation.m_generation;
  m_entries.clear();
  m_return_ref = m_created = Entity();
}
}
#endif