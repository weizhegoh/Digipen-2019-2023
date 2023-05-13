/*****************************************************************
*\file         Game.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "PlayerController.h"
#include "GameWorldState.h"
#include "Switch.h"
#include "LockComponent.h"
#include "Waypoints.h"
#include "FallingFloor.h"
#include "PressurePlate.h"
#include "Hover.h"

namespace Xivi
{
using Game_CList = CList<PlayerController, GameWorldState, Switch, Lock, Waypoints, FallingFloor, PressurePlateC, Hover>;
}