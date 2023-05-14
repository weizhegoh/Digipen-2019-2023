/*****************************************************************
*\file         VirtualFilesystem.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include <Utils/Singleton.h>
#include <vector>
namespace Xivi::Filesystem
{
class Virtual : public Singleton<Virtual>
{
  using PhysicalPath = std::string;
  using VirtualPath = std::string;
  using MappedDir = std::vector<std::string>;
  std::unordered_map<VirtualPath, MappedDir> m_mapped_dirs;
  friend class Singleton<Virtual>;
  Virtual() = default;
  ~Virtual() = default;
public:
  void Mount( const VirtualPath &v_path, const PhysicalPath &p_path );
  void Unmount( const VirtualPath &v_path );
  PhysicalPath ResolvePhysicalPath( const VirtualPath &v_path );
};
}