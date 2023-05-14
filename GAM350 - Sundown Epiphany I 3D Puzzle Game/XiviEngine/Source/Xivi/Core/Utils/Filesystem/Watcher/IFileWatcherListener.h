/*****************************************************************
*\file         IFileWatcherListener.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
namespace Xivi::Utils
{
struct IFileWatcherListener
{
  virtual void OnFileAdded( const std::string &path )
  {};
  virtual void OnFileDeleted( const std::string &path )
  {};
  virtual void OnFileChanged( const std::string &path )
  {};
  virtual void OnFileRenamed( const std::string &path )
  {};
  virtual ~IFileWatcherListener() = default;
};
} // namespace Xivi
