/*****************************************************************
*\file         UIButtonMovementSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "XiviEngine.h"
#include "Xivi/Graphics/Manager/GameStateManager.h"
#include "UIButtonMovementSystem.h"
#include "HelperFunctions/GameGUIHelperFunction.h"

namespace Xivi
{
bool UIButtonMovementSystem::ButtonSort::operator()( const Vec2f &lhs, const Vec2f &rhs ) const noexcept
{
  // Lesser x first if both y are equal
  if ( lhs.y == rhs.y )
  {
    return lhs.x < rhs.x;
  }
  // Bigger y first
  return lhs.y > rhs.y;
}

void UIButtonMovementSystem::OnCreate() noexcept
{
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();
  m_audio_manager = &Audio::AudioManager::Instance();
  m_timer = &Service::Locator::Get<TimerEngine>();
  m_keyboard = Service::Locator::Get<Input::Manager>().Keyboard();
}

void SetPressCooldown( Ptr<TimerEngine> timer, bool &holding_key, float &cd_timer ) noexcept
{
  holding_key = true;
  cd_timer = timer->GetElapsedTime();
}

void UIButtonMovementSystem::OnEvent() noexcept
{
  bool is_instruction_pg = false;
  // Get UI from stack
  m_buttons_arrangement.clear();
  for ( auto &ui : *m_canvas_stack->Get() )
  {
    if ( HaveComponent<GUI::InstructionCanvasTag>( ui ) )
      is_instruction_pg = true;
    if ( HaveComponent<GUI::UIHover>( ui ) )
    {
      auto &pos = GetComponent<Position>( ui ).m_position;
      m_buttons_arrangement.insert( { Vec2f{pos.getZ(), pos.getY()}, ui } );
    }
  }

  m_canvas_stack->SetAllFuncByKey();

  if ( m_hovered_button.Valid() )
  {
    bool found = false;
    for ( auto &[pos, entity] : m_buttons_arrangement )
    {
      if ( entity == m_hovered_button )
      {
        found = true;
        break;
      }
    }

    if ( !found )
    {
      GetComponent<GUI::UIHover>( m_hovered_button ).m_callback_unhover( m_hovered_button );
      m_hovered_button = Entity {};
      return;
    }
  }

  // Do nothing if empty
  if ( m_buttons_arrangement.empty() )
  {
    m_hovered_button = Entity {};
    return;
  }

  // Get the first hover
  if ( !m_hovered_button.Valid() && !m_buttons_arrangement.empty() )
  {
    m_hovered_button = m_buttons_arrangement.begin()->second;
    GetComponent<GUI::UIHover>( m_hovered_button ).m_callback( m_hovered_button );
  }

  // For axis check
  static const auto dead_zone_threshold = 0.3f;
  // For hold cooldown
  static const auto cooldown_switch_button = 0.2f;
  static auto cooldown_timer = m_timer->GetElapsedTime();

  #ifndef XV_EDITOR
  auto &gsm = Service::Locator::Get<GSM::GSManager>();
  auto &cs = gsm.cutSceneFunc;
  if ( cs.isActive )
    return;
  #endif

      // Cooldown over
  m_holding_axis = !( m_holding_axis && m_timer->GetElapsedTime() - cooldown_timer > cooldown_switch_button );

  // For controller
  auto device1 = Service::Locator::Get<Input::Manager>().GetDevice( "Gamepad[0]" );

  bool controller_up = false;
  bool controller_down = false;
  bool controller_left = false;
  bool controller_right = false;
  bool controller_select = false;
  if ( device1 )
  {
    auto &gamepad = *static_cast<Ptr<Input::Device::Gamepad>>( device1 );
    //auto left_axis = gamepad.LeftStickAxis();

    controller_up = gamepad.DPadUp().Pressed();
    controller_down = gamepad.DPadDown().Pressed();
    controller_left = gamepad.DPadLeft().Pressed();
    controller_right = gamepad.DPadRight().Pressed();

    //if (!m_holding_axis && (std::abs(left_axis.X()) > dead_zone_threshold || std::abs(left_axis.Y()) > dead_zone_threshold))
    //{
    //	// Down
    //	if (left_axis.Y() < 0)
    //		controller_down = true;
    //	// Up
    //	if (left_axis.Y() > 0)
    //		controller_up = true;
    //	// Right
    //	if (left_axis.X() > 0)
    //		controller_right = true;
    //	// Left
    //	if (left_axis.X() < 0)
    //		controller_left = true;

    //	// Decide which way to hover
    //	if (controller_down && controller_left)
    //	{
    //		controller_down = left_axis.Y() < left_axis.X();
    //		controller_left = !controller_down;
    //	}
    //	else if (controller_down && controller_right)
    //	{
    //		controller_down = Math::Abs(left_axis.Y()) > left_axis.X();
    //		controller_right = !controller_down;
    //	}
    //	else if (controller_up && controller_left)
    //	{
    //		controller_up = left_axis.Y() > Math::Abs(left_axis.X());
    //		controller_left = !controller_up;
    //	}
    //	else if (controller_up && controller_right)
    //	{
    //		controller_up = left_axis.Y() > left_axis.X();
    //		controller_right = !controller_up;
    //	}
    //}

    // If press 'X'(PS4) or 'A'
    if ( gamepad.South().Triggered() )
      controller_select = true;
  }

  // Check if can change (cooldown over)
  bool trigger_down =
    ( m_keyboard->Down().Pressed() ||
      m_keyboard->Right().Pressed() ||
      m_keyboard->S().Pressed() ||
      m_keyboard->D().Pressed() ||
      controller_down ||
      controller_right ) &&
    !m_holding_axis;

  bool trigger_up =
    ( m_keyboard->Up().Pressed() ||
      m_keyboard->Left().Pressed() ||
      m_keyboard->W().Pressed() ||
      m_keyboard->A().Pressed() ||
      controller_up ||
      controller_left ) &&
    !m_holding_axis;

  bool trigger_select =
    m_keyboard->Enter().Triggered() ||
    controller_select;

  bool press_left =
    ( m_keyboard->Left().Pressed() ||
      m_keyboard->A().Pressed() ||
      controller_left ) &&
    !m_holding_axis;

  bool press_right =
    ( m_keyboard->Right().Pressed() ||
      m_keyboard->D().Pressed() ||
      controller_right ) &&
    !m_holding_axis;

  if ( HaveComponent<GUI::VolumeBarTag>( m_hovered_button ) || HaveComponent<GUI::GammaBarTag>( m_hovered_button ) || is_instruction_pg )
  {
    trigger_down =
      ( m_keyboard->Down().Pressed() ||
        m_keyboard->S().Pressed() ||
        controller_down ) &&
      !m_holding_axis;
    trigger_up =
      ( m_keyboard->Up().Pressed() ||
        m_keyboard->W().Pressed() ||
        controller_up ) &&
      !m_holding_axis;
  }

  // If press can press down
  if ( trigger_down )
  {
    Entity next_button;
    for ( auto it = m_buttons_arrangement.begin(); it != m_buttons_arrangement.end(); ++it )
    {
      // If last button, move to first
      if ( std::next( it, 1 ) != m_buttons_arrangement.end() )
        next_button = ( std::next( it, 1 ) )->second;
      // Go next
      else
        next_button = m_buttons_arrangement.begin()->second;

      auto &pos = GetComponent<Position>( m_hovered_button ).m_position;
      if ( it->first == Vec2f { pos.getZ(), pos.getY() } )
      {
        GetComponent<GUI::UIHover>( m_hovered_button ).m_callback_unhover( m_hovered_button );
        m_hovered_button = next_button;
        GetComponent<GUI::UIHover>( m_hovered_button ).m_callback( m_hovered_button );
        m_audio_manager->playSFX( "SFX_UIHOVER.ogg", false );
        break;
      }
    }
    SetPressCooldown( m_timer, m_holding_axis, cooldown_timer );

    SendEvent<LevelSelectorEvent>( this, m_hovered_button );
  }
  // If can press up
  if ( trigger_up )
  {
    // If first button, move to last
    if ( m_hovered_button == m_buttons_arrangement.begin()->second )
    {
      GetComponent<GUI::UIHover>( m_hovered_button ).m_callback_unhover( m_hovered_button );
      m_hovered_button = std::next( m_buttons_arrangement.begin(), m_buttons_arrangement.size() - 1 )->second;
      GetComponent<GUI::UIHover>( m_hovered_button ).m_callback( m_hovered_button );
      m_audio_manager->playSFX( "SFX_UIHOVER.ogg", false );
    }
    // Go previous
    else
    {
      Entity prev_button;
      for ( auto &[pos, button] : m_buttons_arrangement )
      {
        auto &hover_pos = GetComponent<Position>( m_hovered_button ).m_position;
        if ( pos == Vec2f { hover_pos.getZ(), hover_pos.getY() } )
        {
          GetComponent<GUI::UIHover>( m_hovered_button ).m_callback_unhover( m_hovered_button );
          m_hovered_button = prev_button;
          GetComponent<GUI::UIHover>( m_hovered_button ).m_callback( m_hovered_button );
          m_audio_manager->playSFX( "SFX_UIHOVER.ogg", false );
          break;
        }
        prev_button = button;
      }
    }
    SetPressCooldown( m_timer, m_holding_axis, cooldown_timer );

    SendEvent<LevelSelectorEvent>( this, m_hovered_button );
  }
  // Select
  if ( trigger_select && HaveComponent<GUI::UITrigger>( m_hovered_button ) )
  {
    // Reset level selector
    ECS::Query::Instance l1_1_query;
    l1_1_query.AllOf<GUI::L1_1Tag>();
    ForEach( l1_1_query, [&]( Entity &entity )
    {
      SendEvent<LevelSelectorEvent>( this, entity );
    } );

    if ( !HaveComponent<GUI::VolumeBarTag>( m_hovered_button ) && !HaveComponent<GUI::GammaBarTag>( m_hovered_button ) )
      // Play sfx
      m_audio_manager->playSFX( "SFX_UITRIGGER.ogg", false );
    // Call triggered function
    GetComponent<GUI::UITrigger>( m_hovered_button ).m_callback( m_hovered_button );
  }

  if ( !m_holding_axis )
  {
    // Bar filling for gamma and volume
    SendEvent<UIBarFillEvent>( this, press_left, press_right, m_hovered_button );
    if ( ( press_left || press_right ) )
      SetPressCooldown( m_timer, m_holding_axis, cooldown_timer );
  }
}
}