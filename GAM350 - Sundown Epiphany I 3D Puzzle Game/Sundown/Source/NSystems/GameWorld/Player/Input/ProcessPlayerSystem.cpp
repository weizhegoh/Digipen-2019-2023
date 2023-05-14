/*****************************************************************
*\file         ProcessPlayerSystem.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"

#include "ProcessPlayerSystem.h"
#include "FSM/State.h"
#include "NComponents.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
namespace Xivi
{
void ProcessPlayerSystem::OnEvent() noexcept
{
  auto &grid = GetOrCreateSingletonComponent<GameGrid>();
  auto &pc = GetOrCreateSingletonComponent<PlayerController>();
  auto &game_state = GetOrCreateSingletonComponent<Game::StateComponent>();

  ForEach( [&]( Entity &entity, GridPosition &grid_position, Direction &direction, Player &p )
  {
    pc.playerEntity = entity;

    switch ( pc.action )
    {
      case PlayerAction::MoveForward:
      {
        game_state.Switch( Game::ProcessPlayerMoveState::Instance(), Game::UpdateStatus::Begin );
        break;
      }
      case PlayerAction::MoveBackward:
      {
        game_state.Switch( Game::ProcessPlayerMoveState::Instance(), Game::UpdateStatus::Begin );
        break;
      }
      case PlayerAction::RotateLeft: [[fallthrough]];
      case PlayerAction::RotateRight:
      {
        game_state.Switch( Game::ProcessPlayerRotateState::Instance(), Game::UpdateStatus::Begin );
        break;
      }
      case PlayerAction::Interact:
      {
        if ( grid.GetCellType( grid_position.value + Vec3i( direction.m_dir ) ) == GameType::Box )
        {
          if ( grid.IsEmpty( grid_position.value + Vec3i( direction.m_dir ) + Vec3i( direction.m_dir ) ) )
            game_state.Switch( Game::ProcessPlayerInteractBoxState::Instance(), Game::UpdateStatus::Begin );
          else
            game_state.Switch( Game::IdleState::Instance(), Game::UpdateStatus::Begin );
        }
        else if ( grid.GetCellType( grid_position.value + Vec3i( 0, -1, 0 ) ) == GameType::Switch )
          game_state.Switch( Game::ProcessPlayerInteractSwitchState::Instance(), Game::UpdateStatus::Begin );
        else if (/*player == small boy*/ grid.GetCellType( grid_position.value + Vec3i( 0, -1, 0 ) ) == GameType::PressurePlate )
          game_state.Switch( Game::ProcessPlayerInteractCloneState::Instance(), Game::UpdateStatus::Begin );
        break;
      }
      case PlayerAction::RotateCameraLeft: [[fallthrough]];
      case PlayerAction::RotateCameraRight:
      {
        game_state.Switch( Game::ProcessCameraRotateState::Instance(), Game::UpdateStatus::Begin );
        break;
      }
      case PlayerAction::ViewGoal:
      {
        game_state.Switch( Game::ProcessCameraViewGoalState::Instance(), Game::UpdateStatus::Begin );
        break;
      }
    }
  } );
}
}