/*****************************************************************
*\file         FileDialog.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include <Commdlg.h>
#include "FileDialog.h"
#include "Xivi/Platform/Windows/GLFW/GLFW_Window.h"
#include "Xivi/Core/Application.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Xivi::FileDialogs
{
std::optional<std::string> OpenFile( const char *filter )
{
  OPENFILENAMEA ofn;
  CHAR szFile[260] = { 0 };
  ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
  ofn.lStructSize = sizeof( OPENFILENAME );
  ofn.hwndOwner = glfwGetWin32Window( Service::Locator::Get<GLFW_Window>().GLFWHandle() );
  ofn.lpstrInitialDir =
    ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof( szFile );
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

  if ( GetOpenFileNameA( &ofn ) == TRUE )
    return ofn.lpstrFile;
  return std::nullopt;
}

std::optional<std::string> SaveFile( const char *filter )
{
  OPENFILENAMEA ofn;
  CHAR szFile[260] = { 0 };
  ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
  ofn.lStructSize = sizeof( OPENFILENAME );
  ofn.hwndOwner = glfwGetWin32Window( Service::Locator::Get<GLFW_Window>().GLFWHandle() );
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof( szFile );
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

  // Sets the default extension by extracting it from the filter
  ofn.lpstrDefExt = strchr( filter, '\0' ) + 1;

  if ( GetSaveFileNameA( &ofn ) == TRUE )
    return ofn.lpstrFile;
  return std::nullopt;
}
}