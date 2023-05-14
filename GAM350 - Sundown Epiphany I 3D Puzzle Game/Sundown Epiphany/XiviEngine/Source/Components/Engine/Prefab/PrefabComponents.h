/*****************************************************************
*\file         PrefabComponenets.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

// Data Components
#include "Prefab.h"
#include "ReferencedPrefab.h"

// ExclusiveTag Components
#include "PrefabTag.h"

namespace Xivi
{
using Prefab_CList = CList<Prefab, PrefabTag, ReferencedPrefab>;
}