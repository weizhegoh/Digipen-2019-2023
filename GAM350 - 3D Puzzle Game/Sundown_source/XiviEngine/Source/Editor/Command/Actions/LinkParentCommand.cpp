/*****************************************************************
*\file         LinkParentCommand.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "LinkParentCommand.h"
#include "Events/EngineEvents.h"

namespace Xivi::Editor
{
LinkParentCommand::LinkParentCommand( ECS::World::Instance &world,
                                      Entity &child,
                                      Entity &parent )
  : m_world( world ),
  m_child( child ),
  m_parent( parent )
{}

Ptr<ICommand> LinkParentCommand::Create( ECS::World::Instance &world, Entity &child, Entity &parent )
{
  auto command = new LinkParentCommand( world, child, parent );
  command->Execute();
  return command;
}

void LinkParentCommand::Execute()
{
  m_world.SendGlobalEvent<ParentChildLink>( m_child, m_parent );
  m_world.m_archetype_manager.UpdateStructuralChanges();
}

void LinkParentCommand::Undo()
{
  //m_world.SendGlobalEvent<ParentChildUnlink>( m_child );
  m_world.m_archetype_manager.UpdateStructuralChanges();
}
}
#endif