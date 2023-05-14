/*****************************************************************
*\file         EngineComponents.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "Engine/ParentChild/ParentChildComponents.h"
#include "Engine/Prefab/PrefabComponents.h"
#include "Engine/LocalToParent.h"

#include "Position.h"
#include "Object.h"
#include "Velocity.h"
#include "Graphics/Transform.h"
namespace Xivi
{
using Engine_CList = ParentChild_CList::Extend<Prefab_CList>::Extend<CList<Position, Object, Velocity, LocalToParent>>;
}