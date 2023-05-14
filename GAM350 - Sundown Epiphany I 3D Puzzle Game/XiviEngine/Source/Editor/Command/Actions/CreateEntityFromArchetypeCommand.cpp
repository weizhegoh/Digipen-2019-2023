/*****************************************************************
*\file         CreateEntityFromArchetypeCommand.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "CreateEntityFromArchetypeCommand.h"

namespace Xivi::Editor
{
CreateEntityFromArchetypeCommand::CreateEntityFromArchetypeCommand( ECS::World::Instance &world,
                                                                    const ArchetypeCreateFunc &create_func,
                                                                    const Position &position,
                                                                    Entity &return_ref )
  :m_position( position ),
  m_world( world ),
  m_create_func( create_func ),
  m_return_ref( return_ref )
{}

Ptr<ICommand> CreateEntityFromArchetypeCommand::Create( ECS::World::Instance &world,
                                                        const ArchetypeCreateFunc &create_func,
                                                        const Position &pos,
                                                        Entity &ref_entity )
{
  auto command = new CreateEntityFromArchetypeCommand( world, create_func, pos, ref_entity );
  command->Execute();
  return command;
}

void CreateEntityFromArchetypeCommand::Execute()
{
  m_return_ref = m_created = m_create_func( m_position );
  m_world.m_archetype_manager.UpdateStructuralChanges();
}

void CreateEntityFromArchetypeCommand::Undo()
{
  auto &entry = m_world.m_entity_manager.Get( m_created );
  m_world.DestroyEntity( m_created );
  m_world.m_archetype_manager.UpdateStructuralChanges();
  --entry.m_validation.m_generation;
  m_return_ref = m_created = Entity {};
}
}
#endif