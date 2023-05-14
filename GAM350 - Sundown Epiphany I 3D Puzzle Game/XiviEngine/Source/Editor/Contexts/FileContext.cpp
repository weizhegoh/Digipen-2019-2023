/*****************************************************************
*\file         FileContext.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "FileContext.h"

namespace Xivi::Editor
{
void FileContext::ClearAll() noexcept
{
  prefab_path.clear();
  selection.clear_cache();
  selection_file.clear();
  dir_list = nullptr;
  file_list = nullptr;
}
bool FileContext::DuplicatePrefabPath( const std::string duplicate_path ) const noexcept
{
  for ( auto &[ent, path] : prefab_path )
    if ( duplicate_path == path )
      return true;
  return false;
}

void FileContext::RemovePrefab( const std::string remove_path ) noexcept
{
  for ( auto it = prefab_path.begin(); it != prefab_path.end(); ++it )
    if ( remove_path == it->second )
    {
      prefab_path.erase( it );
      break;
    }
}

Entity FileContext::GetPrefab( const std::string get_path ) const noexcept
{
  for ( auto &[ent, path] : prefab_path )
    if ( get_path == path )
      return ent;
  return Entity {};
}

std::string FileContext::GetPrefab( const Entity get_path ) const noexcept
{
  for ( auto &[ent, path] : prefab_path )
    if ( get_path == ent )
      return path;
  return std::string {};
}

size_t FileContext::PrefabSize() const noexcept
{
  return prefab_path.size();
}
}
#endif