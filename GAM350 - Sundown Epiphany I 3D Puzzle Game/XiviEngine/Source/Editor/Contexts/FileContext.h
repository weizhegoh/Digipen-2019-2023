/*****************************************************************
*\file         FileContext.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include <STL/vector.h>
#include "Xivi/ECS/XVECS.h"
#include "Xivi/Core/Utils/Filesystem/Watcher/SystemDirectory.h"
namespace Xivi::Editor
{
struct FileContext
{
  vector<std::pair<Entity, std::string>> prefab_path;

  std::filesystem::directory_entry selection;
  std::string selection_file;
  Ptr<const std::vector<Utils::Directory>> dir_list;
  Ptr<const std::vector<std::filesystem::path>> file_list;

  void ClearAll() noexcept;
  bool DuplicatePrefabPath( const std::string duplicate_path ) const noexcept;
  void RemovePrefab( const std::string remove_path ) noexcept;
  Entity GetPrefab( const std::string get_path ) const noexcept;
  std::string GetPrefab( const Entity get_path ) const noexcept;
  size_t PrefabSize() const noexcept;
};
}
#endif