/*****************************************************************
*\file         WorldPanel.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "WorldPanel.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "Events/EngineEvents.h"
#include "Xivi/Core/Utils/Filesystem/FileDialog/FileDialog.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"

#include "Editor/System.h"
#include "Editor/Command/Actions/DuplicateEntityCommand.h"
#include "Editor/Command/Actions/UnlinkParentCommand.h"
#include "Editor/Command/Actions/DeleteEntityCommand.h"
#include "Editor/Systems/Base/EventRefreshPanel.h"

#include "Editor/Panels/PrefabPanel.h"

namespace Xivi::Editor
{
using CanvasComponents = CList<Position, Transform, StaticMeshComponent, Colour, AABB, RenderSettings, DebugDrawComponent, UI, Material, GUI::CanvasTag, GUI::GUIActiveTag>;
using GUIWindowComponents = CList<Position, Transform, StaticMeshComponent, Colour, AABB, RenderSettings, DebugDrawComponent, UI, GUI::WindowTag>;
using GUIElementComponents = CList<Position, Transform, StaticMeshComponent, Colour, AABB, RenderSettings, DebugDrawComponent, UI, Material, GUI::GUIActiveTag>;

WorldPanel::WorldPanel( std::string name,
                        Ptr<ECS::World::Instance> world,
                        Ptr<Config> configs,
                        Ptr<EntityContext> entity_context,
                        Ptr<FileContext> file_context,
                        Ptr<ToolContext> tool_context ) :
  IPanel( name, world, configs, entity_context, file_context, tool_context )
{
  m_recursive_search = [&]( Entity entity, const ImGuiTextFilter &world_filter ) noexcept -> bool
  {
    auto &entry = m_world->m_entity_manager.Get( entity );
    if ( world_filter.PassFilter( entry.m_name.c_str() ) )
      return true;
    if ( m_world->HaveComponent<Child>( entity ) )
    {
      auto &childs = m_world->GetComponent<Child>( entity );
      for ( auto &child : childs.m_entities )
        return m_recursive_search( child, world_filter );
    }
    return false;
  };

  m_recur_get_window = [&]( Entity ent, Ptr<ECS::World::Instance> world ) noexcept -> Entity
  {
    if ( !world->HaveComponent<Parent>( ent ) && world->HaveComponent<GUI::WindowTag>( ent ) )
      return ent;
    if ( !world->HaveComponent<Parent>( ent ) )
      return Entity {};
    return m_recur_get_window( world->GetComponent<Parent>( ent ).m_entity, world );
  };

  m_recur_push_gui = [&]( Entity ent ) noexcept -> void
  {
    Service::Locator::Get<GUI::Manager>().Push( ent );
    if ( m_world->HaveComponent<Child>( ent ) )
    {
      for ( auto &child : m_world->GetComponent<Child>( ent ).m_entities )
        m_recur_push_gui( child );
    }
  };

  m_recur_check_file_exist = [&]( const Utils::Directory &dir, const std::string path ) noexcept -> bool
  {
    // Check files of current directory and return true if file found
    for ( const auto &ref : dir.file_vec )
    {
      if ( ref.extension() != ".prefab" )
        continue;
      std::string str = ref.generic_string();
      std::replace( str.begin(), str.end(), '/', '\\' );
      if ( str == path )
        return true;
    }
    // Else go through every directory and return true if file found
    for ( const auto &d : dir.sub_dir )
      if ( m_recur_check_file_exist( d, path ) )
        return true;

    // Else file not found
    return false;
  };

  m_resource_manager = &Service::Locator::Get<RM>();
  m_frame_buffer = &Service::Locator::Get<Graphics::M_FrameBuffer>();
  m_audio_manager = &Audio::AudioManager::Instance();
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();
  m_editor_sys = &System::Instance();

  m_prefab_instance_sig.set( ECS::Component::info<ReferencedPrefab>.m_bit_index );
}

WorldPanel::~WorldPanel()
{
  m_enabled = false;
}

void WorldPanel::Render() noexcept
{
  if ( !m_enabled )
    return;

  ImGuiHelper::CreateImGuiWindow( m_name.c_str(), &m_enabled, [&]
  {
    if ( ImGui::IsWindowHovered( ImGuiHoveredFlags_ChildWindows ) && ImGui::IsMouseClicked( 1 ) )
      ImGui::SetWindowFocus();

    if ( ImGui::IsWindowFocused( ImGuiFocusedFlags_RootAndChildWindows ) )
      ImGui::SetFocusID( ImHashStr( m_name.c_str() ), ImGui::GetCurrentWindow() );
    /*
    Keyboard Input Here!
    */
    // Check if entity is valid

    /*if ( !m_entity_context->selections.empty() )
    {
      if ( Service::Locator::Get<Input::Manager>().Keyboard()->Delete().Triggered() && !m_entity_context->selections.empty() )
      {
        auto &arch = m_world->GetArchetype( m_entity_context->selections[0] );
        if ( arch.GetSignature()[ECS::Component::info<ReferencedPrefab>.m_bit_index] ||
             arch.GetSignature()[ECS::Component::info<Prefab>.m_bit_index] )
          m_delete_prefab_inst = true;
        else
          m_delete_entity = true;
      }
    }*/
    if ( ImGui::CollapsingHeader( "Entities", ImGuiTreeNodeFlags_DefaultOpen ) )
    {
      switch ( m_configs->current_buffer )
      {
        case Editor::Buffer::UIEditor:
        CreateGUIWindow();
        //CreateCanvasEntity();
        [[fallthrough]];
        case Editor::Buffer::LevelEditor:
        RenderEntities();
        break;
        default:
        break;
      }
    }
    if ( m_delete_entity )
    {
      ImGui::OpenPopup( "Confirm Delete Entity" );
      m_delete_entity = false;
    }
    if ( m_delete_prefab_inst )
    {
      ImGui::OpenPopup( "Confirm Delete Prefab/Prefab Instance" );
      m_delete_prefab_inst = false;
    }
    if ( m_override_prefab )
    {
      ImGui::OpenPopup( "Confirm Override Prefab" );
      m_override_prefab = false;
    }
    ModalPopup();
  } );
}

void WorldPanel::CreateEntitesOnPanel( Ptr<ECS::World::Instance> world, ECS::Entity entity, bool is_root ) noexcept
{
  if ( entity.Zombie() )
    return;

  auto &entry = world->m_entity_manager.Get( entity );

  bool has_child = world->HaveComponent<Child>( entity );

  std::string entity_name;
  if ( !is_root )
  {
    static u32 s_child_count = 0;
    if ( entry.m_name.empty() )
    {
      entity_name += "Child Entity " + std::to_string( s_child_count++ );
      entry.m_name = entity_name;
    }
    else
      entity_name = entry.m_name;
  }
  else
    entity_name = entry.m_name;

  ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None;
  if ( m_entity_context->EntityIsSelected( entity ) )
    node_flags |= ImGuiTreeNodeFlags_Selected;
  else /*if ( entity_context.selections.Valid() )*/
  {
    for ( auto &selected_entity : m_entity_context->selections )
    {
      bool found = false;
      auto &arch = world->GetArchetype( selected_entity );
      if ( arch.GetSignature()[ECS::Component::info<ReferencedPrefab>.m_bit_index] )
      {
        Entity ent = selected_entity;
        while ( world->HaveComponent<Parent>( ent ) )
        {
          ent = world->GetComponent<Parent>( ent ).m_entity;
          if ( ent == entity )
          {
            node_flags |= ImGuiTreeNodeFlags_Selected;
            found = true;
            break;
          }
        }
      }
      if ( found )
        break;
    }
  }
  if ( has_child )
    node_flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
  else
    node_flags |= ImGuiTreeNodeFlags_Leaf;

  bool open = false, can_drop = true;

  auto &archetype = world->GetArchetype( entity );

  bool is_prefab_instance = ( archetype.GetSignature() & m_prefab_instance_sig ).any();
  bool is_prefab = ( archetype.GetSignature()[ECS::Component::info<Prefab>.m_bit_index] );
  ECS::Signature root_prefab_sig;
  root_prefab_sig.set( ECS::Component::info<Root>.m_bit_index );
  bool is_prefab_root = ( archetype.GetSignature() & root_prefab_sig ).any();

  if ( /*!( is_prefab_instance && !is_prefab_root ) &&*/ ImGui::TreeNodeEx( ( void * )static_cast<intptr_t>( entity.m_global_index ), node_flags, entity_name.c_str() ) )
  {
    open = true;

    if ( ImGui::IsItemClicked() )
    {
      //if ( Input::Manager::Instance().IsPressed( Input::Key::LeftControl ) )
      //{
      //  if ( entity_context.EntityIsSelected( entity ) )
      //    entity_context.RemoveFromSelectedEntity( entity );
      //  else if ( !entity_context.selections.empty() )
      //  {
      //    auto& arch = GetArchetype( entity_context.selections[0] );
      //    if ( !arch.GetSignature()[ECS::Component::info<Prefab>.m_bit_index] )
      //      entity_context.selections.push_back( entity );
      //    else
      //    {
      //      entity_context.ClearSelections();
      //      entity_context.selections.push_back( entity );
      //      entity_context.selected_entity_name = entry.m_name;
      //    }
      //  }
      //  else
      //  {
      //      entity_context.selections.push_back( entity );
      //      entity_context.selected_entity_name = entry.m_name;
      //  }
      //}
      //else if ( Input::Manager::Instance().IsPressed( Input::Key::LeftShift ) )
      //{
      //  if ( !entity_context.selections.empty() )
      //  {
      //    for ( auto it = entity_context.world_entity.begin(); it != entity_context.world_entity.end(); ++it )
      //    {
      //      // From top to bottom
      //      if ( *it == entity_context.selections[0] )
      //      {
      //        entity_context.ClearSelections();
      //        for ( auto it2 = it; it2 != entity_context.world_entity.end(); ++it2 )
      //        {
      //          entity_context.selections.push_back( *it2 );
      //          auto& entry2 = world->m_entity_manager.Get( *it2 );
      //          entity_context.selected_entity_name = entry2.m_name;
      //          if ( *it2 == entity )
      //            break;
      //        }
      //        break;
      //      }
      //      // From bottom to top
      //      else if ( *it == entity )
      //      {
      //        auto first = entity_context.selections[0];
      //        entity_context.ClearSelections();
      //        entity_context.selections.push_back( first );
      //        for ( auto it2 = it; it2 != entity_context.world_entity.end(); ++it2 )
      //        {
      //          if ( *it2 == first )
      //            break;
      //          entity_context.selections.push_back( *it2 );
      //          auto& entry2 = world->m_entity_manager.Get( *it2 );
      //          entity_context.selected_entity_name = entry2.m_name;
      //        }
      //        break;
      //      }
      //    }
      //  }
      //  else
      //  {
      //    entity_context.selections.push_back( entity );
      //    entity_context.selected_entity_name = entry.m_name;
      //  }
      //}
      //else
      //{
        //if (!graphicManager.picked)
        //{
      if ( ImGuiHelper::s_picker.first )
      {
        ImGuiHelper::s_picker.first = false;
        *ImGuiHelper::s_picker.second = entity;
      }
      else
      {
        m_entity_context->ClearSelections();
        m_audio_manager->StopAllAudio();
        m_entity_context->selections.push_back( entity );
        m_entity_context->selected_entity_name = entry.m_name;
        Draw::OnClick( entity, *m_entity_context );
        //    graphicManager.picked = true;
        //}
        PushCanvas( entity );
      }
//}
    }
    else
    {
      ImGuiHelper::PopupContextItem( "item context menu", ImGuiPopupFlags_MouseButtonRight, [&]
      {
        if ( m_entity_context->selections.empty() )
        {
          m_entity_context->selections.push_back( entity );
          m_entity_context->selected_entity_name = entry.m_name;
          Draw::OnClick( entity, *m_entity_context );
        }
        else
        {
          auto &arch = world->GetArchetype( m_entity_context->selections[0] );
          if ( arch.GetSignature()[ECS::Component::info<Prefab>.m_bit_index] )
          {
            m_entity_context->ClearSelections();
            m_audio_manager->StopAllAudio();
            m_entity_context->selections.push_back( entity );
            m_entity_context->selected_entity_name = entry.m_name;
            Draw::OnClick( entity, *m_entity_context );
          }
        }

        //if ( !is_prefab_instance && !is_prefab )
        //{
        if ( ImGui::Selectable( "Delete" ) )
          m_delete_entity = true;
        else if ( ImGui::Selectable( "Duplicate" ) )
        {
          for ( auto &selected_entity : m_entity_context->selections )
          {
            ECS::Entity tmp = entity;
            m_editor_sys->m_command_history.Commit<DuplicateEntityCommand>( *m_world, tmp, selected_entity );
            m_entity_context->selected_entity_name =
              world->m_entity_manager.Get( selected_entity ).m_name;
            world->m_archetype_manager.UpdateStructuralChanges();
          }
        }
        else if ( !is_prefab_instance && !is_prefab && m_entity_context->selections.size() == 1 && ImGui::Selectable( "Create as prefab" ) )
          SaveEntityAsPrefab( entity );
        else if ( !is_prefab_instance && !is_prefab && !is_root && m_entity_context->selections.size() == 1 && ImGui::Selectable( "Unlink from parent" ) )
        {
          m_editor_sys->m_command_history.Commit<UnlinkParentCommand>( *m_world, entity );
          EventRefreshPanelSystem::OnCall( *m_world );
        }
        AttachEntityToCanvas( entity );
      } );
      bool skip = false;
      ImGuiHelper::DragDropTarget( [&]
      {
        if ( world->HaveComponent<Parent>( entity ) )
        {
          Parent parent = entry.m_pool->GetComponent<Parent>( entry.m_pool_index );
          for ( auto &selected_entity : m_entity_context->selections )
          {
            if ( parent.m_entity == selected_entity )
              can_drop = false;
            else
            {
              while ( world->HaveComponent<Parent>( parent.m_entity ) )
              {
                auto &entry2 = world->m_entity_manager.Get( parent.m_entity );
                parent = entry2.m_pool->GetComponent<Parent>( entry2.m_pool_index );
                if ( parent.m_entity == selected_entity )
                {
                  can_drop = false;
                  break;
                }
              }
            }
            if ( !can_drop )
              break;
          }
        }
        if ( Entity ent = m_entity_context->LastSelectedEntity(); world->HaveComponent<Parent>( m_entity_context->LastSelectedEntity() ) )
        {
          auto &entry2 = world->m_entity_manager.Get( ent );
          Parent parent = entry2.m_pool->GetComponent<Parent>( entry2.m_pool_index );
          if ( parent.m_entity == entity )
            skip = true;
        }
        if ( const ImGuiPayload *payload = ImGui::AcceptDragDropPayload( "Parenting" ); payload && can_drop && !skip )
        {
          for ( auto &selected_entity : m_entity_context->selections )
          {
            m_editor_sys->m_command_history.Commit<LinkParentCommand>( *m_world, selected_entity, entity );

            if ( world->HaveComponent<GridPosition>( selected_entity ) && world->HaveComponent<GridPosition>( entity ) )
              world->GetComponent<GridPosition>( selected_entity ).value = world->GetComponent<GridPosition>( entity ).value;
          }

          EventRefreshPanelSystem::OnCall( *m_world );
        }
      } );

      ImGuiHelper::DragDropSource( ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceAllowNullID, [&]
      {
        ImGui::SetDragDropPayload( "Attach To Property Inspector", &entity, sizeof( Entity ) );
        ImGui::Text( entry.m_name.c_str() );
      } );
    }
  }

  if ( !can_drop && !ImGui::IsMouseDragging( 0 ) )
  {
    ImGui::OpenPopup( "Parent attach to child/descendant" );
  }
  ModalPopup();

  // Drag and drop
  ImGuiDragDropFlags src_flags = ImGuiDragDropFlags_SourceNoDisableHover |
    ImGuiDragDropFlags_SourceAllowNullID;

  ImGuiHelper::DragDropSource( src_flags, [&]
  {
    ImGui::SetDragDropPayload( "Parenting", &m_entity_context->selected_entity_name, sizeof( std::string ) );
    ImGui::Text( m_entity_context->selected_entity_name.c_str() );
  } );

  /*if(!m_entity_context->selections.empty())
    ImGuiHelper::DragDropSource( src_flags, [&]
    {
      ImGui::SetDragDropPayload( "Attach To Property Inspector", &m_entity_context->selections[0], sizeof( Entity ) );
      ImGui::Text( m_entity_context->selected_entity_name.c_str() );
    } );*/

  if ( open )
  {
    if ( has_child )
    {
      auto &child = entry.m_pool->GetComponent<Child>( entry.m_pool_index );
      RecurChild( world, child );
    }
    ImGui::TreePop();
  }
}

void WorldPanel::ModalPopup() noexcept
{
  auto flag = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove;
  ImGuiHelper::PopupModal( "Confirm Delete Entity", nullptr, flag, [&]
  {
    std::string str;
    f32 size = 0.0f;
    if ( m_entity_context->selections.size() == 1 )
    {
      bool has_child = m_world->HaveComponent<Child>( m_entity_context->selections[0] );
      str = "Delete " + m_entity_context->selected_entity_name + "?";
      size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
      ImGui::SetNextItemWidth( size );
      ImGui::Dummy( { 1.0f,12.0f } );
      ImGui::SetCursorPosX( 15.0f ); // Bound to break!
      ImGui::Text( str.c_str() );
      if ( has_child )
      {
        ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 1.00f, 1.00f, 0.00f, 1.00f ) );
        ImGui::Text( "\nWARNING: This entity has child/children. Deleting the entity will delete the child." );
        ImGui::PopStyleColor();
      }
      ImGui::Dummy( { 1.0f,12.0f } );
    }
    else if ( m_entity_context->selections.size() > 1 )
    {
      str = "Multiple entities selected. Delete all selected entities?";
      size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
      ImGui::SetNextItemWidth( size );
      ImGui::Dummy( { 1.0f,12.0f } );
      ImGui::SetCursorPosX( 15.0f ); // Bound to break!
      ImGui::Text( str.c_str() );
      ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 1.00f, 1.00f, 0.00f, 1.00f ) );
      ImGui::Text( "\nWARNING: If selected entity has child/children, they will be deleted as well." );
      ImGui::PopStyleColor();
      ImGui::Dummy( { 1.0f,12.0f } );
    }
    if ( ImGui::Button( ICON_FA_CHECK, { size * 0.25f + 20.0f,40.0f } ) )
    {
      if ( m_entity_context->selections.size() > 1 )
      {
        std::vector<Entity> to_del;
        for ( auto &selected_entity : m_entity_context->selections )
        {
          if ( m_world->HaveComponent<Parent>( selected_entity ) &&
               m_entity_context->EntityIsSelected( m_world->GetComponent<Parent>( selected_entity ).m_entity ) )
            continue;
          to_del.push_back( selected_entity );
        }

        for ( auto ent_to_del : to_del )
          m_editor_sys->m_command_history.Commit<DeleteEntityCommand>( *m_world, ent_to_del );
      }
      else
        m_editor_sys->m_command_history.Commit<DeleteEntityCommand>( *m_world, m_entity_context->selections[0] );

      m_entity_context->ClearSelections();
      m_audio_manager->StopAllAudio();

      if ( !m_canvas_stack->Empty() )
        m_canvas_stack->PopAll();

      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f, 40.0f } ) )
      ImGui::CloseCurrentPopup();
  } );

  ImGuiHelper::PopupModal( "Confirm Delete Prefab/Prefab Instance", nullptr, flag, [&]
  {
    std::string str;
    f32 size = 0.0f;
    if ( m_entity_context->selections.size() == 1 )
      str = "Delete " + m_entity_context->selected_entity_name + "?";
    else if ( m_entity_context->selections.size() > 1 )
      str = "Multiple prefab selected. Delete all selected prefab?";
    size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
    ImGui::SetNextItemWidth( size );
    ImGui::Dummy( { 1.0f, 12.0f } );
    ImGui::SetCursorPosX( 15.0f ); // Bound to break!
    ImGui::Text( str.c_str() );
    ImGui::Dummy( { 1.0f, 12.0f } );
    if ( ImGui::Button( ICON_FA_CHECK, { size * 0.25f + 20.0f,40.0f } ) )
    {
      for ( auto &selected_entity : m_entity_context->selections )
      {
        Entity ent = selected_entity;
        auto &arch = m_world->GetArchetype( selected_entity );
        while ( m_world->HaveComponent<Parent>( ent ) )
          ent = m_world->GetComponent<Parent>( ent ).m_entity;
        if ( arch.GetSignature()[ECS::Component::info<Prefab>.m_bit_index] )
          for ( auto it = m_file_context->prefab_path.begin(); it != m_file_context->prefab_path.end(); ++it )
          {
            auto &[entity, path] = *it;
            if ( entity == ent )
            {
              m_file_context->prefab_path.erase( it );
              break;
            }
          }
        m_editor_sys->m_command_history.Commit<DeleteEntityCommand>( *m_world, ent );
      }

      m_entity_context->ClearSelections();
      m_audio_manager->StopAllAudio();

      if ( !m_canvas_stack->Empty() )
        m_canvas_stack->PopAll();

      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
      ImGui::CloseCurrentPopup();
  } );

  ImGuiHelper::PopupModal( "Confirm Override Prefab", nullptr, flag, [&]
  {
    std::string str( "Prefab \"" + m_prefab_path + "\" existed, do you want to override?" );
    f32 size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
    ImGui::SetNextItemWidth( size );
    ImGui::Dummy( { 1.0f,12.0f } );
    ImGui::SetCursorPosX( 15.0f ); // Bound to break!
    ImGui::Text( str.c_str() );
    ImGui::Dummy( { 1.0f,12.0f } );
    if ( ImGui::Button( ICON_FA_CHECK, { size * 0.25f + 20.0f,40.0f } ) )
    {
      Entity tmp = m_file_context->GetPrefab( m_prefab_path );
      if ( tmp.Valid() && !m_prefab_exist )
      {
        m_entity_context->RemoveFromSelectedEntity( tmp );
        m_editor_sys->GetPanel<PrefabPanel>( "Prefab" )->RemovePrefab( tmp );
        //m_editor_sys->m_command_history.Commit<DeleteEntityCommand>( *m_world, tmp );
      }
      m_file_context->RemovePrefab( m_prefab_path );
      SaveEntityAsPrefab( m_prefab_entity, true, m_prefab_exist );
      m_prefab_path.clear();
      m_prefab_exist = false;
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
      ImGui::CloseCurrentPopup();
  } );
  ImGuiHelper::PopupModal( "Parent attach to child/descendant", nullptr, flag, [&]
  {
    std::string str( "ERROR: Attaching parent to child/descendant" );
    f32 size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
    ImGui::SetNextItemWidth( size );
    ImGui::Dummy( { 1.0f,12.0f } );
    ImGui::SetCursorPosX( 10.0f ); // Bound to break!
    ImGui::Text( str.c_str() );
    ImGui::Dummy( { 1.0f,12.0f } );
    ImGui::SameLine();
    if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
      ImGui::CloseCurrentPopup();
  } );
}

void WorldPanel::RecurChild( Ptr<ECS::World::Instance> world, Child &child ) noexcept
{
  ImGuiHelper::Indent( 1.0f, [&]
  {
    for ( auto &c : child.m_entities )
      CreateEntitesOnPanel( world, c );
  } );
}

void WorldPanel::GeneratePrefabName( Entity entity, Entity prefab ) noexcept
{
  auto &entry = m_world->m_entity_manager.Get( entity );
  auto &prefab_entry = m_world->m_entity_manager.Get( prefab );
  prefab_entry.m_name = entry.m_name;
  if ( m_world->HaveComponent<Child>( entity ) )
  {
    auto &childs = m_world->GetComponent<Child>( entity );
    auto &prefab_childs = m_world->GetComponent<Child>( prefab );
    for ( auto i = 0; i < childs.m_entities.size(); ++i )
      GeneratePrefabName( childs.m_entities[i], prefab_childs.m_entities[i] );
  }
}

void WorldPanel::RenderEntities() noexcept
{
  auto count = 0u;
  auto entity_count = m_world->m_entity_manager.m_count;
  ImGuiHelper::ChildWindow( "World Details", { 0, 0 }, false, ImGuiWindowFlags_AlwaysAutoResize, [&]
  {
    static ImGuiTextFilter world_filter;
    ImGuiHelper::Indent( 1.0f, [&]
    {
      ImGui::Text( "Search: " );
      ImGui::SameLine();
      world_filter.Draw( "##World Filter" );
    } );

    auto &entites = m_configs->current_buffer == Editor::Buffer::LevelEditor ?
      m_entity_context->world_entity :
      m_entity_context->ui_entity;

    for ( auto &entity : entites )
    {
      auto &entry = m_world->m_entity_manager.m_entities[entity.m_global_index];
      if ( entry.m_validation.m_zombie || m_world->HaveComponent<Parent>( entity ) )
        continue;
      CreateEntitesOnPanel( m_world, entity, true );

      if ( ++count == entity_count )
        break;
    }
  } );
}

// --------------------------------------- Prefab --------------------------------------- //
bool WorldPanel::DupPrefabPath( Entity &entity, const std::string path ) noexcept
{
  if ( m_file_context->DuplicatePrefabPath( path ) )
  {
    m_override_prefab = true;
    m_prefab_path = path;
    m_prefab_entity = entity;
    return true;
  }
  return false;
}

std::string WorldPanel::SaveEntityAsPrefab( Entity &entity, bool is_popup, bool prefab_existed ) noexcept
{
  // If function is called from popup modal
  if ( is_popup )
  {
    if ( !prefab_existed )
    {
      // only allow creating of prefab for 1 selected entity for now
      m_world->SendGlobalEvent<CreatePrefab>( entity, m_prefab_path );

      std::string str = m_prefab_path;
      str = str.substr( str.find_last_of( '\\' ) );
      str.erase( 0, 1 );
      auto &entry = m_world->m_entity_manager.Get( entity );
      entry.m_name = str;
    }
    m_file_context->prefab_path.push_back( { entity, m_prefab_path } );
    m_world->SendGlobalEvent<SerialisePrefab>( entity, m_prefab_path );

    return m_prefab_path;
  }

  std::optional<std::string> path = FileDialogs::SaveFile( "Prefab (*.prefab)\0*.prefab\0" );
  if ( path )
  {
    auto &dir = Utils::SystemDirectory::Instance().ConstRoot();
    if ( !m_recur_check_file_exist( dir, *path ) )
    {
      if ( !prefab_existed )
      {
        m_world->SendGlobalEvent<CreatePrefab>( entity, *path );
        std::string str = *path;
        str = str.substr( str.find_last_of( '\\' ) );
        str.erase( 0, 1 );
        auto &entry = m_world->m_entity_manager.Get( entity );
        entry.m_name = str;
      }
      m_file_context->prefab_path.push_back( { entity, *path } );

      m_world->SendGlobalEvent<SerialisePrefab>( entity, *path );
    }
    else
    {
      m_override_prefab = true;
      m_prefab_path = *path;
      m_prefab_entity = entity;
      m_prefab_exist = prefab_existed;
    }
    return *path;
  }
  return std::string {};
}

// --------------------------------------- GUI --------------------------------------- //
void WorldPanel::CreateGUIWindow() noexcept
{
  ImGuiHelper::Group( [&]
  {
    ImGui::Text( "" );
    ImGui::Text( " " );
    ImGui::SameLine();
    if ( ImGui::Button( ICON_FA_PLUS, { 30.0f, 30.0f } ) )
    {
      auto &archetype = m_world->GetOrCreateArchetype<GUIWindowComponents>( "Static Renders" );
      auto entity = archetype.CreateEntity( [&]( Position &pos,
                                            Transform &xform,
                                            StaticMeshComponent &mesh,
                                            Colour &colour,
                                            AABB &aabb,
                                            RenderSettings &renderSettings ) noexcept
      {
        pos.m_position = m_frame_buffer->CameraUI().CameraDirection();
        xform.scale = Vec3f { 10.f,10.f,0.f };
        xform.axis = Vec3f { 0.f,90.f,0.f };
        aabb.AABB_mMin = 0;
        aabb.AABB_mMax = 0;
        aabb.type = CollisionType::AABB;
        colour.colour = Vec3f { 1.f,1.f,1.f };
        renderSettings.onLight = true;
        renderSettings.onRender = true;
        mesh.model = m_resource_manager->Get<Graphics::StaticModel>( "Assets/Models/binary/Object/plane.BinaryObj" );
      } );
      auto &entry = m_world->m_entity_manager.Get( entity );
      entry.m_name = "GUI Window";

      m_entity_context->ClearSelections();
      m_audio_manager->StopAllAudio();
      m_entity_context->selections.push_back( entity );
      Draw::OnClick( entity, *m_entity_context );
      PushCanvas( entity );
    }
    ImGui::SameLine();
    ImGui::Text( "Add Window" );
    ImGui::Text( "" );
  } );
}

void WorldPanel::CreateCanvasEntity( Entity gui_window ) noexcept
{
  AddItem<CanvasComponents>( gui_window, "Canvas", true );
}

void WorldPanel::AttachEntityToCanvas( Entity entity ) noexcept
{
  if ( m_world->HaveComponent<GUI::WindowTag>( entity ) && ImGui::Selectable( "Add Canvas" ) )
    CreateCanvasEntity( entity );

  else if ( !m_world->HaveComponent<GUI::CanvasTag>( entity ) )
    return;

  /*if (ImGui::Selectable("Add Text"))
    AddTextEntity(entity);
  else*/ if ( ImGui::Selectable( "Add Image" ) )
    AddImageEntity( entity );
  else if ( ImGui::Selectable( "Add Checkbox" ) )
    AddCheckbox( entity );
  else if ( ImGui::Selectable( "Add Drag" ) )
    AddDrag( entity );
  else if ( ImGui::Selectable( "Add Trigger" ) )
    AddTrigger( entity );
  else if ( ImGui::Selectable( "Add Bar" ) )
    AddBar( entity );
}

void WorldPanel::AddTextEntity( Entity canavas ) noexcept
{
  //AddItem<GUIElementComponents>(canavas, "Text");
}

void WorldPanel::AddImageEntity( Entity canavas ) noexcept
{
  AddItem<GUIElementComponents>( canavas, "Image" );
}

void WorldPanel::AddCheckbox( Entity canavas ) noexcept
{
  AddItem<GUIElementComponents::Extend<CList<GUI::UICheckBox, GUI::UIInteractableTag>>>( canavas, "Checkbox" );
}

void WorldPanel::AddDrag( Entity canavas ) noexcept
{
  AddItem<GUIElementComponents::Extend<CList<GUI::UIDrag, GUI::UIInteractableTag>>>( canavas, "Dragging" );
}

void WorldPanel::AddTrigger( Entity canavas ) noexcept
{
  AddItem<GUIElementComponents::Extend<CList<GUI::UITrigger, GUI::UIHover, GUI::UIInteractableTag>>>( canavas, "Trigger" );
}

void WorldPanel::AddBar( Entity canavas ) noexcept
{
  AddItem<GUIElementComponents::Extend<CList<GUI::UIBar>>>( canavas, "Bar" );
}

void WorldPanel::PushCanvas( Entity entity ) noexcept
{
  if ( !m_canvas_stack->Empty() )
    m_canvas_stack->PopAll();
  Entity window = m_recur_get_window( entity, m_world );
  if ( window.Valid() )
    m_recur_push_gui( window );
}
}
#endif