/*****************************************************************
*\file         GameInteractablePopupSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "XiviEngine.h"
#include "Xivi/Graphics/Manager/GameStateManager.h"
#include "GameInteractablePopupSystem.h"
#include "HelperFunctions/GameGUIHelperFunction.h"
#include "NComponents.h"

namespace Xivi
{
void GameInteractablePopupSystem::OnCreate() noexcept
{
  m_canvas_interaction_popup.AllOf<GUI::GameInteratablePopup>();
}

void GameInteractablePopupSystem::OnUpdate() noexcept
{
  if ( m_paused )
    return;

  SendGlobalEvent<ActivateSwitchPopupDialogue>();
  SendGlobalEvent<ActivatePressurePlatePopupDialogue>();
  SendGlobalEvent<ActivateCheesePopupDialogue>();
  SendGlobalEvent<ActivateCartPopupDialogue>();

  auto &grid = GetOrCreateSingletonComponent<GameGrid>();
  bool step_on_interactable = false;
  ForEach( [&]( Player &player, GridPosition &grid_position )
  {
    if ( grid.IsInteractableType( m_world, grid_position.value + Vec3i( 0, -1, 0 ) ) )
      step_on_interactable = true;

    return step_on_interactable;
  } );

  auto count = 0;
  ForEach( m_canvas_interaction_popup, [&]( Entity &entity, RenderSettings &render_settings )
  {
    render_settings.transparent = step_on_interactable ?
      1.0f :
      0.0f;
    ++count;
  } );
}
}