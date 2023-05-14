/*****************************************************************
*\file         PlayerController.h
*\author(s)    Tan Tong Wee, Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "NComponents/Entity/EntityDirection.h"

namespace Xivi
{
enum class PlayerAction
{
  None,
  MoveUp, // old
  MoveDown, // old old
  MoveLeft, // old old old
  MoveRight, // old old old old
  MoveForward,
  MoveBackward,
  RotateLeft,
  RotateRight,
  Interact,
  RotateCameraLeft,
  RotateCameraRight,
  ViewGoal
};

enum class CurrScale
{
  Normal,
  Grow
};

DefineSingletonComponent( PlayerController );
Entity playerEntity;
i32 camera_angle;
PlayerAction action;
bool lock_control = false;

//Clone usage
bool changeClone = false;
Entity cloneEntity;
Vec3i previous_tile;

bool fallFloor = false;
int currenturn = 0;
//scaling system usage
CurrScale currScale;
EndDefine();
}