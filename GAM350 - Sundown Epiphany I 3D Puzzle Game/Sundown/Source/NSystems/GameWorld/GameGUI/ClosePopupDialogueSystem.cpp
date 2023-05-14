/*****************************************************************
*\file         ClosePopupDialogueSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "ClosePopupDialogueSystem.h"
#include "NComponents/Entity/Player.h"
#include "NComponents/Grid/Grid.h"

namespace Xivi
{
void CloseSwitchPopupDialogueSystem::OnEvent() noexcept
{
  auto &grid = GetOrCreateSingletonComponent<GameGrid>();
  bool step_on_switch = false;
  ForEach( [&]( Player &player, GridPosition &grid_position )
  {
    if ( grid.GetCellType( grid_position.value + Vec3i( 0, -1, 0 ) ) == GameType::Switch )
      step_on_switch = true;

    return step_on_switch;
  } );

  if ( !step_on_switch )
    return;

  ForEach( [&]( GUI::SwitchPopupDialogue &switch_dialogue, RenderSettings &render_settings )
  {
    if ( switch_dialogue.m_closed )
      return;

    if ( !switch_dialogue.m_activated )
      return;

    render_settings.transparent = 0.0f;
    switch_dialogue.m_closed = true;
  } );
}

void ClosePressurePlatePopupDialogueSystem::OnEvent() noexcept
{
  auto &grid = GetOrCreateSingletonComponent<GameGrid>();
  bool step_on_pp = false;
  ForEach( [&]( Player &player, GridPosition &grid_position )
  {
    if ( grid.GetCellType( grid_position.value + Vec3i( 0, -1, 0 ) ) == GameType::PressurePlate )
      step_on_pp = true;

    return step_on_pp;
  } );

  if ( !step_on_pp )
    return;

  ForEach( [&]( GUI::PressurePlatePopupDialogue &pp_dialogue, RenderSettings &render_settings )
  {
    if ( pp_dialogue.m_closed )
      return;

    if ( !pp_dialogue.m_activated )
      return;

    render_settings.transparent = 0.0f;
    pp_dialogue.m_closed = true;
  } );
}

void CloseCheesePopupDialogueSystem::OnEvent() noexcept
{
  ForEach( [&]( GUI::CheesePopupDialogue &cheese_dialogue, RenderSettings &render_settings )
  {
    if ( cheese_dialogue.m_closed )
      return;

    if ( !cheese_dialogue.m_activated )
      return;

    render_settings.transparent = 0.0f;
    cheese_dialogue.m_closed = true;
    SendGlobalEvent<ActivateObstaclePopupDialogue>();
  } );
}

void CloseObstaclePopupDialogueSystem::OnEvent() noexcept
{
  ForEach( [&]( GUI::ObstaclePopupDialogue &o_dialogue, RenderSettings &render_settings )
  {
    if ( o_dialogue.m_closed )
      return;

    if ( !o_dialogue.m_activated )
      return;

    render_settings.transparent = 0.0f;
    o_dialogue.m_closed = true;
    SendGlobalEvent<ActivateHolePopupDialogue>();
  } );
}

void CloseHolePopupDialogueSystem::OnEvent() noexcept
{
  ForEach( [&]( GUI::HolePopupDialogue &h_dialogue, RenderSettings &render_settings )
  {
    if ( h_dialogue.m_closed )
      return;

    if ( !h_dialogue.m_activated )
      return;

    render_settings.transparent = 0.0f;
    h_dialogue.m_closed = true;
  } );
}

void CloseCartPopupDialogueSystem::OnEvent() noexcept
{
  ForEach( [&]( GUI::CartPopupDialogue &cart_dialogue, RenderSettings &render_settings )
  {
    if ( cart_dialogue.m_closed )
      return;

    if ( !cart_dialogue.m_activated )
      return;

    render_settings.transparent = 0.0f;
    cart_dialogue.m_closed = true;
  } );
}
}