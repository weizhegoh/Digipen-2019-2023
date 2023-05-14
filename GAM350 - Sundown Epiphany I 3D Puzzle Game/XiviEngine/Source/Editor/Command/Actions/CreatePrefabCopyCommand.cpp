/*****************************************************************
*\file         CreatePrefabCopyCommand.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "CreatePrefabCopyCommand.h"
#include "Events/EngineEvents.h"
#include "Editor/System.h"
#include "Editor/Command/Actions/CreatePrefabInstanceCommand.h"

namespace Xivi::Editor
{
CreatePrefabCopyCommand::CreatePrefabCopyCommand( ECS::World::Instance &world,
                                                  const Entity &create_from,
                                                  const Position &position,
                                                  Entity &return_ref )
  :m_position( position ),
  m_world( world ),
  m_create_from( create_from ),
  m_return_ref( return_ref )
{
  m_prefab_sig.set( ECS::Component::info<PrefabTag>.m_bit_index );
  m_prefab_sig.set( ECS::Component::info<Prefab>.m_bit_index );

  // Get all entries for undoing later
  m_recorder = [&]( Entity entity ) -> void
  {
    m_entries.push_back( &m_world.m_entity_manager.Get( entity ) );

    if ( m_world.HaveComponent<Child>( entity ) )
      m_world.GetEntity( entity, [&]( Child &child )
    {
      for ( auto &c : child.m_entities )
        m_recorder( c );
    } );
  };

  m_recur_remove_prefab = [&]( Entity entity ) -> void
  {
    m_world.AddOrRemoveComponents<CList<>, CList<Prefab, PrefabTag>>( entity );
    if ( m_world.HaveComponent<Child>( entity ) )
      m_world.GetEntity( entity, [&]( Child &child )
    {
      for ( auto &c : child.m_entities )
        m_recur_remove_prefab( c );
    } );
  };
}

Ptr<ICommand> CreatePrefabCopyCommand::Create( ECS::World::Instance &world,
                                               const Entity &create_from,
                                               const Position &pos,
                                               Entity &created )
{
  auto command = new CreatePrefabCopyCommand( world, create_from, pos, created );
  command->Execute();
  return command;
}

void CreatePrefabCopyCommand::Execute()
{
  m_created = m_create_from;

  m_world.SendGlobalEvent<CloneEntity>( m_created );

  m_recur_remove_prefab( m_created );

  m_world.m_archetype_manager.UpdateStructuralChanges();
  m_return_ref = m_created;

  m_world.GetEntity( m_created, [&]( Position &pos )
  {
    pos = m_position;
  } );

  m_recorder( m_created );
}

void CreatePrefabCopyCommand::Undo()
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