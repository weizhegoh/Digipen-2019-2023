/*****************************************************************
*\file         InitFuncMapping.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "XiviEngine.h"
#include "Xivi/Graphics/Manager/GameStateManager.h"
#include "NSystems/GameWorld/GameGUI/UIButtonMovementSystem.h"
#include "Xivi/Core/Declaration.h"

namespace Xivi::FuncMapping
{
  // ---------------------------------- Button Function Mapping ---------------------------------- //
  // Initialize function mapping
void InitFuncMapping( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager, Audio::AudioManager &audio_mgr ) noexcept;
void Unhover( ECS::World::Instance &world ) noexcept;
// For internal usage
template <ECS::Component::Concepts::IsComponent C>
void LevelToGo( ECS::World::Instance &world, const GSM::GS_STATES state_to_change_to ) noexcept
{
  auto hover_entity = &world.GetSystem<UIButtonMovementSystem>().m_hovered_button;
  if ( !world.HaveComponent<C>( *hover_entity ) )
    return;

  #ifndef XV_EDITOR
  auto &gsm = Service::Locator::Get<GSM::GSManager>();
  gsm.ChangeState( state_to_change_to );
  #endif

      /*ECS::Query::Instance lvl;
      lvl.AllOf<C>();

      world.ForEach(lvl, [&]
        {
          auto& gsm = Service::Locator::Get<GSM::GSManager>();
          gsm.ChangeState(state_to_change_to);
        });*/
}
}