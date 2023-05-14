/*****************************************************************
*\file         EditorSystem.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

// Command
#include "Editor/Command/CommandSystems.h"

// Panels
#include "Editor/Panels/EditorPanelSystems.h"

// Base
#include "Editor/Base/EditorBaseSystems.h"

namespace Xivi
{
using Notifier_Editor_SList =
List::Notifier_EditorBase_SList::Extend<
  List::Notifier_EditorCommand_SList>::Extend<
  List::Notifier_EditorPanel_SList>;

using Global_Editor_SList =
List::Global_EditorBase_SList::Extend<
  List::Global_EditorCommand_SList>::Extend<
  List::Global_EditorPanel_SList>;

using Update_Editor_SList =
List::Update_EditorBase_SList::Extend<
  List::Update_EditorCommand_SList>::Extend<
  List::Update_EditorPanel_SList>;
}