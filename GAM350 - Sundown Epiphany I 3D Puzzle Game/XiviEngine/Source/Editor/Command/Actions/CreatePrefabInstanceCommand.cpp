/*****************************************************************
*\file         CreatePrefabInstanceCommand.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "CreatePrefabInstanceCommand.h"
#include "Events/EngineEvents.h"

namespace Xivi::Editor
{
CreatePrefabInstanceCommand::CreatePrefabInstanceCommand( ECS::World::Instance &world,
                                                          const Entity &create_from,
                                                          const Position &position,
                                                          Entity &return_ref )
  :m_position( position ),
  m_world( world ),
  m_create_from( create_from ),
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

Ptr<ICommand> CreatePrefabInstanceCommand::Create( ECS::World::Instance &world,
                                                   const Entity &create_from,
                                                   const Position &pos,
                                                   Entity &created )
{
  auto command = new CreatePrefabInstanceCommand( world, create_from, pos, created );
  command->Execute();
  return command;
}

void CreatePrefabInstanceCommand::Execute()
{
  // Create the prefab instance and link
  m_created = m_create_from;
  m_world.SendGlobalEvent<CreatePrefabInstanceInternal>( m_created );
  m_world.m_archetype_manager.UpdateStructuralChanges();
  m_return_ref = m_created;

  m_world.GetEntity( m_created, [&]( Position &pos )
  {
    pos = m_position;
  } );

  m_recorder( m_created );
}

void CreatePrefabInstanceCommand::Undo()
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