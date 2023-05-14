/*****************************************************************
*\file         Filesystem.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "Filesystem.h"

namespace Xivi::Filesystem
{
  //****************************************************************************
  // FileSystem Root / Temp Path Utilities
  //****************************************************************************

void root( fs::path path )
{
  if ( dir_exists( path ) )
    fs::current_path( path );
  else
  {
    XV_CORE_ERROR( "[Filesystem] Invalid path" );
    abort();
  }
}

fs::path root()
{
  return fs::current_path();
}

fs::path temp_dir()
{
  return fs::temp_directory_path();
}

//****************************************************************************
// FileSystem Directories Utilities
//****************************************************************************
bool create_dir( fs::path dir_name )
{
  return fs::create_directories( dir_name );
}

void copy_dir( fs::path source, fs::path dest )
{
  if ( dir_exists( source ) )
  {
    constexpr fs::copy_options options = fs::copy_options::update_existing |
      fs::copy_options::recursive;

    fs::copy( source, dest, options );
  }
  else
    XV_CORE_ERROR( "[Filesystem] Source directory cannot be found" );
}

bool delete_dir( fs::path dir_name )
{
  if ( dir_exists( dir_name ) )
  {
    std::error_code ec;
    bool result = fs::remove( dir_name, ec );
    if ( ec )
      XV_CORE_ERROR( "[Filesystem] {}", ec.message() );
    return result;
  }
  else
    XV_CORE_ERROR( "[Filesystem] Directory cannot be found" );
  return false;
}

size_t delete_all_dir( fs::path dir_name )
{
  if ( dir_exists( dir_name ) )
  {
    std::error_code ec;
    size_t result = fs::remove_all( dir_name, ec );
    if ( ec )
      XV_CORE_ERROR( "[Filesystem] {}", ec.message() );
    return result;
  }
  else
    XV_CORE_ERROR( "[Filesystem] Directory cannot be found" );
  return 0;
}

bool dir_exists( fs::path dir_name )
{
  return fs::exists( dir_name ) &&
    fs::is_directory( dir_name );
}

std::vector<fs::directory_entry> dir_list( fs::path dir )
{
  std::vector<fs::directory_entry> dir_list;
  for ( auto &ref : fs::directory_iterator( dir ) )
    if ( fs::is_directory( ref ) )
      dir_list.push_back( ref );

  return dir_list;
}

std::vector<fs::directory_entry> recursive_dir_list( fs::path dir )
{
  std::vector<fs::directory_entry> dir_list;
  for ( auto &ref : fs::recursive_directory_iterator( dir ) )
    if ( is_directory( ref ) )
      dir_list.push_back( ref );

  return dir_list;
}

//****************************************************************************
// FileSystem Files Utilities
//****************************************************************************

fs::path create_file( fs::path filename )
{
  if ( !filename.parent_path().empty() )
    if ( !dir_exists( filename.parent_path() ) )
    {
      XV_CORE_ERROR( "[Filesystem] Directory cannot be found" );
      return fs::path();
    }

  std::ofstream dst( filename, std::ios::binary | std::ios::app );
  dst.close();
  return fs::path( filename );
}

bool delete_file( fs::path filename )
{
  if ( file_exists( filename ) )
    return fs::remove( filename );
  else
    XV_CORE_ERROR( "[Filesystem] File cannot be found" );

  return false;
}

void copy_file( fs::path source, fs::path dest )
{
  if ( file_exists( source ) )
  {
    constexpr fs::copy_options options = fs::copy_options::update_existing;

    fs::copy_file( source, dest, options );
  }
  else
    XV_CORE_ERROR( "[Filesystem] Source file cannot be found" );
}

bool file_exists( fs::path filename )
{
  return fs::exists( filename ) && fs::is_regular_file( filename );
}

size_t file_size( fs::path filename )
{
  return fs::file_size( filename );
}

std::vector<fs::path> file_list( fs::path dir )
{
  std::vector<fs::path> file_list;
  for ( auto &ref : fs::directory_iterator( dir ) )
    if ( fs::is_regular_file( ref ) )
      file_list.push_back( ref.path() );

  return file_list;
}
} // namespace FileUtils