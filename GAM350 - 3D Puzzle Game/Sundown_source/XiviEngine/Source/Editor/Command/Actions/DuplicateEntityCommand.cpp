/*****************************************************************
*\file         DuplicateEntityCommand.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "DuplicateEntityCommand.h"
#include "Components/EngineComponents.h"
#include "Events/EngineEvents.h"

namespace Xivi::Editor
{
DuplicateEntityCommand::DuplicateEntityCommand( ECS::World::Instance &world,
                                                Entity &create_from,
                                                Entity &return_ref )
  : m_world( world ),
  m_clone_from( create_from ),
  m_return_ref( return_ref )
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
}

Ptr<ICommand> DuplicateEntityCommand::Create( ECS::World::Instance &world,
                                              Entity &clone_from,
                                              Entity &created )
{
  auto command = new DuplicateEntityCommand( world, clone_from, created );
  command->Execute();
  return command;
}

void DuplicateEntityCommand::Execute()
{
  // Create the prefab instance and link
  m_created = m_clone_from;
  m_world.SendGlobalEvent<CloneEntity>( m_created );
  m_world.m_archetype_manager.UpdateStructuralChanges();
  m_return_ref = m_created;
  m_recorder( m_created );
}

void DuplicateEntityCommand::Undo()
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