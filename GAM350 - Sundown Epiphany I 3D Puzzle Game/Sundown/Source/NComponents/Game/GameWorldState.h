/*****************************************************************
*\file         GameWorldState.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "FSM/State.h"

namespace Xivi
{
enum class WorldState
{
  Paused = 0,
  Idle,
  ProcessPlayer,
  ProcessInteractableBox,
  ProcessPlayerScale,
  ProcessInteractableSwitch,
  ProcessUnlock,
  ProcessPlayerMovement,
  ProcessPlayerRotation,
  ProcessGoal,
  ProcessCollectible,
  ProcessFallingFloor,
  ProcessPressurePlate,
  ProcessEnvironment, // Moving Platform & Moving Obstacles
  ProcessFalling,
  ProcessGameOver
};

enum class UpdateState
{
  Begin = 0,
  Processing,
  Interpolating,
  Interpolated,
  End
};

DefineSingletonComponent( GameWorldState );
Ptr<Game::State> current_state;
WorldState current;
WorldState next;
UpdateState status;
EndDefine();
}