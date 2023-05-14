/*****************************************************************
*\file         ProcessPlayerRotateState.cpp
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

namespace Xivi::Game
{
void ProcessPlayerRotateState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();

  auto [dir, rotDir, transform] = world.GetComponents<Direction, EntityDirection, Transform>( pc.playerEntity );
  if ( pc.action == PlayerAction::RotateLeft )
  {
    transform.axisOffset.setY( ( transform.axisOffset.getY() - 90.f ) );
    if ( transform.axisOffset.getY() < -360 )
    {
      transform.axis.setY( 0.0f );
      transform.axisOffset.setY( -90.f );
    }
  }
  else
  {
    transform.axisOffset.setY( ( transform.axisOffset.getY() + 90.f ) );
    if ( transform.axisOffset.getY() > 360 )
    {
      transform.axis.setY( 0.0f );
      transform.axisOffset.setY( 90.f );
    }
  }
  // Music here
  Audio::AudioManager::Instance().playSFX( "SFX_ROTATE.ogg", false );
  transform.ratio = 0.f;
  world.AddOrRemoveComponents<CList<Rotate>>( pc.playerEntity );
  state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, IdleState::Instance(), UpdateStatus::Begin );
}

void ProcessPlayerRotateState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

void ProcessPlayerRotateState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

State &ProcessPlayerRotateState::Instance()
{
  static ProcessPlayerRotateState s;
  return s;
}
}