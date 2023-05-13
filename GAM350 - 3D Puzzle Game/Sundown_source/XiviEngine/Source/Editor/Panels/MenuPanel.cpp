/*****************************************************************
*\file         MenuPanel.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "MenuPanel.h"
#include "Systems/Audio/Audio.h"
#include "Xivi/GUI/GUIManager.h"

#include "Editor/ImGui/Helper/ImGuiHelperFunction.h"
#include <imgui_internal.h>
#include "Xivi/Core/Utils/Filesystem/FileDialog/FileDialog.h"
#include "Editor/ImGui/Helper/DrawComponent.h"

#include "Editor/Events/EditorEvents.h"
#include "Events/EngineEvents.h"
#include "Events/GraphicsEvents.h"
#include "Components/Engine/Prefab/ReferencedPrefab.h"

#include "Editor/System.h"
#include "Editor/Command/Actions/DuplicateEntityCommand.h"
#include "Editor/Systems/Base/EventCreateSkyBox.h"
#include "Editor/Systems/Base/EventRefreshPanel.h"

namespace Xivi::Editor
{
MenuPanel::MenuPanel( std::string name,
                      Ptr<ECS::World::Instance> world,
                      Ptr<Config> configs,
                      Ptr<EntityContext> entity_context,
                      Ptr<FileContext> file_context,
                      Ptr<ToolContext> tool_context,
                      Ptr<std::vector<UniquePtr<IPanel>>> panels ) :
  IPanel( name, world, configs, entity_context, file_context, tool_context ),
  m_panels { panels }
{
  m_audio_manager = &Audio::AudioManager::Instance();
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();
  m_resource_manager = &Service::Locator::Get<RM>();
}

MenuPanel::~MenuPanel()
{
  m_enabled = false;
}

void MenuPanel::Render() noexcept
{
  // Quick hack to put here
  auto &pc = m_world->GetOrCreateSingletonComponent<PlayerController>();

  if ( !m_enabled )
    return;
  if ( ImGui::BeginMainMenuBar() )
  {
    FileShortcut( m_world );
    FileMenu( m_world );
    ImGui::Separator();
    EditMenu( m_world );
    ImGui::Separator();
    WindowMenu( m_world );
    ImGui::EndMainMenuBar();
  }
}

void MenuPanel::SaveLevelAs( Ptr<ECS::World::Instance> world ) noexcept
{
  std::optional<std::string> path = FileDialogs::SaveFile( "XV Scene (*.xv)\0*.xv\0" );
  if ( path )
  {
    project_path = *path;
    world->SerialiseWorld( *path );
  }
}

void MenuPanel::SaveLevel( Ptr<ECS::World::Instance> world ) noexcept
{
  if ( project_path.empty() )
    SaveLevelAs( world );
  else
    world->SerialiseWorld( project_path );
}

void MenuPanel::OpenLevel( Ptr<ECS::World::Instance> world ) noexcept
{
  std::optional<std::string> path = FileDialogs::OpenFile( "XV Scene (*.xv)\0*.xv\0" );
  if ( path )
  {
    project_path = *path;
    world->Clear();
    m_file_context->ClearAll();
    m_entity_context->ClearSelections();
    m_audio_manager->StopAllAudio();
    m_entity_context->copies.clear();
    if ( !m_canvas_stack->Empty() )
      m_canvas_stack->PopAll();
    world->DeserialiseWorld( *path );
    ImGuiHelper::s_picker.first = false;
    m_world->SendGlobalEvent<RelinkComponentEvent>();
    EventRefreshPanelSystem::OnCall( *m_world );
  }
}

void MenuPanel::NewFile( Ptr<ECS::World::Instance> world ) noexcept
{
  project_path.clear();
  world->Clear();
  m_file_context->ClearAll();
  m_entity_context->ClearSelections();
  m_audio_manager->StopAllAudio();
  m_entity_context->copies.clear();
  EventCreateSkyBoxSystem::OnCall( *m_world, m_resource_manager );
  ImGuiHelper::s_picker.first = false;
  m_world->SendGlobalEvent<RelinkComponentEvent>();
  if ( !m_canvas_stack->Empty() )
    m_canvas_stack->PopAll();
  EventRefreshPanelSystem::OnCall( *m_world );
}

void MenuPanel::FileShortcut( Ptr<ECS::World::Instance> world ) noexcept
{
  auto &keyboard = *Service::Locator::Get<Input::Manager>().Keyboard();

  if ( keyboard.LeftControl().Pressed() || keyboard.RightControl().Pressed() )
  {
    if ( keyboard.N().Triggered() )
      NewFile( world );
    else if ( keyboard.O().Triggered() )
      OpenLevel( world );
    else if ( !keyboard.LeftShift().Pressed() && keyboard.S().Triggered() )
      SaveLevel( world );
    else if ( keyboard.LeftShift().Pressed() && keyboard.S().Triggered() )
      SaveLevelAs( world );
  }
}

void MenuPanel::FileMenu( Ptr<ECS::World::Instance> world ) noexcept
{
  ImGuiHelper::Menu( "Files", [&]
  {
    if ( ImGui::MenuItem( "New Level (Ctrl + N)" ) )
    {
      if ( !m_canvas_stack->Empty() )
        m_canvas_stack->PopAll();
      NewFile( world );
    }
    if ( ImGui::MenuItem( "Open Level (Ctrl + O)" ) )
    {
      if ( !m_canvas_stack->Empty() )
        m_canvas_stack->PopAll();
      OpenLevel( world );
      m_canvas_stack->SetAllFuncByKey();
    }
    if ( ImGui::MenuItem( "Save Level (Ctrl + S)" ) )
    {
      SaveLevel( world );
    }
    if ( ImGui::MenuItem( "Save Level As... (Ctrl + LShift + S)" ) )
    {
      SaveLevelAs( world );
    }
  } );
}

void MenuPanel::CopyEntity() noexcept
{
  m_entity_context->copies.clear();
  ImGuiHelper::s_picker.first = false;
  for ( auto &entity : m_entity_context->selections )
    m_entity_context->copies.push_back( entity );
}

void MenuPanel::PasteEntity( Ptr<ECS::World::Instance> world ) noexcept
{
  //if ( editor.m_entity_context.m_copies.Valid() )
  ImGuiHelper::s_picker.first = false;
  if ( m_entity_context->copies.empty() )
    return;
  if ( !m_entity_context->selections.empty() )
  {
    m_entity_context->ClearSelections();
    m_audio_manager->StopAllAudio();
  }
  for ( auto &entity : m_entity_context->copies )
  {
    auto &archetype = m_world->GetArchetype( entity );
    ECS::Signature m_prefab_inst_sig;
    m_prefab_inst_sig.set( ECS::Component::info<ReferencedPrefab>.m_bit_index );
    auto is_prefab_inst = ( archetype.GetSignature() & m_prefab_inst_sig ).any();
    Entity ent;
    Editor::System::Instance().m_command_history.Commit<DuplicateEntityCommand>( *m_world, entity, ent );
    m_entity_context->selections.push_back( ent );
    m_entity_context->selected_entity_name =
      world->m_entity_manager.m_entities[ent.Index()].m_name;

    Draw::OnClick( entity, *m_entity_context );
  }
}

void MenuPanel::DuplicateEntityF( Ptr<ECS::World::Instance> world ) noexcept
{
  std::vector<Entity> tmp;
  ImGuiHelper::s_picker.first = false;
  for ( auto &entity : m_entity_context->selections )
  {
    Entity ent;
    Editor::System::Instance().m_command_history.Commit<DuplicateEntityCommand>( *m_world, entity, ent );
    tmp.push_back( ent );
    m_entity_context->selected_entity_name =
      world->m_entity_manager.m_entities[ent.Index()].m_name;
  }
  m_entity_context->ClearSelections();
  m_audio_manager->StopAllAudio();
  m_entity_context->selections = tmp;
}

void MenuPanel::RedoFn() noexcept
{
  ImGuiHelper::s_picker.first = false;
  m_entity_context->ClearSelections();
  m_audio_manager->StopAllAudio();
  Editor::System::Instance().m_command_history.Redo();
  m_world->SendGlobalEvent<RelinkComponentEvent>();
}

void MenuPanel::EditShortcut( Ptr<ECS::World::Instance> world ) noexcept
{
  auto &keyboard = *Service::Locator::Get<Input::Manager>().Keyboard();

  if ( keyboard.LeftControl().Pressed() )
  {
    if ( keyboard.C().Triggered() )
      CopyEntity();
    else if ( keyboard.V().Triggered() )
      PasteEntity( world );
    else if ( keyboard.Z().Triggered() )
    {
      Editor::System::Instance().m_command_history.Undo();
      ImGuiHelper::s_picker.first = false;
    }
    else if ( keyboard.Y().Triggered() )
      RedoFn();
    else if ( keyboard.D().Triggered() )
      DuplicateEntityF( world );
  }
}

void MenuPanel::EditMenu( Ptr<ECS::World::Instance> world ) noexcept
{
  ImGuiHelper::Menu( "Edit", [&]
  {
    if ( ImGui::MenuItem( "Undo (Ctrl + Z)" ) )
    {
      Editor::System::Instance().m_command_history.Undo();
      ImGuiHelper::s_picker.first = false;
    }
    if ( ImGui::MenuItem( "Redo (Ctrl + Y)" ) )
    {
      RedoFn();
    }
    //if (ImGui::MenuItem("Undo History"))
    //{
    //    // Todo
    //}
    if ( ImGui::MenuItem( "Copy (Ctrl + C)" ) )
    {
      CopyEntity();
    }
    if ( ImGui::MenuItem( "Paste (Ctrl + V)" ) )
    {
      PasteEntity( world );
    }
    if ( ImGui::MenuItem( "Duplicate (Ctrl + D)" ) )
    {
      DuplicateEntityF( world );
    }
  } );
}

void MenuPanel::WindowMenu( Ptr<ECS::World::Instance> world ) noexcept
{
  ImGuiHelper::Menu( "Windows", [&]
  {
    for ( auto &panel : *m_panels )
    {
      std::string str( panel->GetName() );
      if ( str == m_name )
        continue;

      ImGui::PushItemFlag( ImGuiItemFlags_SelectableDontClosePopup, true );
      if ( ImGui::MenuItem( str.c_str(), nullptr, panel->IsEnabled() ) )
        panel->ToggleEnable();
      ImGui::PopItemFlag();
    }
  } );
}
}
#endif