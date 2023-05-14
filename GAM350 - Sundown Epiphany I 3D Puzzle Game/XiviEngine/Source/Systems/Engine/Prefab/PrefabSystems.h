/*****************************************************************
*\file         PrefabSystem.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

// Notifier
#include "PrefabCleanupSystem.h"
#include "UnlinkPrefabSystem.h"
#include "UnlinkPrefabInstanceSystem.h"

// Global
#include "CreatePrefabSystem.h"
#include "CreatePrefabInstanceSystem.h"
#include "SerialisePrefabSystem.h"
#include "DeserialisePrefabSystem.h"

#ifdef XV_ENGINE_IMPL
#include "PrefabCleanupSystem.inl"
#include "UnlinkPrefabSystem.inl"
#include "UnlinkPrefabInstanceSystem.inl"

#include "CreatePrefabSystem.inl"
#include "CreatePrefabInstanceSystem.inl"
#include "SerialisePrefabSystem.inl"
#include "DeserialisePrefabSystem.inl"
#endif

namespace Xivi::List
{
using Notifier_Prefab_SList = SList<
  PrefabCleanupSystem,
  UnlinkPrefabSystem,
  UnlinkPrefabInstanceSystem
>;

using Global_Prefab_SList = SList<
  CreatePrefabSystem,
  CreatePrefabInstanceSystem,
  SerialisePrefabSystem,
  DeserialisePrefabSystem>;

using Update_Prefab_SList = SList<>;
}