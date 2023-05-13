/*****************************************************************
*\file         EntityCreateSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#include "EntityCreateSystem.h"
#ifdef XV_EDITOR
#include "Editor/System.h"
#include "Editor/ImGui/Helper/DrawComponent.h"
#endif
#include "Components/GraphicsComponents.h"
#include "Editor/Systems/Base/EventRefreshPanel.h"

namespace Xivi::Editor
{
void EntityCreateSystem::OnCreate() noexcept
{
  m_prefab_sig.set( ECS::Component::info<PrefabTag>.m_bit_index );
  m_ui_sig.set( ECS::Component::info<UI>.m_bit_index );
  m_skybox_sig.set( ECS::Component::info<SkyBoxMaterial>.m_bit_index );
}

void EntityCreateSystem::OnFrameStart() noexcept
{
    /*static bool changes = false;
    for (auto& [entity, is_prefab, is_ui] : m_to_add)
    {
        changes = true;
        auto& e_desc = m_world.m_entity_manager.m_entities[entity.m_global_index];
        entity.m_validation = e_desc.m_validation;
        Editor::System::Instance().AddEntity(entity, is_prefab, is_ui);
    }
    if(changes)
    {
        changes = false;
        m_to_add.clear();
        m_world.m_archetype_manager.UpdateStructuralChanges();
        EventRefreshPanelSystem::OnCall(m_world);
    }*/

  #ifdef XV_EDITOR
  if ( m_need_refresh_panel )
  {
    m_need_refresh_panel = false;
    EventRefreshPanelSystem::OnCall( m_world );
    if ( m_to_select_entity.empty() )
      return;
    auto &entity_context = Editor::System::Instance().m_entity_context;
    Entity ent = m_to_select_entity[0];
    entity_context.selections.push_back( ent );
    Draw::OnClick( ent, entity_context );
    m_to_select_entity.clear();
  }
  #endif
}

void EntityCreateSystem::OnNotify( Entity &entity ) noexcept
{
  #ifdef XV_EDITOR
  auto &archetype = m_world.GetArchetype( entity );
  bool is_prefab = ( archetype.GetSignature() & m_prefab_sig ).any();
  bool is_ui = ( archetype.GetSignature() & m_ui_sig ).any();

  bool is_skybox = ( archetype.GetSignature() & m_skybox_sig ).any();
  if ( !is_skybox )
      //m_to_add.push_back({ entity, is_prefab, is_ui });
  {
    m_need_refresh_panel = true;
    m_to_select_entity.push_back( entity );
  }
  #endif
}
}