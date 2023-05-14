/*****************************************************************
*\file         ProcessPlayerInteractSwitchState.cpp
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
#include "NEvents/StepEvents.h"

namespace Xivi::Game
{
static unordered_map<Vec3i, RotationDirection, RotateHasher> rotation_direction_map =
{
  { Vec3i{-1,0,0}, RotationDirection::Up},
  { Vec3i{1,0,0}, RotationDirection::Down},
  { Vec3i{0,0,1}, RotationDirection::Left },
  { Vec3i{0,0,-1}, RotationDirection::Right}
};

static unordered_map<RotationDirection, Vec3f> rotation_direction_vec_map =
{
  {  RotationDirection::Up,Vec3f{0.f,90.f,0.f}},
  { RotationDirection::Down,Vec3f{0.f,270.f,0.f}},
  { RotationDirection::Left, Vec3f{0.f,0.f,0.f}},
  {  RotationDirection::Right,Vec3f{0.f,180.f,0.f}}
};

void ProcessPlayerInteractSwitchState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();
  auto &state = world.GetOrCreateSingletonComponent<GameWorldState>();

  auto [player_gridpos, player_dir, player_transform] = world.GetComponents<GridPosition, Direction, Transform>( pc.playerEntity );
  auto &switch_dir = world.GetComponent< EntityDirection>( grid.GetCellEntity( player_gridpos.value + Vec3i( 0, -1, 0 ) ) );

  auto rotate_dir = rotation_direction_map[player_dir.m_dir];

  if ( rotate_dir != switch_dir.value )
  {
    // rotate player to face switch
    player_transform.axisOffset = rotation_direction_vec_map[switch_dir.value];
    player_transform.ratio = 0.f;
    world.AddOrRemoveComponents<CList<Rotate>>( pc.playerEntity );
  }

  state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, *this, UpdateStatus::Process );
}

void ProcessPlayerInteractSwitchState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();
  auto &state = world.GetOrCreateSingletonComponent<GameWorldState>();

  auto &player_gridpos = world.GetComponent<GridPosition>( pc.playerEntity );
  auto [child, switch_control] = world.GetComponents<Child, Switch>( grid.GetCellEntity( player_gridpos.value + Vec3i( 0, -1, 0 ) ) );
  for ( auto &entity : child.m_entities )
  {
    if ( !world.HaveComponent<SwitchObj>( entity ) )
      continue;

    //*
    // Old Code switching to single trigger
    //*
    // Activate switch
    world.AddOrRemoveComponents<CList<Triggered>>( switch_control.linked_entity );
    //auto &lock = world.GetComponent<Lock>( switch_control.linked_entity );
    auto [type] = world.GetComponents<EntityType>( switch_control.linked_entity );

    // Set reverse animation as default for activated
    AnimationState::AnimState slap_anim = AnimationState::AnimState::anim_open_switch;
    if ( !switch_control.triggered )
      slap_anim = AnimationState::AnimState::anim_close_switch;

    auto &mesh = world.GetComponent<AnimationMeshComponent>( entity );
    std::string filename = mesh.model.File();
    std::string switch_anim;

    if ( filename == "Assets/Models/binary/Animation/Switch_Pressed.BinaryAnimated" )
      switch_anim = "Assets/Models/binary/Animation/Switch_Pressed_Rev.BinaryAnimated";
    else if ( filename == "Lever2_Activated.BinaryAnimated" )
      switch_anim = "Lever2_Reversed.BinaryAnimated";
    else if ( filename == "Assets/Models/binary/Animation/Console_Pull_Down.BinaryAnimated" )
      switch_anim = "Assets/Models/binary/Animation/Console_Pull_Up.BinaryAnimated";
    else if ( filename == "Assets/Models/binary/Animation/lever3_activated.BinaryAnimated" )
      switch_anim = "Assets/Models/binary/Animation/lever3_reversed.BinaryAnimated";

    switch_control.triggered = !switch_control.triggered;

    world.AddOrRemoveComponents<CList<Animating>>( pc.playerEntity, [&]( AnimationState &anim_state )
    {
      anim_state.UpdateState( slap_anim );
    } );
    world.AddOrRemoveComponents<CList<Animating>>( entity, [&]( AnimatorComponent &anim )
    {
      anim.ActivateAnimationNextRun( 1, switch_anim );
    } );

    Audio::AudioManager::Instance().playSFX( "SFX_BUTTONSLAP.ogg", false );
    Audio::AudioManager::Instance().playSFX( "SFX_LEVERSWITCH.ogg", false );
    world.SendGlobalEvent<CloseSwitchPopupDialogue>();
    break;
  }
  // Set player to be interpolated
  //
  //pc.playerEntity = AddOrRemoveComponents<CList<Move>>( pc.playerEntity );
  state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, *this, UpdateStatus::End );
}

void ProcessPlayerInteractSwitchState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  auto &player_gridpos = world.GetComponent<GridPosition>( pc.playerEntity );
  auto [child, switch_control] = world.GetComponents<Child, Switch>( grid.GetCellEntity( player_gridpos.value + Vec3i( 0, -1, 0 ) ) );
  for ( auto &entity : child.m_entities )
  {
    if ( !world.HaveComponent<SwitchObj>( entity ) )
      continue;

  // Set reverse animation as default for activated
    auto &mesh = world.GetComponent<AnimationMeshComponent>( entity );
    std::string filename = mesh.model.File();
    std::string switch_anim;
    if ( filename == "Assets/Models/binary/Animation/Switch_Pressed_Rev.BinaryAnimated" )
      switch_anim = "Assets/Models/binary/Animation/Switch_Pressed.BinaryAnimated";
    else if ( filename == "Lever2_Reversed.BinaryAnimated" )
      switch_anim = "Lever2_Activated.BinaryAnimated";
    else if ( filename == "Assets/Models/binary/Animation/Console_Pull_Up.BinaryAnimated" )
      switch_anim = "Assets/Models/binary/Animation/Console_Pull_Down.BinaryAnimated";
    else if ( filename == "Assets/Models/binary/Animation/lever3_reversed.BinaryAnimated" )
      switch_anim = "Assets/Models/binary/Animation/lever3_activated.BinaryAnimated";

    world.AddOrRemoveComponents<CList<Animating>>( entity, [&]( AnimatorComponent &anim )
    {
      anim.ActivateAnimationNextRun( 1, switch_anim );
    } );
  }

  state_component.Set( InterpolateState::Instance(), UpdateStatus::Process, ProcessUnlockState::Instance(), UpdateStatus::Begin );
}

State &ProcessPlayerInteractSwitchState::Instance()
{
  static ProcessPlayerInteractSwitchState s;
  return s;
}
}