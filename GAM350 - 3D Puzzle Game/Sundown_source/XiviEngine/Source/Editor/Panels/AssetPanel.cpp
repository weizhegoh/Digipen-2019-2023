/*****************************************************************
*\file         AssetPanel.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "AssetPanel.h"
#include <imgui_internal.h>
#include "Xivi/Core/Application.h"
#include "AssetPanelBrowser.h"
#include "Editor/ImGui/Helper/ImGuiHelperFunction.h"
namespace Xivi::Editor
{
AssetPanel::AssetPanel( std::string name,
                        Ptr<ECS::World::Instance> world,
                        Ptr<Config> configs,
                        Ptr<EntityContext> entity_context,
                        Ptr<FileContext> file_context,
                        Ptr<ToolContext> tool_context ) :
  IPanel( name, world, configs, entity_context, file_context, tool_context )
{
  Utils::SystemDirectory::Instance().Initialise();
  Utils::SystemDirectory::Instance().StartWatch();

  m_sync = [&]( const Utils::Directory &check,
                std::filesystem::directory_entry &selection,
                Ptr<const std::vector<Utils::Directory>> &dir_list,
                Ptr < const std::vector<Filesystem::fs::path>> &file_list )
  {
    if ( check.cur_dir == selection )
    {
      dir_list = &check.sub_dir;
      file_list = &check.file_vec;
      return true;
    }
    else
      for ( auto &dir : check.sub_dir )
        if ( m_sync( dir, selection, dir_list, file_list ) )
          return true;
    return false;
  };

  DirectoryBrowserInit( *m_world );
  FileBrowserInit( *m_world );
}

AssetPanel::~AssetPanel()
{
  Utils::SystemDirectory::Instance().StopWatch();
  Utils::SystemDirectory::Instance().Shutdown();
  m_enabled = false;
}

void AssetPanel::Render() noexcept
{
    // Retarget dir_list & file_list if sync occurs
  if ( Utils::SystemDirectory::Instance().Sync() )
    m_sync( Utils::SystemDirectory::Instance().ConstRoot(), m_file_context->selection, m_file_context->dir_list, m_file_context->file_list );

  if ( !m_enabled )
    return;
  ImGuiHelper::CreateImGuiWindow( m_name.c_str(), &m_enabled, [&]
  {
    if ( ImGui::IsWindowHovered( ImGuiHoveredFlags_ChildWindows ) && ImGui::IsMouseClicked( 1 ) )
      ImGui::SetWindowFocus();

    if ( ImGui::IsWindowFocused( ImGuiFocusedFlags_RootAndChildWindows ) )
      ImGui::SetFocusID( ImHashStr( m_name.c_str() ), ImGui::GetCurrentWindow() );

    DrawDirectoryBrowser( m_file_context->selection, m_file_context->dir_list, m_file_context->file_list, m_file_context );
    ImGui::SameLine();
    DrawFileBrowser( m_file_context->selection, m_file_context->dir_list, m_file_context->file_list, m_file_context );
    if ( Application::Get().DragDropFiles().size() )
      ImGuiHelper::DragDropSource( ImGuiDragDropFlags_SourceExtern, [&]
    {
      ImGui::SetDragDropPayload( "Explorer Files", nullptr, 0 );
      ImGui::BeginTooltip();
      ImGui::Text( "Explorer Files..." );
      ImGui::EndTooltip();
      ImGui::EndDragDropSource();
    } );

    if ( m_file_context->selection.exists() )
    {
      ImGuiHelper::DragDropTarget( [&]
      {
        if ( ImGui::AcceptDragDropPayload( "Explorer Files" ) )
        {
          for ( const auto &path : Application::Get().DragDropFiles() )
          {
            auto dest = m_file_context->selection.path() / path.filename();
            Filesystem::copy_file( path, dest );
          }
          Application::Get().DragDropFiles().clear();
        }
      } );
    }
  } );
}
}
#endif