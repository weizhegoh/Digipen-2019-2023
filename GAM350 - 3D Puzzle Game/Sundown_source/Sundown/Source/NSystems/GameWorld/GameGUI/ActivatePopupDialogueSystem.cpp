/*****************************************************************
*\file         ActivatePopupDialogueSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "ActivatePopupDialogueSystem.h"
#include "NComponents/Entity/Player.h"
#include "NComponents/Grid/Grid.h"

namespace Xivi
{
void PlaySFXForPopUpDialogue() noexcept
{
  Audio::AudioManager::Instance().playSFX( "SFX_DIALOGUE_POPUP.ogg", false );
}

void ActivateSwitchPopupDialogueSystem::OnEvent() noexcept
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
    if ( switch_dialogue.m_activated )
      return;

    render_settings.transparent = 1.0f;
    switch_dialogue.m_activated = true;
    PlaySFXForPopUpDialogue();
  } );
}

void ActivatePressurePlatePopupDialogueSystem::OnEvent() noexcept
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
    if ( pp_dialogue.m_activated )
      return;

    PlaySFXForPopUpDialogue();
    render_settings.transparent = 1.0f;
    pp_dialogue.m_activated = true;
  } );
}

void ActivateCheesePopupDialogueSystem::OnEvent() noexcept
{
  auto &grid = GetOrCreateSingletonComponent<GameGrid>();
  bool near_cheese = false;
  ForEach( [&]( Player &player, GridPosition &grid_position )
  {
    if ( grid.GetCellType( grid_position.value + Vec3i( 1, 0, 0 ) ) == GameType::Cheese ||
         grid.GetCellType( grid_position.value + Vec3i( -1, 0, 0 ) ) == GameType::Cheese ||
         grid.GetCellType( grid_position.value + Vec3i( 0, 0, 1 ) ) == GameType::Cheese ||
         grid.GetCellType( grid_position.value + Vec3i( 0, 0, -1 ) ) == GameType::Cheese )
      near_cheese = true;

    return near_cheese;
  } );

  if ( !near_cheese )
    return;

  ForEach( [&]( GUI::CheesePopupDialogue &cheese_dialogue, RenderSettings &render_settings )
  {
    if ( cheese_dialogue.m_activated )
      return;

    PlaySFXForPopUpDialogue();
    render_settings.transparent = 1.0f;
    cheese_dialogue.m_activated = true;
  } );
}

void ActivateObstaclePopupDialogueSystem::OnEvent() noexcept
{
  ForEach( [&]( GUI::ObstaclePopupDialogue &o_dialogue, RenderSettings &render_settings )
  {
    if ( o_dialogue.m_activated )
      return;

    PlaySFXForPopUpDialogue();
    render_settings.transparent = 1.0f;
    o_dialogue.m_activated = true;
  } );
}

void ActivateHolePopupDialogueSystem::OnEvent() noexcept
{
  ForEach( [&]( GUI::HolePopupDialogue &h_dialogue, RenderSettings &render_settings )
  {
    if ( h_dialogue.m_activated )
      return;

    PlaySFXForPopUpDialogue();
    render_settings.transparent = 1.0f;
    h_dialogue.m_activated = true;
  } );
}

void ActivateCartPopupDialogueSystem::OnEvent() noexcept
{
  auto &grid = GetOrCreateSingletonComponent<GameGrid>();
  bool facing_cart = false;
  ForEach( [&]( Entity &entity, GridPosition &grid_position, Direction &direction, Player &player )
  {
    if ( grid.GetCellType( grid_position.value + Vec3i( direction.m_dir ) ) == GameType::Box )
      facing_cart = true;
    return facing_cart;
  } );

  if ( !facing_cart )
    return;

  ForEach( [&]( GUI::CartPopupDialogue &cart_dialogue, RenderSettings &render_settings )
  {
    if ( cart_dialogue.m_activated )
      return;

    PlaySFXForPopUpDialogue();
    render_settings.transparent = 1.0f;
    cart_dialogue.m_activated = true;
  } );
}
}