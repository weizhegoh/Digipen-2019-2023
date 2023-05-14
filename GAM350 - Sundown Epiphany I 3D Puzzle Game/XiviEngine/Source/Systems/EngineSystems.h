/*****************************************************************
*\file         EngineSystems.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

// Base
#include "Engine/Base/BaseSystems.h"

// Parent Child
#include "Engine/ParentChild/ParentChildSystems.h"

// Prefab
#include "Engine/Prefab/PrefabSystems.h"

namespace Xivi
{
using Notifier_Engine_SList =
List::Notifier_Base_SList::Extend<
  List::Notifier_ParentChild_SList>::Extend<
  List::Notifier_Prefab_SList>;

using Global_Engine_SList =
List::Global_Base_SList::Extend<
  List::Global_ParentChild_SList>::Extend<
  List::Global_Prefab_SList>;

using Update_Engine_SList =
List::Update_Base_SList::Extend<
  List::Update_ParentChild_SList>::Extend<
  List::Update_Prefab_SList>;
}