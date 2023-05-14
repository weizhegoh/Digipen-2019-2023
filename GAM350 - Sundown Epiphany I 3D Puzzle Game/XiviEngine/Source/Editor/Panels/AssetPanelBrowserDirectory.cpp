/*****************************************************************
*\file         AssetPanelBrowserDirectory.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "AssetPanelBrowser.h"
#include "Editor/ImGui/Helper/ImGuiHelperFunction.h"
namespace Xivi::Editor
{
void DirectoryBrowserInit( Xivi::ECS::World::Instance & ) noexcept
{};

void Recurse( const Utils::Directory &dir, ImGuiTreeNodeFlags flags,
              std::filesystem::directory_entry &selection,
              Ptr<const std::vector<Utils::Directory>> &dir_list,
              Ptr < const std::vector<Filesystem::fs::path>> &file_list,
              Ptr<FileContext> &file_context ) noexcept
{
  for ( const auto &d : dir.sub_dir )
  {
    ImGuiTreeNodeFlags recurse_flags = flags & ~( ImGuiTreeNodeFlags_Selected );

    if ( d.sub_dir.empty() )
      recurse_flags = ImGuiTreeNodeFlags_Leaf;

    if ( selection.exists() && selection == d.cur_dir )
      recurse_flags |= ImGuiTreeNodeFlags_Selected;

    bool open = ImGui::TreeNodeEx( d.cur_dir.path().filename().generic_string().c_str(), recurse_flags );
    if ( ImGui::IsItemClicked() )
    {
      selection = d.cur_dir;
      dir_list = &d.sub_dir;
      file_list = &d.file_vec;
      file_context->selection_file.clear();
    }

    if ( open )
    {
      Recurse( d, recurse_flags, selection, dir_list, file_list, file_context );
      ImGui::TreePop();
    }
  }
}

void DrawDirectoryBrowser( std::filesystem::directory_entry &selection,
                           Ptr<const std::vector<Utils::Directory>> &dir_list,
                           Ptr < const std::vector<Filesystem::fs::path>> &file_list,
                           Ptr<FileContext> &file_context ) noexcept
{
  auto region = ImGui::GetContentRegionAvail();

  ImGuiHelper::ChildWindow( "Directories",
                            { region.x / 4.0f, region.y },
                            true,
                            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar,
                            [&]
  {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    auto &dir = Utils::SystemDirectory::Instance().ConstRoot();
    if ( dir.sub_dir.empty() )
      flags = ImGuiTreeNodeFlags_Leaf;

    if ( selection.exists() && selection == dir.cur_dir )
      flags |= ImGuiTreeNodeFlags_Selected;

    bool open = ImGui::CollapsingHeader( dir.cur_dir.path().filename().generic_string().c_str(), flags | ImGuiTreeNodeFlags_DefaultOpen );
    if ( ImGui::IsItemClicked() )
    {
      selection = dir.cur_dir;
      dir_list = &dir.sub_dir;
      file_list = &dir.file_vec;
      file_context->selection_file.clear();
    }
    if ( open )
      Recurse( dir, flags, selection, dir_list, file_list, file_context );
  } );
}
}
#endif