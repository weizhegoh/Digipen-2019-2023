/*****************************************************************
*\file         EntityContext.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include <Types/Base.h>
#include <STL/array.h>
#include <STL/vector.h>
#include <STL/unordered_map.h>
#include "Xivi/ECS/XVECS.h"

// Assets
#include <Resource/Manager.h>
#include "Xivi/Core/Declaration.h"
#include "Xivi/Graphics/Model/Model.h"
#include "Xivi/Graphics/Texture/Texture.h"
#include "Xivi/Graphics/Model/Animation/Animation.h"

namespace Xivi::Editor
{
struct EntityContext
{
  using ComponentSnapshot = array<byte, 4096>;
  using EntitiesComponentSnapshots = vector<ComponentSnapshot>;
  using EntityList = vector<Entity>;
  using SelectedEntities = vector<Entity>;
  using SnapshotDescriptor = Ptr<const ECS::Component::Descriptor>;

  const u64 reserve_count = 10;

  EntityList world_entity {};
  EntityList prefab_entity {};
  EntityList ui_entity {};
  SelectedEntities selections {};
  SelectedEntities copies {};
  EntitiesComponentSnapshots snapshots {};
  SnapshotDescriptor descriptor { nullptr };
  std::string selected_entity_name {};

  unordered_map<ECS::Component::ID, Ptr<byte>> cached_map;
  ECS::Signature selected_signature;

  bool EntityIsSelected( const ECS::Entity entity ) const noexcept;

  void RemoveFromSelectedEntity( const ECS::Entity entity ) noexcept;

  EntityContext();
  ~EntityContext();

  void ClearSelections() noexcept;

  void ResetComponentSnapshot();
  void SetComponentSnapshot( ECS::World::Instance &world, SnapshotDescriptor snapshot_descriptor );

  // For enum type snapshot
  void SetComponentSnapshot( ECS::World::Instance &world, SnapshotDescriptor snapshot_descriptor, rttr::property &prop, rttr::variant &result );

  // For resource type snapshot
  template <typename DataType>
  void SetComponentSnapshot( ECS::World::Instance &world, SnapshotDescriptor snapshot_descriptor, rttr::property &prop, Resource::ID key )
  {
    ResetComponentSnapshot();
    descriptor = snapshot_descriptor;

    for ( auto &selection : selections )
    {
      snapshot_descriptor->m_copy( snapshots.emplace_back().data(), world.GetComponent( selection, snapshot_descriptor ) );
      auto value = prop.get_value( snapshot_descriptor->m_reflect( world.GetComponent( selection, snapshot_descriptor ) ) );
      auto &resource = value.get_value<std::reference_wrapper<Resource::Handler<DataType>>>().get();
      resource = Service::Locator::Get<RM>().Get<DataType>( key );
    }
  }

  ECS::Entity &LastSelectedEntity() noexcept;
};
}
#endif