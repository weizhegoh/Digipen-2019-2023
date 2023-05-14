/*****************************************************************
*\file         ProcessCameraRotateState.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#include "../State.h"
#include "NComponents.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "Systems/Audio/Audio.h"
#include "NEvents/StepEvents.h"

namespace Xivi::Game
{
/*
Static Var
*/
static f32 start;

/*
Helper
*/

bool CompareFloat( const f32 &lhs, const f32 &rhs )
{
  return std::fabs( lhs - rhs ) < Math::s_EPSILON;
};

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

/*
State
*/

void ProcessCameraRotateState::Begin( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();

  world.ForEach( [&]( Camera_c &camera_c )
  {
      //Camera Update Values
    //if ( camera_c.mMovable )
    //{
    start = camera_c.mTheta_initial;
    switch ( pc.action )
    {
      case PlayerAction::RotateCameraLeft:
      {
        camera_c.ratio = 0.f;
        camera_c.mTheta_initial += 45.f;
        break;
      }
      case PlayerAction::RotateCameraRight:
      {
        camera_c.ratio = 0.f;
        camera_c.mTheta_initial -= 45.f;
        break;
      }
    }
  } );

  state_component.Switch( *this, UpdateStatus::Process );
}

void ProcessCameraRotateState::Process( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{
  const auto &dt = Service::Locator::Get<TimerEngine>().GetDeltaTime();
  auto &pc = world.GetOrCreateSingletonComponent<PlayerController>();
  world.ForEach( [&]( Camera_c &camera_c )
  {
    camera_c.ratio += dt * 2.f;

    if ( camera_c.ratio < 1.f )
    {
      camera_c.SetTheta = Math::Lerp( start, camera_c.mTheta_initial, EaseInOut( camera_c.ratio ) );
    }
    else
    {
      if ( camera_c.mTheta_initial > 360.f )
        camera_c.mTheta_initial = 45.f;
      else if ( camera_c.mTheta_initial < 0.f )
        camera_c.mTheta_initial = 315.f;
      else if ( CompareFloat( camera_c.mTheta_initial, 360.f ) )
        camera_c.mTheta_initial = 0.f;

      camera_c.SetTheta = camera_c.mTheta_initial;
      camera_c.ratio = 0.f;
      pc.camera_angle = static_cast<i32>( camera_c.SetTheta );
      state_component.Switch( IdleState::Instance(), UpdateStatus::Begin );
    }
    XV_CORE_INFO( "Theta - {}", camera_c.SetTheta );
  } );
}

void ProcessCameraRotateState::End( StateComponent &state_component, ECS::World::Instance &world ) noexcept
{}

State &ProcessCameraRotateState::Instance()
{
  static ProcessCameraRotateState s;
  return s;
}
}