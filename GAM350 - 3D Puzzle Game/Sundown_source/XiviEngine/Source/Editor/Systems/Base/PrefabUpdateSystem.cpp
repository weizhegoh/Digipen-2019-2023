/*****************************************************************
*\file         PrefabUpdateSystem.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "PrefabUpdateSystem.h"

namespace Xivi::Editor
{
void PrefabUpdateSystem::OnCall( ECS::World::Instance &world, const Entity &entity, const ECS::Component::Descriptor &descriptor ) noexcept
{
  world.GetEntity( entity, [&]( Prefab &prefab )
  {
    std::for_each( prefab.referenced_entities.begin(),
                   prefab.referenced_entities.end(),
                   [&]( Entity &e )
    {
      world.GetEntity( e, [&]( ReferencedPrefab &ref_prefab )
      {
        XV_CORE_INFO( "Referenced Prefab Entity - {}", ref_prefab.m_entity.m_global_index );

        for ( auto & override : ref_prefab.m_overrides )
        {
          if ( override == descriptor.m_id )
            return;
        }

        auto src = world.GetComponent( entity, &descriptor );
        auto dest = world.GetComponent( e, &descriptor );
        if ( descriptor.m_copy )
          descriptor.m_copy( dest, src );
        else
          std::memcpy( dest, src, descriptor.m_size );
      } );
    } );
  } );
}
}
#endif