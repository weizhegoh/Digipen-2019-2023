/*****************************************************************
*\file         ViewportPanel.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "ViewportPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "Editor/Events/EditorEvents.h"
#include "Events/GraphicsEvents.h"
#include "Events/EngineEvents.h"
#include "Editor/ImGui/Helper/DrawComponent.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Xivi/Platform/Windows/GLFW/GLFW_Window.h"
#include "Systems/Audio/Audio.h"
#include "Xivi/GUI/GUIManager.h"

#include "Editor/System.h"
#include "Editor/Command/Actions/CreateEntityFromArchetypeCommand.h"
#include "Editor/Command/Actions/CreateEntityFromAssetCommand.h"
#include "Editor/Command/Actions/CreatePrefabInstanceCommand.h"
#include "Editor/Command/Actions/CreatePrefabCopyCommand.h"
#include "Editor/Command/Actions/UpdateComponentCommand.h"
#include "Editor/Command/Actions/DragDuplicateEntityCommand.h"
#include "Editor/Command/Actions/DuplicateEntityCommand.h"
#include "Editor/Command/Actions/RemoveComponentsCommand.h"
#include "Editor/Systems/Base/LocalCoordsUpdateSystem.h"

#include "../Sundown/Source/NComponents/Grid/Grid.h"

namespace Xivi::Editor
{
ViewportPanel::ViewportPanel( std::string name,
                              Ptr<ECS::World::Instance> world,
                              Ptr<Config> configs,
                              Ptr<EntityContext> entity_context,
                              Ptr<FileContext> file_context,
                              Ptr<ToolContext> tool_context ) : IPanel( name, world, configs, entity_context, file_context, tool_context )
{
  m_prev_state = m_configs->current_state;
  m_resource_manager = &Service::Locator::Get<RM>();

  m_audio_manager = &Audio::AudioManager::Instance();
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();

  m_keyboard = Service::Locator::Get<Input::Manager>().Keyboard();
  m_mouse = Service::Locator::Get<Input::Manager>().Mouse();
  m_frame_buffer = &Service::Locator::Get<Graphics::M_FrameBuffer>();
  m_editor_sys = &System::Instance();

  m_key_w = m_keyboard->W();
  m_key_e = m_keyboard->E();
  m_key_r = m_keyboard->R();
  m_key_lalt = m_keyboard->LeftAlt();

  m_mouse_lb = m_mouse->LeftButton();
  m_mouse_rb = m_mouse->RightButton();

  m_mouse_pos = m_mouse->Position();

  m_recur_push = [&]( Entity ent ) -> void
  {
    m_canvas_stack->Push( ent );
    if ( m_world->HaveComponent<Child>( ent ) )
    {
      for ( auto &child : m_world->GetComponent<Child>( ent ).m_entities )
        m_recur_push( child );
    }
  };

  m_recur_update_animator = [&]( Entity ent ) -> void
  {
    if ( m_world->HaveComponent<AnimatorComponent>( ent ) )
      m_world->GetComponent<AnimatorComponent>( ent ).UpdateAnimation( 0.f );
  };

  auto mousePosition = m_mouse->Position();

  m_frame_buffer->CameraEditor( 0 ).SetMode( Graphics::Camera3D::Fps );
  m_frame_buffer->CameraOnUpdate(
    static_cast<float>( Service::Locator::Get<GLFW_Window>().Width() ),
    static_cast<float>( Service::Locator::Get<GLFW_Window>().Height() ),
    static_cast<unsigned int>( Input::MouseButton::Left ),
    static_cast<unsigned int>( Input::MouseButton::Right ),
    static_cast<int>( mousePosition.X() ), static_cast<int>( mousePosition.Y() ),
    true, true );
}

ViewportPanel::~ViewportPanel()
{
  m_enabled = false;
}

void GridSnap( ECS::World::Instance &world, const Entity &entity, Vec3f &pos ) noexcept
{
  if ( pos.getX() < -25.0f || pos.getY() < -25.0f || pos.getZ() < -25.0f )
    return;

  if ( !world.HaveComponents<GridPosition, EntityType>( entity ) )
    return;

  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  Vec3i cell;

  auto result_cell = abs( pos + vec3_one ) / grid.cell_size;
  cell.x = Math::clamp( (int) result_cell.getX(), 0, grid.grid_size.x - 1 );
  cell.y = Math::clamp( (int) result_cell.getY(), 0, grid.grid_size.y - 1 );
  cell.z = Math::clamp( (int) result_cell.getZ(), 0, grid.grid_size.z - 1 );

  auto &grid_pos = world.GetComponent<GridPosition>( entity );
 /* if ( grid_pos.value == cell )
    return;*/
  auto &type = world.GetComponent<EntityType>( entity );
  if ( grid.GetCellType( cell ) == GameType::Empty )
  {
    if ( grid_pos.value.x != -1 && grid.GetCellEntity( grid_pos.value ) == entity ) // Null value check
      grid.UpdateCell( grid_pos.value, GameType::Empty );
    grid_pos.value = cell;
    grid.UpdateCell( cell, type.value, entity );
    pos = grid.WorldSpace( cell ) + grid_pos.offset;
  }
  else
    pos = grid.WorldSpace( grid_pos.value ) + grid_pos.offset;
}

void ViewportPanel::Render() noexcept
{
  if ( !m_enabled )
    return;

  ImGuiHelper::CreateImGuiWindow( m_name.c_str(), &m_enabled, [&]
  {
    if ( ImGui::IsWindowHovered() && ImGui::IsMouseClicked( 1 ) )
      ImGui::SetWindowFocus();

    if ( ImGui::IsWindowFocused( ImGuiFocusedFlags_RootAndChildWindows ) )
      ImGui::SetFocusID( ImHashStr( m_name.c_str() ), ImGui::GetCurrentWindow() );

    ImGuiStyle &style = ImGui::GetStyle();
    auto frame_padding = style.FramePadding;
    style.FramePadding = { 8.0f, 8.0f };
    style.FramePadding = frame_padding;

    auto mousePosition = m_mouse->Position();

    if ( ImGui::IsWindowFocused() && m_frame_buffer->isEditor() )
    {
      m_frame_buffer->CameraEditor( 0 ).SetMode( Graphics::Camera3D::Fps );
      m_frame_buffer->CameraOnUpdate(
        static_cast<float>( Service::Locator::Get<GLFW_Window>().Width() ),
        static_cast<float>( Service::Locator::Get<GLFW_Window>().Height() ),
        static_cast<unsigned int>( Input::MouseButton::Left ),
        static_cast<unsigned int>( Input::MouseButton::Right ),
        static_cast<int>( mousePosition.X() ), static_cast<int>( mousePosition.Y() ),
        true, true );
    }

    ImVec2 render_pos = ImGui::GetCursorScreenPos();
    ImVec2 render_size = ImGui::GetContentRegionAvail();
    float windowWidth = static_cast<float>( ImGui::GetWindowWidth() );
    float windowHeight = static_cast<float>( ImGui::GetWindowHeight() );

    /*
    This segment is the code used to draw an image into the imgui window
    */

    glBindFramebuffer( GL_FRAMEBUFFER, 0 ); // back to default
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    ImVec2 viewport_size = ImGui::GetContentRegionAvail();
    uint64_t textureID = 0;

    switch ( m_configs->current_buffer )
    {
      case Editor::Buffer::UIEditor:
      textureID = m_frame_buffer->ImageUI( 0 );
      break;
      case Editor::Buffer::LevelEditor:
      switch ( m_configs->current_state )
      {
        case Editor::State::Stop:
        textureID = m_frame_buffer->ImageEditor( 0 );
        if ( m_prev_state != Editor::State::Stop )
          m_frame_buffer->ToggleList();
        break;
        case Editor::State::RunningPaused:
        case Editor::State::Running:
        textureID = m_frame_buffer->ImageGame( 0 );
        if ( m_prev_state == Editor::State::Stop )
          m_frame_buffer->ToggleList();
        break;
      }
      default:
      break;
    }

    /*if (m_prev_state != m_configs->m_cur_state)
        m_frame_buffer->ToggleList();*/
    m_prev_state = m_configs->current_state;
    XV_ASSERT_MSG( textureID != 0, "No camera image loaded" );
    ImGui::Image( std::bit_cast<void *>( textureID ), viewport_size, ImVec2 { 0, 1 }, ImVec2 { 1, 0 } );

    if ( m_configs->current_state == Editor::State::Running )
      return;

    if ( ImGui::IsWindowFocused() )
    {
      // Changes in the enum structure of ImGuizmo::Operation, thus unable to use m_tooltip enum as variable
      if ( !m_mouse_rb.Pressed() )
      {
        if ( m_key_w.Triggered() )
          m_tool_context->m_gizmo_type = ImGuizmo::OPERATION::TRANSLATE;
        else if ( m_key_e.Triggered() )
          m_tool_context->m_gizmo_type = ImGuizmo::OPERATION::ROTATE;
        else if ( m_key_r.Triggered() )
          m_tool_context->m_gizmo_type = ImGuizmo::OPERATION::SCALE;
      }
    }

    if ( m_mouse_rb.Triggered() && ImGuiHelper::s_picker.first )
      ImGuiHelper::s_picker.first = false;

    float cursorViewPortDistanceX = m_mouse_pos.X() - render_pos.x;
    float cursorViewPortDistanceY = m_mouse_pos.Y() - render_pos.y;
    float mousepoint_x = ( ( cursorViewPortDistanceX / render_size.x ) ) * static_cast<float>( Service::Locator::Get<GLFW_Window>().Width() );
    float mousepoint_y = ( ( cursorViewPortDistanceY / render_size.y ) ) * static_cast<float>( Service::Locator::Get<GLFW_Window>().Height() );

    ImGuiHelper::s_mouse_pos_offsetted = { mousepoint_x, mousepoint_y };

    Entity ent;
    /*if ( entity_context.m_selections.Valid() )
      ent = entity_context.m_selections;*/
    for ( auto i = static_cast<int>( m_entity_context->selections.size() ) - 1; i >= 0; --i )
    {
      ent = m_entity_context->selections[i];
      if ( ent.Valid() )
        break;
    }

    bool can_drag_gizmo = ent.Valid() && m_world->HaveComponents<Position, Transform>( ent ) && !m_world->HaveComponents<Parent, GridPosition>( ent );
    if ( can_drag_gizmo )
      m_world->GetEntity( ent, [&]( Position &pos, Transform &trans )
    {
      auto &archetype = m_world->GetArchetype( ent );

      // Early exist if prefab
      if ( archetype.GetSignature()[ECS::Component::info<Prefab>.m_bit_index] )
        return;
    // Gizmo logic
      ImGuizmo::SetOrthographic( false );
      ImGuizmo::SetDrawlist();
      ImGuizmo::SetRect( ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight );

      // Get camera view matrix
      Math::Matrix4 cameraProjection = m_frame_buffer->Camera( 0 ).ProjMat_3D();
      Math::Matrix4 cameraView = m_frame_buffer->Camera( 0 ).ViewMat_3D();

      // Entity transform
      glm::mat4 tmpMatrix;
      ImGuizmo::RecomposeMatrixFromComponents( std::bit_cast<const float *>( &pos.m_position ),
                                               std::bit_cast<const float *>( &trans.axis ),
                                               std::bit_cast<const float *>( &trans.scale ),
                                               std::bit_cast<float *>( &tmpMatrix ) );

      const float *snap = nullptr;
      Vec3f trans_snap;
      if ( ImGuizmo::IsUsing() )
        switch ( m_tool_context->m_gizmo_type )
        {
          case ImGuizmo::OPERATION::TRANSLATE:
          if ( m_tool_context->m_translation_snap.grid_snap )
          {
            trans_snap.setAll( m_tool_context->m_translation_snap.value );
            snap = trans_snap.get();
          }
          break;
          case ImGuizmo::OPERATION::ROTATE:
          if ( m_tool_context->m_rotation_snap.grid_snap )
            snap = &m_tool_context->m_rotation_snap.value;
          break;
          case ImGuizmo::OPERATION::SCALE:
          if ( m_tool_context->m_scale_snap.grid_snap )
            snap = &m_tool_context->m_scale_snap.value;
          break;
        }

      ImGuizmo::Manipulate( std::bit_cast<float *>( &cameraView ),
                            std::bit_cast<float *>( &cameraProjection ),
                            (ImGuizmo::OPERATION) m_tool_context->m_gizmo_type,
                            ImGuizmo::LOCAL,
                            std::bit_cast<float *>( &tmpMatrix ),
                            nullptr,
                            snap );

      if ( ImGuizmo::IsUsing() )
      {
        switch ( m_gizmo_state )
        {
          case GizmoState::Default:
          {
            if ( m_key_lalt.Pressed() )
              m_gizmo_state = GizmoState::Duplicating;
            else
            {
              m_gizmo_state = GizmoState::Manipulating;
              switch ( m_tool_context->m_gizmo_type )
              {
                case ImGuizmo::OPERATION::TRANSLATE:
                {
                  m_entity_context->SetComponentSnapshot( *m_world, &ECS::Component::info<Position> );
                  break;
                }
                case ImGuizmo::OPERATION::ROTATE:
                case ImGuizmo::OPERATION::SCALE:
                {
                  m_entity_context->SetComponentSnapshot( *m_world, &ECS::Component::info<Transform> );
                }
              }
            }
            break;
          }
          case GizmoState::Duplicating:
          {
            if ( m_key_lalt.Pressed() && ImGui::IsMouseDragging( 0 ) )
            {
              m_duplicate_ref = ent;
              m_gizmo_state = GizmoState::ManipulatingDuplicated;
              std::vector<Entity> tmp { m_entity_context->selections.size() };
              for ( auto i = 0; i < m_entity_context->selections.size(); ++i )
              {
                auto new_ent = m_entity_context->selections[i];
                m_world->SendGlobalEvent<CloneEntity>( new_ent );

                tmp[i] = new_ent;
              }
              m_entity_context->ClearSelections();
              m_audio_manager->StopAllAudio();
              m_entity_context->selections = tmp;
              Draw::OnClick( tmp[tmp.size() - 1], *m_entity_context );
              // entity_context.m_selections.push_back( ent );
              switch ( m_tool_context->m_gizmo_type )
              {
                case ImGuizmo::OPERATION::TRANSLATE:
                m_entity_context->descriptor = &ECS::Component::info<Position>;
                break;
                case ImGuizmo::OPERATION::ROTATE:
                case ImGuizmo::OPERATION::SCALE:
                m_entity_context->descriptor = &ECS::Component::info<Transform>;
                break;
              }
            }
            else if ( m_key_lalt.Released() )
              m_gizmo_state = GizmoState::Manipulating;
            break;
          }
          case GizmoState::Manipulating:
          {
            if ( ImGui::IsMouseDragging( 0 ) )
              m_gizmo_state = GizmoState::Manipulated;
          }
        }

        float mPos[3], mRotate[3], mScale[3];
        ImGuizmo::DecomposeMatrixToComponents( reinterpret_cast<float *>( &tmpMatrix ), mPos, mRotate, mScale );

        switch ( m_tool_context->m_gizmo_type )
        {
          case ImGuizmo::OPERATION::TRANSLATE:
          {
            auto diff = pos.m_position;
            pos.m_position = Math::Vector3( mPos[0], mPos[1], mPos[2] );
            GridSnap( *m_world, ent, pos.m_position );
            diff = pos.m_position - diff;
            if ( m_world->HaveComponent<Parent>( ent ) )
              LocalCoordsUpdateSystem::OnCall( *m_world, ent );
            for ( auto &selected_entity : m_entity_context->selections )
            {
              if ( selected_entity == ent )
                continue;
              auto &selected_pos = m_world->GetComponent<Position>( selected_entity ).m_position;
              selected_pos += diff;
              if ( m_world->HaveComponent<Parent>( selected_entity ) )
                LocalCoordsUpdateSystem::OnCall( *m_world, selected_entity );
            }
            break;
          }
          case ImGuizmo::OPERATION::ROTATE:
          {
            auto diff = trans.axis;
            trans.axis = Math::Vector3( mRotate[0], mRotate[1], mRotate[2] );
            diff = trans.axis - diff;
            for ( auto &selected_entity : m_entity_context->selections )
            {
              if ( selected_entity == ent )
                continue;
              auto &selected_axis = m_world->GetComponent<Transform>( selected_entity ).axis;
              selected_axis += diff;
            }
            break;
          }
          case ImGuizmo::OPERATION::SCALE:
          {
              // auto diff = trans.scale;
              // trans.scale = Math::Vector3( mScale[0], mScale[1], mScale[2] );
              // diff = trans.scale - diff;
              // for ( auto& selected_entity : entity_context.m_selections )
              // {
              //   if ( selected_entity == ent )
              //     continue;
              //   auto& selected_scale = GetComponent<Transform>( selected_entity ).scale;
              //   selected_scale += diff;
              // }
              // break;
            trans.scale = Math::Vector3( mScale[0], mScale[1], mScale[2] );
            if ( m_world->HaveComponent<Parent>( ent ) )
              LocalCoordsUpdateSystem::OnCall( *m_world, ent );
          }
        }
      }
      else if ( m_mouse_lb.Triggered() && ImGui::IsWindowFocused() )
      {
          //auto &graphicManager = Graphics::Manager::Instance();
          //if ( !graphicManager.picked )
          //{
        float t = -1.f;
        float min_t = 0.f;
        m_world->ForEach( [&]( ECS::Entity &entity, Position &pos, Transform &transform, AABB &aabb )
        {
          if ( m_frame_buffer->isEditor() && m_frame_buffer->GetPickingAABBCheck(
            mousepoint_x,
            mousepoint_y,
            (float) Service::Locator::Get<GLFW_Window>().Width(),
            (float) Service::Locator::Get<GLFW_Window>().Height(),
            aabb, t ) )
          {
            if ( t < min_t )
            {
              if ( ImGuiHelper::s_picker.first )
              {
                ImGuiHelper::s_picker.first = false;
                *ImGuiHelper::s_picker.second = entity;
              }
              else
              {
                bool is_editor = !m_world->HaveComponent<UI>( entity ) && m_configs->current_buffer == Editor::Buffer::LevelEditor;
                bool is_ui = m_world->HaveComponent<UI>( entity ) && m_configs->current_buffer == Editor::Buffer::UIEditor;
                if ( is_editor || is_ui )
                {
                  min_t = t;
                  /*if (Input::Manager::Instance().IsPressed(Input::Key::LeftControl))
                  {
                      if (entity_context.EntityIsSelected(entity))
                          entity_context.RemoveFromSelectedEntity(entity);
                      else if (!entity_context.m_selections.empty())
                      {
                          auto& arch = GetArchetype(entity_context.m_selections[0]);
                          if (!arch.GetSignature()[ECS::Component::info<Prefab>.m_bit_index])
                              entity_context.m_selections.push_back(entity);
                          else
                          {
                              entity_context.m_selections.clear();
                              entity_context.m_selections.push_back(entity);
                              auto& entry = m_world.m_entity_manager.m_entities[entity.Index()];
                              entity_context.m_selected_entity_name = entry.m_name;
                          }
                      }
                      else
                      {
                          entity_context.m_selections.push_back(entity);
                          auto& entry = m_world.m_entity_manager.m_entities[entity.Index()];
                          entity_context.m_selected_entity_name = entry.m_name;
                      }
                  }
                  else
                  {*/
                  m_entity_context->ClearSelections();
                  m_audio_manager->StopAllAudio();
                  m_entity_context->selections.push_back( entity );
                  auto &entry = m_world->m_entity_manager.m_entities[entity.Index()];
                  m_entity_context->selected_entity_name = entry.m_name;
                  Draw::OnClick( entity, *m_entity_context );

                  XV_CORE_INFO( "aabb min: {}, {}, {} ", aabb.AABB_mMin.getX(), aabb.AABB_mMin.getY(), aabb.AABB_mMin.getZ() );
                  XV_CORE_INFO( "aabb max: {}, {}, {} ", aabb.AABB_mMax.getX(), aabb.AABB_mMax.getY(), aabb.AABB_mMax.getZ() );
                  XV_CORE_INFO( "mousepoint: {}, {} ", mousepoint_x, mousepoint_y );
                  //graphicManager.picked = true;
                  //}
                }
              }
            }
          }
          // if (m_frame_buffer->isEditor())
          // {
          //     if (!HaveComponent<UI>(entity))
          //     {
          //         if (m_frame_buffer->GetPickingAABBCheck(
          //             mousepoint_x,
          //             mousepoint_y,
          //             (float)Application::Get().WindowHandle()->Width(),
          //             (float)Application::Get().WindowHandle()->Height(),
          //             aabb, t))
          //         {
          //             if (t < min_t)
          //             {
          //                 min_t = t;
          //                 entity_context.m_selected_entity = entity;
          //             }
          //         }
          //     }
          // }
          // else
          // {
          // }
        } );
      }

      if ( m_mouse_lb.Released() )
      {
        switch ( m_gizmo_state )
        {
          case GizmoState::Manipulated:
          {
            m_editor_sys->m_command_history.Commit<UpdateComponentCommand>( *m_world,
                                                                            ent,
                                                                            *m_entity_context->descriptor,
                                                                            m_entity_context->snapshots[0][0] );
            m_gizmo_state = GizmoState::Default;
            break;
          }
          case GizmoState::ManipulatingDuplicated:
          {
            m_editor_sys->m_command_history.Commit<DragDuplicateEntityCommand>( *m_world,
                                                                                m_duplicate_ref,
                                                                                *m_entity_context->descriptor,
                                                                                *m_world->GetComponent( ent, m_entity_context->descriptor ),
                                                                                m_entity_context->selections[0] );
            [[fallthrough]];
          }
          default:
          m_gizmo_state = GizmoState::Default;
        }
      }
    } );

    static Entity modalpopup_copyinstance;
    static Entity modalpopup_prefab;
    static Vec3f modalpopup_create_pos;
                         // To be reworked in the future for CreateFromAsset (Animation Mesh and Static Mesh)
    ImGuiHelper::DragDropTarget( [&]
    {
      auto create_at = m_frame_buffer->Camera( 0 ).MouseToWorldPosition(
        mousepoint_x,
        mousepoint_y,
        (float) Service::Locator::Get<GLFW_Window>().Width(),
        (float) Service::Locator::Get<GLFW_Window>().Height(),
        200.f );
      if ( const ImGuiPayload *asset_create = ImGui::AcceptDragDropPayload( "CreateFromAsset" ); asset_create )
      {
        m_entity_context->ClearSelections();
        m_audio_manager->StopAllAudio();
        m_entity_context->selections.emplace_back();

        m_editor_sys->m_command_history.Commit<CreateEntityFromAssetCommand>( *m_world,
                                                                              *static_cast<Ptr<AssetCreateFunc>>( asset_create->Data ),
                                                                              Position { create_at },
                                                                              m_file_context->selection_file,
                                                                              m_entity_context->selections[0] );

        m_entity_context->selected_entity_name = m_world->m_entity_manager.Get( m_entity_context->selections[0] ).m_name;

        Draw::OnClick( m_entity_context->selections[0], *m_entity_context );
      }
      else if ( const ImGuiPayload *texture = ImGui::AcceptDragDropPayload( "UpdateTexture" ); texture )
      {
        float t = -1.f;
        float min_t = 0.f;
        bool dragged_to_ent = false;
        // Picking
        m_world->ForEach( [&]( ECS::Entity &entity, Position &pos, Transform &transform, AABB &aabb )
        {
          bool is_editor = !m_world->HaveComponent<UI>( entity ) && m_configs->current_buffer == Editor::Buffer::LevelEditor;
          bool is_ui = m_world->HaveComponent<UI>( entity ) && m_configs->current_buffer == Editor::Buffer::UIEditor;
          if ( !( is_editor || is_ui ) )
            return;

          if ( m_frame_buffer->GetPickingAABBCheck(
            mousepoint_x,
            mousepoint_y,
            (float) Service::Locator::Get<GLFW_Window>().Width(),
            (float) Service::Locator::Get<GLFW_Window>().Height(),
            aabb, t ) )
          {
            if ( t < min_t )
            {
              dragged_to_ent = true;
              min_t = t;
              m_entity_context->selections[0] = entity;
            }
          }
        } );
        if ( dragged_to_ent )
        {
          m_entity_context->SetComponentSnapshot( *m_world, &ECS::Component::info<Material> );
          m_world->GetEntity( m_entity_context->selections[0], [&]( Material &material )
          {
            material.diffuse = m_resource_manager->Get<Graphics::Texture>( *static_cast<Ptr<std::string>>( texture->Data ) );
            material.specular = m_resource_manager->Get<Graphics::Texture>( *static_cast<Ptr<std::string>>( texture->Data ) );
          } );
          m_editor_sys->m_command_history.Commit<UpdateComponentCommand>( *m_world,
                                                                          m_entity_context->selections[0],
                                                                          ECS::Component::info<Material>,
                                                                          m_entity_context->snapshots[0][0] );
        }
      }
      else if ( const ImGuiPayload *archetype_create = ImGui::AcceptDragDropPayload( "CreateFromArchetype" ); archetype_create )
      {
        Entity ent;
        m_editor_sys->m_command_history.Commit<CreateEntityFromArchetypeCommand>( *m_world, *static_cast<Ptr<ArchetypeCreateFunc>>( archetype_create->Data ),
                                                                                  Position { create_at },
                                                                                  ent );

        m_world->SendGlobalEvent<RelinkComponentEvent>();
        m_entity_context->ClearSelections();
        m_audio_manager->StopAllAudio();
        m_entity_context->selections.push_back( ent );
        m_entity_context->selected_entity_name = m_world->m_entity_manager.Get( ent ).m_name;
        Draw::OnClick( ent, *m_entity_context );
      }
      else if ( const ImGuiPayload *payload3 = ImGui::AcceptDragDropPayload( "CreateFromPrefab" ); payload3 )
      {
        modalpopup_prefab = *static_cast<Entity *>( payload3->Data );
        modalpopup_create_pos = create_at;
        /*Entity ent;
        Entity payload_n = *static_cast<Entity*>(payload3->Data);

        m_editor_sys->m_command_history.Commit<CreatePrefabInstanceCommand>(*m_world,
            payload_n,
            Position{ create_at },
            ent);*/
        ImGui::OpenPopup( "Create Prefab Instance/Copy" );
      }
    } );
    if ( modalpopup_prefab.Valid() )
      ModalPopupInstanceCopy( modalpopup_prefab, modalpopup_copyinstance, modalpopup_create_pos );
  } );
}

void ViewportPanel::ModalPopupInstanceCopy( Entity &prefab, Entity &new_ent, const Vec3f &pos ) noexcept
{
  auto flag = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove;
  ImGuiHelper::PopupModal( "Create Prefab Instance/Copy", nullptr, flag, [&]
  {
    static bool instance = true;
    static bool copy = false;
    std::string str( "Create Prefab Instance/Copy?" );
    f32 size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
    ImGui::SetNextItemWidth( size );
    ImGui::Dummy( { 1.0f,12.0f } );
    ImGui::SetCursorPosX( 15.0f ); // Bound to break!
    ImGui::Text( str.c_str() );
    ImGui::Dummy( { 1.0f,12.0f } );
    if ( ImGui::RadioButton( "Instance", instance ) )
    {
      instance = true;
      copy = false;
    }
    if ( ImGui::RadioButton( "Copy", copy ) )
    {
      copy = true;
      instance = false;
    }
    if ( ImGui::Button( ICON_FA_CHECK, { size * 0.25f + 20.0f,40.0f } ) )
    {
      if ( instance )
        m_editor_sys->m_command_history.Commit<CreatePrefabInstanceCommand>( *m_world,
                                                                             prefab,
                                                                             Position { pos },
                                                                             new_ent );
      if ( copy )
        m_editor_sys->m_command_history.Commit<CreatePrefabCopyCommand>( *m_world,
                                                                         prefab,
                                                                         Position { pos },
                                                                         new_ent );

      m_world->SendGlobalEvent<RelinkComponentEvent>();
      // Update in the future when consolidate entity selection
      m_entity_context->ClearSelections();
      m_audio_manager->StopAllAudio();
      m_entity_context->selections.push_back( new_ent );
      m_entity_context->selected_entity_name = m_world->m_entity_manager.Get( new_ent ).m_name;
      Draw::OnClick( new_ent, *m_entity_context );
      if ( m_world->HaveComponent<GUI::CanvasTag>( new_ent ) )
      {
        auto &canvas_stack = Service::Locator::Get<GUI::Manager>();
        if ( !canvas_stack.Empty() )
          canvas_stack.PopAll();
        m_recur_push( new_ent );
      }
      m_recur_update_animator( new_ent );
      prefab = Entity {};
      new_ent = Entity {};
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
      ImGui::CloseCurrentPopup();
  } );
}
}
#endif