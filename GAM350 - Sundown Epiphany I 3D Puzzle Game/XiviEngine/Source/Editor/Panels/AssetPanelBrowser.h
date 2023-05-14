/*****************************************************************
*\file         AssetPanelBrowser.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "Editor/Contexts/FileContext.h"
#include <imgui.h>

namespace Xivi::Editor
{
void PasteAsset( Ptr<FileContext> file_context ) noexcept;
void CopyAsset( const Filesystem::fs::path file, Ptr<FileContext> file_context ) noexcept;
void CutAsset( const Filesystem::fs::path file, Ptr<FileContext> file_context ) noexcept;
void DeleteAsset( const Filesystem::fs::path file, Ptr<FileContext> file_context ) noexcept;

void DirectoryBrowserInit( Xivi::ECS::World::Instance & ) noexcept;

void FileBrowserInit( Xivi::ECS::World::Instance & ) noexcept;

void DrawDirectoryBrowser( std::filesystem::directory_entry &selection,
                           Ptr<const std::vector<Utils::Directory>> &dir_list,
                           Ptr < const std::vector<std::filesystem::path>> &file_list,
                           Ptr<FileContext> &file_context ) noexcept;

void DrawFileBrowser( std::filesystem::directory_entry &selection,
                      Ptr<const std::vector<Utils::Directory>> &dir_list,
                      Ptr < const std::vector<Filesystem::fs::path>> &file_list,
                      Ptr<FileContext> &file_context ) noexcept;
}

#endif