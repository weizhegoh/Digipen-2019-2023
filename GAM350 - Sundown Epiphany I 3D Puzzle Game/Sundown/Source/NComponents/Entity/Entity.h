/*****************************************************************
*\file         Entity.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "EntityDirection.h"
#include "EntityType.h"
#include "NewPosition.h"
#include "NewTransform.h"
#include "Player.h"
namespace Xivi
{
using Entity_CList = CList<EntityDirection, EntityType, NewPosition, NewTransform, Player>;
}