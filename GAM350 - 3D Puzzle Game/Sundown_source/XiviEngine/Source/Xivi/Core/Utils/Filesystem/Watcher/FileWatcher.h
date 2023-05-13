/*****************************************************************
*\file         FileWatcher.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi::Utils
{
struct IFileWatcherListener;

class FileWatcher
{
  std::vector<std::unique_ptr<IFileWatcherListener>> m_listeners;
  std::filesystem::path m_directory;
  std::atomic<bool> m_running { false };
  std::atomic<bool> m_join { false };

public:
  FileWatcher( std::filesystem::path directory );
  const std::filesystem::path &GetDirectory();
  void AddListener( IFileWatcherListener *listener );
  void Start();
  void Stop();
  void OnFileAdded( std::filesystem::path file );
  void OnFileDeleted( std::filesystem::path file );
  void OnFileChanged( std::filesystem::path file );
  void OnFileRenamed( std::filesystem::path file );

  static void Thread( FileWatcher &fileWatcher );
};
} // namespace Xivi