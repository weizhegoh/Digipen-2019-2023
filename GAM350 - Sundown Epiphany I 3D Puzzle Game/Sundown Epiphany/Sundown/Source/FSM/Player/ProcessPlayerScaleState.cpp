/*****************************************************************
*\file         ProcessPlayerScaleState.cpp
*\author(s)    Kenric Tan Wei Liang

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
void ProcessPlayerScaleState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();
  auto [transform, newTransform, newPos, pos, player] = world.GetComponents<Transform, NewTransform, NewPosition, Position, Player>( pc.playerEntity );

  if ( pc.currScale == CurrScale::Grow )
  {
    newTransform.prevScale = transform.scale;
    player.prevOffsetPos = pos.m_position;
    newTransform.scale = newTransform.prevScale * player.scale;
    auto offset = ( newTransform.scale.getY() - transform.scale.getY() ) / 4;
    newPos.value = pos.m_position;
    newPos.value.setY( newPos.value.getY() + offset );
    // SFX big
    Audio::AudioManager::Instance().playSFX( "SFX_GROW.ogg", false );
    state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, ProcessPlayerInteractBoxState::Instance(), UpdateStatus::Process );
  }
  else
  {
    newTransform.scale = newTransform.prevScale;
    newPos.value = player.prevOffsetPos;
    // SFX small
    Audio::AudioManager::Instance().playSFX( "SFX_SHRINK.ogg", false );
    state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, ProcessFallingState::Instance(), UpdateStatus::Begin );
  }

  newPos.ratio = newTransform.ratio = 0.f;
  pc.playerEntity = world.AddOrRemoveComponents<CList<Scale, Move>>( pc.playerEntity );
}

void ProcessPlayerScaleState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

void ProcessPlayerScaleState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

State &ProcessPlayerScaleState::Instance()
{
  static ProcessPlayerScaleState s;
  return s;
}
}