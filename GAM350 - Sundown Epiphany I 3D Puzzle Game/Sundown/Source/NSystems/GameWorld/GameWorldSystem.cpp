/*****************************************************************
*\file         GameWorldSystem.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "Xivi/ECS/XVECS.h"
#include "GameWorldSystem.h"
#include "NComponents/Game/GameWorldState.h"
#include "Components/GraphicsComponents.h"
#include "NComponents/Tags.h"
#include "NComponents.h"
#include "NSystems/GameWorld/Pause/PauseUnpauseSystem.h"
#ifndef XV_EDITOR
#include "Xivi/Graphics/Manager/GameStateManager.h"
#endif

namespace Xivi
{
#pragma region EasingFunction
f32 EaseIn( f32 t )
{
  return t * t;
}

f32 Flip( f32 t )
{
  return 1 - t;
}

f32 EaseOut( f32 t )
{
  auto f = Flip( t );
  return Flip( f * f );
}

f32 EaseInOut( f32 t )
{
  return Math::Lerp( EaseIn( t ), EaseOut( t ), t );
}
#pragma endregion

static unordered_map<Game::UpdateStatus, std::string> status_map
{
  {Game::UpdateStatus::Begin, "Begin"},
  {Game::UpdateStatus::Process, "Process"},
  {Game::UpdateStatus::End, "End"}
};

static bool init { true };

void GameWorldSystem::OnFrameStart() noexcept
{
  if ( m_paused )
    return;

  ECS::Query::Instance direction_initialise;
  direction_initialise.AllOf<DirectionInitialisationTag>();
  ForEach( direction_initialise, [&]( Entity &entity, Direction &direction, Transform &transform, Position &position )
  {
    setX( direction.m_dir, -std::sin( Math::DegToRad( transform.axis.getY() ) ) );
    setY( direction.m_dir, 0.f );
    setZ( direction.m_dir, std::cos( Math::DegToRad( transform.axis.getY() ) ) );
    AddOrRemoveComponents<CList<>, CList<DirectionInitialisationTag>>( entity );
  } );

  ECS::Query::Instance camera_init;
  camera_init.AllOf<CameraInitalized>();
  auto &pc = GetOrCreateSingletonComponent<PlayerController>();
  ForEach( camera_init, [&]( Entity &entity, Camera_c &c )
  {
    pc.camera_angle = static_cast<i32>( c.SetTheta );
  } );

  auto &state = GetOrCreateSingletonComponent<Game::StateComponent>();
  if ( state.init )
  {
    ForEach( []( Transform &transform )
    {
      transform.axisOffset = transform.axis;
    } );
    state.init = false;
  }

  m_world.m_archetype_manager.UpdateStructuralChanges();
}

void GameWorldSystem::OnUpdate() noexcept
{
  if ( m_paused )
    return;

  #ifndef XV_EDITOR
  auto &gsm = Service::Locator::Get<GSM::GSManager>();
  auto &cs = gsm.cutSceneFunc;
  if ( cs.isActive )
    return;
  #endif

  if ( GetSystem<PauseUnpauseSystem>().m_paused )
    return;

  SendEvent<PlayerInputEvent>( this );
  auto &state = GetOrCreateSingletonComponent<Game::StateComponent>();
  XV_CORE_INFO( "State - {}:{} ", state.current->Name(), status_map[state.current_status] );

  #ifndef XV_EDITOR
    if (gsm.isMainMenu() && state.current == &Game::IdleState::Instance())
    {
        state.SetCurrent(Game::ProcessEnvironmentState::Instance(), Game::UpdateStatus::Begin);
    }
  #endif
  state.Process( m_world );
  auto &grid = GetOrCreateSingletonComponent<GameGrid>();

  #pragma region Hovering
  auto Ease = []( const f32 &ratio, const HoverType &type )
  {
    switch ( type )
    {
      case HoverType::EaseIn: return EaseIn( ratio );
      case HoverType::EaseOut: return EaseOut( ratio );
      case HoverType::EaseInOut: return EaseInOut( ratio );
      case HoverType::Normal: return ratio;
      default: return 0.f;
    }
  };

  // Do Hover Cheese Effect
  ForEach( [&]( Entity &entity, Transform &transform, Position &position, Hover &hover )
  {
    // Handle Linear
    if ( hover.linear_ratio < 1.f )
      hover.linear_ratio = hover.linear_ratio + Service::Locator::Get<TimerEngine>().GetScaledDeltaTime() * hover.linear_speed;
    else
    {
      std::swap( hover.start, hover.end );
      hover.linear_ratio = 0.f;
    }
    position.m_position = Math::SIMD::lerp( hover.start, hover.end, Ease( hover.linear_ratio, hover.linear_type ) );

    // Handle Angular
    if ( hover.rotate_ratio < 1.f )
      hover.rotate_ratio = hover.rotate_ratio + Service::Locator::Get<TimerEngine>().GetScaledDeltaTime();
    else
    {
      std::swap( hover.rotate_speed_start, hover.rotate_speed_end );
      hover.rotate_ratio = 0.f;
    }

    hover.rotate_speed = Math::Lerp( hover.rotate_speed_start, hover.rotate_speed_end, hover.rotate_ratio );

    transform.axis += ( hover.rotate_dir * hover.rotate_speed );
    if ( transform.axis.getX() > 360.f )
      transform.axis.setX( 0.f );

    if ( transform.axis.getY() > 360.f )
      transform.axis.setY( 0.f );

    if ( transform.axis.getZ() > 360.f )
      transform.axis.setZ( 0.f );
  } );
  #pragma endregion

  #ifndef XV_EDITOR
  auto &pc = GetOrCreateSingletonComponent<PlayerController>();
  static f32 god_mode_timer;
  static Vec3f god_mode_color_f( 1.0f, 0.f, 0.f );
  static Vec3f god_mode_color_s( 0.f, 0.5f, 1.f );
  static f32 transparency_timer;
  static f32 transparency_start { 0.2f };
  static f32 transparency_end { 0.6f };

  GetEntity( pc.cloneEntity, []( RenderSettings &settings )
  {
    if ( transparency_timer < 1.f )
      transparency_timer += Service::Locator::Get<TimerEngine>().GetScaledDeltaTime();
    else
    {
      transparency_timer = 0.f;
      f32 tmp( transparency_start );
      transparency_start = transparency_end;
      transparency_end = tmp;
    }
    settings.transparent = Math::Lerp( transparency_start, transparency_end, transparency_timer );
  } );

  if ( Game::god_mode )
  {
    if ( god_mode_timer < 1.f )
      god_mode_timer += Service::Locator::Get<TimerEngine>().GetScaledDeltaTime();
    else
    {
      god_mode_timer = 0.f;
      Vec3f tmp( god_mode_color_f );
      god_mode_color_f = god_mode_color_s;
      god_mode_color_s = tmp;
    }
    GetEntity( pc.playerEntity, [&]( Colour &color )
    {
      color.colour = Math::SIMD::lerp( god_mode_color_f, god_mode_color_s, god_mode_timer );
    } );
  }
  else
  {
    god_mode_timer = 0.f;
    GetEntity( pc.playerEntity, [&]( Colour &color )
    {
      color.colour = Vec3f( 1.0f, 1.0f, 1.0f );
    } );
  }

  #endif
}

void GameWorldSystem::OnPause() noexcept
{
  m_paused = true;
  init = true;
}
}