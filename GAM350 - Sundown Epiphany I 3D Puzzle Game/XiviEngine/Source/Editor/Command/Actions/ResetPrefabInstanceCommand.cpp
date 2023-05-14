/*****************************************************************
*\file         ResetPrefabInstanceCommand.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "ResetPrefabInstanceCommand.h"
#include "CreatePrefabInstanceCommand.h"
#include "DeleteEntityCommand.h"
#include "Editor/System.h"
#include "Events/EngineEvents.h"
#include "Editor/Systems/Base/EventRefreshPanel.h"
#include "Events/GraphicsEvents.h"

namespace Xivi::Editor
{
ResetPrefabInstanceCommand::ResetPrefabInstanceCommand( ECS::World::Instance &world,
                                                        Entity prefab_inst,
                                                        Entity &new_inst )
  : m_world( world ),
  m_old_prefab_inst( prefab_inst ),
  m_new_prefab_inst( new_inst )
{
  m_editor_sys = &System::Instance();
}

Ptr<ICommand> ResetPrefabInstanceCommand::Create( ECS::World::Instance &world,
                                                  Entity prefab_inst,
                                                  Entity &new_inst )
{
  XV_CORE_ASSERT_MSG( world.HaveComponent<ReferencedPrefab>( prefab_inst ), "Entity is not prefab instance" );
  auto command = new ResetPrefabInstanceCommand( world, prefab_inst, new_inst );
  command->Execute();
  return command;
}

void ResetPrefabInstanceCommand::Execute()
{
  auto name = m_world.m_entity_manager.Get( m_old_prefab_inst ).m_name;
  auto pos = m_world.GetComponent<Position>( m_old_prefab_inst ).m_position;
  auto prefab = m_world.GetComponent<ReferencedPrefab>( m_old_prefab_inst ).m_entity;
  m_editor_sys->m_command_history.Commit<DeleteEntityCommand>( m_world, m_old_prefab_inst );
  m_editor_sys->m_command_history.Commit<CreatePrefabInstanceCommand>( m_world,
                                                                       prefab,
                                                                       Position { pos },
                                                                       m_new_prefab_inst );
  EventRefreshPanelSystem::OnCall( m_world );
  m_world.SendGlobalEvent<RelinkComponentEvent>();
  m_world.m_entity_manager.Get( m_new_prefab_inst ).m_name = name;
}

void ResetPrefabInstanceCommand::Undo()
{
  Editor::System::Instance().m_command_history.Undo();
  Editor::System::Instance().m_command_history.Undo();
  EventRefreshPanelSystem::OnCall( m_world );
  m_world.SendGlobalEvent<RelinkComponentEvent>();
}
}
#endif