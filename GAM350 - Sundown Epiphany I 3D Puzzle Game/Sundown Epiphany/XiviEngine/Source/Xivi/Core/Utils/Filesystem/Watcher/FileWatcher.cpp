/*****************************************************************
*\file         FileWatcher.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "FileWatcher.h"
#include "IFileWatcherListener.h"
#include "SystemDirectory.h"

namespace Xivi::Utils
{
FileWatcher::FileWatcher( std::filesystem::path directory ) : m_directory( directory )
{}

const std::filesystem::path &FileWatcher::GetDirectory()
{
  return m_directory;
}

void FileWatcher::AddListener( IFileWatcherListener *listener )
{
  m_listeners.push_back( UniquePtr<IFileWatcherListener>( listener ) );
}

void FileWatcher::Start()
{
  if ( !m_running.load() )
  {
    m_running.store( true );
    m_join.store( false );
    std::thread( FileWatcher::Thread, std::ref( *this ) ).detach();
  }
}

void FileWatcher::Stop()
{
  m_running.store( false );
  while ( !m_join.load() );
}

void FileWatcher::OnFileAdded( std::filesystem::path file )
{
  for ( auto &ref : m_listeners )
    ref->OnFileAdded( file.generic_string() );
}
void FileWatcher::OnFileDeleted( std::filesystem::path file )
{
  for ( auto &ref : m_listeners )
    ref->OnFileDeleted( file.generic_string() );
}
void FileWatcher::OnFileChanged( std::filesystem::path file )
{
  for ( auto &ref : m_listeners )
    ref->OnFileChanged( file.generic_string() );
}
void FileWatcher::OnFileRenamed( std::filesystem::path file )
{
  for ( auto &ref : m_listeners )
    ref->OnFileRenamed( file.generic_string() );
}

void FileWatcher::Thread( FileWatcher &fileWatcher )
{
  HANDLE hDir = CreateFile( fileWatcher.GetDirectory().generic_string().c_str(),    // path to the directory
                            FILE_LIST_DIRECTORY,                                    // access (read/write) mode
                            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, // share mode
                            NULL,                                                   // no child process
                            OPEN_EXISTING,                                          // only open existing
                            FILE_FLAG_BACKUP_SEMANTICS,                             // file attributes for directory handle
                            NULL                                                    // no template
  );

  wchar_t file[MAX_PATH];
  FILE_NOTIFY_INFORMATION buffer[1024];
  DWORD bytes;

  while ( ReadDirectoryChangesW(
    hDir,           // handle to directory
    &buffer,        // read results buffer
    sizeof( buffer ), // length of buffer
    TRUE,          // not monitoring subdirectories
    FILE_NOTIFY_CHANGE_SECURITY |
    FILE_NOTIFY_CHANGE_CREATION |
    FILE_NOTIFY_CHANGE_LAST_ACCESS |
    FILE_NOTIFY_CHANGE_LAST_WRITE |
    FILE_NOTIFY_CHANGE_SIZE |
    FILE_NOTIFY_CHANGE_ATTRIBUTES |
    FILE_NOTIFY_CHANGE_DIR_NAME |
    FILE_NOTIFY_CHANGE_FILE_NAME, // filter conditions
    &bytes,                           // bytes returned
    NULL,                             // overlapped buffer
    NULL                              // completion routine
    ) &&
    fileWatcher.m_running.load() )
  {
    FILE_NOTIFY_INFORMATION *information;
    information = (FILE_NOTIFY_INFORMATION *) ( (char *) buffer );
    wcscpy_s( file, L"" );

    wcsncpy_s( file, information->FileName, information->FileNameLength / 2 );

    file[information->FileNameLength / 2] = NULL;

    if ( buffer[0].Action )
      SystemDirectory::Instance().m_sync.store( true );

    //switch (buffer[0].Action)
    //{
    //case FILE_ACTION_ADDED:
    //  fileWatcher.OnFileAdded(file);
    //  break;
    //case FILE_ACTION_MODIFIED:
    //  fileWatcher.OnFileChanged(file);
    //  break;
    //case FILE_ACTION_REMOVED:
    //  fileWatcher.OnFileDeleted(file);
    //  break;
    //case FILE_ACTION_RENAMED_OLD_NAME:
    //  fileWatcher.OnFileRenamed(file);
    //  break;
    //case FILE_ACTION_RENAMED_NEW_NAME:
    //  fileWatcher.OnFileRenamed(file);
    //  break;
    //}

    Sleep( 1000 );
  }

  CloseHandle( hDir );
  fileWatcher.m_join.store( true );
}
} // namespace Xivi