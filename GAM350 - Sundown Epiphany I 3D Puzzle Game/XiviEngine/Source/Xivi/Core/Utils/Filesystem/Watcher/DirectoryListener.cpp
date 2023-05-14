/*****************************************************************
*\file         DirectoryListener.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "DirectoryListener.h"
#include "SystemDirectory.h"
#include "Xivi/Core/Utils/Filesystem/Filesystem.h"

namespace Xivi::Utils
{
void DirectoryListener::OnFileAdded( const std::string &path )
{
  (void) path;
  while ( SystemDirectory::Instance().m_sync.load() )
    continue;
  SystemDirectory::Instance().m_sync.store( true );
}

void DirectoryListener::OnFileDeleted( const std::string &path )
{
  (void) path;
  while ( SystemDirectory::Instance().m_sync.load() )
    continue;
  SystemDirectory::Instance().m_sync.store( true );
}
void DirectoryListener::OnFileChanged( const std::string &path )
{
  (void) path;
  while ( SystemDirectory::Instance().m_sync.load() )
    continue;
  SystemDirectory::Instance().m_sync.store( true );
}

void DirectoryListener::OnFileRenamed( const std::string &path )
{
  (void) path;
  while ( SystemDirectory::Instance().m_sync.load() )
    continue;
  SystemDirectory::Instance().m_sync.store( true );
}
}