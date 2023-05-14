/*****************************************************************
*\file         ProcessPlayerInputSystem.cpp
*\author(s)    Tan Tong Wee
               Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"

#include "ProcessPlayerInputSystem.h"
#include "NComponents/Game/Game.h" // PlayerController, GameWorldState
#include "NEvents/StepEvents.h"
#include <Input/Manager.h>
#include <Service/Locator.h>
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "FSM/State.h"
namespace Xivi
{
static f32 input_dt { 0.f };
void ProcessPlayerInputSystem::OnEvent() noexcept
{
  #pragma region CameraControlLock
  bool control = true;
  ECS::Query::Instance cameraInitQuery;
  cameraInitQuery.AllOf<Camera_c>();
  ForEach( cameraInitQuery, [&]( Entity &entity, Camera_c &camera )
  {
    if ( camera.toGoal )
      control = false;
  } );
  if ( !control )
    return;
  #pragma endregion

  auto &keyboard = *Service::Locator::Get<Input::Manager>().Keyboard();
  auto gamepad = Ptr<Input::Device::Gamepad>( Service::Locator::Get<Input::Manager>().GetDevice( Input::Device::Type::Gamepad ) );

  // Early exit when no keys are pressed
  if ( !keyboard.AnyKey().Value() || ( gamepad && !gamepad->AnyKey().Value() ) )
  {
    input_dt = 0.f;
    return;
  }

  const auto &dt = Service::Locator::Get<TimerEngine>().GetDeltaTime();
  auto &pc = GetOrCreateSingletonComponent<PlayerController>();

  input_dt -= dt;

  if ( input_dt > 0.f )
    return;

// Character Action Checks
  if ( pc.lock_control )
    return;

  auto w = keyboard.W();
  auto a = keyboard.A();
  auto s = keyboard.S();
  auto d = keyboard.D();
  auto f = keyboard.F();
  auto left = keyboard.Left();
  auto right = keyboard.Right();
  auto tab = keyboard.Tab();
  auto f4 = keyboard.F4();

  if ( f4.Released() || ( gamepad && ( gamepad->LeftTrigger().Value() > 0.5f ) && ( gamepad->RightTrigger().Value() > 0.5f ) && gamepad->X().Pressed() ) )
    Game::god_mode = !Game::god_mode;

  if ( w.Pressed() || ( gamepad && gamepad->DPadUp().Pressed() ) )
    pc.action = PlayerAction::MoveForward;
  else if ( s.Pressed() || ( gamepad && gamepad->DPadDown().Pressed() ) )
    pc.action = PlayerAction::MoveBackward;
  else if ( a.Pressed() || ( gamepad && gamepad->DPadLeft().Pressed() ) )
  {
    input_dt = 0.2f;
    pc.action = PlayerAction::RotateLeft;
  }
  else if ( d.Pressed() || ( gamepad && gamepad->DPadRight().Pressed() ) )
  {
    input_dt = 0.2f;
    pc.action = PlayerAction::RotateRight;
  }
  else if ( f.Triggered() || ( gamepad && gamepad->Square().Triggered() ) )
    pc.action = PlayerAction::Interact;
  else if ( left.Pressed() || ( gamepad && gamepad->LeftShoulder().Pressed() ) )
    pc.action = PlayerAction::RotateCameraLeft;
  else if ( right.Pressed() || ( gamepad && gamepad->RightShoulder().Pressed() ) )
    pc.action = PlayerAction::RotateCameraRight;
  else if ( tab.Triggered() || ( gamepad && gamepad->Triangle().Triggered() ) )
    pc.action = PlayerAction::ViewGoal;
  else
    pc.action = PlayerAction::None;

  if ( pc.action != PlayerAction::None )
  {
    pc.lock_control = true;
    SendEvent<ProcessPlayerEvent>( this );
  }
}
}