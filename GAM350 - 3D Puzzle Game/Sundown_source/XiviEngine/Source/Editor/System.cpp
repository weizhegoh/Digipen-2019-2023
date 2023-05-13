/*****************************************************************
*\file         System.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "System.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <IconsFontAwesome5.h>
#include "Editor/ImGui/Style.h"

#include <Service/Locator.h>
#include "Xivi/Platform/Windows/GLFW/GLFW_Window.h"

// Update
#include "Panels/PropertyInspectorPanel.h"
#include "Panels/MenuPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/AssetPanel.h"
#include "Panels/PrefabPanel.h"
#include "Panels/WorldPanel.h"
#include "Panels/SettingsPanel.h"
#include "Panels/ArchetypePanel.h"
#include "Panels/ButtonPanel.h"
#include "Panels/ConsolePanel.h"
#include "Panels/GridPanel.h"
#include "Panels/AudioPanel.h"
#include "Panels/LightPanel.h"

#include "Systems/Base/EventCreateSkyBox.h"
#include "Systems/Base/EventRefreshPanel.h"
#include "Systems/Base/LockingWindowSystem.h"

namespace Xivi::Editor
{
void System::Update() noexcept
{
    // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGuizmo::BeginFrame();
  // Note: Switch this to true to enable dockspace
  static bool dockspaceOpen = true;
  static bool opt_fullscreen_persistant = true;
  bool opt_fullscreen = opt_fullscreen_persistant;
  static ImGuiDockNodeFlags dockspace_flags = 0;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
  // because it would be confusing to have two docking targets within each others.
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  if ( opt_fullscreen )
  {
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos( viewport->Pos );
    ImGui::SetNextWindowSize( viewport->Size );
    ImGui::SetNextWindowViewport( viewport->ID );
    ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
    ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  }

  // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
  if ( dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode )
    window_flags |= ImGuiWindowFlags_NoBackground;

  ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
  ImGui::Begin( "MyDockSpace", &dockspaceOpen, window_flags );
  ImGui::PopStyleVar();

  if ( opt_fullscreen )
    ImGui::PopStyleVar( 2 );

// DockSpace
  ImGuiIO &io = ImGui::GetIO();
  if ( io.ConfigFlags & ImGuiConfigFlags_DockingEnable )
  {
    ImGuiID dockspace_id = ImGui::GetID( "MyDockSpace" );
    ImGui::DockSpace( dockspace_id, ImVec2( 0.0f, 0.0f ), dockspace_flags );
  }

  for ( auto &panel : m_panels )
    panel->Render();

  UpdateBaseSystems();

  ImGui::End();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

  if ( ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
  {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent( backup_current_context );
  }
}
void System::Init() noexcept
{
  XV_CORE_INFO( "Initializing Editor..." );

  auto glfw_handle = Service::Locator::Get<GLFW_Window>().GLFWHandle();

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;     // Enable Keyboard Controls

  ImFontConfig icons_config;
  icons_config.MergeMode = true;
  icons_config.GlyphMinAdvanceX = 16.0f; // Use if you want to make the icon monospaced
  icons_config.PixelSnapH = true;

  static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
  io.FontDefault = io.Fonts->AddFontFromFileTTF( "../XiviEngine/Assets/Fonts/Roboto-Medium.ttf", 18.0f );
  m_configs.fonts = io.Fonts->AddFontFromFileTTF( "../XiviEngine/Assets/Fonts/fa-solid-900.ttf", 18.0f, &icons_config, icons_ranges );

  WindowStyle();
  ThemeStyle();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL( glfw_handle, true );
  ImGui_ImplOpenGL3_Init( "#version 410" );

  PushAllPanels();
  InitBaseSystems();
}

void System::Free() noexcept
{
  m_command_history.Clear();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  m_panels.clear();
}

void System::AddEntity( Entity entity, bool is_prefab, bool is_ui ) noexcept
{
  if ( is_prefab )
    m_entity_context.prefab_entity.push_back( entity );
  else if ( is_ui )
    m_entity_context.ui_entity.push_back( entity );
  else
    m_entity_context.world_entity.push_back( entity );
}

void System::RemoveEntity( Entity entity, bool is_prefab, bool is_ui ) noexcept
{
  if ( is_prefab )
    for ( auto it = m_entity_context.prefab_entity.begin(); it != m_entity_context.prefab_entity.end(); ++it )
    {
      if ( *it == entity )
      {
        m_entity_context.prefab_entity.erase( it );
        break;
      }
    }
  else if ( is_ui )
    for ( auto it = m_entity_context.ui_entity.begin(); it != m_entity_context.ui_entity.end(); ++it )
    {
      if ( *it == entity )
      {
        m_entity_context.ui_entity.erase( it );
        break;
      }
    }
  else
    for ( auto it = m_entity_context.world_entity.begin(); it != m_entity_context.world_entity.end(); ++it )
    {
      if ( *it == entity )
      {
        m_entity_context.world_entity.erase( it );
        break;
      }
    }
}

void System::OnPause() noexcept
{
  if ( m_configs.current_state != Editor::State::Stop )
    m_configs.current_state = Editor::State::RunningPaused;
}

void System::OnResume() noexcept
{
  m_configs.current_state = Editor::State::Running;
}

void System::PushAllPanels() noexcept
{
  auto world = &Service::Locator::Get<ECS::World::Instance>();
  // Add all the panels
  m_panels.push_back( MakeUnique<MenuPanel>( "Menu", world, &m_configs, &m_entity_context, &m_file_context, &m_tool_context, &m_panels ) );
  m_panels.push_back( MakeUnique<PropertyInspectorPanel>( "Property Inspector", world, &m_configs, &m_entity_context, &m_file_context, &m_tool_context ) );
  m_panels.push_back( MakeUnique<ViewportPanel>( "Viewport", world, &m_configs, &m_entity_context, &m_file_context, &m_tool_context ) );
  m_panels.push_back( MakeUnique<PrefabPanel>( "Prefab", world, &m_configs, &m_entity_context, &m_file_context, &m_tool_context ) );
  m_panels.push_back( MakeUnique<AssetPanel>( "Asset", world, &m_configs, &m_entity_context, &m_file_context, &m_tool_context ) );
  m_panels.push_back( MakeUnique<ArchetypePanel>( "Archetype", world, &m_configs, &m_entity_context, &m_file_context, &m_tool_context ) );
  m_panels.push_back( MakeUnique<AudioPanel>( "Audio", world, &m_configs, &m_entity_context, &m_file_context, &m_tool_context ) );
  m_panels.push_back( MakeUnique<ButtonPanel>( "Button", world, &m_configs, &m_entity_context, &m_file_context, &m_tool_context ) );
  m_panels.push_back( MakeUnique<ConsolePanel>( "Console", world, &m_configs, &m_entity_context, &m_file_context, &m_tool_context ) );
  m_panels.push_back( MakeUnique<SettingsPanel>( "Setting", world, &m_configs, &m_entity_context, &m_file_context, &m_tool_context ) );
  m_panels.push_back( MakeUnique<WorldPanel>( "World", world, &m_configs, &m_entity_context, &m_file_context, &m_tool_context ) );
  m_panels.push_back( MakeUnique<GridPanel>( "Grid", world, &m_configs, &m_entity_context, &m_file_context, &m_tool_context ) );
  m_panels.push_back( MakeUnique<LightPanel>( "Light", world, &m_configs, &m_entity_context, &m_file_context, &m_tool_context ) );

  // Sort by name
  std::sort( m_panels.begin(), m_panels.end(), [=]( UniquePtr<IPanel> &lhs, UniquePtr<IPanel> &rhs )
  {
    return lhs->GetName() < rhs->GetName();
  } );
}

void System::InitBaseSystems() noexcept
{
  auto &world = Service::Locator::Get<ECS::World::Instance>();
  EventCreateSkyBoxSystem::Init( world );
  LockingWindowSystem::Init( world );
  EventRefreshPanelSystem::Init();
}

void System::UpdateBaseSystems() noexcept
{
  LockingWindowSystem::Update();
  EventRefreshPanelSystem::Update();
}
}
#endif