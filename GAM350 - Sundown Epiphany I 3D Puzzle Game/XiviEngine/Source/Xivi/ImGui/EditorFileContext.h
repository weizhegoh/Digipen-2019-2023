/*****************************************************************
*\file         EditorFileContext.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct FileContext
{
  std::vector<std::pair<Entity, std::string>> m_prefab_path;

  std::filesystem::directory_entry m_selection;
  std::string m_selection_file;
  Ptr<const std::vector<Utils::Directory>> m_dir_list;
  Ptr<const std::vector<Filesystem::fs::path>> m_file_list;

  bool DuplicatePrefabPath( const std::string prefab_path ) const noexcept
  {
    for ( auto &[ent, path] : m_prefab_path )
      if ( prefab_path == path )
        return true;
    return false;
  }

  void RemovePrefab( const std::string prefab_path ) noexcept
  {
    for ( auto it = m_prefab_path.begin(); it != m_prefab_path.end(); ++it )
      if ( prefab_path == it->second )
      {
        m_prefab_path.erase( it );
        break;
      }
  }

  Entity GetPrefab( const std::string prefab_path ) const noexcept
  {
    for ( auto &[ent, path] : m_prefab_path )
      if ( prefab_path == path )
        return ent;
    return Entity {};
  }

  std::string GetPrefab( const Entity prefab ) const noexcept
  {
    for ( auto &[ent, path] : m_prefab_path )
      if ( prefab == ent )
        return path;
    return std::string {};
  }
};
}