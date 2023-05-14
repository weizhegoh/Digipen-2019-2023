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
static unordered_map<i32, std::pair<Vec3i, Vec3f>> up_map
{
  {0,{Vec3i{1,0,0}, Vec3f{0.f,270.f,0.f}}},
  {45,{Vec3i{0,0,1}, Vec3f{0.f,0.f,0.f}}},
  {90,{Vec3i{0,0,1}, Vec3f{0.f,0.f,0.f}}},
  {135,{Vec3i{0,0,1}, Vec3f{0.f,0.f,0.f}}},
  {180,{Vec3i{-1,0,0}, Vec3f{0.f,90.f,0.f}}},
  {225,{Vec3i{0,0,-1}, Vec3f{0.f,180.f,0.f}}},
  {270,{Vec3i{0,0,-1}, Vec3f{0.f,180.f,0.f}}},
  {315,{Vec3i{0,0,-1}, Vec3f{0.f,180.f,0.f}}},
};

static unordered_map<i32, std::pair<Vec3i, Vec3f>> down_map
{
  {0,{Vec3i{-1,0,0}, Vec3f{0.f,90.f,0.f}}},
  {45,{Vec3i{0,0,-1}, Vec3f{0.f,180.f,0.f}}},
  {90,{Vec3i{0,0,-1}, Vec3f{0.f,180.f,0.f}}},
  {135,{Vec3i{0,0,-1}, Vec3f{0.f,180.f,0.f}}},
  {180,{Vec3i{1,0,0}, Vec3f{0.f,270.f,0.f}}},
  {225,{Vec3i{0,0,1}, Vec3f{0.f,0.f,0.f}}},
  {270,{Vec3i{0,0,1}, Vec3f{0.f,0.f,0.f}}},
  {315,{Vec3i{0,0,1}, Vec3f{0.f,0.f,0.f}}},
};

static unordered_map<i32, std::pair<Vec3i, Vec3f>> left_map
{
  {0,{Vec3i{0,0,-1}, Vec3f{0.f,180.f,0.f}}},
  {45,{Vec3i{1,0,0}, Vec3f{0.f,270.f,0.f}}},
  {90,{Vec3i{1,0,0}, Vec3f{0.f,270.f,0.f}}},
  {135,{Vec3i{1,0,0}, Vec3f{0.f,270.f,0.f}}},
  {180,{Vec3i{0,0,1}, Vec3f{0.f,0.f,0.f}}},
  {225,{Vec3i{-1,0,0}, Vec3f{0.f,90.f,0.f}}},
  {270,{Vec3i{-1,0,0}, Vec3f{0.f,90.f,0.f}}},
  {315,{Vec3i{-1,0,0}, Vec3f{0.f,90.f,0.f}}},
};

static unordered_map<i32, std::pair<Vec3i, Vec3f>> right_map
{
  {0,{Vec3i{0,0,1}, Vec3f{0.f,0.f,0.f}}},
  {45,{Vec3i{-1,0,0}, Vec3f{0.f,90.f,0.f}}},
  {90,{Vec3i{-1,0,0}, Vec3f{0.f,90.f,0.f}}},
  {135,{Vec3i{-1,0,0}, Vec3f{0.f,90.f,0.f}}},
  {180,{Vec3i{0,0,-1}, Vec3f{0.f,180.f,0.f}}},
  {225,{Vec3i{1,0,0}, Vec3f{0.f,270.f,0.f}}},
  {270,{Vec3i{1,0,0}, Vec3f{0.f,270.f,0.f}}},
  {315,{Vec3i{1,0,0}, Vec3f{0.f,270.f,0.f}}},
};

static PlayerAction action_cache {};

void ProcessPlayerDirectionState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();

  auto RotateDirection = [&]( const Vec3i &new_direction_i, const Vec3f &new_direction_f )
  {
    auto [transform, direction] = world.GetComponents<Transform, Direction>( pc.playerEntity );
    auto curr_direction = Vec3i( direction.m_dir );

    if ( curr_direction == new_direction_i )
      pc.action = PlayerAction::MoveForward;
    else
    {
      pc.action = PlayerAction::MoveForward;
      transform.axisOffset = new_direction_f;
      transform.ratio = 0.f;
      world.AddOrRemoveComponents<CList<Rotate>>( pc.playerEntity );
    }
  };

  switch ( pc.action )
  {
    case PlayerAction::MoveUp:
    {
      auto &[vi, vf] = up_map[pc.camera_angle];
      RotateDirection( vi, vf );
      break;
    }
    case PlayerAction::MoveDown:
    {
      auto &[vi, vf] = down_map[pc.camera_angle];
      RotateDirection( vi, vf );
      break;
    }
    case PlayerAction::MoveLeft:
    {
      auto &[vi, vf] = left_map[pc.camera_angle];
      RotateDirection( vi, vf );
      break;
    }
    case PlayerAction::MoveRight:
    {
      auto &[vi, vf] = right_map[pc.camera_angle];
      RotateDirection( vi, vf );
      break;
    }
  }
  state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, *this, UpdateStatus::Process );
}

void ProcessPlayerDirectionState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  auto [grid_position, direction] = world.GetComponents<GridPosition, Direction>( pc.playerEntity );
  switch ( pc.action )
  {
    case PlayerAction::MoveForward:
    {
      if ( grid.GetCellType( grid_position.value + Vec3i( direction.m_dir ) ) == GameType::Box )
      {
        if ( grid.IsEmpty( grid_position.value + Vec3i( direction.m_dir ) + Vec3i( direction.m_dir ) ) )
          state_component.Switch( Game::ProcessPlayerInteractBoxState::Instance(), Game::UpdateStatus::Begin );
        else
          state_component.Switch( Game::ProcessPlayerMoveState::Instance(), Game::UpdateStatus::Begin );
      }
      else
        state_component.Switch( Game::ProcessPlayerMoveState::Instance(), Game::UpdateStatus::Begin );
      break;
    }
    case PlayerAction::MoveBackward:
    {
      state_component.Switch( Game::ProcessPlayerMoveState::Instance(), Game::UpdateStatus::Begin );
      break;
    }
  }
}

void ProcessPlayerDirectionState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

State &ProcessPlayerDirectionState::Instance()
{
  static ProcessPlayerDirectionState s;
  return s;
}
}