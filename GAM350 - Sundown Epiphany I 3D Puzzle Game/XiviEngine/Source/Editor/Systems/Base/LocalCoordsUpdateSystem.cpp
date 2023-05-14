/*****************************************************************
*\file         LocalCoordsUpdateSystem.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "LocalCoordsUpdateSystem.h"
#include "Components/GUI/UI.h"

namespace Xivi::Editor
{
void LocalCoordsUpdateSystem::OnCall( ECS::World::Instance &world, const Entity &entity ) noexcept
{
  if ( world.HaveComponent<UI>( entity ) )
    return;
  world.GetEntity( entity, [&]( const Parent &parent, Position &child_pos, LocalToParent &local, Transform &child_transform )
  {
    world.GetEntity( parent.m_entity, [&]( Position &pos, Transform &transform )
    {
      local.m_position = child_pos.m_position - pos.m_position;
      local.scale = child_transform.scale / transform.scale;
    } );
  } );
}
}
#endif