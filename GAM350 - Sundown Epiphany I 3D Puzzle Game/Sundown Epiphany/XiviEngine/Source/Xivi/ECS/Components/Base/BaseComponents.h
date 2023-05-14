/*****************************************************************
*\file         BaseComponents.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

// Data Components
#include "Entity.h"
#include "ReferenceCount.h"
#include "SharedFilter.h"

// Exclusive Tag Components
#include "ShareAsData.h"

namespace Xivi::ECS
{
using Base_CList = CList<Entity, ReferenceCount, SharedFilter, ShareAsData>;
}
