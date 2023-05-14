/*****************************************************************
*\file         ProcessFallingFloorState.cpp
*\author(s)    Kenric Tan Wei Liang

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
#include "Xivi/Core/Declaration.h"
#include <Resource/Manager.h>
namespace Xivi::Game
{
void ProcessFallingFloorState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();

  // Get falling floor tile
  auto [grid_pos, playerPos] = world.GetComponents<GridPosition, Position>( pc.playerEntity );
  auto falling_floor_entity = grid.GetCellEntity( grid_pos.value + Vec3i( 0, -1, 0 ) );

  auto [falling_floor, fallingMaterial, pos, newPos] = world.GetComponents<FallingFloor, Material, Position, NewPosition>( falling_floor_entity );

  if ( --falling_floor.counter == 0 )
  {
    grid.UpdateCell( grid_pos.value + Vec3i( 0, -1, 0 ), GameType::Hole, falling_floor_entity );
    world.AddOrRemoveComponents<CList<Falling>>( falling_floor_entity );
    state_component.SetCurrent( ProcessFallingState::Instance(), UpdateStatus::Begin ); // Let falling system handle endgame
  }
  else
  {
    Audio::AudioManager::Instance().playSFX( "SFX_FLOORBREAK.ogg", false );
    pos.shaky = true;
    playerPos.shaky = true;
    fallingMaterial.diffuse = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/Broken3_Texture_Diffuse.dds" );
    fallingMaterial.specular = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/Broken3_Texture_Diffuse.dds" );
    state_component.SetCurrent( ProcessEnvironmentState::Instance(), UpdateStatus::Begin );
  }
  // Process Animation
}

void ProcessFallingFloorState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

void ProcessFallingFloorState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

State &ProcessFallingFloorState::Instance()
{
  static ProcessFallingFloorState s;
  return s;
}
}