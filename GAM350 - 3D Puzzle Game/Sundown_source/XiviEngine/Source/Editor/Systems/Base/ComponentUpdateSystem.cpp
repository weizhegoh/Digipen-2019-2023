/*****************************************************************
*\file         ComponentUpdateSystem.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "ComponentUpdateSystem.h"

namespace Xivi::Editor
{
void ComponentUpdateSystem::OnCall( ECS::World::Instance &world, const Entity &entity, const ECS::Component::Descriptor &descriptor ) noexcept
{
  if ( world.HaveComponent<Prefab>( entity ) )
    PrefabUpdateSystem::OnCall( world, entity, descriptor );
  else if ( world.HaveComponent<ReferencedPrefab>( entity ) )
    PrefabInstanceOverrideSystem::OnCall( world, entity, descriptor );
}
}
#endif