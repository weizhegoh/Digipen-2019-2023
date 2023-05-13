/*****************************************************************
*\file         ArchetypePanel.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "IPanel.h"

#include "Editor/ImGui/Helper/DrawComponent.h"

namespace Xivi::Editor
{
using TypesOfArchpetype = std::array<const char *, 9>;

// Button
using SwitchType = CList<Position, Transform,
  StaticMeshComponent, Material, Colour, AABB,
  RenderSettings, ParticleEmitter, ParticleSettings>;

// Grid
using GridType = CList<EntityType, GridPosition, EntityDirection, NewPosition>;

using StaticGridType = StaticRenderType::Extend<GridType>;
using AnimationGridType = AnimationRenderType::Extend<GridType>;

using PlayerType = CList<
  Position,
  Transform,
  AnimationMeshComponent,
  AnimationState,
  AnimatorComponent,
  Material,
  Colour,
  RenderSettings,
  Direction,
  AABB,
  DirectionInitialisationTag,
  LightData>
  ::Extend<GridType>
  ::Extend<CList<NewPosition, NewTransform, Player>>;

using CameraType = CList<Position, Transform, Camera_c, CameraInitalized>;

struct ArchetypePanel : public IPanel
{
  using ArchetypeLabel = const char *;
  using ArchetypeName = const char *;
  using ArchetypeGroup = const char *;
  using ArchetypeDescriptor = tuplet::tuple<ArchetypeLabel, ArchetypeName, ArchetypeCreateFunc, ArchetypeGroup>;

  std::vector<ArchetypeDescriptor> m_archetypes_creator;
  Ptr<const ArchetypeCreateFunc> m_create_ptr { nullptr };
  std::string m_create_ptr_name {};

  const TypesOfArchpetype m_archetype_types
  {
      "Game Tile",
      "Game Object to Attach to Tile",
      "Props",
      "Standalone Game Object",
      "Lighting",
      "Camera",
      "Basic Shape",
      "Collider",
      "Animation"
  };

  ArchetypePanel( std::string name,
                  Ptr<ECS::World::Instance> world,
                  Ptr<Config> configs,
                  Ptr<EntityContext> entity_context,
                  Ptr<FileContext> file_context,
                  Ptr<ToolContext> tool_context );

  ~ArchetypePanel();

  void Render() noexcept override;

private:
  void AddGameArchetype() noexcept;
  template <ECS::Component::Concepts::IsComponentList List>
  void PrintComponentsName() noexcept
  {
    [&] <typename... Cs>( CList<Cs...> )
    {
      ( ImGui::Text( std::string( ECS::Component::info<Cs>.m_name ).c_str() ), ... );
    }
    ( List() );
  }

  template <ECS::Component::Concepts::IsComponentList List = CList<>>
  Entity CreateStaticGridArchetype( Ptr<ECS::World::Instance> world,
                                    Ptr<EntityContext> ent_context,
                                    const std::string archetype_name,
                                    const std::string texture_diffuse,
                                    const std::string texture_specular,
                                    const std::string model,
                                    const Position &create_pos,
                                    GameType game_type ) noexcept
  {
    auto &archetype = world->GetOrCreateArchetype<StaticGridType::Extend<List>>( archetype_name );
    auto entity = archetype.CreateEntity( [&]( Position &pos, Transform &xform, StaticMeshComponent &mesh, Material &material,
                                          Colour &colour, AABB &aabb, RenderSettings &renderSettings, EntityType &entity_type ) noexcept
    {
      pos = create_pos;
      xform.scale = Vec3f { 50.f, 50.f, 50.f };
      aabb.AABB_mMin = 0;
      aabb.AABB_mMax = 0;
      aabb.type = CollisionType::AABB;
      colour.colour = Vec3f { 1.f, 1.f, 1.f };
      material.diffuse = Service::Locator::Get<RM>().Get<Graphics::Texture>( texture_diffuse );
      material.specular = Service::Locator::Get<RM>().Get<Graphics::Texture>( texture_specular );
      material.normal = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.bump = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.shinness = 128.f;
      renderSettings.onLight = true;
      renderSettings.onRender = true;
      mesh.model = Service::Locator::Get<RM>().Get<Graphics::StaticModel>( model );
      mesh.unitScales = mesh.model->unitScales;
      entity_type.value = game_type;
    } );
    auto &entry = world->m_entity_manager.Get( entity );
    entry.m_name = archetype_name;
    Draw::OnClick( entity, *ent_context );
    return entity;
  }

  template <ECS::Component::Concepts::IsComponentList List = CList<>>
  Entity CreateAnimationGridArchetype( Ptr<ECS::World::Instance> world,
                                       Ptr<EntityContext> ent_context,
                                       const std::string archetype_name,
                                       const std::string texture_diffuse,
                                       const std::string texture_specular,
                                       const std::string model,
                                       const Position &create_pos,
                                       GameType game_type ) noexcept
  {
    auto &archetype = world->GetOrCreateArchetype<AnimationGridType::Extend<List>>( archetype_name );
    auto entity = archetype.CreateEntity( [&]( Position &pos, Transform &xform, AnimationMeshComponent &mesh,
                                          Material &material, Colour &colour, AABB &aabb, RenderSettings &renderSettings, AnimatorComponent &animator,
                                          EntityType &entity_type ) noexcept
    {
      pos = create_pos;
      xform.scale = Vec3f { 50.f, 50.f, 50.f };
      xform.axis = Vec3f { 90.f, 0.f, 0.f };
      aabb.AABB_mMin = 0;
      aabb.AABB_mMax = 0;
      aabb.type = CollisionType::AABB;
      colour.colour = Vec3f { 1.f,1.f,1.f };
      material.diffuse = Service::Locator::Get<RM>().Get<Graphics::Texture>( texture_diffuse );
      material.specular = Service::Locator::Get<RM>().Get<Graphics::Texture>( texture_specular );
      material.normal = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.bump = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.shinness = 128.f;
      renderSettings.onLight = true;
      renderSettings.onRender = true;
      mesh.model = Service::Locator::Get<RM>().Get<Graphics::AnimationModel>( model );
      animator.UpdateAnimation( 0.f );
      animator.isActive = false;
      animator.numRun = 1;
      entity_type.value = game_type;
    } );
    auto &entry = world->m_entity_manager.Get( entity );
    entry.m_name = archetype_name;
    Draw::OnClick( entity, *ent_context );
    return entity;
  }

  template <ECS::Component::Concepts::IsComponentList List = CList<>>
  Entity CreateStaticArchetype( Ptr<ECS::World::Instance> world,
                                Ptr<EntityContext> ent_context,
                                const std::string archetype_name,
                                const std::string texture_diffuse,
                                const std::string texture_specular,
                                const std::string model,
                                const Position &create_pos ) noexcept
  {
    auto &archetype = world->GetOrCreateArchetype<StaticRenderType::Extend<List>>( archetype_name );
    auto entity = archetype.CreateEntity( [&]( Position &pos, Transform &xform, StaticMeshComponent &mesh, Material &material, Colour &colour, AABB &aabb, RenderSettings &renderSettings ) noexcept
    {
      pos = create_pos;
      xform.scale = Vec3f { 50.f, 50.f, 50.f };
      aabb.AABB_mMin = 0;
      aabb.AABB_mMax = 0;
      aabb.type = CollisionType::AABB;
      colour.colour = Vec3f { 1.f,1.f,1.f };
      material.diffuse = Service::Locator::Get<RM>().Get<Graphics::Texture>( texture_diffuse );
      material.specular = Service::Locator::Get<RM>().Get<Graphics::Texture>( texture_specular );
      material.normal = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.bump = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.shinness = 128.f;
      renderSettings.onLight = true;
      renderSettings.onRender = true;
      mesh.model = Service::Locator::Get<RM>().Get<Graphics::StaticModel>( model );
      mesh.unitScales = mesh.model->unitScales;
    } );
    auto &entry = world->m_entity_manager.Get( entity );
    entry.m_name = archetype_name;
    Draw::OnClick( entity, *ent_context );
    return entity;
  }

  template <ECS::Component::Concepts::IsComponentList List = CList<>>
  Entity CreateAnimationArchetype( Ptr<ECS::World::Instance> world,
                                   Ptr<EntityContext> ent_context,
                                   const std::string archetype_name,
                                   const std::string texture_diffuse,
                                   const std::string texture_specular,
                                   const std::string model,
                                   const Position &create_pos ) noexcept
  {
    auto &archetype = world->GetOrCreateArchetype<AnimationRenderType::Extend<List>>( archetype_name );
    auto entity = archetype.CreateEntity( [&]( Position &pos, Transform &xform, AnimationMeshComponent &mesh,
                                          Material &material, Colour &colour, AABB &aabb, RenderSettings &renderSettings, AnimatorComponent &animator ) noexcept
    {
      pos = create_pos;
      xform.scale = Vec3f { 50.f,50.f,50.f };
      aabb.AABB_mMin = 0;
      aabb.AABB_mMax = 0;
      aabb.type = CollisionType::AABB;
      colour.colour = Vec3f { 1.f,1.f,1.f };
      material.diffuse = Service::Locator::Get<RM>().Get<Graphics::Texture>( texture_diffuse );
      material.specular = Service::Locator::Get<RM>().Get<Graphics::Texture>( texture_specular );
      material.normal = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.bump = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.shinness = 128.f;
      renderSettings.onLight = true;
      renderSettings.onRender = true;
      mesh.model = Service::Locator::Get<RM>().Get<Graphics::AnimationModel>( model );
      animator.UpdateAnimation( 0.f );
      animator.isActive = false;
      animator.numRun = 1;
    } );
    auto &entry = world->m_entity_manager.Get( entity );
    entry.m_name = archetype_name;
    Draw::OnClick( entity, *ent_context );
    return entity;
  }
};
}
#endif