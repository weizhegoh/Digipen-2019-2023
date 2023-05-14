/*****************************************************************
*\file         DirectoryListener.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "IFileWatcherListener.h"

namespace Xivi::Utils
{
struct DirectoryListener : IFileWatcherListener
{
  void OnFileAdded( const std::string &path ) override;
  void OnFileDeleted( const std::string &path ) override;
  void OnFileChanged( const std::string &path ) override;
  void OnFileRenamed( const std::string &path ) override;
};
}