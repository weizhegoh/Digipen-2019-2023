/*****************************************************************
*\file         ProcessPlayerRotateState.cpp
*\author(s)    Tan Tong Wee
               Kenric Tan Wei Liang

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
  /**
   * @brief
   * Helper functions
   */

void ProcessJump( ECS::World::Instance &world,
                  PlayerController &pc,
                  AnimationState &state,
                  GameGrid &grid,
                  NewPosition &new_pos,
                  Position &pos,
                  Direction &dir,
                  GridPosition &grid_pos,
                  Vec3i original_grid )
{
  state.UpdateState( AnimationState::AnimState::anim_jump_loop );
  Audio::AudioManager::Instance().playSFX( "SFX_JUMP.ogg", false );
  new_pos.bezier_point0 = pos.m_position;
  new_pos.speed = 3.f;
  Vec3f y_offset;
  if ( original_grid.y - grid_pos.value.y >= 1 )
  {
    y_offset.setY( grid.cell_size.getY() * 2.f );
    y_offset.setX( grid.cell_size.getX() * dir.m_dir.getX() * .2f );
    y_offset.setZ( grid.cell_size.getZ() * dir.m_dir.getZ() * .2f );
  }
  else
  {
    y_offset.setY( grid.cell_size.getY() * 1.5f );
    y_offset.setX( grid.cell_size.getX() * -dir.m_dir.getX() * .75f );
    y_offset.setZ( grid.cell_size.getZ() * -dir.m_dir.getZ() * .75f );
  }

  new_pos.bezier_point1 = grid.WorldSpace( grid_pos.value ) + y_offset;
  pc.playerEntity = world.AddOrRemoveComponents<CList<Move, BezierTag>>( pc.playerEntity );
}

void ProcessMoveBackward( ECS::World::Instance &world,
                          PlayerController &pc,
                          AnimationState &state,
                          GameGrid &grid,
                          NewPosition &new_pos,
                          Position &pos,
                          GridPosition &grid_pos )
{
  state.UpdateState( AnimationState::AnimState::anim_jump_loop );
  Audio::AudioManager::Instance().playSFX( "SFX_JUMP.ogg", false );
  new_pos.bezier_point0 = pos.m_position;
  new_pos.speed = 3.f;
  new_pos.bezier_point1 = ( pos.m_position + grid.WorldSpace( grid_pos.value ) ) / 2.f;
  new_pos.bezier_point1.setY( new_pos.bezier_point1.getY() + 25.f );
  pc.playerEntity = world.AddOrRemoveComponents<CList<Move, BezierTag>>( pc.playerEntity );
}

void ProcessMoveForward( ECS::World::Instance &world, PlayerController &pc, AnimationState &state )
{
  state.UpdateState( AnimationState::AnimState::anim_running );
  Audio::AudioManager::Instance().playSFX( "SFX_MOVEMENT.ogg", false );
  pc.playerEntity = world.AddOrRemoveComponents<CList<Move>>( pc.playerEntity );
}

/**
 * @brief
 * Static variables
 */

static f32 original_speed {};
static Cell next_step_cell {};
static Vec3i next_step_grid {};
static Entity pressure_plate_entity {};
static Ptr<State> movement_end_state;
static UpdateStatus movement_end_status;
static bool jump = false;

void ProcessPlayerMoveState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  jump = false;
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();

  auto [pos, newPos, gridPos, dir, player, animationState, transform] =
    world.GetComponents<Position, NewPosition, GridPosition, Direction, Player, AnimationState, Transform>( pc.playerEntity );

  // Reinitialised
  pressure_plate_entity = Entity {};
  original_speed = newPos.speed;
  next_step_cell = Cell {};

  bool moved = false;
  Vec3i original_grid = gridPos.value;

  auto Update = [&]()
  {
    pc.previous_tile = original_grid;
    moved = true;
    pos.shaky = false;
    ++player.moves_count;
    world.SendGlobalEvent<UpdateGUIMoveEvent>( player.moves_total - player.moves_count );
    next_step_grid = gridPos.value;
    newPos.value = grid.WorldSpace( gridPos.value ) + gridPos.offset;
    newPos.ratio = 0.f;
  };

  switch ( pc.action )
  {
    case PlayerAction::MoveForward:
    {
      if ( grid.MovePlayerForward( gridPos.value, Vec3i( dir.m_dir ), jump, next_step_cell ) )
      {
        Update();
        if ( jump )
          ProcessJump( world, pc, animationState, grid, newPos, pos, dir, gridPos, original_grid );
        else
          ProcessMoveForward( world, pc, animationState );
      }
      break;
    }
    case PlayerAction::MoveBackward:
    {
      if ( grid.MovePlayerBackward( gridPos.value, Vec3i( -dir.m_dir ), next_step_cell ) )
      {
        Update();
        ProcessMoveBackward( world, pc, animationState, grid, newPos, pos, gridPos );
      }
      break;
    }
  }

  if ( moved )
  {
    // Already moved at this point
    state_component.Set( InterpolateState::Instance(), UpdateStatus::Process,
                         *this, UpdateStatus::Process );

                       // Check original floor cell type for pressure plate
    Vec3i original_grid_floor = original_grid + Vec3i( 0, -1, 0 );

    if ( grid.GetCellType( original_grid_floor ) == GameType::PressurePlate )
    {
      auto processPressure = [&]
      {
        pressure_plate_entity = grid.GetCellEntity( original_grid_floor );
        auto [switch_control, child] = world.GetComponents<Switch, Child>( pressure_plate_entity );
        world.AddOrRemoveComponents<CList<Activated, Animating>>( switch_control.linked_entity );
        for ( auto c : child.m_entities )
          if ( world.HaveComponent<PressurePlateObj>( c ) )
          {
            auto &animator = world.GetComponent<AnimatorComponent>( c );
            animator.ActivateAnimationNextRun( 1, "Assets/Models/binary/Animation/PressurePlate_Pressed.BinaryAnimated" );
          }
        Audio::AudioManager::Instance().playSFX( "SFX_PLATERELEASE.ogg", false );
      };

      if ( pc.cloneEntity.Valid() )
      {
        auto &gridPosClone = world.GetComponent<GridPosition>( pc.cloneEntity );
        auto gridFloorClone = gridPosClone.value + Vec3i( 0, -1, 0 );
        if ( !( original_grid_floor == gridFloorClone ) )
          processPressure();
      }
      else
        processPressure();
    }
  }
  else
    state_component.Switch( IdleState::Instance(), UpdateStatus::Begin );
}

void ProcessPlayerMoveState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();
  auto [anim_state, pos] = world.GetComponents<AnimationState, NewPosition>( pc.playerEntity );
  anim_state.UpdateState( AnimationState::AnimState::anim_idle );
  pos.speed = original_speed;
  if ( jump )
    Audio::AudioManager::Instance().playSFX( "SFX_JUMPLAND.ogg", false );

  state_component.current_status = UpdateStatus::End;
}

void ProcessPlayerMoveState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();

  // Process pressure plate tile that you are leaving
  if ( pressure_plate_entity.Valid() )
  {
    state_component.Set( ProcessUnlockState::Instance(), UpdateStatus::Begin, *this, UpdateStatus::End );
    pressure_plate_entity = Entity();
  }
  else
  {
    auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
    auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();
    state_component.Switch( ProcessEnvironmentState::Instance(), UpdateStatus::Begin );
    if ( next_step_cell.type == GameType::Cheese )
    {
      world.DestroyEntity( next_step_cell.entity );
      state_component.Switch( ProcessCollectibleState::Instance(), UpdateStatus::Begin );
    }
    else
    {
      auto next_cell_type_floor = grid.GetCellType( next_step_grid + Vec3i( 0, -1, 0 ) );
      switch ( next_cell_type_floor )
      {
        case GameType::Goal: state_component.Switch( ProcessGoalState::Instance(), UpdateStatus::Begin ); break;
        case GameType::FallingFloor: state_component.Switch( ProcessFallingFloorState::Instance(), UpdateStatus::Begin ); break;
        case GameType::PressurePlate: state_component.Switch( ProcessPressurePlateState::Instance(), UpdateStatus::Begin ); break;
        case GameType::Hole: state_component.Switch( ProcessFallingState::Instance(), UpdateStatus::Begin ); break;
      }
    }
  }
}

State &ProcessPlayerMoveState::Instance()
{
  static ProcessPlayerMoveState s;
  return s;
}
}