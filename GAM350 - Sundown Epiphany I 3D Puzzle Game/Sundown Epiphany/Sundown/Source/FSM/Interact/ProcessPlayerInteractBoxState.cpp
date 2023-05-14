/*****************************************************************
*\file         ProcessPlayerInteractBoxState.cpp
*\author(s)    Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#include "../State.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "NComponents.h"
#include "Systems/Audio/Audio.h"
#include "NEvents/StepEvents.h"

namespace Xivi::Game
{
void ProcessPlayerInteractBoxState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();

  pc.currScale = CurrScale::Grow;
  state_component.SetCurrent( ProcessPlayerScaleState::Instance(), UpdateStatus::Begin );
}

void ProcessPlayerInteractBoxState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();

  auto [playerGridPos, playerdir, playerAnimationState] = world.GetComponents<GridPosition, Direction, AnimationState>( pc.playerEntity );
  // XV_CORE_WARN( "player dir {},{},{}", playerdir.m_dir.getX(), playerdir.m_dir.getY(), playerdir.m_dir.getZ() );
  Vec3i currEntityPos( playerdir.m_dir );
  currEntityPos += playerGridPos.value;
  // XV_CORE_WARN( "after pos {},{},{}", currEntityPos.x, currEntityPos.y, currEntityPos.z );
  Entity currEntity = grid.GetCellEntity( currEntityPos );
  auto [newPos, gridPos, dir] = world.GetComponents<NewPosition, GridPosition, Direction>( currEntity );
  playerAnimationState.UpdateState( AnimationState::AnimState::anim_push );
  world.SendGlobalEvent<CloseCartPopupDialogue>();

  if ( grid.MoveBox( gridPos.value, Vec3i( playerdir.m_dir ) ) )
  {
    newPos.value = grid.WorldSpace( gridPos.value ) + gridPos.offset;
    newPos.ratio = 0.f;
    // Play sfx here
    Audio::AudioManager::Instance().playLoop( "LOOP_PUSHOBJECT.ogg", false );
    world.AddOrRemoveComponents<CList<Move>>( currEntity );
    state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, *this, UpdateStatus::End );
  }
  else
    state_component.current_status = UpdateStatus::End;
}

void ProcessPlayerInteractBoxState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();
  auto [playerAnimationState, animator] = world.GetComponents<AnimationState, AnimatorComponent>( pc.playerEntity );
  if ( playerAnimationState._animCurrentState == AnimationState::AnimState::anim_push && animator.endFrame )
  {
    playerAnimationState.UpdateState( AnimationState::AnimState::anim_idle );
    Audio::AudioManager::Instance().StopSpecificLoop( "LOOP_PUSHOBJECT.ogg" );
    pc.currScale = CurrScale::Normal;
    state_component.SetCurrent( ProcessPlayerScaleState::Instance(), UpdateStatus::Begin );
  }
}

State &ProcessPlayerInteractBoxState::Instance()
{
  static ProcessPlayerInteractBoxState s;
  return s;
}
}