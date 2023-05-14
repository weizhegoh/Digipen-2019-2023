/*****************************************************************
*\file         ButtonPanel.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "ButtonPanel.h"

#include "Systems/Audio/Audio.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Xivi/GUI/GUIManager.h"

#include "Events/GraphicsEvents.h"
#include "Editor/Events/EditorEvents.h"
#include "Editor/ImGui/Helper/ImGuiHelperFunction.h"

#include "Editor/Systems/Base/EventRefreshPanel.h"

#include <IconsFontAwesome5.h>

namespace Xivi::Editor
{
ButtonPanel::ButtonPanel( std::string name,
                          Ptr<ECS::World::Instance> world,
                          Ptr<Config> configs,
                          Ptr<EntityContext> entity_context,
                          Ptr<FileContext> file_context,
                          Ptr<ToolContext> tool_context ) :
  IPanel( name, world, configs, entity_context, file_context, tool_context )
{
    // Initialise Button Panel Systems
  m_enabled = true;

  m_audio_manager = &Audio::AudioManager::Instance();
  m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
  m_framebuffer = &Service::Locator::Get<Graphics::M_FrameBuffer>();
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();

  // Initialise Default Variables
  m_tmp_file_path = Filesystem::temp_dir().generic_string() + "tmp.xv";

  // Translate
  m_toolset.push_back( {
    ICON_FA_ARROWS_ALT,
    "Translate",
    7,
    [&]() noexcept
  {
    m_tool_context->m_gizmo_type = ImGuizmo::OPERATION::TRANSLATE;
  } } );

  // Rotate
  m_toolset.push_back( {
    ICON_FA_SYNC,
    "Rotate",
    120,
    [&]() noexcept
  {
    m_tool_context->m_gizmo_type = ImGuizmo::OPERATION::ROTATE;
  } } );

  // Scale
  m_toolset.push_back( {
    ICON_FA_EXPAND_ARROWS_ALT,
    "Scale",
    896,
    [&]() noexcept
  {
    m_tool_context->m_gizmo_type = ImGuizmo::OPERATION::SCALE;
  } } );

  // Play
  m_controls.push_back( {
    ICON_FA_PLAY,
    "Play",
    1u,
    [&]() noexcept
  {
    switch ( m_configs->current_state )
    {
      case Editor::State::Stop:
      {
        m_world->SerialiseWorld( m_tmp_file_path );
        m_canvas_stack->SetAllFuncByKey();
        [[fallthrough]];
      }
      case Editor::State::RunningPaused:
      {
        m_world->Resume();
        m_configs->current_state = Editor::State::Running;
        m_entity_context->ClearSelections();
        ImGuiHelper::s_picker.first = false;
        //AudioEngine::ResumeChannels();
      }
    }
  } } );

  // Pause
  m_controls.push_back( {
    ICON_FA_PAUSE ,
    "Pause",
    2u,
    [&]() noexcept
  {
    if ( m_configs->current_state == Editor::State::Running )
    {
      m_world->Pause();
      m_configs->current_state = Editor::State::RunningPaused;
      //AudioEngine::PauseChannels();
    }
  } } );

  // Stop
  m_controls.push_back( {
    ICON_FA_STOP ,
    "Stop",
    4u,
    [&]() noexcept
  {
    if ( m_configs->current_state != Editor::State::Stop )
    {
        // To be updated
        m_world->Pause();
        /*m_entity_context->m_copies =
          m_entity_context->selections =
          ECS::Entity();*/
        m_entity_context->ClearSelections();
        m_audio_manager->StopAllAudio();
        m_world->Clear();
        m_world->DeserialiseWorld( m_tmp_file_path );
        m_world->SendGlobalEvent<RelinkComponentEvent>();
        EventRefreshPanelSystem::OnCall( *m_world );
        m_configs->current_state = Editor::State::Stop;
        ImGuiHelper::s_picker.first = false;
        //AudioEngine::StopChannels();
      }
    } } );

  m_editor_buffer.push_back( {
    ICON_FA_BACKSPACE,
    "Level Editor",
    1u,
    [&]() noexcept
  {
    Service::Locator::Get<Graphics::M_FrameBuffer>().isUI = false;
    m_configs->current_buffer = Editor::Buffer::LevelEditor;
    m_entity_context->ClearSelections();
    m_audio_manager->StopAllAudio();
  } } );

  m_editor_buffer.push_back( {
    ICON_FA_USER_EDIT,
    "UI Editor",
    2u,
    [&]() noexcept
  {
    Service::Locator::Get<Graphics::M_FrameBuffer>().isUI = true;
    m_configs->current_buffer = Editor::Buffer::UIEditor;
    m_entity_context->ClearSelections();
    m_audio_manager->StopAllAudio();
    ImGuiHelper::s_picker.first = false;
  } } );

  m_graphics_setting.push_back( {
    "Wire Frame",
    m_graphic_manager->wireFrameMode,
    [&]() noexcept
  {
    m_graphic_manager->wireFrameMode = !m_graphic_manager->wireFrameMode;
    return m_graphic_manager->wireFrameMode;
  } } );

  m_graphics_setting.push_back( {
    "Render Debug Drawing",
    m_graphic_manager->debugRender,
    [&]() noexcept
  {
    m_graphic_manager->debugRender = !m_graphic_manager->debugRender;
    return m_graphic_manager->debugRender;
  } } );

  m_graphics_setting.push_back( {
    "Enable Shadow",
    m_graphic_manager->shadowRender,
    [&]() noexcept
  {
    m_graphic_manager->shadowRender = !m_graphic_manager->shadowRender;
    return m_graphic_manager->shadowRender;
  } } );

  m_graphics_setting.push_back( {
    "Enable Animation",
    m_graphic_manager->enableAnimation,
    [&]() noexcept
  {
    m_graphic_manager->enableAnimation = !m_graphic_manager->enableAnimation;
    return m_graphic_manager->enableAnimation;
  } } );

  m_graphics_setting.push_back( {
    "Game FreeCam",
    m_graphic_manager->enableFreeCam,
    [&]() noexcept
  {
    m_graphic_manager->enableFreeCam = !m_graphic_manager->enableFreeCam;
    return m_graphic_manager->enableFreeCam;
  } } );

  m_graphics_setting.push_back( {
  "Enable Gamma",
  m_graphic_manager->gamma,
  [&]() noexcept
{
  m_graphic_manager->gamma = !m_graphic_manager->gamma;
  return m_graphic_manager->gamma;
} } );
}

ButtonPanel::~ButtonPanel()
{
  m_enabled = false;
}

void ButtonPanel::Render() noexcept
{
  // Early exist
  if ( !m_enabled )
    return;

  auto window_flag = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse;
  ImGuiHelper::CreateImGuiWindow( m_name.c_str(), &m_enabled, window_flag, [&]
  {
    static const int column = 3;
    ImGuiHelper::Table( "##Button_Panel", column, ImGuiTableFlags_None, [&]
    {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      static const int first_column = 2;
      ImGuiHelper::Table( "##playstop_uieditor_buttons", first_column, ImGuiTableFlags_None, [&]
      {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        //if (/*m_configs->current_buffer == Editor::Buffer::LevelEditor && */ImGui::BeginTabBar("Controls", ImGuiTabBarFlags_FittingPolicyResizeDown))
        ImGuiHelper::TabBar( "Controls", ImGuiTabBarFlags_FittingPolicyResizeDown, [&]
        {
          for ( const auto &[icon, name, play_mask, callback] : m_controls )
          {
              // 0001 Stop (Play)
              // 0110 Running (Pause/Stop)
              // 0101 RunningPause (Play/Stop)
            if ( !( static_cast<u8>( m_configs->current_state ) & play_mask ) )
              continue;
            if ( ImGui::TabItemButton( icon ) )
              callback();
            if ( ImGui::IsItemHovered() )
              ImGui::SetTooltip( name );
          }
        } );

    //ImGui::SameLine(ImGui::GetWindowWidth() - 300.f / 2);

        ImGui::TableNextColumn();

        //if (ImGui::BeginTabBar("UI button", ImGuiTabBarFlags_FittingPolicyResizeDown))
        ImGuiHelper::TabBar( "UI button", ImGuiTabBarFlags_FittingPolicyResizeDown, [&]
        {
          for ( const auto &[icon, name, mask, callback] : m_editor_buffer )
          {
            if ( static_cast<u8>( m_configs->current_buffer ) & mask )
              continue;
            if ( ImGui::TabItemButton( icon ) )
              callback();
            if ( ImGui::IsItemHovered() )
              ImGui::SetTooltip( name );
          }
        } );
      } );
  //ImGui::SameLine(ImGui::GetWindowWidth() - 300.0f * m_toolset.size());

      ImGui::TableNextColumn();
      static const int second_column = 1;
      ImGuiHelper::Table( "##toolset_buttons", second_column, ImGuiTableFlags_None, [&]
      {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        //if (ImGui::BeginTabBar("Toolset", ImGuiTabBarFlags_FittingPolicyResizeDown))
        ImGuiHelper::TabBar( "Toolset", ImGuiTabBarFlags_FittingPolicyResizeDown, [&]
        {
          for ( const auto &[icon, name, mask, callback] : m_toolset )
          {
            auto flag = m_tool_context->m_gizmo_type ^ mask ? ImGuiTabItemFlags_None : ImGuiTabItemFlags_SetSelected;

            if ( ImGui::BeginTabItem( icon, nullptr, flag ) )
              ImGui::EndTabItem();

            if ( ImGui::IsItemClicked() )
              callback();

            if ( ImGui::IsItemHovered() )
              ImGui::SetTooltip( name );

          //ImGui::SameLine();
          //SnappingGizmo(name);
          }
        } );

        ImGui::SameLine();

        SnappingGizmo();
      } );

      ImGui::TableNextColumn();

      static const int third_column = 2;
      ImGuiHelper::Table( "##graphics_settings_buttons", third_column, ImGuiTableFlags_None, [&]
      {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        const char *empty = "Graphics Settings";

        ImGuiHelper::ItemWidth( -1.0f, [&]
        {
          if ( ImGui::BeginCombo( "##graphics_settings", empty ) )
          {
            for ( auto &[name, active, callback] : m_graphics_setting )
            {
              if ( ImGui::RadioButton( name, active ) )
                active = callback();
            }
            ImGui::EndCombo();
          }
        } );

        ImGui::TableNextColumn();

        ImGui::Text( "Cam Sentivity" );
        ImGui::SameLine();
        ImGuiHelper::ItemWidth( -1.0f, [&]
        {
          ImGui::SliderInt( "##slider", &m_tool_context->m_camera_scalar, 1, 10 );
          m_framebuffer->cameraSentivity = m_tool_context->m_camera_speed * static_cast<f32>( m_tool_context->m_camera_scalar );
        } );
      } );
    } );
  } );
}

void ButtonPanel::SnappingGizmo() noexcept
{
  std::string str;
  static ImGuizmo::OPERATION prev_gizmo_type = m_tool_context->m_gizmo_type;
  static Ptr<SnappingList> list_of_snap_val = m_tool_context->m_gizmo_type == ImGuizmo::OPERATION::TRANSLATE ?
    &m_trans_snap_vals :
    m_tool_context->m_gizmo_type == ImGuizmo::OPERATION::ROTATE ?
    &m_rot_snap_vals :
    &m_scale_snap_vals;

  static bool *is_snapping = m_tool_context->m_gizmo_type == ImGuizmo::OPERATION::TRANSLATE ?
    &m_tool_context->m_translation_snap.grid_snap :
    m_tool_context->m_gizmo_type == ImGuizmo::OPERATION::ROTATE ?
    &m_tool_context->m_rotation_snap.grid_snap :
    &m_tool_context->m_scale_snap.grid_snap;

  switch ( m_tool_context->m_gizmo_type )
  {
    case ImGuizmo::OPERATION::TRANSLATE:
    str = m_tool_context->m_translation_snap.str_value;
    if ( prev_gizmo_type != ImGuizmo::OPERATION::TRANSLATE )
    {
      list_of_snap_val = &m_trans_snap_vals;
      is_snapping = &m_tool_context->m_translation_snap.grid_snap;
      prev_gizmo_type = ImGuizmo::OPERATION::TRANSLATE;
    }
    break;
    case ImGuizmo::OPERATION::ROTATE:
    str = m_tool_context->m_rotation_snap.str_value;
    if ( prev_gizmo_type != ImGuizmo::OPERATION::ROTATE )
    {
      list_of_snap_val = &m_rot_snap_vals;
      is_snapping = &m_tool_context->m_rotation_snap.grid_snap;
      prev_gizmo_type = ImGuizmo::OPERATION::ROTATE;
    }
    break;
    case ImGuizmo::OPERATION::SCALE:
    str = m_tool_context->m_scale_snap.str_value;
    if ( prev_gizmo_type != ImGuizmo::OPERATION::SCALE )
    {
      list_of_snap_val = &m_scale_snap_vals;
      is_snapping = &m_tool_context->m_scale_snap.grid_snap;
      prev_gizmo_type = ImGuizmo::OPERATION::SCALE;
    }
    break;
  }

  ImGui::Text( "Snap:" );
  ImGui::SameLine();
  ImGui::Checkbox( "##snapping", is_snapping );

  ImGui::SameLine();

  const char *current_item = str.c_str();
  ImGuiHelper::ItemWidth( -1.0f, [&]
  {
    if ( ImGui::BeginCombo( "##snapping_value", current_item ) )
    {
        //for ( unsigned i = 0; i < list_of_snap_val->size(); ++i )
      for ( const auto &[str_value, value] : *list_of_snap_val )
      {
          //f32 selected_val = ( *list_of_snap_val )[i];
        f32 selected_val = value;
        bool is_selected = ( current_item == str_value );
        if ( ImGui::Selectable( str_value, is_selected ) )
        {
          switch ( m_tool_context->m_gizmo_type )
          {
            case ImGuizmo::OPERATION::TRANSLATE:
            m_tool_context->m_translation_snap.value = selected_val;
            m_tool_context->m_translation_snap.str_value = str_value;
            break;
            case ImGuizmo::OPERATION::ROTATE:
            m_tool_context->m_rotation_snap.value = selected_val;
            m_tool_context->m_rotation_snap.str_value = str_value;
            break;
            case ImGuizmo::OPERATION::SCALE:
            m_tool_context->m_scale_snap.value = selected_val;
            m_tool_context->m_scale_snap.str_value = str_value;
            break;
          }
        }
        if ( is_selected )
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
  } );
}

void ButtonPanel::SnappingGizmo( const std::string name ) noexcept
{
  std::string str;
  //static ImGuizmo::OPERATION prev_gizmo_type = m_tool_context->m_gizmo_type;
  Ptr<SnappingList> list_of_snap_val = nullptr;

  bool *is_snapping = nullptr;

  if ( name == "Translate" )
  {
    str = m_tool_context->m_translation_snap.str_value;
    list_of_snap_val = &m_trans_snap_vals;
    is_snapping = &m_tool_context->m_translation_snap.grid_snap;
  }
  else if ( name == "Rotate" )
  {
    str = m_tool_context->m_rotation_snap.str_value;
    list_of_snap_val = &m_rot_snap_vals;
    is_snapping = &m_tool_context->m_rotation_snap.grid_snap;
  }
  else
  {
    str = m_tool_context->m_scale_snap.str_value;
    list_of_snap_val = &m_scale_snap_vals;
    is_snapping = &m_tool_context->m_scale_snap.grid_snap;
  }

  XV_ASSERT_MSG( list_of_snap_val, "No container selected" );
  XV_ASSERT_MSG( is_snapping, "No snapping bool" );

 /* switch (m_tool_context->m_gizmo_type)
  {
  case ImGuizmo::OPERATION::TRANSLATE:
      str = m_tool_context->m_translation_snap.str_value;
      if (prev_gizmo_type != ImGuizmo::OPERATION::TRANSLATE)
      {
          list_of_snap_val = &m_trans_snap_vals;
          is_snapping = &m_tool_context->m_translation_snap.grid_snap;
          prev_gizmo_type = ImGuizmo::OPERATION::TRANSLATE;
      }
      break;
  case ImGuizmo::OPERATION::ROTATE:
      str = std::to_string(m_tool_context->m_rotation_snap.value);
      if (prev_gizmo_type != ImGuizmo::OPERATION::ROTATE)
      {
          list_of_snap_val = &m_rot_snap_vals;
          is_snapping = &m_tool_context->m_rotation_snap.grid_snap;
          prev_gizmo_type = ImGuizmo::OPERATION::ROTATE;
      }
      break;
  case ImGuizmo::OPERATION::SCALE:
      str = std::to_string(m_tool_context->m_scale_snap.value);
      if (prev_gizmo_type != ImGuizmo::OPERATION::SCALE)
      {
          list_of_snap_val = &m_scale_snap_vals;
          is_snapping = &m_tool_context->m_scale_snap.grid_snap;
          prev_gizmo_type = ImGuizmo::OPERATION::SCALE;
      }
      break;
  }*/

  const char *current_item = str.c_str();
  ImGui::PushID( current_item );
  ImGui::Text( "Snap:" );
  ImGui::SameLine();
  ImGui::Checkbox( "##snapping", is_snapping );

  ImGui::SameLine();

  ImGuiHelper::ItemWidth( -1.0f, [&]
  {
    if ( ImGui::BeginCombo( ( "##snapping_value" + str ).c_str(), current_item, ImGuiComboFlags_PopupAlignLeft ) )
    {
        /*for (unsigned i = 0; i < list_of_snap_val->size(); ++i)
        {
            f32 selected_val = (*list_of_snap_val)[i];
            bool is_selected = (current_item == std::to_string(selected_val).c_str());
            if (ImGui::Selectable(std::to_string(selected_val).c_str(), is_selected))
            {
                switch (m_tool_context->m_gizmo_type)
                {
                case ImGuizmo::OPERATION::TRANSLATE:
                    m_tool_context->m_translation_snap.value = selected_val;
                    break;
                case ImGuizmo::OPERATION::ROTATE:
                    m_tool_context->m_rotation_snap.value = selected_val;
                    break;
                case ImGuizmo::OPERATION::SCALE:
                    m_tool_context->m_scale_snap.value = selected_val;
                    break;
                }
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }*/
      for ( const auto &[str_value, value] : *list_of_snap_val )
      {
        bool is_selected = ( current_item == str_value );
        if ( ImGui::Selectable( str_value, is_selected ) )
        {
            /*switch (m_tool_context->m_gizmo_type)
            {
            case ImGuizmo::OPERATION::TRANSLATE:
                m_tool_context->m_translation_snap.value = selected_val;
                break;
            case ImGuizmo::OPERATION::ROTATE:
                m_tool_context->m_rotation_snap.value = selected_val;
                break;
            case ImGuizmo::OPERATION::SCALE:
                m_tool_context->m_scale_snap.value = selected_val;
                break;
            }*/
          if ( name == "Translate" )
          {
            m_tool_context->m_translation_snap.value = value;
            m_tool_context->m_translation_snap.str_value = str_value;
          }
          else if ( name == "Rotate" )
          {
            m_tool_context->m_rotation_snap.value = value;
            m_tool_context->m_rotation_snap.str_value = str_value;
          }
          else
          {
            m_tool_context->m_scale_snap.value = value;
            m_tool_context->m_scale_snap.str_value = str_value;
          }
        }
        if ( is_selected )
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
  } );
  ImGui::PopID();
}
}

#endif