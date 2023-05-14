/*****************************************************************
*\file         ProcessCameraViewGoalState.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#include "../State.h"
#include "NComponents.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "Systems/Audio/Audio.h"
#include "NEvents/StepEvents.h"

namespace Xivi::Game
{
void ProcessCameraViewGoalState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  world.ForEach( [&]( Entity &entity, Camera_c &camera_c )
  {
    world.AddOrRemoveComponents<CList<CameraSetTarget>, CList<>>( entity );
  } );
  state_component.Switch( *this, UpdateStatus::Process );
}

void ProcessCameraViewGoalState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  world.ForEach( [&]( Camera_c &camera_c )
  {
    if ( !camera_c.toGoal )
      state_component.Switch( IdleState::Instance(), UpdateStatus::Begin );
  } );
}

void ProcessCameraViewGoalState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

State &ProcessCameraViewGoalState::Instance()
{
  static ProcessCameraViewGoalState s;
  return s;
}
}