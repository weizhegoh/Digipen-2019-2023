/*****************************************************************
*\file         GameGrid.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "../Entity/EntityType.h"
#include "Xivi/Core/Utils/Math.h"
namespace Xivi
{
struct Cell
{
  Entity entity;
  GameType type { GameType::Empty };
};

struct GridCells
{
  vector<vector<vector<Cell>>> value {};

  Cell &operator[]( const Vec3i & );
  const Cell &operator[]( const Vec3i & ) const;
};

DefineSingletonComponent( GameGrid );
Vec3f cell_size { 50.0f, 50.0f, 50.0f };
Vec3i grid_size { 0,0,0 };
GridCells grid_cells {};

GameGrid();

// Grid Modifier
void SetCellSize( const Vec3f &size );
void SetGridSize( const Vec3i &size );
void Clear();

// Cell Update
void UpdateCell( const Vec3i &cell, const GameType &type, const Entity &entity = Entity() );
void UpdateCell( const Vec3i &original_cell, const Vec3i &new_cell );

// Move Cell
bool MovePlayerForward( Vec3i &cell_to_move, const Vec3i &direction, bool &jump, Cell &output_displaced_cell );
bool MovePlayerBackward( Vec3i &cell_to_move, const Vec3i &direction, Cell &outpt_displaced_cell );

bool MovePlayer( Vec3i &cell_to_move, const Vec3i &direction, bool &jump, Cell &output_displaced_cell );
bool MoveBox( Vec3i &cell_to_move, const Vec3i &direction );
bool MovePlatform( Vec3i &cell_to_move, const Vec3i &direction, vector<std::pair<Entity, Vec3i>> &cells_above );
bool MoveObstacles( Vec3i &cell_to_move, const Vec3i &direction, Cell &output_displaced_cell );
bool MovePlatformOrObstacles( Vec3i &cell_to_move, const Vec3i &direction );
bool DropBox( Vec3i &cell_to_move ); // Drop box till non-empty space or hole
bool DropPlayer( Vec3i &cell_to_move ); // Drop player and move through till non-empty space
bool DropPlayerEndGame( Vec3i &cell_to_move ); // Drop player and move through till non-empty space then check game end

// Cell Details
bool IsEmpty( const Vec3i &cell );
bool IsMovableObject( const Vec3i &cell );
bool IsCollectibleType( const Vec3i &cell );
bool IsFloorType( const Vec3i &cell );
bool IsInteractableType( ECS::World::Instance &world, const Vec3i &cell );
GameType GetCellType( const Vec3i &cell );
Entity GetCellEntity( const Vec3i &cell );

// Cell Space Conversion
Vec3f WorldSpace( const Vec3i &cell );
bool BoundsCheck( const Vec3i &cell );
private:
  bool InternalMovePlayer( Vec3i &cell_to_move, Vec3i next_step, bool &jump, Cell &output_displaced_cell );
  bool MoveCell( Vec3i &cell_to_move, const Vec3i &direction );
  bool isBlockedTile( const Vec3i &cell, const Vec3i &direction );

  EndDefine();
}
namespace Xivi::Components::Reflect::GameGrid
{
RTTR_REGISTRATION
{
rttr::registration::class_<Xivi::Cell>( "Cell" )
.property( "Entity", &Xivi::Cell::entity )
.property( "Type", &Xivi::Cell::type );

rttr::registration::class_<Xivi::GridCells>( "Grid Cells" )
.property( "Cells", &Xivi::GridCells::value );

rttr::registration::class_<Xivi::GameGrid>( std::string( Xivi::GameGrid::info.m_name ) )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::GameGrid>.m_id ) )
.property( "Grid Size", &Xivi::GameGrid::grid_size )( rttr::policy::prop::as_reference_wrapper )
.property( "Cell Size", &Xivi::GameGrid::cell_size )( rttr::policy::prop::as_reference_wrapper )
.property( "Cells", &Xivi::GameGrid::grid_cells )( rttr::policy::prop::as_reference_wrapper );
}
}