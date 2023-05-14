/*****************************************************************
*\file         UnlinkParentCommand.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "UnlinkParentCommand.h"
#include "Events/EngineEvents.h"

namespace Xivi::Editor
{
UnlinkParentCommand::UnlinkParentCommand( ECS::World::Instance &world,
                                          Entity &child,
                                          Entity &parent )
  :m_world( world ),
  m_child( child ),
  m_parent( parent )
{}

Ptr<ICommand> UnlinkParentCommand::Create( ECS::World::Instance &world, Entity &child )
{
  auto command = new UnlinkParentCommand( world, child, world.GetComponent<Parent>( child ).m_entity );
  command->Execute();
  return command;
}

void UnlinkParentCommand::Execute()
{
  m_world.SendGlobalEvent<ParentChildUnlink>( m_child );
  m_world.m_archetype_manager.UpdateStructuralChanges();
}

void UnlinkParentCommand::Undo()
{
  m_world.SendGlobalEvent<ParentChildLink>( m_child, m_parent );
  m_world.m_archetype_manager.UpdateStructuralChanges();
}
}
#endif