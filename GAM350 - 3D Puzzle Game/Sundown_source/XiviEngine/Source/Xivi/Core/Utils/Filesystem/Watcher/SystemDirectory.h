/*****************************************************************
*\file         SystemDirectory.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include <vector>
#include <filesystem>
#include <Utils/Singleton.h>
#include "FileWatcher.h"

namespace Xivi::Utils
{
struct Directory
{
  std::filesystem::directory_entry cur_dir;
  std::vector<Directory> sub_dir;
  std::vector<std::filesystem::path> file_vec;
  GUID<struct DirectoryID> m_id;
  void Initialise();
};

class SystemDirectory : public Singleton<SystemDirectory>
{
  friend class Singleton<SystemDirectory>;
  SystemDirectory() = default;
  ~SystemDirectory() = default;

  Directory m_root;
  UniquePtr<FileWatcher> m_watcher { nullptr };

public:
  std::atomic<bool> m_sync { false };
  void Initialise();
  void AddListener( IFileWatcherListener *listener );
  void StartWatch();
  void StopWatch();
  bool Sync();
  void Shutdown();

  Directory &Root();
  const Directory &ConstRoot();
};
}