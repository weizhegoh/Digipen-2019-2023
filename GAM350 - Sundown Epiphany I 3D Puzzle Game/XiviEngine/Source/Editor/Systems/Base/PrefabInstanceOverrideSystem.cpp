/*****************************************************************
*\file         PrefabInstanceOverrideSystem.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "PrefabInstanceOverrideSystem.h"

namespace Xivi::Editor
{
void PrefabInstanceOverrideSystem::OnCall( ECS::World::Instance &world, const Entity &entity, const ECS::Component::Descriptor &descriptor ) noexcept
{
  world.GetEntity( entity, [&]( ReferencedPrefab &ref_prefab )
  {
    if ( !world.HaveComponent( ref_prefab.m_entity, descriptor ) )
      return;

    ref_prefab.m_overrides.push_back( descriptor.m_id );
  } );
}
}
#endif