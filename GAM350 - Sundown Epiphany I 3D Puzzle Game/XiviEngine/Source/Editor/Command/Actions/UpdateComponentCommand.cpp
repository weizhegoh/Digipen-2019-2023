/*****************************************************************
*\file         UpdateComponentCommand.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "UpdateComponentCommand.h"
#include "Editor/Events/EditorEvents.h"

#include "Editor/Systems/Base/ComponentUpdateSystem.h"

namespace Xivi::Editor
{
UpdateComponentCommand::UpdateComponentCommand( ECS::World::Instance &world,
                                                Entity entity,
                                                const ECS::Component::Descriptor &descriptor,
                                                UniquePtr<byte[]> &&component_initial_snapshot,
                                                UniquePtr<byte[]> &&component_update_snapshot )
  : m_world( world ),
  m_entity( entity ),
  m_descriptor( descriptor ),
  m_component_initial_snapshot( std::move( component_initial_snapshot ) ),
  m_component_update_snapshot( std::move( component_update_snapshot ) )
{}

UpdateComponentCommand::~UpdateComponentCommand()
{
  if ( m_descriptor.m_destructor )
  {
    m_descriptor.m_destructor( m_component_initial_snapshot.get() );
    m_descriptor.m_destructor( m_component_update_snapshot.get() );
  }
}

Ptr<ICommand> UpdateComponentCommand::Create( ECS::World::Instance &world,
                                              Entity &entity,
                                              const ECS::Component::Descriptor &descriptor,
                                              byte &component_initial_snapshot )
{
  Ptr<byte> snapshot = &component_initial_snapshot;
  UniquePtr<byte[]> component_initial = MakeUnique<byte[]>( descriptor.m_size );
  UniquePtr<byte[]> component_update = MakeUnique<byte[]>( descriptor.m_size );
  auto component = world.GetComponent( entity, descriptor );
  if ( descriptor.m_copy )
  {
    descriptor.m_copy( component_update.get(), component );
    descriptor.m_copy( component_initial.get(), snapshot );
  }
  else
  {
    std::memcpy( component_update.get(), component, descriptor.m_size );
    std::memcpy( component_initial.get(), snapshot, descriptor.m_size );
  }

  // Cannot execute as the update already processed
  auto command = new UpdateComponentCommand { world, entity, descriptor, std::move( component_initial ), std::move( component_update ) };

  // When command is triggered, value is already updated since ImGui properties is binded by reference
  // Just send event to update component changes
  ComponentUpdateSystem::OnCall( world, entity, descriptor );
  return command;
}

void UpdateComponentCommand::Execute()
{
  auto component = m_world.GetComponent( m_entity, m_descriptor );
  if ( m_descriptor.m_copy )
    m_descriptor.m_copy( component, m_component_update_snapshot.get() );
  else
    std::memcpy( component, m_component_update_snapshot.get(), m_descriptor.m_size );
  ComponentUpdateSystem::OnCall( m_world, m_entity, m_descriptor );
  m_world.m_archetype_manager.UpdateStructuralChanges();
}

void UpdateComponentCommand::Undo()
{
  auto component = m_world.GetComponent( m_entity, m_descriptor );
  if ( m_descriptor.m_copy )
    m_descriptor.m_copy( component, m_component_initial_snapshot.get() );
  else
    std::memcpy( component, m_component_initial_snapshot.get(), m_descriptor.m_size );
  ComponentUpdateSystem::OnCall( m_world, m_entity, m_descriptor );
  m_world.m_archetype_manager.UpdateStructuralChanges();
}
}
#endif