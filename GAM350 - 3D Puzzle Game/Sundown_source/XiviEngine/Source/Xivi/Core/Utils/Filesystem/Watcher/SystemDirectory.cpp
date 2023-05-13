/*****************************************************************
*\file         SystemDirectory.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "SystemDirectory.h"
#include "DirectoryListener.h"
#include "Xivi/Core/Utils/Filesystem/Filesystem.h"

namespace Xivi::Utils
{
void Directory::Initialise()
{
  file_vec.clear();
  sub_dir.clear();
  file_vec.reserve( 256 );
  sub_dir.reserve( 16 );
  for ( auto &file : Filesystem::file_list( cur_dir ) )
    file_vec.push_back( file );

  for ( auto &dir : Filesystem::dir_list( cur_dir ) )
  {
    auto str = dir.path().generic_string();
    sub_dir.emplace_back( Directory { dir } ).Initialise();
  }
  m_id = Hash::XXH3_64( cur_dir.path().generic_string() );
}

void SystemDirectory::Initialise()
{
  XV_CORE_INFO( "[SystemDirectory] Initialising..." );
  m_root = Directory { std::filesystem::directory_entry( Filesystem::root() ) };
  XV_CORE_TRACE( "[SystemDirectory] Registering directories..." );
  m_root.Initialise();
  if ( m_watcher )
  {
    m_watcher->Stop();
    m_watcher.reset();
  }
  m_watcher = MakeUnique<FileWatcher>( Filesystem::root() );
}

void SystemDirectory::AddListener( IFileWatcherListener *listener )
{
  m_watcher->AddListener( listener );
}

void SystemDirectory::StartWatch()
{
  XV_CORE_INFO( "[SystemDirectory] Start directory listener..." );
  m_watcher->AddListener( new DirectoryListener() );
  m_watcher->Start();
}

void SystemDirectory::StopWatch()
{
  XV_CORE_INFO( "[SystemDirectory] Stop directory listener" );
  m_watcher->Stop();
}

Directory &SystemDirectory::Root()
{
  return m_root;
}

const Directory &SystemDirectory::ConstRoot()
{
  return m_root;
}

bool SystemDirectory::Sync()
{
  if ( m_sync.load() )
  {
    m_root.Initialise();
    m_sync.store( false );
    return true;
  }
  return false;
}

void SystemDirectory::Shutdown()
{
  XV_CORE_INFO( "[SystemDirectory] Shutting down..." );
  m_watcher->Stop();
  m_watcher.reset();
  XV_CORE_TRACE( "[SystemDirectory] Shutdown" );
}
}