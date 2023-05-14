/*****************************************************************
*\file         UpdateGUICollectableSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "UpdateGUICollectableSystem.h"

namespace Xivi
{
void UpdateGUICollectableSystem::OnEvent( const i32 &collectable_count ) noexcept
{
  ECS::Query::Instance game_menu_cheese;
  game_menu_cheese.AllOf<GUI::GUICheeseGameMenu>();
  ForEach( game_menu_cheese, [&]( GUI::UICheese &cheese, RenderSettings &render_settings )
  {
    render_settings.transparent = cheese.m_id <= collectable_count ?
      1.0f :
      0.0f;
  } );
}
}