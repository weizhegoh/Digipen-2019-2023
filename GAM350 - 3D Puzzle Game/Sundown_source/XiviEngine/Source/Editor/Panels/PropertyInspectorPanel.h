/*****************************************************************
*\file         PropertyInspectorPanel.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "IPanel.h"
#ifdef XV_EDITOR

namespace Xivi
{
struct Child;
}

namespace Xivi::Audio
{
class AudioManager;
}

namespace Xivi::Editor
{
struct System;
struct PropertyInspectorPanel : public IPanel
{
  PropertyInspectorPanel( std::string name,
                          Ptr<ECS::World::Instance> world,
                          Ptr<Config> configs,
                          Ptr<EntityContext> entity_context,
                          Ptr<FileContext> file_context,
                          Ptr<ToolContext> tool_context );

  ~PropertyInspectorPanel();

  void Render() noexcept override;

  void SaveButton( const Entity selected_entity, const ECS::Archetype::Instance &arch ) noexcept;

  void SearchBarPrefab( Ptr<ECS::World::Instance> world,
                        ECS::Component::DescriptorManager &desc_mgr,
                        const std::string ent_name,
                        Entity entity,
                        const ECS::Signature common_sig,
                        Ptr<RM> resource_manager,
                        Ptr<EntityContext> entity_context ) noexcept;

  void SearchBarEntity( Ptr<ECS::World::Instance> world,
                        ECS::Component::DescriptorManager &desc_mgr,
                        Ptr<ECS::Pool::Instance> pool,
                        ECS::Pool::Index pool_index,
                        const ECS::Signature common_sig,
                        const Entity entity,
                        Ptr<RM> resource_manager,
                        Ptr<EntityContext> entity_context,
                        ECS::Archetype::Instance &arch ) noexcept;

  void PrefabName( Ptr<ECS::World::Instance> world, ECS::Entity::Descriptor &entry ) noexcept;

  void EntityName( Ptr<ECS::World::Instance> world, ECS::Entity::Descriptor &entry ) noexcept;

  void EntityTag( Ptr<ECS::World::Instance> world, ECS::Component::DescriptorManager &desc_mgr, ECS::Entity entity, ECS::Archetype::Instance &arch, ImGuiTextFilter &filter ) noexcept;

  void RenderPrefabEntity( Ptr<ECS::World::Instance> world,
                           ECS::Component::DescriptorManager &desc_mgr,
                           Entity entity,
                           std::string entity_name ) noexcept;

  void RenderPrefabProperties( Ptr<ECS::World::Instance> world, Entity entity ) noexcept;

  void RecurChildProperties( Ptr<ECS::World::Instance> world, Child &child ) noexcept;

  std::string Trim( const std::string &str ) noexcept;

  std::string RemoveExtraSpace( const std::string &str ) noexcept;

  bool RecursiveSearch( Entity entity, const ImGuiTextFilter &world_filter ) noexcept;

  void RecurRenderPrefabComponents( Ptr<ECS::World::Instance> world,
                                    ECS::Component::DescriptorManager &desc_mgr,
                                    Entity entity,
                                    std::string entity_name,
                                    const ImGuiTextFilter prop_filter,
                                    bool ignore_indentation = true ) noexcept;

  bool RecurCheckForSelection( Ptr<ECS::World::Instance> world,
                               ECS::Component::DescriptorManager &desc_mgr,
                               Entity entity,
                               const ECS::Signature common_sig,
                               Ptr<RM> resource_manager,
                               Ptr<EntityContext> entity_context ) noexcept;

private:
  ECS::Signature m_component_can_add_remove;
  ECS::Signature m_prefab_instance_sig;
  ECS::Signature m_component_can_show;
  Ptr<RM> m_resource_manager { nullptr };
  Ptr<Audio::AudioManager> m_audio_manager { nullptr };
  Ptr<System> m_editor_sys { nullptr };
  bool m_remove_component { false };
  std::function<Entity( Entity )> m_get_root;
};
}
#endif