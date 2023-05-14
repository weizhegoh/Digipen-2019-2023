/*****************************************************************
*\file         VirtualFilesystem.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "Xivi/Core/Utils/Filesystem/Filesystem.h"
#include "VirtualFilesystem.h"

namespace Xivi::Filesystem
{
void Virtual::Mount( const VirtualPath &v_path, const PhysicalPath &p_path )
{
  m_mapped_dirs[v_path].push_back( p_path );
}

void Virtual::Unmount( const VirtualPath &v_path )
{
  auto it = m_mapped_dirs.find( v_path );
  if ( it != m_mapped_dirs.end() )
    m_mapped_dirs.erase( it );
}

Virtual::PhysicalPath Virtual::ResolvePhysicalPath( const VirtualPath &v_path )
{
  PhysicalPath p_path = v_path;
  std::replace( p_path.begin(), p_path.end(), '\\', '/' );

  auto v_dir = p_path.substr( 0, p_path.find_first_of( '/' ) );
  if ( !m_mapped_dirs.contains( v_dir ) )
  {
    if ( dir_exists( p_path ) || file_exists( p_path ) )
      return p_path;
    else
      return std::string();
  }

  const PhysicalPath sub_path = p_path.substr( v_dir.size(), p_path.size() - v_dir.size() );

  for ( const PhysicalPath &m_p_path : m_mapped_dirs[v_dir] )
  {
    const PhysicalPath newPath = m_p_path + sub_path;
    if ( dir_exists( newPath ) || file_exists( newPath ) )
      return newPath;
  }
  return std::string();
}
}