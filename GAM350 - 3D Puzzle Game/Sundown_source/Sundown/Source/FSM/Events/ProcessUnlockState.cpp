/*****************************************************************
*\file         ProcessUnlockState.cpp
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
static Ptr<State> next_state { nullptr };
static UpdateStatus next_status { UpdateStatus::Begin };

void ProcessUnlockState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();

  #pragma region Activation
  ECS::Query::Instance activated;
  activated.AllOf<Activated>();
  next_state = state_component.next;
  next_status = state_component.next_status;
  state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, *this, UpdateStatus::Process );
  world.ForEach( activated, [&]( Entity &entity, Lock &lock, EntityType &type, GridPosition &grid_pos )
  {
    lock.activated = lock.activated ? false : true;

    if ( lock.activated )
    {
      switch ( type.value )
      {
        case GameType::MovingPlatform:
        {
          auto [waypoints, new_pos] = world.GetComponents<Waypoints, NewPosition>( entity );
          auto next_waypoint = waypoints.GetNextWaypoint();
          auto next_waypoint_dir = next_waypoint - grid_pos.value;

          vector<std::pair<Entity, Vec3i>> cells_above_platform;
          if ( grid.MovePlatform( grid_pos.value, next_waypoint_dir, cells_above_platform ) )
          {
            for ( auto &[entity_platform, index] : cells_above_platform )
            {
              world.AddOrRemoveComponents<CList<Move>>( entity_platform, [&]( GridPosition &above_grid_pos, NewPosition &above_new_pos )
              {
                above_grid_pos.value = index; // Sync moved obj grid pos
                above_new_pos.value = grid.WorldSpace( index ) + above_grid_pos.offset; // Move obj
                above_new_pos.ratio = 0.f;
              } );
            }
            new_pos.value = grid.WorldSpace( grid_pos.value ) + grid_pos.offset;
            new_pos.ratio = 0.f;
            world.AddOrRemoveComponents<CList<Move>, CList<Activated>>( entity );
          }
          break;
        }
        case GameType::MovingObstacle:
        {
          auto [waypoints, new_pos] = world.GetComponents<Waypoints, NewPosition>( entity );
          auto next_waypoint = waypoints.GetNextWaypoint();
          auto next_waypoint_dir = next_waypoint - grid_pos.value;
          Cell displaced_cell;
          if ( grid.MoveObstacles( grid_pos.value, next_waypoint_dir, displaced_cell ) )
          {
            if ( waypoints.GetNextDirection() != next_waypoint_dir )
            {
              auto dir = waypoints.GetNextDirection();
              rotate_entity.push_back( entity );
              rotation.push_back( directional_map[dir] );
            }

            if ( displaced_cell.entity.Valid() )
              world.AddOrRemoveComponents<CList<Move>>( displaced_cell.entity, [&]( GridPosition &displaced_grid_pos, NewPosition &displaced_new_pos )
            {
              displaced_grid_pos.value += next_waypoint_dir;
              displaced_new_pos.value = grid.WorldSpace( displaced_grid_pos.value ) + displaced_grid_pos.offset;
              displaced_new_pos.ratio = 0.f;
            } );
            new_pos.value = grid.WorldSpace( grid_pos.value ) + grid_pos.offset;
            new_pos.ratio = 0.f;
            world.AddOrRemoveComponents<CList<Move>, CList<Activated>>( entity );
          }
          break;
        }
        case GameType::Door:
        {
          type.value = GameType::UnlockedDoor;
          grid.UpdateCell( grid_pos.value, GameType::UnlockedDoor, entity );
          // Update Animation required
          // remove activated component
          auto &child = world.GetComponent<Child>( entity );
          for ( auto &c_entity : child.m_entities )
          {
            if ( !world.HaveComponent<DoorObj>( c_entity ) )
              continue;

            world.AddOrRemoveComponents<CList<Animating>>( c_entity, [&]( AnimatorComponent &anim )
            {
              anim.ActivateAnimationNextRun( 1, "Assets/Models/binary/Animation/Door_Close.BinaryAnimated" );
            } );
          }
          world.AddOrRemoveComponents<CList<>, CList<Activated>>( entity );
          Audio::AudioManager::Instance().playSFX( "SFX_OPENDOOR.ogg", false );
        }
        break;
      }
    }
    else if ( type.value == GameType::UnlockedDoor )
    {
      type.value = GameType::Door;
      grid.UpdateCell( grid_pos.value, GameType::Door, entity );
    // GameType::Door close and locked back in positon
    // remove activated component
      auto &child = world.GetComponent<Child>( entity );
      for ( auto &c_entity : child.m_entities )
      {
        if ( !world.HaveComponent<DoorObj>( c_entity ) )
          continue;

        world.AddOrRemoveComponents<CList<Animating>>( c_entity, [&]( AnimatorComponent &anim )
        {
          anim.ActivateAnimationNextRun( 1, "Assets/Models/binary/Animation/Door_Open.BinaryAnimated" );
        } );
      }
      world.AddOrRemoveComponents<CList<>, CList<Activated>>( entity );
      Audio::AudioManager::Instance().playSFX( "SFX_CLOSEDOOR.ogg", false );
    }
    else
      world.AddOrRemoveComponents<CList<>, CList<Activated>>( entity );
  } );
  #pragma endregion

  #pragma region Trigger
  ECS::Query::Instance triggered;
  triggered.AllOf<Triggered>();
  world.ForEach( triggered, [&]( Entity &entity, EntityType &type, GridPosition &grid_pos )
  {
    switch ( type.value )
    {
      case GameType::MovingPlatform:
      {
        auto [waypoints, new_pos] = world.GetComponents<Waypoints, NewPosition>( entity );
        auto next_waypoint = waypoints.GetNextWaypoint();
        auto next_waypoint_dir = next_waypoint - grid_pos.value;

        vector<std::pair<Entity, Vec3i>> cells_above_platform;
        if ( grid.MovePlatform( grid_pos.value, next_waypoint_dir, cells_above_platform ) )
        {
          for ( auto &[entity_platform, index] : cells_above_platform )
          {
            world.AddOrRemoveComponents<CList<Move>>( entity_platform, [&]( GridPosition &above_grid_pos, NewPosition &above_new_pos )
            {
              above_grid_pos.value = index; // Sync moved obj grid pos
              above_new_pos.value = grid.WorldSpace( index ) + above_grid_pos.offset; // Move obj
              above_new_pos.ratio = 0.f;
            } );
          }
          //todo moving platform sound
          Audio::AudioManager::Instance().playSFX( "SFX_MOVINGPLATFORM.ogg", false );
          new_pos.value = grid.WorldSpace( grid_pos.value ) + grid_pos.offset;
          new_pos.ratio = 0.f;
          world.AddOrRemoveComponents<CList<Move>, CList<Triggered>>( entity );
        }
        break;
      }
      case GameType::MovingObstacle:
      {
        auto [waypoints, new_pos] = world.GetComponents<Waypoints, NewPosition>( entity );
        auto next_waypoint = waypoints.GetNextWaypoint();
        auto next_waypoint_dir = next_waypoint - grid_pos.value;
        Cell displaced_cell;
        if ( grid.MoveObstacles( grid_pos.value, next_waypoint_dir, displaced_cell ) )
        {
          if ( waypoints.GetNextDirection() != next_waypoint_dir )
          {
            auto dir = waypoints.GetNextDirection();
            rotate_entity.push_back( entity );
            rotation.push_back( directional_map[dir] );
          }

          if ( displaced_cell.entity.Valid() )
            world.AddOrRemoveComponents<CList<Move>>( displaced_cell.entity, [&]( GridPosition &displaced_grid_pos, NewPosition &displaced_new_pos )
          {
            Audio::AudioManager::Instance().playSFX( "SFX_OBSTACLEPUSH.ogg", false );
            displaced_grid_pos.value += next_waypoint_dir;
            displaced_new_pos.value = grid.WorldSpace( displaced_grid_pos.value ) + displaced_grid_pos.offset;
            displaced_new_pos.ratio = 0.f;
          } );
          //todo moving obstacle sound
          Audio::AudioManager::Instance().playSFX( "SFX_MOVINGOBS.ogg", false );
          new_pos.value = grid.WorldSpace( grid_pos.value ) + grid_pos.offset;
          new_pos.ratio = 0.f;
          world.AddOrRemoveComponents<CList<Move>, CList<Triggered>>( entity );
        }
        break;
      }
      case GameType::Door:
      {
        type.value = GameType::UnlockedDoor;
        grid.UpdateCell( grid_pos.value, GameType::UnlockedDoor, entity );
        // Update Animation required
        // remove activated component
        auto &child = world.GetComponent<Child>( entity );
        for ( auto &c_entity : child.m_entities )
        {
          if ( !world.HaveComponent<DoorObj>( c_entity ) )
            continue;

          world.AddOrRemoveComponents<CList<Animating>>( c_entity, [&]( AnimatorComponent &anim )
          {
            anim.ActivateAnimationNextRun( 1, "Assets/Models/binary/Animation/Door_Close.BinaryAnimated" );
          } );
        }
        world.AddOrRemoveComponents<CList<>, CList<Triggered>>( entity );
        Audio::AudioManager::Instance().playSFX( "SFX_OPENDOOR.ogg", false );
      }
      break;
      case GameType::UnlockedDoor:
      {
        type.value = GameType::Door;
        grid.UpdateCell( grid_pos.value, GameType::Door, entity );
      // GameType::Door close and locked back in positon
      // remove activated component
        auto &child = world.GetComponent<Child>( entity );
        for ( auto &c_entity : child.m_entities )
        {
          if ( !world.HaveComponent<DoorObj>( c_entity ) )
            continue;

          world.AddOrRemoveComponents<CList<Animating>>( c_entity, [&]( AnimatorComponent &anim )
          {
            anim.ActivateAnimationNextRun( 1, "Assets/Models/binary/Animation/Door_Open.BinaryAnimated" );
          } );
        }
        world.AddOrRemoveComponents<CList<>, CList<Triggered>>( entity );
        Audio::AudioManager::Instance().playSFX( "SFX_CLOSEDOOR.ogg", false );
      }
      break;
      default:
      world.AddOrRemoveComponents<CList<>, CList<Triggered>>( entity );
    }
  } );

  #pragma endregion
}

void ProcessUnlockState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  for ( auto i = 0; i < rotate_entity.size(); ++i )
  {
    world.AddOrRemoveComponents<CList<Rotate>>( rotate_entity[i], [&]( Transform &transform )
    {
      transform.axisOffset.setX( transform.axis.getX() );
      transform.axisOffset.setY( transform.axis.getY() );
      transform.axisOffset.setZ( rotation[i] );
      transform.ratio = 0.f;
    } );
  }
  rotate_entity.clear();
  rotation.clear();
  state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, *next_state, next_status );
}

void ProcessUnlockState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

State &ProcessUnlockState::Instance()
{
  static ProcessUnlockState s;
  return s;
}
}