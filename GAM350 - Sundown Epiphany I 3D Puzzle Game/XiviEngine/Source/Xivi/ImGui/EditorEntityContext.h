/*****************************************************************
*\file         EditorEntityContext.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct EntityContext
{
  using ComponentSnapshot = std::array<byte, 4096>;
  using EntitiesComponentSnapshots = std::vector<ComponentSnapshot>;
  using EntityList = std::vector<Entity>;
  using SelectedEntities = std::vector<Entity>;
  using SnapshotDescriptor = Ptr<const ECS::Component::Descriptor>;

  const u64 reserve_count = 10;

  EntityList m_world_entity;
  EntityList m_prefab_entity;
  EntityList m_ui_entity;
  SelectedEntities m_selections {};
  SelectedEntities m_copies {};
  EntitiesComponentSnapshots snapshots {};
  SnapshotDescriptor m_descriptor { nullptr };
  std::string m_selected_entity_name {};

  bool EntityIsSelected( const ECS::Entity entity ) const noexcept
  {
    for ( const auto &selected_entity : m_selections )
      if ( selected_entity == entity )
        return true;
    return false;
  }

  void RemoveFromSelectedEntity( const ECS::Entity entity ) noexcept
  {
    std::erase( m_selections, entity );
  }

  EntityContext()
  {
    m_selections.reserve( reserve_count );
    m_copies.reserve( reserve_count );
    snapshots.reserve( reserve_count );
  }

  ~EntityContext()
  {
    ResetComponentSnapshot();
  }

  void ResetComponentSnapshot()
  {
    if ( m_descriptor && m_descriptor->m_destructor )
      for ( auto &snapshot : snapshots )
        m_descriptor->m_destructor( snapshot.data() );
    snapshots.clear();
  }

  void SetComponentSnapshot( ECS::World::Instance &world, SnapshotDescriptor descriptor )
  {
    ResetComponentSnapshot();
    m_descriptor = descriptor;

    if ( descriptor->m_copy )
      for ( auto &selection : m_selections )
        descriptor->m_copy( snapshots.emplace_back().data(), world.GetComponent( selection, descriptor ) );
    else
      for ( auto &selection : m_selections )
        std::memcpy( snapshots.emplace_back().data(), world.GetComponent( selection, descriptor ), descriptor->m_size );
  }

  // For enum type snapshot
  void SetComponentSnapshot( ECS::World::Instance &world, SnapshotDescriptor descriptor, rttr::property &prop, rttr::variant &result )
  {
    ResetComponentSnapshot();
    m_descriptor = descriptor;

    if ( descriptor->m_copy )
      for ( auto &selection : m_selections )
      {
        descriptor->m_copy( snapshots.emplace_back().data(), world.GetComponent( selection, descriptor ) );
        prop.set_value( descriptor->m_reflect( world.GetComponent( selection, descriptor ) ), result );
      }
    else
      for ( auto &selection : m_selections )
      {
        std::memcpy( snapshots.emplace_back().data(), world.GetComponent( selection, descriptor ), descriptor->m_size );
        prop.set_value( descriptor->m_reflect( world.GetComponent( selection, descriptor ) ), result );
      }
  }

  // For resource type snapshot
  template <typename DataType>
  void SetComponentSnapshot( ECS::World::Instance &world, SnapshotDescriptor descriptor, rttr::property &prop, Resource::ID key )
  {
    ResetComponentSnapshot();
    m_descriptor = descriptor;

    for ( auto &selection : m_selections )
    {
      descriptor->m_copy( snapshots.emplace_back().data(), world.GetComponent( selection, descriptor ) );
      auto value = prop.get_value( descriptor->m_reflect( world.GetComponent( selection, descriptor ) ) );
      auto &resource = value.get_value<std::reference_wrapper<Resource::Handler<DataType>>>().get();
      resource = Service::Locator::Get<RM>().Get<DataType>( key );
    }
  }

  ECS::Entity &LastSelectedEntity() noexcept
  {
    return m_selections.back();
  }
};
}