/*****************************************************************
*\file         EditorEvents.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#ifdef XV_EDITOR
#include <Types/Base.h>

namespace Xivi
{
struct Position;

using ArchetypeCreateFunc = Func<Entity( const Position & )>;
using AssetCreateFunc = Func<Entity( const Position &, const std::string & )>;
}

namespace Xivi::Utils
{
struct Directory;
}

namespace Xivi::Editor
{
struct FileContext;

// Editor
struct Directories : ECS::Event::Instance<std::filesystem::directory_entry &,
  Ptr<const std::vector<Utils::Directory>> &,
  Ptr < const std::vector<Filesystem::fs::path>> &,
  Ptr<FileContext> &>
{};
struct Files : ECS::Event::Instance<std::filesystem::directory_entry &,
  Ptr<const std::vector<Utils::Directory>> &,
  Ptr < const std::vector<Filesystem::fs::path>> &,
  Ptr<FileContext> &>
{};
}

namespace Xivi
{
// Editor Event List
using EditorBase_EList = EList<
  Editor::Directories,
  Editor::Files>;

// Command Event List
using EditorCommand_EList = EList<>;

using Editor_EList = EditorBase_EList::Extend<EditorCommand_EList>;
}

#endif