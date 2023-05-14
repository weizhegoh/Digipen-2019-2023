/*****************************************************************
*\file         PrefabPanel.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "PrefabPanel.h"
#include "Systems/Audio/Audio.h"
#include "Xivi/GUI/GUIManager.h"
#include "Editor/ImGui/Helper/DrawComponent.h"
#include "Events/GraphicsEvents.h"
#include "Events/EngineEvents.h"
#include "Xivi/Core/Utils/Filesystem/FileDialog/FileDialog.h"

#include "Editor/System.h"
#include "Editor/Command/Actions/DeleteEntityCommand.h"
#include "Editor/Panels/WorldPanel.h"

#include <imgui_internal.h>
#include <IconsFontAwesome5.h>
namespace Xivi::Editor
{
PrefabPanel::PrefabPanel( std::string name,
                          Ptr<ECS::World::Instance> world,
                          Ptr<Config> configs,
                          Ptr<EntityContext> entity_context,
                          Ptr<FileContext> file_context,
                          Ptr<ToolContext> tool_context ) :
  IPanel( name, world, configs, entity_context, file_context, tool_context )
{
  m_audio_manager = &Audio::AudioManager::Instance();
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();
  m_editor_sys = &System::Instance();

  m_render_prefab_components = [&]( Entity entity ) noexcept
  {
    ImGuiHelper::Indent( 1.0f, [&]
    {
      auto &child_entry = m_world->m_entity_manager.Get( entity );
      std::string child_name = child_entry.m_name;
      ImGui::Text( child_name.c_str() );
      if ( m_world->HaveComponent<Child>( entity ) )
      {
        auto &childs = m_world->GetComponent<Child>( entity );

        for ( auto &child : childs.m_entities )
          m_render_prefab_components( child );
      }
    } );
  };
}

PrefabPanel::~PrefabPanel()
{
  m_enabled = false;
}

void PrefabPanel::Render() noexcept
{
  if ( !m_enabled )
    return;

  ImGuiHelper::CreateImGuiWindow( m_name.c_str(), &m_enabled, [&]
  {
    if ( ImGui::IsWindowHovered( ImGuiHoveredFlags_ChildWindows ) && ImGui::IsMouseClicked( 1 ) )
      ImGui::SetWindowFocus();

    ImGuiHelper::ChildWindow( "Prefab Details", { 0, 0 }, false, ImGuiWindowFlags_AlwaysAutoResize, [&]
    {
      ImGuiHelper::Group( [&]
      {
        ImGui::Text( "" );
        ImGui::Text( " " );
        ImGui::SameLine();
        if ( ImGui::Button( ICON_FA_PLUS, { 30.0f, 30.0f } ) )
          AddPrefabFromFile();
        ImGui::SameLine();
        ImGui::Text( "Add Prefab" );
        ImGui::Text( "" );
      } );

      static ImGuiTextFilter prefab_filter;
      ImGui::Text( "Search: " );
      ImGui::SameLine();
      prefab_filter.Draw( "##PrefabFiltering", 200 );
      ImGui::Text( "" );
      for ( auto &prefab : m_entity_context->prefab_entity )
      {
        auto &entry = m_world->m_entity_manager.Get( prefab );
        auto has_parent = m_world->HaveComponent<Parent>( prefab );
        if ( has_parent )
          continue;

        if ( prefab_filter.PassFilter( entry.m_name.c_str() ) )
        {
          ImGui::PushID( entry.m_name.c_str() );
          RenderPrefab( prefab, entry.m_name );
          ImGui::PopID();
        }
      }

      if ( m_open_confirmation )
      {
        ImGui::OpenPopup( "Confirm Deleting Prefab" );
        m_open_confirmation = false;
      }

      ModalPopup();

      ImGuiHelper::PopupContextWindow( "prefab context menu", ImGuiPopupFlags_MouseButtonRight, false, [&]
      {
        if ( ImGui::Selectable( "Load prefab from file" ) )
          AddPrefabFromFile();
      } );
    } );

    ImGuiHelper::DragDropTarget( [&]
    {
      if ( const ImGuiPayload *payload = ImGui::AcceptDragDropPayload( "CreateFromAssetToPrefab" ); payload )
      {
        std::string payload_n = *static_cast<std::string *>( payload->Data );
        XV_CORE_INFO( "{} is dropped into prefab panel", payload_n );
        if ( payload_n.substr( payload_n.find_last_of( '.' ) ) == ".prefab" )
        {
          bool exist = false;
          for ( auto &[prefab, pathname] : m_file_context->prefab_path )
          {
            if ( pathname == payload_n )
            {
              exist = true;
              m_prefab_to_del = prefab;
              break;
            }
          }
          if ( !exist )
          {
            CreatePrefab( payload_n );
          }
          else
          {
            m_override_prefab = true;
            prefab_path = payload_n/*payload_n.substr(0, payload_n.find_first_of('.'))*/;
          }
        }
      }
    } );
    if ( m_override_prefab )
    {
      ImGui::OpenPopup( "Confirm Override" );
      m_override_prefab = false;
    }
    ModalPopup();
  } );
}

void PrefabPanel::AddPrefabFromFile() noexcept
{
  std::optional<std::string> path = FileDialogs::OpenFile( "Prefab (*.prefab)\0*.prefab\0" );
  if ( path )
  {
    auto is_dup = [&]() noexcept -> bool
    {
      for ( auto &[ent, path_str] : m_file_context->prefab_path )
        if ( path_str == path.value() )
          return true;
      return false;
    };
    if ( !is_dup() )
    {
      Entity result;
      m_world->SendGlobalEvent<DeserialisePrefab>( result, path.value() );
      m_world->SendGlobalEvent<RelinkComponentEvent>();
      auto &entry = m_world->m_entity_manager.Get( result );
      m_entity_context->ClearSelections();
      m_audio_manager->StopAllAudio();
      m_entity_context->selections.push_back( result );
      m_entity_context->selected_entity_name = entry.m_name;
      m_file_context->prefab_path.push_back( { result, path.value() } );
      Draw::OnClick( result, *m_entity_context );
    }
  }
}

void PrefabPanel::RenderPrefab( Entity prefab, const std::string prefab_name ) noexcept
{
  ImGuiHelper::Group( [&]
  {
    bool selected = m_entity_context->EntityIsSelected( prefab );
    if ( !selected && !m_entity_context->selections.empty() )
    {
      for ( auto &selected_entity : m_entity_context->selections )
      {
        Entity ent = selected_entity;
        while ( m_world->HaveComponent<Parent>( ent ) )
        {
          ent = m_world->GetComponent<Parent>( ent ).m_entity;
          if ( prefab == ent )
          {
            selected = true;
            break;
          }
        }
        if ( selected )
          break;
      }
    }
    if ( selected )
      ImGui::GetFocusID() == ImHashStr( "Asset" ) ?
      ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.50f, 0.50f, 0.50f, 0.85f ) ) :
      ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 1.00f, 0.55f, 0.33f, 0.90f ) );
    if ( prefab.Valid() && ImGui::Button( ICON_FA_STICKY_NOTE, { 60.0f, 60.0f } ) )
    {
        /*if (Input::Manager::Instance().IsPressed(Input::Key::LeftControl))
        {
            if (entity_context.EntityIsSelected(prefab))
                entity_context.RemoveFromSelectedEntity(prefab);
            else if(!entity_context.selections.empty())
            {
                auto& arch = GetArchetype(entity_context.selections[0]);
                if(arch.GetSignature()[ECS::Component::info<Prefab>.m_bit_index])
                    entity_context.selections.push_back(prefab);
                else
                {
                    entity_context.ClearSelections();
                    entity_context.selections.push_back(prefab);
                    entity_context.selected_entity_name = prefab_name;
                }
            }
        }
        else
        {*/
      m_entity_context->ClearSelections();
      m_audio_manager->StopAllAudio();
      m_entity_context->selections.push_back( prefab );
      m_entity_context->selected_entity_name = prefab_name;
      Draw::OnClick( prefab, *m_entity_context );
  //}
    }
    if ( selected )
      ImGui::PopStyleColor();
    ImGui::SameLine();

    ImGui::Text( prefab_name.c_str() );
  } );

// Drag and drop
  ImGuiDragDropFlags src_flags = ImGuiDragDropFlags_SourceNoDisableHover |
    ImGuiDragDropFlags_SourceAllowNullID;

  ImGuiHelper::DragDropSource( src_flags, [&]
  {
    ImGui::SetDragDropPayload( "CreateFromPrefab", &prefab, sizeof( Entity ) );
    ImGui::Text( prefab_name.c_str() );
  } );

  ImGuiHelper::PopupContextItem( "delete prefab", ImGuiPopupFlags_MouseButtonRight, [&]
  {
    if ( ImGui::Selectable( "Delete" ) )
    {
      m_prefab_to_del = prefab;
      m_open_confirmation = true;
    }
    else if ( ImGui::Selectable( "Save" ) )
    {
      bool found = false;
      for ( auto &selected_entity : m_entity_context->selections )
        for ( auto &[entity, path_str] : m_file_context->prefab_path )
        {
          if ( entity == selected_entity )
          {
            m_world->SendGlobalEvent<SerialisePrefab>( selected_entity, path_str );
            found = true;
            break;
          }
        }
      if ( !found )
      {
        auto pathname = m_editor_sys->GetPanel<WorldPanel>( "World" )->SaveEntityAsPrefab( m_entity_context->selections[0], false, true );
        m_file_context->prefab_path.push_back( { m_entity_context->selections[0], pathname } );
      }
    }
  } );
  if ( ImGuiHelper::IsItemHovered() )
  {
    ImVec2 m = ImGui::GetIO().MousePos;
    ImGui::SetNextWindowPos( ImVec2( m.x - 10, m.y ) );
    ImGuiHelper::Tooltip( [&]
    {
      ImGui::Text( ( "Name: " + prefab_name ).c_str() );
      ImGui::Spacing();
      ImGui::Text( "Component list:" );
      auto has_child = m_world->HaveComponent<Child>( prefab );
      if ( has_child )
      {
        auto &childs = m_world->GetComponent<Child>( prefab );
        for ( auto &child : childs.m_entities )
          m_render_prefab_components( child );
      }
    } );
  }
}

void PrefabPanel::ModalPopup() noexcept
{
  auto flag = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove;
  ImGuiHelper::PopupModal( "Confirm Deleting Prefab", nullptr, flag, [&]
  {
    auto &prefab_entry = m_world->m_entity_manager.Get( m_prefab_to_del );
    std::string name = prefab_entry.m_name;
    std::string str( "Delete \"" + name + "\" from prefab panel?" );
    f32 size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
    ImGui::SetNextItemWidth( size );
    ImGui::Dummy( { 1.0f,12.0f } );
    ImGui::SetCursorPosX( 15.0f ); // Bound to break!
    ImGui::Text( str.c_str() );
    ImGui::Dummy( { 1.0f,12.0f } );
    if ( ImGui::Button( ICON_FA_CHECK, { size * 0.25f + 20.0f,40.0f } ) )
    {
      RemovePrefab( m_prefab_to_del );
      m_prefab_to_del = Entity {};
      m_entity_context->selected_entity_name.clear();
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
    {
      ImGui::CloseCurrentPopup();
      m_prefab_to_del = Entity {};
    }
  } );
  ImGuiHelper::PopupModal( "Confirm Override", nullptr, flag, [&]
  {
    std::string str( "Prefab \"" + prefab_path + "\" existed in the world, do you want to override?" );
    f32 size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
    ImGui::SetNextItemWidth( size );
    ImGui::Dummy( { 1.0f,12.0f } );
    ImGui::SetCursorPosX( 15.0f ); // Bound to break!
    ImGui::Text( str.c_str() );
    ImGui::Dummy( { 1.0f,12.0f } );
    if ( ImGui::Button( ICON_FA_CHECK, { size * 0.25f + 20.0f,40.0f } ) )
    {
      RemovePrefab( m_prefab_to_del );
      m_prefab_to_del = Entity {};

      CreatePrefab( prefab_path );

      m_entity_context->selected_entity_name.clear();

      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
    {
      ImGui::CloseCurrentPopup();
      m_prefab_to_del = Entity {};
    }
  } );
}

void PrefabPanel::CreatePrefab( const std::string filename ) noexcept
{
  Entity result;
  m_world->SendGlobalEvent<DeserialisePrefab>( result, filename );
  m_world->SendGlobalEvent<RelinkComponentEvent>();
  m_file_context->prefab_path.push_back( { result, filename } );
  auto &entry = m_world->m_entity_manager.Get( result );
  m_entity_context->ClearSelections();
  m_audio_manager->StopAllAudio();
  m_entity_context->selections.push_back( result );
  Draw::OnClick( result, *m_entity_context );
  m_entity_context->selected_entity_name = entry.m_name;
  m_canvas_stack->SetAllFuncByKey();
}

void PrefabPanel::RemovePrefab( Entity entity ) noexcept
{
  XV_ASSERT_MSG( entity.Valid(), "No prefab to delete" );
  m_editor_sys->m_command_history.Commit<DeleteEntityCommand>( *m_world, entity );
  /*for ( auto it = m_file_context->prefab_path.begin(); it != m_file_context->prefab_path.end(); ++it )
  {
    if ( it->first == entity )
    {
      System::Instance().m_command_history.Commit<DeleteEntityCommand>(*m_world, it->first);
      m_file_context->prefab_path.erase( it );
      break;
    }
  }*/
}
}
#endif