/*****************************************************************
*\file         EntityContext.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "EntityContext.h"

namespace Xivi::Editor
{
bool EntityContext::EntityIsSelected( const ECS::Entity entity ) const noexcept
{
  for ( const auto &selected_entity : selections )
    if ( selected_entity == entity )
      return true;
  return false;
}

void EntityContext::RemoveFromSelectedEntity( const ECS::Entity entity ) noexcept
{
  std::erase( selections, entity );
}

EntityContext::EntityContext()
{
  selections.reserve( reserve_count );
  copies.reserve( reserve_count );
  snapshots.reserve( reserve_count );
}

EntityContext::~EntityContext()
{
  ResetComponentSnapshot();
}

void EntityContext::ClearSelections() noexcept
{
  selections.clear();
  selected_entity_name.clear();
  cached_map.clear();
}

void EntityContext::ResetComponentSnapshot()
{
  if ( descriptor && descriptor->m_destructor )
    for ( auto &snapshot : snapshots )
      descriptor->m_destructor( snapshot.data() );
  snapshots.clear();
}

void EntityContext::SetComponentSnapshot( ECS::World::Instance &world, SnapshotDescriptor snapshot_descriptor )
{
  ResetComponentSnapshot();
  descriptor = snapshot_descriptor;

  if ( snapshot_descriptor->m_copy )
    for ( auto &selection : selections )
      snapshot_descriptor->m_copy( snapshots.emplace_back().data(), world.GetComponent( selection, snapshot_descriptor ) );
  else
    for ( auto &selection : selections )
      std::memcpy( snapshots.emplace_back().data(), world.GetComponent( selection, snapshot_descriptor ), snapshot_descriptor->m_size );
}

// For enum type snapshot
void EntityContext::SetComponentSnapshot( ECS::World::Instance &world, SnapshotDescriptor snapshot_descriptor, rttr::property &prop, rttr::variant &result )
{
  ResetComponentSnapshot();
  descriptor = snapshot_descriptor;

  if ( snapshot_descriptor->m_copy )
    for ( auto &selection : selections )
    {
      snapshot_descriptor->m_copy( snapshots.emplace_back().data(), world.GetComponent( selection, snapshot_descriptor ) );
      prop.set_value( snapshot_descriptor->m_reflect( world.GetComponent( selection, snapshot_descriptor ) ), result );
    }
  else
    for ( auto &selection : selections )
    {
      std::memcpy( snapshots.emplace_back().data(), world.GetComponent( selection, snapshot_descriptor ), snapshot_descriptor->m_size );
      prop.set_value( snapshot_descriptor->m_reflect( world.GetComponent( selection, snapshot_descriptor ) ), result );
    }
}

ECS::Entity &EntityContext::LastSelectedEntity() noexcept
{
  return selections.back();
}
}

#endif