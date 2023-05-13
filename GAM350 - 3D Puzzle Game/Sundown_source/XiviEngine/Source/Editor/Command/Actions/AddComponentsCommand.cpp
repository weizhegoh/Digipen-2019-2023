/*****************************************************************
*\file         AddComponentsCommand.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "AddComponentsCommand.h"
#include "Components/EngineComponents.h"
namespace Xivi::Editor
{
AddComponentsCommand::AddComponentsCommand( ECS::World::Instance &world,
                                            Entity &entity,
                                            ECS::DescriptorSpan add )
  :m_world( world ),
  m_entity( entity )
{
  for ( auto a : add )
    m_add.push_back( a );
}

Ptr<ICommand> AddComponentsCommand::Create( ECS::World::Instance &world, Entity &entity, ECS::DescriptorSpan add )
{
  auto command = new AddComponentsCommand( world, entity, add );
  command->Execute();
  return command;
}

void AddComponentsCommand::Execute()
{
  m_entity = m_world.AddOrRemoveComponents( m_entity, m_add );
  if ( m_world.HaveComponent<Prefab>( m_entity ) )
  {
    auto prefab_instance = m_world.GetComponent<Prefab>( m_entity );
    for ( auto &instance : prefab_instance.referenced_entities )
      instance = m_world.AddOrRemoveComponents( instance, m_add );
  }

  m_world.m_archetype_manager.UpdateStructuralChanges();
}

void AddComponentsCommand::Undo()
{
  m_entity = m_world.AddOrRemoveComponents( m_entity, {}, m_add );
  if ( m_world.HaveComponent<Prefab>( m_entity ) )
  {
    auto prefab_instance = m_world.GetComponent<Prefab>( m_entity );
    for ( auto &instance : prefab_instance.referenced_entities )
      instance = m_world.AddOrRemoveComponents( instance, {}, m_add );
  }
  m_world.m_archetype_manager.UpdateStructuralChanges();
}
}
#endif