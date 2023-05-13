/*****************************************************************
*\file         ProcessPressurePlateState.cpp
*\author(s)    Tan Tong Wee
               Kenric Tan Wei Liang

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

namespace Xivi::Game
{
void ProcessPressurePlateState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();

  auto [grid_pos, new_position] = world.GetComponents<GridPosition, NewPosition>( pc.playerEntity );
  auto switch_tile_entity = grid.GetCellEntity( grid_pos.value + Vec3i( 0, -1, 0 ) );
  auto presPos = grid_pos.value + Vec3i( 0, -1, 0 );

  if ( pc.cloneEntity.Valid() )
  {
    auto &gridPosClone = world.GetComponent<GridPosition>( pc.cloneEntity );
    auto gridFloorClone = gridPosClone.value + Vec3i( 0, -1, 0 );
    auto presPos = grid_pos.value + Vec3i( 0, -1, 0 );
    if ( presPos == gridFloorClone )
    {
      state_component.Switch( ProcessEnvironmentState::Instance(), UpdateStatus::Begin );
      return;
    }
  }
  // Get tile's children below player
  if ( world.HaveComponent<Child>( switch_tile_entity ) )
  {
    auto [child, switch_control] = world.GetComponents<Child, Switch>( switch_tile_entity );
    world.AddOrRemoveComponents<CList<Activated>>( switch_control.linked_entity );
    bool no_pressure_plate_obj { true };
    for ( auto &entity : child.m_entities ) // Get pressure plate entity
      if ( world.HaveComponent<PressurePlateObj>( entity ) )
      {
        no_pressure_plate_obj = false;
        world.GetComponent<AnimatorComponent>( entity ).ActivateAnimationNextRun( 1, "Assets/Models/binary/Animation/PressurePlate_PressedReverse.BinaryAnimated" );
        Audio::AudioManager::Instance().playSFX( "SFX_PLATEPRESS.ogg", false );
        state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, ProcessPressurePlateState::Instance(), UpdateStatus::Process );
      }

    if ( no_pressure_plate_obj ) state_component.Switch( IdleState::Instance(), UpdateStatus::Begin );
  }
  else
  {
    XV_CORE_ERROR( "Missing switch object on switch tile" );
    state_component.Switch( ProcessEnvironmentState::Instance(), UpdateStatus::Begin );
  }
}

void ProcessPressurePlateState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  state_component.Set( ProcessUnlockState::Instance(), UpdateStatus::Begin, ProcessEnvironmentState::Instance(), UpdateStatus::Begin );
}

void ProcessPressurePlateState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

State &ProcessPressurePlateState::Instance()
{
  static ProcessPressurePlateState s;
  return s;
}
}