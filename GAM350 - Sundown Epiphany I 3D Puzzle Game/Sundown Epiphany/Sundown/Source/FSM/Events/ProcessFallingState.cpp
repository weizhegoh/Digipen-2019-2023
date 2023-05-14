/*****************************************************************
*\file         ProcessFallingState.cpp
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
#ifndef XV_EDITOR
#include "Xivi/Graphics/Manager/GameStateManager.h"
#endif
namespace Xivi::Game
{
void ProcessFallingState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();

  ECS::Query::Instance check_fall;
  check_fall.AllOf<CheckFall>();
  world.ForEach( check_fall, [&]( Entity &entity, GridPosition &grid_pos )
  {
    if ( auto floor_cell = grid_pos.value + Vec3i( 0, -1, 0 );
         grid.BoundsCheck( floor_cell ) &&
         grid.GetCellType( floor_cell ) == GameType::Empty )
      world.AddOrRemoveComponents<CList<Falling>>( entity );
  } );
  state_component.SetCurrent( *this, UpdateStatus::Process );
}

// Process Environmental Falling
void ProcessFallingState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();

  ECS::Query::Instance environmental;
  environmental.Set<
    ECS::Query::All<Falling>,
    ECS::Query::Any<>,
    ECS::Query::None<Player>>( );

  auto DropOutOfWorld = [&]( Entity &entity, GridPosition &grid_pos, NewPosition &new_pos, Position &pos )
  {
    new_pos.value = pos.m_position;
    new_pos.value.setY( new_pos.value.getY() - 1000.0f );
    new_pos.ratio = 0.f;
    world.AddOrRemoveComponents<CList<Move>, CList<Falling>>( entity );
  };

  world.ForEach( environmental, [&]( Entity &entity, GridPosition &grid_pos, Position &pos, NewPosition &new_pos, EntityType &type )
  {
    switch ( type.value )
    {
      case GameType::Box:
      {
        if ( grid.DropBox( grid_pos.value ) && grid_pos.value != Vec3i( -999, -999, -999 ) )
        {
          new_pos.ratio = 0.f;
          new_pos.value = grid.WorldSpace( grid_pos.value ) + grid_pos.offset;
          world.AddOrRemoveComponents<CList<Move>, CList<Falling>>( entity );
          Audio::AudioManager::Instance().playSFX( "SFX_JUMPLAND.ogg", false );
        }
        else if ( grid_pos.value == Vec3i( -999, -999, -999 ) )
          DropOutOfWorld( entity, grid_pos, new_pos, pos );
        break;
      }
      case GameType::FallingFloor:
      {
        DropOutOfWorld( entity, grid_pos, new_pos, pos );
        break;
      }
    }
  } );
  state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, *this, UpdateStatus::End );
}

// Process Player Falling at End
void ProcessFallingState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
#ifndef XV_EDITOR
  auto &gsm = Service::Locator::Get<GSM::GSManager>();
  if ( gsm.isMainMenu() )
  {
    state_component.SetCurrent( IdleState::Instance(), UpdateStatus::Begin );
    return;
  }
#endif

  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();
  auto [grid_pos, new_pos] = world.GetComponents<GridPosition, NewPosition>( pc.playerEntity );

  bool dropped { grid.DropPlayer( grid_pos.value ) };

  if ( grid.DropPlayerEndGame( grid_pos.value ) )
  {
    Audio::AudioManager::Instance().playSFX( "SFX_PLAYERFALL.ogg", false );

    new_pos.value.setY( -1000.f );
    new_pos.ratio = 0.f;
    new_pos.speed = 0.2f;
    world.AddOrRemoveComponents<CList<Move>, CList<Falling>>( pc.playerEntity );
    world.ForEach( [&]( Camera_c &camera_c )
    {
      camera_c.dontFollow = true;
    } );
    state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, ProcessGameOverState::Instance(), UpdateStatus::Begin );
  }
  else if ( dropped )
  {
    new_pos.value = grid.WorldSpace( grid_pos.value ) + grid_pos.offset;
    new_pos.ratio = 0.f;
    world.AddOrRemoveComponents<CList<Move>, CList<Falling>>( pc.playerEntity );
    Audio::AudioManager::Instance().playSFX( "SFX_JUMPLAND.ogg", false );

    state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, IdleState::Instance(), UpdateStatus::Begin );
  }
  else
    state_component.SetCurrent( IdleState::Instance(), UpdateStatus::Begin );
}

State &ProcessFallingState::Instance()
{
  static ProcessFallingState s;
  return s;
}
}