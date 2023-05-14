/*****************************************************************
*\file         GameGrid.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "GameGrid.h"
#include "NComponents.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"

namespace Xivi
{
constexpr Vec3i downward( 0, -1, 0 );
constexpr Vec3i upward( 0, 1, 0 );

Cell &GridCells::operator[]( const Vec3i &v )
{
  return value[v.x][v.y][v.z];
}

const Cell &GridCells::operator[]( const Vec3i &v ) const
{
  return value[v.x][v.y][v.z];
}

GameGrid::GameGrid()
{
  SetGridSize( Vec3i { 5,5,5 } );
}

// Grid Modifier
void GameGrid::SetCellSize( const Vec3f &size )
{
  cell_size = size;
}

void GameGrid::SetGridSize( const Vec3i &size )
{
  vector<vector<vector<Cell>>> new_cells( size.x, vector<vector<Cell>>( size.y, vector<Cell>( size.z ) ) );

  auto loop_x = size.x > grid_size.x ? grid_size.x : size.x;
  auto loop_y = size.y > grid_size.y ? grid_size.y : size.y;
  auto loop_z = size.z > grid_size.z ? grid_size.z : size.z;

  for ( auto x = 0; x < loop_x; ++x )
    for ( auto y = 0; y < loop_y; ++y )
      for ( auto z = 0; z < loop_z; ++z )
        new_cells[x][y][z] = grid_cells.value[x][y][z];

  std::swap( grid_cells.value, new_cells );
  grid_size = size;
}

void GameGrid::Clear()
{
  for ( auto x = 0; x < grid_size.x; ++x )
    for ( auto y = 0; y < grid_size.y; ++y )
      for ( auto z = 0; z < grid_size.z; ++z )
        grid_cells.value[x][y][z] = Cell();
}

// Cell Update
void GameGrid::UpdateCell( const Vec3i &cell, const GameType &type, const Entity &entity )
{
  auto &grid_cell = grid_cells[cell];
  grid_cell.type = type;
  grid_cell.entity = entity;
}

void GameGrid::UpdateCell( const Vec3i &original_cell, const Vec3i &new_cell )
{
  auto &original_grid_cell = grid_cells[original_cell];
  auto &new_grid_cell = grid_cells[new_cell];

  new_grid_cell = original_grid_cell;
  original_grid_cell.entity = Entity();
  original_grid_cell.type = GameType::Empty;
}

bool GameGrid::MovePlayerForward( Vec3i &cell_to_move, const Vec3i &direction, bool &jump, Cell &output_displaced_cell )
{
  XV_CORE_ASSERT( GetCellType( cell_to_move ) == GameType::Player );
  Vec3i next_step = cell_to_move + direction;

  if ( !BoundsCheck( next_step ) )
    return false;

  if ( IsEmpty( next_step ) ) // Check footing
  {
    auto next_step_floor = next_step + downward;
    if ( !BoundsCheck( next_step_floor ) ) // Check within bounds
      return false;
    if ( !IsFloorType( next_step_floor ) ) // If not floor type
    {
      if ( IsEmpty( next_step_floor ) ) // If is empty
      {
        auto next_step_floor_floor = next_step_floor + downward;
        if ( !BoundsCheck( next_step_floor_floor ) )
          return false;
        if ( !IsFloorType( next_step_floor_floor ) ) // if no place to jump down to
          return false;
        else // Jump down
        {
          next_step = next_step_floor;
          jump = true;
        }
      }
      else if ( grid_cells[next_step_floor].type == GameType::Cheese )
      {
        next_step = next_step_floor;
        jump = true;
      }
      else// Not floor type and is not empty
        return false;
    }
  }
  else if ( IsFloorType( next_step ) )
  {
    auto next_step_above = next_step + upward;
    if ( !BoundsCheck( next_step_above ) )
      return false;
    if ( ( !IsEmpty( next_step_above ) && grid_cells[next_step_above].type != GameType::Cheese ) ||
         !IsEmpty( cell_to_move + upward ) ) // Can jump on top of it
      return false;
    next_step = next_step_above;
    jump = true;
  }
  else if ( !IsCollectibleType( next_step ) ) // if not moving into collectible
    return false;

  output_displaced_cell = grid_cells[next_step];
  UpdateCell( cell_to_move, next_step );
  cell_to_move = next_step;
  return true;
}

bool GameGrid::MovePlayerBackward( Vec3i &cell_to_move, const Vec3i &direction, Cell &output_displaced_cell )
{
  XV_CORE_ASSERT( GetCellType( cell_to_move ) == GameType::Player );
  Vec3i next_step = cell_to_move + direction;
  if ( !BoundsCheck( next_step ) )
    return false;

  if ( IsEmpty( next_step ) ) // Check footing
  {
    auto next_step_floor = next_step + downward;
    if ( !BoundsCheck( next_step_floor ) ) // Check within bounds
      return false;
    if ( !IsFloorType( next_step_floor ) ) // If not floor type
      return false;
  }
  else if ( !IsCollectibleType( next_step ) ) // if not moving into collectible
    return false;

  output_displaced_cell = grid_cells[next_step];
  UpdateCell( cell_to_move, next_step );
  cell_to_move = next_step;
  return true;
}

// Move Cell
bool GameGrid::MovePlayer( Vec3i &cell_to_move, const Vec3i &direction, bool &jump, Cell &output_displaced_cell )
{
  XV_CORE_ASSERT( GetCellType( cell_to_move ) == GameType::Player );

  Vec3i next_step = cell_to_move + direction;
  return InternalMovePlayer( cell_to_move, next_step, jump, output_displaced_cell );
}

bool GameGrid::MoveBox( Vec3i &cell_to_move, const Vec3i &direction )
{
  XV_CORE_ASSERT( GetCellType( cell_to_move ) == GameType::Box );
  if ( !isBlockedTile( cell_to_move, direction ) && MoveCell( cell_to_move, direction ) )
    return true;
  return false;
}

bool GameGrid::MovePlatform( Vec3i &cell_to_move, const Vec3i &direction, vector<std::pair<Entity, Vec3i>> &cells_above )
{
  XV_CORE_ASSERT( GetCellType( cell_to_move ) == GameType::MovingPlatform );

  auto cell_above = cell_to_move + upward;
  vector<Vec3i> indexer;
  while ( BoundsCheck( cell_above ) && IsMovableObject( cell_above ) )
  {
    indexer.push_back( cell_above );
    cell_above += upward;
  }

  if ( direction.y == upward.y )
  {
    bool moved { false };

    indexer.insert( indexer.begin(), cell_to_move );
    for ( auto rev_it = indexer.rbegin(); rev_it != indexer.rend(); ++rev_it )
      if ( MoveCell( *rev_it, direction ) )
      {
        cells_above.push_back( { grid_cells[*rev_it].entity, *rev_it } );
        moved = true;
      }

    // Removed the base platform
    cell_to_move = cells_above.back().second;
    cells_above.pop_back();
    if ( moved )
      return true;
    return false;
  }

  if ( MoveCell( cell_to_move, direction ) )
  {
    for ( auto &index : indexer )
      if ( MoveCell( index, direction ) )
        cells_above.push_back( { grid_cells[index].entity, index } );
      else
        break;
    return true;
  }
  return false;
}

bool GameGrid::MoveObstacles( Vec3i &cell_to_move, const Vec3i &direction, Cell &output_displaced_cell )
{
  XV_CORE_ASSERT( GetCellType( cell_to_move ) == GameType::MovingObstacle );

  auto first_displaced_cell = cell_to_move + direction;

  if ( !BoundsCheck( first_displaced_cell ) )
    return false;

  if ( IsEmpty( first_displaced_cell ) )
    return MoveCell( cell_to_move, direction );

  auto second_displaced_cell = first_displaced_cell + direction;

  if ( !BoundsCheck( second_displaced_cell ) )
    return false;

  if ( !IsEmpty( second_displaced_cell ) || grid_cells[second_displaced_cell + downward].type == GameType::Blocked )
    return false;

  UpdateCell( first_displaced_cell, second_displaced_cell );
  output_displaced_cell = grid_cells[second_displaced_cell];
  UpdateCell( cell_to_move, first_displaced_cell );
  cell_to_move = first_displaced_cell;
  return true;
}

bool GameGrid::MovePlatformOrObstacles( Vec3i &cell_to_move, const Vec3i &direction )
{
  XV_CORE_ASSERT( GetCellType( cell_to_move ) == GameType::MovingPlatform ||
                  GetCellType( cell_to_move ) == GameType::MovingObstacle );
  if ( MoveCell( cell_to_move, direction ) )
    return true;
  return false;
}

bool GameGrid::DropBox( Vec3i &cell_to_move )
{
  XV_CORE_ASSERT( GetCellType( cell_to_move ) == GameType::Box );
  bool moved = false;
  while ( MoveCell( cell_to_move, downward ) )
    moved = true;
  if ( auto downward_cell = cell_to_move + downward;
       BoundsCheck( downward ) )
  {
    if ( GetCellType( downward_cell ) == GameType::Hole )
    {
      UpdateCell( cell_to_move, downward_cell );
      cell_to_move += downward;
      moved = true;
    }
    else if ( !IsFloorType( downward_cell ) )
    {
      grid_cells[cell_to_move].type = GameType::Empty;
      grid_cells[cell_to_move].entity = Entity();
      cell_to_move = Vec3i( -999, -999, -999 );
    }
  }
  else if ( moved && !BoundsCheck( downward_cell ) )
  {
    grid_cells[cell_to_move].type = GameType::Empty;
    grid_cells[cell_to_move].entity = Entity();
    cell_to_move = Vec3i( -999, -999, -999 );
  }

  return moved;
}

bool GameGrid::DropPlayer( Vec3i &cell_to_move )
{
  XV_CORE_ASSERT( GetCellType( cell_to_move ) == GameType::Player );
  bool moved = false;
  while ( MoveCell( cell_to_move, downward ) )
    moved = true;
  return moved;
}

bool GameGrid::DropPlayerEndGame( Vec3i &cell_to_move )
{
  XV_CORE_ASSERT( GetCellType( cell_to_move ) == GameType::Player );
  if ( GetCellType( cell_to_move + downward ) == GameType::Hole )
    return true;
  return false;
}

bool GameGrid::IsEmpty( const Vec3i &cell )
{
  return grid_cells[cell].type == GameType::Empty;
}

bool GameGrid::IsMovableObject( const Vec3i &cell )
{
  return grid_cells[cell].type == GameType::Player || grid_cells[cell].type == GameType::Box;
}

bool GameGrid::IsCollectibleType( const Vec3i &cell )
{
  return grid_cells[cell].type == GameType::Cheese ? true : false;
}

bool GameGrid::IsFloorType( const Vec3i &cell )
{
  switch ( grid_cells[cell].type )
  {
    case GameType::Goal: [[fallthrough]];
    case GameType::Floor: [[fallthrough]];
    case GameType::FallingFloor: [[fallthrough]];
    case GameType::MovingPlatform: [[fallthrough]];
    case GameType::PressurePlate: [[fallthrough]];
    case GameType::Switch: [[fallthrough]];
    case GameType::UnlockedDoor: [[fallthrough]];
    case GameType::Hole: [[fallthrough]];
    case GameType::Box:
    return true;
  }
  return false;
}

bool GameGrid::IsInteractableType( ECS::World::Instance &world, const Vec3i &cell )
{
  switch ( grid_cells[cell].type )
  {
    case GameType::PressurePlate: [[fallthrough]];
    case GameType::Switch:
    return true;
  }

  bool found = false;
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  world.ForEach( [&]( Entity &entity, GridPosition &grid_position, Direction &direction, Player &p )
  {
    if ( grid.GetCellType( grid_position.value + Vec3i( direction.m_dir ) ) == GameType::Box )
      found = true;
    return found;
  } );
  return found;
}

GameType GameGrid::GetCellType( const Vec3i &cell )
{
  XV_CORE_ASSERT( BoundsCheck( cell ) );
  return grid_cells[cell].type;
}

Entity GameGrid::GetCellEntity( const Vec3i &cell )
{
  XV_CORE_ASSERT( BoundsCheck( cell ) );
  return grid_cells[cell].entity;
}

Vec3f GameGrid::WorldSpace( const Vec3i &cell )
{
  auto min = Vec3f { cell_size * Vec3f{(f32) cell.x, (f32) cell.y,(f32) cell.z} };
  auto max = Vec3f { cell_size * Vec3f{(f32) ( cell.x + 1 ), (f32) ( cell.y + 1 ), (f32) ( cell.z + 1 )} };
  return ( min + max ) * 0.5f;
}

bool GameGrid::InternalMovePlayer( Vec3i &cell_to_move, Vec3i next_step, bool &jump, Cell &output_displaced_cell )
{
  if ( !BoundsCheck( next_step ) )
    return false;

  if ( IsEmpty( next_step ) ) // Check footing
  {
    auto next_step_floor = next_step + downward;
    if ( !BoundsCheck( next_step_floor ) ) // Check within bounds
      return false;
    if ( !IsFloorType( next_step_floor ) ) // If not floor type
    {
      if ( IsEmpty( next_step_floor ) ) // If is empty
      {
        auto next_step_floor_floor = next_step_floor + downward;
        if ( !BoundsCheck( next_step_floor_floor ) )
          return false;
        if ( !IsFloorType( next_step_floor_floor ) ) // if no place to jump down to
          return false;
        else // Jump down
        {
          next_step = next_step_floor;
          jump = true;
        }
      }
      else if ( grid_cells[next_step_floor].type == GameType::Cheese )
      {
        next_step = next_step_floor;
        jump = true;
      }
      else// Not floor type and is not empty
        return false;
    }
  }
  else if ( IsFloorType( next_step ) )
  {
    auto next_step_above = next_step + upward;
    if ( !BoundsCheck( next_step_above ) )
      return false;
    if ( ( !IsEmpty( next_step_above ) && grid_cells[next_step_above].type != GameType::Cheese ) ||
         !IsEmpty( cell_to_move + upward ) ) // Can jump on top of it
      return false;
    next_step = next_step_above;
    jump = true;
  }
  else if ( !IsCollectibleType( next_step ) ) // if not moving into collectible
    return false;

  output_displaced_cell = grid_cells[next_step];
  UpdateCell( cell_to_move, next_step );
  cell_to_move = next_step;
  return true;
}
bool GameGrid::MoveCell( Vec3i &cell_to_move, const Vec3i &direction )
{
  if ( IsEmpty( cell_to_move ) ) return false;

  Vec3i next_step = cell_to_move + direction;
  if ( !BoundsCheck( next_step ) || !IsEmpty( next_step ) )
    return false;
  UpdateCell( cell_to_move, next_step );
  cell_to_move = next_step;
  return true;
}

bool GameGrid::BoundsCheck( const Vec3i &cell )
{
  if ( cell.x < 0 || cell.x >= grid_size.x ) return false;
  if ( cell.y < 0 || cell.y >= grid_size.y ) return false;
  if ( cell.z < 0 || cell.z >= grid_size.z ) return false;
  return true;
}

bool GameGrid::isBlockedTile( const Vec3i &cell, const Vec3i &direction )
{
  Vec3i next_step = cell + direction;
  Vec3i floor_step = next_step + downward;

  if ( BoundsCheck( next_step ) && BoundsCheck( floor_step ) )
  {
    if ( GetCellType( floor_step ) == GameType::Blocked )
      return true;

    else
      return false;
  }
  return false;
}
}