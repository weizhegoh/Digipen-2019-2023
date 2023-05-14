/*****************************************************************
*\file         State.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "Xivi/ECS/XVECS.h"

namespace Xivi::Game
{
struct StateComponent;

/**
 * @brief
 * Base Game::State Interface
 */

struct State
{
  virtual void Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
  {};
  virtual void Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
  {};
  virtual void End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
  {};
  virtual std::string Name() noexcept = 0;
  virtual ~State() = default;
};

#define DefineGameState(STATE)                                                               \
  struct STATE : Game::State                                                                 \
  {                                                                                          \
    void Begin(StateComponent &state_component, ECS::World::Instance &) noexcept override;   \
    void Process(StateComponent &state_component, ECS::World::Instance &) noexcept override; \
    void End(StateComponent &state_component, ECS::World::Instance &) noexcept override;     \
    std::string Name() noexcept override { return #STATE; }                                  \
    static Game::State &Instance();                                                          \
  }

  /**
   * @brief
   * All the game states
   */
DefineGameState( IdleState );
DefineGameState( InterpolateState );
DefineGameState( ProcessPlayerDirectionState );
DefineGameState( ProcessPlayerMoveState );
DefineGameState( ProcessPlayerRotateState );
DefineGameState( ProcessPlayerScaleState );
DefineGameState( ProcessPlayerInteractBoxState );
DefineGameState( ProcessPlayerInteractSwitchState );
DefineGameState( ProcessPlayerInteractCloneState );

DefineGameState( ProcessEnvironmentState );
DefineGameState( ProcessFallingState );
DefineGameState( ProcessUnlockState );
DefineGameState( ProcessGameOverState );

DefineGameState( ProcessCollectibleState );
DefineGameState( ProcessGoalState );
DefineGameState( ProcessFallingFloorState );
DefineGameState( ProcessHoleState );
DefineGameState( ProcessPressurePlateState );

DefineGameState( ProcessCameraRotateState );
DefineGameState( ProcessCameraViewGoalState );

/**
 * @brief
 * Base State Component
 */

enum class UpdateStatus
{
  Begin,
  Process,
  End
};

DefineSingletonComponent( StateComponent );
Ptr<State> current { nullptr };
Ptr<State> next { nullptr };
UpdateStatus current_status { UpdateStatus::Begin };
UpdateStatus next_status { UpdateStatus::Begin };
bool init { true };
StateComponent()
{
  current = next = &IdleState::Instance();
  current_status = next_status = UpdateStatus::Begin;
}

void Switch()
{
  current = next;
  current_status = next_status;
  next = &IdleState::Instance();
  next_status = UpdateStatus::Begin;
}

void Switch( State &state, UpdateStatus status )
{
  current = &state;
  current_status = status;
  next = &IdleState::Instance();
  next_status = UpdateStatus::Begin;
}

void Set( State &c, UpdateStatus cs, State &n, UpdateStatus ns )
{
  current = &c;
  current_status = cs;
  next = &n;
  next_status = ns;
}

void SetCurrent( State &c, UpdateStatus cs )
{
  current = &c;
  current_status = cs;
}

void SetNext( State &n, UpdateStatus ns )
{
  next = &n;
  next_status = ns;
}

void Process( ECS::World::Instance &world )
{
  switch ( current_status )
  {
    case UpdateStatus::Begin:
    current->Begin( *this, world );
    break;
    case UpdateStatus::Process:
    current->Process( *this, world );
    break;
    case UpdateStatus::End:
    current->End( *this, world );
    break;
  }
}
EndDefine();

struct RotateHasher
{
  u64 operator()( const Vec3i &vec ) const
  {
    return Hash::XXH3_64( vec );
  }
};

inline unordered_map<Vec3i, f32, RotateHasher> directional_map =
{
    {Vec3i{-1, 0, 0}, 180.f},
    {Vec3i{1, 0, 0}, 0.f},
    {Vec3i{0, 0, 1}, 90.f},
    {Vec3i{0, 0, -1}, 270.f} };

inline vector<Entity> rotate_entity;
inline vector<f32> rotation;
inline bool god_mode { false };
}