/*****************************************************************
*\file         LockingWindowSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "LockingWindowSystem.h"
#include <imgui_internal.h>
#include "Editor/ImGui/Helper/ImGuiHelperFunction.h"
#include "Editor/Panels/WorldPanel.h"

namespace Xivi::Editor
{
void LockingWindowSystem::EntityShortcut( ECS::World::Instance &world, Editor::System &editor_sys ) noexcept
{
  auto &keyboard = *Service::Locator::Get<Input::Manager>().Keyboard();
  auto menu_sys = editor_sys.GetPanel<MenuPanel>( "Menu" );
  if ( keyboard.LeftControl().Pressed() || keyboard.RightControl().Pressed() )
  {
    if ( keyboard.C().Triggered() )
      menu_sys->CopyEntity();

    if ( keyboard.V().Triggered() )
      menu_sys->PasteEntity( &world );

    if ( keyboard.Z().Triggered() )
    {
      Editor::System::Instance().m_command_history.Undo();
      ImGuiHelper::s_picker.first = false;
    }

    if ( keyboard.Y().Triggered() )
      menu_sys->RedoFn();

    if ( keyboard.D().Triggered() )
      menu_sys->DuplicateEntityF( &world );
  }

  /*if (editor_sys.m_configs.m_selections.Valid() &&
      !editor_sys.m_configs.m_selections.Zombie())
  {
      auto& world_sys = GetSystem<WorldPanelSystem>();
      if (Input::Manager::Instance().IsTriggered(Input::Key::Delete))
      {
          auto& arch = GetArchetype(editor_sys.m_configs.m_selections);
          if (arch.GetSignature()[ECS::Component::info<ReferencedPrefab>.m_bit_index] ||
              arch.GetSignature()[ECS::Component::info<Prefab>.m_bit_index])
          {
              world_sys.m_delete_prefab_inst = true;
          }
          else
          {
              world_sys.m_delete_entity = true;
          }
      }

      world_sys.ModalPopup();
  }*/
}

void DeleteEntityShortcut( ECS::World::Instance &world, Editor::System &editor_sys ) noexcept
{
  if ( !editor_sys.m_entity_context.selections.empty() )
  {
    if ( Service::Locator::Get<Input::Manager>().Keyboard()->Delete().Triggered() && !editor_sys.m_entity_context.selections.empty() )
    {
      auto world_panel = editor_sys.GetPanel<WorldPanel>( "World" );
      auto &arch = world.GetArchetype( editor_sys.m_entity_context.selections[0] );
      if ( arch.GetSignature()[ECS::Component::info<ReferencedPrefab>.m_bit_index] ||
           arch.GetSignature()[ECS::Component::info<Prefab>.m_bit_index] )
        world_panel->m_delete_prefab_inst = true;
      else
        world_panel->m_delete_entity = true;
    }
  }
}

void LockingWindowSystem::Init( ECS::World::Instance &world ) noexcept
{
  auto &editor_sys = Editor::System::Instance();
  // On focus viewport
  m_input_mapping[ImHashStr( "Viewport" )]
    = [&]
  {
    EntityShortcut( world, editor_sys );
    DeleteEntityShortcut( world, editor_sys );
  };

  // On focus world
  m_input_mapping[ImHashStr( "World" )]
    = [&]
  {
    EntityShortcut( world, editor_sys );
    DeleteEntityShortcut( world, editor_sys );
  };

  // On focus propery inspector
  m_input_mapping[ImHashStr( "Property Inspector" )]
    = [&]
  {
    EntityShortcut( world, editor_sys );
  };

  // On focus achetype
  m_input_mapping[ImHashStr( "Archetype" )]
    = [&]
  {
    EntityShortcut( world, editor_sys );
    DeleteEntityShortcut( world, editor_sys );
  };

  // On focus asset
  auto &file_context = editor_sys.m_file_context;
  auto &keyboard = *Service::Locator::Get<Input::Manager>().Keyboard();

  m_input_mapping[ImHashStr( "Asset" )]
    = [&]
  {
    if ( keyboard.LeftControl().Pressed() || keyboard.RightControl().Pressed() )
    {
      if ( keyboard.C().Triggered() )
        CopyAsset( Filesystem::fs::path( file_context.selection_file ), &file_context );

      if ( keyboard.V().Triggered() )
        PasteAsset( &file_context );

      if ( keyboard.X().Triggered() )
        CutAsset( Filesystem::fs::path( file_context.selection_file ), &file_context );
    }

    if ( keyboard.Delete().Triggered() )
      DeleteAsset( Filesystem::fs::path( file_context.selection_file ), &file_context );
  };
}

void LockingWindowSystem::Update() noexcept
{
  auto iter = m_input_mapping.find( ImGui::GetFocusID() );
  if ( iter != m_input_mapping.end() )
    iter->second();
}
}
#endif