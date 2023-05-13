/*****************************************************************
*\file         EditorBaseSystems.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#ifdef XV_EDITOR
// Notifier
#include "EntityCreateSystem.h"
#include "EntityDestroySystem.h"

// Global
#include "EventRefreshPanel.h"

// Update
#include "LockingWindowSystem.h"

namespace Xivi::List
{
using Notifier_EditorBase_SList = SList<
  /*Editor::EntityCreateSystem,
  Editor::EntityDestroySystem*/>;

using Global_EditorBase_SList = SList<>;

using Update_EditorBase_SList = SList<>;
}
#endif