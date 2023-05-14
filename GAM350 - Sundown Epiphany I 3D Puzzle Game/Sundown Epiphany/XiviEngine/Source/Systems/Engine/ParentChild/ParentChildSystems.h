/*****************************************************************
*\file         ParentChildSystems.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

// Notifer
#include "ParentChildCleanupSystem.h"

// Global
#include "ParentChildLinkSystem.h"
#include "ParentChildUnlinkSystem.h"
#include "ParentChildRemoveSystem.h"
#include "ParentChildTransformSystem.h"

// Update
#include "LocalToParentUpdateSystem.h"

#ifdef XV_ENGINE_IMPL
#include "ParentChildCleanupSystem.inl"

#include "ParentChildLinkSystem.inl"
#include "ParentChildUnlinkSystem.inl"
#include "ParentChildRemoveSystem.inl"
#include "ParentChildTransfromSystem.inl"

#include "LocalToParentUpdateSystem.inl"

#endif

namespace Xivi::List
{
using Notifier_ParentChild_SList = SList<ParentChildCleanupSystem>;

using Global_ParentChild_SList = SList<
  ParentChildLinkSystem,
  ParentChildUnlinkSystem,
  ParentChildRemoveSystem,
  ParentChildTransformSystem
>;

using Update_ParentChild_SList = SList<LocalToParentUpdateSystem>;
}