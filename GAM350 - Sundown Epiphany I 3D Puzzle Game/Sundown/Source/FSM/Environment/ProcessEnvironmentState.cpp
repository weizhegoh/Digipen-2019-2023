/*****************************************************************
*\file         ProcessEnvironmentState.cpp
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
void ProcessEnvironmentState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();

  ECS::Query::Instance no_move;
  no_move.NoneOf<Move>();
  world.ForEach( no_move, [&]( Entity &entity, Lock &lock, EntityType &type, GridPosition &grid_pos, NewPosition &new_pos, Waypoints &waypoints )
  {
    if ( lock.activated )
    {
      XV_CORE_ASSERT( type.value == GameType::MovingObstacle || type.value == GameType::MovingPlatform );
      auto next_waypoint_dir = waypoints.GetNextWaypoint() - grid_pos.value;

      if ( type.value == GameType::MovingPlatform )
      {
        vector<std::pair<Entity, Vec3i>> cells_above_platform;
        if ( grid.MovePlatform( grid_pos.value, next_waypoint_dir, cells_above_platform ) )
        {
          for ( auto &[entity_above, index] : cells_above_platform )
          {
            world.AddOrRemoveComponents<CList<Move>>( entity_above, [&]( GridPosition &above_grid_pos, NewPosition &above_new_pos )
            {
                    //Audio::AudioManager::Instance().playSFX("SFX_PLAYERFALL.ogg", false);

              above_grid_pos.value = index; // Sync moved obj grid pos
              above_new_pos.value = grid.WorldSpace( index ) + above_grid_pos.offset; // Move obj
              above_new_pos.ratio = 0.f;
            } );
          }

          new_pos.value = grid.WorldSpace( grid_pos.value ) + grid_pos.offset;
          new_pos.ratio = 0.f;
          world.AddOrRemoveComponents<CList<Move>>( entity );
        }
      }
      else if ( type.value == GameType::MovingObstacle )
      {
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
            Audio::AudioManager::Instance().playSFX( "SFX_OBSTACLEPUSH.ogg", false );
          } );
          new_pos.value = grid.WorldSpace( grid_pos.value ) + grid_pos.offset;
          new_pos.ratio = 0.f;
          world.AddOrRemoveComponents<CList<Move>>( entity );
        }
        else
        {
          if ( waypoints.reverse )
            ++waypoints.current_index;
          else
            --waypoints.current_index;
        }
      }
    }
  } );
  state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, *this, UpdateStatus::Process );
}

void ProcessEnvironmentState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
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
  state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, ProcessFallingState::Instance(), UpdateStatus::Begin );
}

void ProcessEnvironmentState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

State &ProcessEnvironmentState::Instance()
{
  static ProcessEnvironmentState s;
  return s;
}
}