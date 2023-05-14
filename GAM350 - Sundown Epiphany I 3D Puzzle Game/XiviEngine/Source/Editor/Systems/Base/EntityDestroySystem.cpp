/*****************************************************************
*\file         EntityDestroySystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#include "EntityDestroySystem.h"
#ifdef XV_EDITOR
#include "Editor/System.h"
#endif
#include "Components/GraphicsComponents.h"
#include "../Sundown/Source/NComponents.h"

namespace Xivi::Editor
{
void EntityDestroySystem::OnCreate() noexcept
{
  m_prefab_sig.set( ECS::Component::info<PrefabTag>.m_bit_index );
  m_ui_sig.set( ECS::Component::info<UI>.m_bit_index );
}

void EntityDestroySystem::OnFrameStart() noexcept
{
  #ifdef XV_EDITOR
  for ( const auto &[entity, is_prefab, is_ui] : m_to_remove )
    Editor::System::Instance().RemoveEntity( entity, is_prefab, is_ui );

  m_to_remove.clear();
  #endif
}

void EntityDestroySystem::OnNotify( Entity &entity ) noexcept
{
  #ifdef XV_EDITOR
  auto &archetype = m_world.GetArchetype( entity );
  bool is_prefab = ( archetype.GetSignature() & m_prefab_sig ).any();
  bool is_ui = ( archetype.GetSignature() & m_ui_sig ).any();

  auto &entity_context = Editor::System::Instance().m_entity_context;
  if ( entity_context.EntityIsSelected( entity ) )
    entity_context.RemoveFromSelectedEntity( entity );
  #endif
          // Remove attached entity if the attached entity is getting destroyed
  ForEach( [&]( Switch &s )
  {
    if ( s.linked_entity == entity )
      s.linked_entity = Entity {};
  } );

// Update grid details
  if ( HaveComponent<GridPosition>( entity ) )
  {
    auto &grid_pos = GetComponent<GridPosition>( entity );
    auto &grid = m_world.GetOrCreateSingletonComponent<GameGrid>();
    if ( grid.GetCellEntity( grid_pos.value ) == entity && grid.BoundsCheck( grid_pos.value ) )
      m_world.GetOrCreateSingletonComponent<GameGrid>().UpdateCell( grid_pos.value, GameType::Empty );
  }

  #ifdef XV_EDITOR
  m_to_remove.push_back( { entity, is_prefab, is_ui } );
  #endif
}
}