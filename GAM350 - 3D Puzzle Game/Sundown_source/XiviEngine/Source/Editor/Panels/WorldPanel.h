/*****************************************************************
*\file         WorldPanel.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "IPanel.h"
#include "Editor/ImGui/Helper/DrawComponent.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Editor/Command/Actions/LinkParentCommand.h"

namespace Xivi::Audio
{
class AudioManager;
}

namespace Xivi::Graphics
{
class M_FrameBuffer;
}

namespace Xivi::GUI
{
class Manager;
}

namespace Xivi
{
struct Child;
}

namespace Xivi::Editor
{
struct WorldPanel : public IPanel
{
  WorldPanel( std::string name,
              Ptr<ECS::World::Instance> world,
              Ptr<Config> configs,
              Ptr<EntityContext> entity_context,
              Ptr<FileContext> file_context,
              Ptr<ToolContext> tool_context );

  ~WorldPanel();

  void Render() noexcept override;

  void CreateEntitesOnPanel( Ptr<ECS::World::Instance> world, ECS::Entity entity, bool is_root = false ) noexcept;

  void ModalPopup() noexcept;

  void RecurChild( Ptr<ECS::World::Instance> world, Child &child ) noexcept;

  void GeneratePrefabName( Entity entity, Entity prefab ) noexcept;

  void RenderEntities() noexcept;

  // --------------------------------------- Prefab --------------------------------------- //
  bool DupPrefabPath( Entity &entity, const std::string path ) noexcept;

  std::string SaveEntityAsPrefab( Entity &entity, bool is_popup = false, bool prefab_existed = false ) noexcept;

  // --------------------------------------- GUI --------------------------------------- //
  void CreateGUIWindow() noexcept;
  void CreateCanvasEntity( Entity gui_window ) noexcept;
  void AttachEntityToCanvas( Entity entity ) noexcept;
  void AddTextEntity( Entity canavas ) noexcept;
  void AddImageEntity( Entity canavas ) noexcept;
  void AddCheckbox( Entity canavas ) noexcept;
  void AddDrag( Entity canavas ) noexcept;
  void AddBar( Entity canavas ) noexcept;
  void AddTrigger( Entity canavas ) noexcept;
  void PushCanvas( Entity entity ) noexcept;

  template <ECS::Component::Concepts::IsComponentList ComponentList>
  void AddItem( Entity parent, std::string name, bool is_canvas = false )
  {
    auto scale = is_canvas ?
      Vec3f { 1.f, 1.f, 0.f } :
      Vec3f { .3f, .3f, 0.f };
    auto &archetype = m_world->GetOrCreateArchetype<ComponentList>( "Static Renders" );
    auto entity = archetype.CreateEntity( [&]( Position &pos,
                                          Transform &xform,
                                          StaticMeshComponent &mesh,
                                          Material &material,
                                          Colour &colour,
                                          AABB &aabb,
                                          RenderSettings &renderSettings ) noexcept
    {
      pos.m_position = m_frame_buffer->CameraUI().CameraDirection();
      xform.scale = scale;
      xform.axis = Vec3f { 0.f, 90.f, 0.f };
      aabb.AABB_mMin = 0;
      aabb.AABB_mMax = 0;
      aabb.type = CollisionType::AABB;
      colour.colour = Vec3f { 1.f, 1.f, 1.f };
      material.diffuse = m_resource_manager->Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.specular = m_resource_manager->Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.bump = m_resource_manager->Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.normal = m_resource_manager->Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.shinness = 128.f;
      renderSettings.onLight = true;
      renderSettings.onRender = true;
      mesh.model = m_resource_manager->Get<Graphics::StaticModel>( "Assets/Models/binary/Object/plane.BinaryObj" );
      mesh.unitScales = Math::Vector3( 1.f );
    } );
    auto &entry = m_world->m_entity_manager.Get( entity );
    entry.m_name = name;
    m_editor_sys->m_command_history.Commit<LinkParentCommand>( *m_world, entity, parent );

    m_entity_context->ClearSelections();
    m_audio_manager->StopAllAudio();
    m_entity_context->selections.push_back( entity );
    Draw::OnClick( entity, *m_entity_context );
    PushCanvas( entity );
  }

  bool m_delete_entity { false };
  bool m_delete_prefab_inst { false };

private:
  bool m_override_prefab { false };
  bool m_prefab_exist { false };
  Entity m_prefab_entity;
  std::string m_prefab_path;
  ECS::Signature m_prefab_instance_sig;
  std::function<bool( Entity, const ImGuiTextFilter & )> m_recursive_search;
  std::function<void( Entity )> m_recur_push_gui;
  std::function<Entity( Entity, Ptr<ECS::World::Instance> )> m_recur_get_window;
  std::function<bool( const Utils::Directory &, const std::string )> m_recur_check_file_exist;
  Ptr<RM> m_resource_manager { nullptr };
  Ptr<Audio::AudioManager> m_audio_manager { nullptr };
  Ptr<Graphics::M_FrameBuffer> m_frame_buffer { nullptr };
  Ptr<GUI::Manager> m_canvas_stack { nullptr };
  Ptr<System> m_editor_sys { nullptr };
};
}
#endif