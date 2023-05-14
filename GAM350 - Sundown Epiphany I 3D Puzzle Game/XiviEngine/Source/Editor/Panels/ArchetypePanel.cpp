/*****************************************************************
*\file         ArchetypePanel.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "ArchetypePanel.h"

#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "Components/ParticleComponents.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <IconsFontAwesome5.h>
#include "Editor/ImGui/Helper/ImGuiHelperFunction.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"

namespace Xivi::Editor
{
void ArchetypePanel::AddGameArchetype() noexcept
{
    // Game Floor
  m_archetypes_creator.push_back(
    {
      ICON_FA_CUBE,
      "Floor",
      [&]( const Position &create_pos )
  {
    return CreateStaticGridArchetype( m_world, m_entity_context, "Floor", "Assets/Textures/Floor_1_Texture_Diffuse.dds", "Assets/Textures/Floor_1_Texture_Specular.dds", "Assets/Models/binary/Static/Floor_1_Model.BinaryStatic", create_pos, GameType::Floor );
  } ,
    "Game Tile" } );

// Game Falling Floor
  m_archetypes_creator.push_back(
    {
      ICON_FA_CUBE,
      "Falling Floor",
      [&]( const Position &create_pos )
  {
    return CreateStaticGridArchetype<CList<FallingFloor>>( m_world, m_entity_context, "Falling Floor", "Assets/Textures/FallingFloor_1_Texture_Diffuse.dds", "Assets/Textures/FallingFloor_1_Texture_Specular.dds", "Assets/Models/binary/Static/Floor_1_Model.BinaryStatic", create_pos, GameType::FallingFloor );
  } ,
    "Game Tile" } );

// Door Tile
  m_archetypes_creator.push_back(
    {
      ICON_FA_CUBE,
      "Door Tile",
      [&]( const Position &create_pos )
  {
    return CreateStaticGridArchetype<CList<Lock>>( m_world, m_entity_context, "Door Tile", "Assets/Textures/Floor_1_Texture_Diffuse.dds", "Assets/Textures/Floor_1_Texture_Specular.dds", "Assets/Models/binary/Static/Floor_1_Model.BinaryStatic", create_pos, GameType::Door );
  } ,
    "Game Tile" } );

// Pressure Plate Tile
  m_archetypes_creator.push_back(
    {
      ICON_FA_CUBE,
      "Pressure Plate Tile",
      [&]( const Position &create_pos )
  {
    return CreateStaticGridArchetype<CList<Switch>>( m_world, m_entity_context, "Pressure Plate Tile", "Assets/Textures/Floor_1_Texture_Diffuse.dds", "Assets/Textures/Floor_1_Texture_Specular.dds", "Assets/Models/binary/Static/Floor_1_Model.BinaryStatic", create_pos, GameType::PressurePlate );
  } ,
    "Game Tile" } );

// Switch Tile
  m_archetypes_creator.push_back(
    {
      ICON_FA_CUBE,
      "Switch Tile",
      [&]( const Position &create_pos )
  {
    return CreateStaticGridArchetype<CList<Switch>>( m_world, m_entity_context, "Switch Tile", "Assets/Textures/Floor_1_Texture_Diffuse.dds", "Assets/Textures/Floor_1_Texture_Specular.dds", "Assets/Models/binary/Static/Floor_1_Model.BinaryStatic", create_pos, GameType::Switch );
  } ,
    "Game Tile" } );

// Game Hole Tile
  m_archetypes_creator.push_back(
    {
      ICON_FA_CUBE,
      "Hole Tile",
      [&]( const Position &create_pos )
  {
    auto entity = CreateStaticGridArchetype( m_world, m_entity_context, "Hole Tile", "Assets/Textures/Floor_1_Texture_Diffuse.dds", "Assets/Textures/Floor_1_Texture_Specular.dds", "Assets/Models/binary/Static/Hole_1_Model.BinaryStatic", create_pos, GameType::Hole );
    m_world->GetComponent<GridPosition>( entity ).offset = Vec3f{ .0f, .0f, -25.f };
    m_world->GetComponent<Transform>( entity ).scale = Vec3f{ 100.f, 100.f, 100.f };
    return entity;
  } ,
    "Game Tile" } );

// Game Blocked Tile
  m_archetypes_creator.push_back(
    {
      ICON_FA_CUBE,
      "Blocked Tile",
      [&]( const Position &create_pos )
  {
    return CreateStaticGridArchetype( m_world, m_entity_context, "Blocked Tile", "Assets/Textures/Floor_1_Texture_Diffuse.dds", "Assets/Textures/Floor_1_Texture_Specular.dds", "Assets/Models/binary/Static/Floor_1_Model.BinaryStatic", create_pos, GameType::Blocked );
  } ,
    "Game Tile" } );

// Game Goal Tile
  m_archetypes_creator.push_back(
    {
      ICON_FA_CUBE,
      "Goal Tile",
      [&]( const Position &create_pos )
  {
    return CreateStaticGridArchetype( m_world, m_entity_context, "Goal Tile", "Assets/Textures/Floor_1_Texture_Diffuse.dds", "Assets/Textures/Floor_1_Texture_Specular.dds", "Assets/Models/binary/Static/Floor_1_Model.BinaryStatic", create_pos, GameType::Goal );
  } ,
    "Game Tile" } );

// Game Door
  m_archetypes_creator.push_back(
    {
      ICON_FA_DOOR_CLOSED,
      "Door",
      [&]( const Position &create_pos )
  {
    return CreateAnimationArchetype<CList<DoorObj>>( m_world, m_entity_context, "Door", "Assets/Textures/default.dds", "Assets/Textures/default.dds", "Assets/Models/binary/Animation/Door_Open.BinaryAnimated", create_pos );
  } ,
    "Game Object to Attach to Tile" } );

// Game Cheese
  m_archetypes_creator.push_back(
    {
      ICON_FA_CHEESE,
      "Cheese",
      [&]( const Position &create_pos )
  {
    return CreateStaticGridArchetype( m_world, m_entity_context, "Cheese", "Assets/Textures/Cheese_1_Texture_Diffuse.dds", "Assets/Textures/Cheese_1_Texture_Diffuse.dds", "Assets/Models/binary/Static/Cheese_1_Model.BinaryStatic", create_pos, GameType::Cheese );
  } ,
    "Standalone Game Object" } );

// Game Movable Cart
  m_archetypes_creator.push_back(
    {
      ICON_FA_SHOPPING_CART,
      "Moving Cart",
      [&]( const Position &create_pos )
  {
    auto entity = CreateStaticGridArchetype<CList<Lock, NewPosition, NewTransform>>( m_world, m_entity_context, "Moving Cart", "Assets/Textures/MovableObject_1_Texture_Diffuse.dds", "Assets/Textures/MovableObject_1_Texture_Specular.dds", "Assets/Models/binary/Static/MovableObject_1_Model.BinaryStatic", create_pos, GameType::Box );
    m_world->GetComponent<NewPosition>( entity ).speed = 7.5f;
    m_world->GetComponent<Transform>( entity ).scale = Vec3f{ 80.f, 120.f, 50.f };
    return entity;
  } ,
    "Standalone Game Object" } );

// Game Movable Platform
  m_archetypes_creator.push_back(
    {
      ICON_FA_SQUARE,
      "Moving Platform",
      [&]( const Position &create_pos )
  {
    auto entity = CreateStaticGridArchetype<CList<Waypoints, Lock, NewPosition, NewTransform>>( m_world, m_entity_context, "Moving Platform", "Assets/Textures/MovingPlatform_1_Texture_Diffuse.dds", "Assets/Textures/MovingPlatform_1_Model_Texture_Specular.dds", "Assets/Models/binary/Static/MovingPlatform_1_Model.BinaryStatic", create_pos, GameType::MovingPlatform );
    m_world->GetComponent<NewPosition>( entity ).speed = 7.5f;
    return entity;
  } ,
    "Standalone Game Object" } );

// Game Movable Obstacle
  m_archetypes_creator.push_back(
    {
      ICON_FA_ROBOT,
      "Moving Obstacle",
      [&]( const Position &create_pos )
  {
    auto entity = CreateStaticGridArchetype<CList<Waypoints, Lock, NewPosition, NewTransform>>( m_world, m_entity_context, "Moving Obstacle", "Assets/Textures/MovingHazard_1_Texture_Diffuse.dds", "Assets/Textures/MovingHazard_1_Texture_Specular.dds", "Assets/Models/binary/Static/MovingHazard_1_Model.BinaryStatic", create_pos, GameType::MovingObstacle );
    m_world->GetComponent<NewPosition>( entity ).speed = 7.5f;
    m_world->GetComponent<Direction>( entity ).speed = 100.f;
    return entity;
  } ,
    "Standalone Game Object" } );

// Game Pressure Plate
  m_archetypes_creator.push_back(
    {
      ICON_FA_PLUS_SQUARE,
      "Pressure Plate",
      [&]( const Position &create_pos )
  {
    return CreateAnimationArchetype<CList<PressurePlateObj>>( m_world, m_entity_context, "Pressure Plate", "Assets/Textures/PressurePlate_Pressed_Texture_Defuse.dds", "Assets/Textures/PressurePlate_Pressed_Texture_Specular.dds", "Assets/Models/binary/Animation/PressurePlate_Pressed.BinaryAnimated", create_pos );
  } ,
    "Game Object to Attach to Tile" } );

// Game Switch
  m_archetypes_creator.push_back(
    {
      ICON_FA_SLASH,
      "Switch",
      [&]( const Position &create_pos )
  {
    return CreateAnimationArchetype<CList<SwitchObj>>( m_world, m_entity_context, "Switch", "Assets/Textures/Switch_1_Texture_Diffuse.dds", "Assets/Textures/Switch_1_Texture_Specular.dds", "Assets/Models/binary/Animation/Switch_1_Pressed.BinaryAnimated", create_pos );
  },
    "Game Object to Attach to Tile" } );

// Game Goal
  m_archetypes_creator.push_back(
    {
      ICON_FA_AWARD,
      "Goal",
      [&]( const Position &create_pos )
  {
    return CreateStaticArchetype<CList<GoalObj>>( m_world, m_entity_context, "Goal", "Assets/Textures/Goal_1_Texture_Diffuse.dds", "Assets/Textures/Goal_1_Texture_Specular.dds", "Assets/Models/binary/Static/Trap_1_Model.BinaryStatic", create_pos );
  },
    "Game Object to Attach to Tile" } );

// Game Microscope
  m_archetypes_creator.push_back(
    {
      ICON_FA_MICROSCOPE,
      "Microscope",
      [&]( const Position &create_pos )
  {
    return CreateStaticArchetype( m_world, m_entity_context, "Microscope", "Assets/Textures/PropTextures.dds", "Assets/Textures/PropTextures.dds", "Assets/Models/binary/Static/Microscope.BinaryStatic", create_pos );
  },
    "Props" } );

// Game Microscope Voxel
  m_archetypes_creator.push_back(
    {
      ICON_FA_MICROSCOPE,
      "Microscope Voxel",
      [&]( const Position &create_pos )
  {
    auto entity = CreateStaticArchetype( m_world, m_entity_context, "Microscope Voxel", "Assets/Textures/PropTextures.dds", "Assets/Textures/PropTextures.dds", "Assets/Models/binary/Static/MicroscopeVoxel.BinaryStatic", create_pos );
    m_world->GetComponent<Transform>( entity ).scale = Vec3f{ 45.f, 60.f, 45.f };
    return entity;
  },
    "Props" } );

// Game DigitalMicroscope
  m_archetypes_creator.push_back(
    {
      ICON_FA_MICROSCOPE,
      "Digital Microscope",
      [&]( const Position &create_pos )
  {
    return CreateStaticArchetype( m_world, m_entity_context, "Digital Microscope", "Assets/Textures/PropTextures.dds", "Assets/Textures/PropTextures.dds", "Assets/Models/binary/Static/DigitalMicroscope.BinaryStatic", create_pos );
  },
    "Props" } );

// Game Capsule
  m_archetypes_creator.push_back(
    {
      ICON_FA_CAPSULES,
      "Capsule",
      [&]( const Position &create_pos )
  {
    auto entity = CreateStaticArchetype( m_world, m_entity_context, "Capsule", "Assets/Textures/PropTextures.dds", "Assets/Textures/PropTextures.dds", "Assets/Models/binary/Static/Capsule.BinaryStatic", create_pos );
    m_world->GetComponent<Transform>( entity ).scale = Vec3f{ 75.f, 75.f, 75.f };
    return entity;
  },
    "Props" } );

// Game AIBot
  m_archetypes_creator.push_back(
    {
      ICON_FA_ROBOT,
      "AI Bot",
      [&]( const Position &create_pos )
  {
    return CreateStaticArchetype( m_world, m_entity_context, "AI Bot", "Assets/Textures/PropTextures.dds", "Assets/Textures/PropTextures.dds", "Assets/Models/binary/Static/AIBot.BinaryStatic", create_pos );
  },
    "Props" } );

// Game Flower
  m_archetypes_creator.push_back(
    {
      ICON_FA_SUN,
      "Flower",
      [&]( const Position &create_pos )
  {
    return CreateStaticArchetype( m_world, m_entity_context, "Flower", "Assets/Textures/PropTextures.dds", "Assets/Textures/PropTextures.dds", "Assets/Models/binary/Static/Flower.BinaryStatic", create_pos );
  },
    "Props" } );

// Game Lightstand
  m_archetypes_creator.push_back(
    {
      ICON_FA_TRAFFIC_LIGHT,
      "Lightstand",
      [&]( const Position &create_pos )
  {
    auto entity = CreateStaticArchetype( m_world, m_entity_context, "Lightstand", "Assets/Textures/PropTextures.dds", "Assets/Textures/PropTextures.dds", "Assets/Models/binary/Static/Lightstand.BinaryStatic", create_pos );
    m_world->GetComponent<Transform>( entity ).scale = Vec3f{ 50.f, 75.f, 50.f };
    return entity;
  },
    "Props" } );

// Game Prop
  m_archetypes_creator.push_back(
    {
      ICON_FA_CUBE,
      "Prop",
      [&]( const Position &create_pos )
  {
    return CreateStaticArchetype( m_world, m_entity_context, "Prop", "Assets/Textures/PropTextures.dds", "Assets/Textures/PropTextures.dds", "Assets/Models/binary/Static/Prop.BinaryStatic", create_pos );
  },
    "Props" } );

// Game Prop2
  m_archetypes_creator.push_back(
    {
      ICON_FA_CUBE,
      "Prop1",
      [&]( const Position &create_pos )
  {
    return CreateStaticArchetype( m_world, m_entity_context, "Prop1", "Assets/Textures/PropTextures.dds", "Assets/Textures/PropTextures.dds", "Assets/Models/binary/Static/Prop1.BinaryStatic", create_pos );
  },
    "Props" } );

// Game Prop2
  m_archetypes_creator.push_back(
    {
      ICON_FA_CUBE,
      "Prop2",
      [&]( const Position &create_pos )
  {
    return CreateStaticArchetype( m_world, m_entity_context, "Prop2", "Assets/Textures/PropTextures.dds", "Assets/Textures/PropTextures.dds", "Assets/Models/binary/Static/Prop2.BinaryStatic", create_pos );
  },
    "Props" } );

// Game Barricade
  m_archetypes_creator.push_back(
    {
      ICON_FA_CUBE,
      "Barricade",
      [&]( const Position &create_pos )
  {
    return CreateStaticArchetype( m_world, m_entity_context, "Barricade", "Assets/Textures/PropTextures.dds", "Assets/Textures/PropTextures.dds", "Assets/Models/binary/Static/Barricade.BinaryStatic", create_pos );
  },
    "Props" } );

// Game ScifiRat
  m_archetypes_creator.push_back(
    {
      ICON_FA_MOUSE,
      "ScifiRat",
      [&]( const Position &create_pos )
  {
    auto entity = CreateStaticArchetype( m_world, m_entity_context, "ScifiRat", "Assets/Textures/PropTextures.dds", "Assets/Textures/PropTextures.dds", "Assets/Models/binary/Static/ScifiRat.BinaryStatic", create_pos );
    m_world->GetComponent<Transform>( entity ).scale = Vec3f{ 40.f, 40.f, 40.f };
    return entity;
  },
    "Props" } );
}

ArchetypePanel::ArchetypePanel( std::string name,
                                Ptr<ECS::World::Instance> world,
                                Ptr<Config> configs,
                                Ptr<EntityContext> entity_context,
                                Ptr<FileContext> file_context,
                                Ptr<ToolContext> tool_context ) :
  IPanel( name, world, configs, entity_context, file_context, tool_context )
{
    // Cube Factory
  m_archetypes_creator.push_back(
    {
      ICON_FA_CUBE,
      "Cube",
      [&]( const Position &create_pos )
  {
    return CreateStaticArchetype( m_world, m_entity_context, "Cube", "Assets/Textures/default.dds", "Assets/Textures/default.dds", "Assets/Models/binary/Object/cube.BinaryObj", create_pos );
  },
      "Basic Shape" } );

//   Sphere Factory
  m_archetypes_creator.push_back(
    {
      ICON_FA_CIRCLE,
      "Sphere",
      [&]( const Position &create_pos )
  {
    return CreateStaticArchetype( m_world, m_entity_context, "Sphere", "Assets/Textures/default.dds", "Assets/Textures/default.dds", "Assets/Models/binary/Object/Sphere.BinaryObj", create_pos );
   },
      "Basic Shape" } );

  // Plane Factory
  m_archetypes_creator.push_back(
    {
      ICON_FA_SQUARE,
      "Plane",
      [&]( const Position &create_pos )
  {
    return CreateStaticArchetype( m_world, m_entity_context, "Plane", "Assets/Textures/default.dds", "Assets/Textures/default.dds", "Assets/Models/binary/Object/plane.BinaryObj", create_pos );
  },
      "Basic Shape" } );

  // Point Light Factory
  m_archetypes_creator.push_back(
    {
      ICON_FA_LIGHTBULB,
      "Point Light",
      [&]( const Position &create_pos )
  {
    auto &archetype = m_world->GetOrCreateArchetype<LightType>( "Point Light" );
    auto entity = archetype.CreateEntity( [&]( Position &pos, Transform &xform, StaticMeshComponent &mesh, Colour &colour, LightProperty &lightProperty, QuadraticFunction &qFunc, RenderSettings &renderSettings ) noexcept
    {
      pos = create_pos;
      xform.scale = Vec3f { .1f, .1f, .1f };
      mesh.model = Service::Locator::Get<RM>().Get<Graphics::StaticModel>( "Assets/Models/binary/Object/Sphere.BinaryObj" );
      mesh.unitScales = mesh.model->unitScales;
      qFunc.constant = 1.0f;
      qFunc.linear = 0.1f;
      // qFunc.quadratic = 0.032f;
      qFunc.quadratic = 0.0f;
      lightProperty.radius = 1000.f;
      lightProperty.ambient = Vec4f { 1.f, 1.f, 1.f,1.f };
      lightProperty.diffuse = Vec4f { 0.5f, 0.5f, 0.5f,1.f };
      lightProperty.specular = Vec4f { 0.5f, 0.5f, 0.5f,1.f };
      renderSettings.onLight = false;
      renderSettings.onRender = true;
      colour.colour = Vec3f { 1.f, 1.f, 1.f };
      auto &frameBuffer = Service::Locator::Get<Graphics::Manager>().frameBuffer;
      frameBuffer->CreateShadowDepthMap();
      lightProperty.frameBufferID = frameBuffer->GetDepthMapFBO();
      lightProperty.shadowMapID = frameBuffer->GetDepthMap();
    } );
    auto &entry = m_world->m_entity_manager.Get( entity );
    entry.m_name = "Point Light";
    Draw::OnClick( entity, *m_entity_context );
    return entity;
  },
      "Lighting" } );

  // Directional Light Factory
  m_archetypes_creator.push_back(
    {
      ICON_FA_SUN,
      "Directional Lighting",
      [&]( const Position &create_pos )
  {
    auto &archetype = m_world->GetOrCreateArchetype<DirLightType>( "Directional Lighting" );
    auto entity = archetype.CreateEntity( [&]( Position &pos, Transform &xform, StaticMeshComponent &mesh, Colour &colour, LightProperty &lightProperty, Direction &dir, RenderSettings &renderSettings ) noexcept
    {
      pos = create_pos;
      xform.scale = Vec3f { 1.f, 1.f, 1.f };
      mesh.model = Service::Locator::Get<RM>().Get<Graphics::StaticModel>( "Assets/Models/binary/Object/cube.BinaryObj" );
      mesh.unitScales = mesh.model->unitScales;
      lightProperty.ambient = Vec4f { .001f, .001f, .001f,1.f };
      lightProperty.diffuse = Vec4f { 0.010f, 0.010f, 0.010f,1.f };
      lightProperty.specular = Vec4f { 0.010f, 0.010f, 0.010f,1.f };
      renderSettings.onLight = false;
      renderSettings.onRender = true;
      dir.m_dir = Vec3f{ 0.f, -1.f, 0.f };
      dir.endPoint = Vec3f{ 0.f, 0.f, 0.f };
      colour.colour = Vec3f { 1.f, 1.f, 1.f };
      auto &frameBuffer = Service::Locator::Get<Graphics::Manager>().frameBuffer;
      frameBuffer->CreateShadowDepthMap();
      lightProperty.frameBufferID = frameBuffer->GetDepthMapFBO();
      lightProperty.shadowMapID = frameBuffer->GetDepthMap();
    } );
    auto &entry = m_world->m_entity_manager.Get( entity );
    entry.m_name = "Directional Lighting";
    Draw::OnClick( entity, *m_entity_context );
    return entity;
  },
      "Lighting" } );

  // Collider box Factory
  m_archetypes_creator.push_back(
    {
      ICON_FA_BOX,
      "Collider Box",
      [&]( const Position &create_pos )
  {
    auto &archetype = m_world->GetOrCreateArchetype<ColliderBoxType>( "Collider Box" );
    auto entity = archetype.CreateEntity( [&]( Position &pos, Transform &xform, StaticMeshComponent &mesh, Colour &colour, AABB &aabb, RenderSettings &renderSettings ) noexcept
    {
      pos = create_pos;
      xform.scale = Vec3f { 1.f, 1.f, 1.f };
      mesh.model = Service::Locator::Get<RM>().Get<Graphics::StaticModel>( "Assets/Models/binary/Object/cube.BinaryObj" );
      mesh.unitScales = mesh.model->unitScales;
      renderSettings.onLight = true;
      renderSettings.onRender = true;
      colour.colour = Vec3f { 1.f, 0.f, 0.f };
    } );
    auto &entry = m_world->m_entity_manager.Get( entity );
    entry.m_name = "Collider Box";
    Draw::OnClick( entity, *m_entity_context );
    return entity;
  },
      "Collider" } );

  // Animation Factory
  m_archetypes_creator.push_back(
    {
      ICON_FA_WATER,
      "Animation",
      [&]( const Position &create_pos )
  {
    return CreateAnimationArchetype( m_world, m_entity_context, "Animation", "Assets/Textures/default.dds", "Assets/Textures/default.dds", "Assets/Models/binary/Animation/idle.BinaryAnimated", create_pos );
  },
      "Animation" } );

  m_archetypes_creator.push_back(
    {
      ICON_FA_CAMERA,
      "Camera",
      [&]( const Position &create_pos )
  {
    auto &archetype = m_world->GetOrCreateArchetype<CameraType>( "Camera" );
    auto entity = archetype.CreateEntity( [&]( Position &pos, Camera_c &cam ) noexcept
    {
      pos = create_pos;
      cam.maxDistance = 300.0f;
      cam.minDistance = 100.0f;
      cam.mPhi_initial = 120.0f;
      cam.sentivity = 0.05f;
    } );
    auto &entry = m_world->m_entity_manager.Get( entity );
    entry.m_name = "Camera";
    Draw::OnClick( entity, *m_entity_context );
    return entity;
  },
    "Camera" } );

  // Player Factory
  m_archetypes_creator.push_back(
    {
      ICON_FA_BABY,
      "Player",
      [&]( const Position &create_pos )
  {
    auto &archetype = m_world->GetOrCreateArchetype<PlayerType>( "Player" );
    auto entity = archetype.CreateEntity( [&]( Position &pos, Transform &xform, AnimationMeshComponent &mesh,
        Material &material, Colour &colour, RenderSettings &renderSettings, AABB &aabb, EntityType &entity_type,
        Direction &dir, NewPosition &new_position, NewTransform &new_transform, GridPosition &grid_pos ) noexcept
    {
      dir.speed = 100.f;
      new_position.speed = 6.f;
      new_transform.speed = 5.f;
      grid_pos.offset = Vec3f( 0.f, -6.5f, 0.f );
      pos = create_pos;
      xform.scale = Vec3f { 100.f, 100.f, 100.f };
      aabb.AABB_mMin = 0;
      aabb.AABB_mMax = 0;
      aabb.type = CollisionType::AABB;
      colour.colour = Vec3f{ 1.f, 1.f, 1.f };
      material.diffuse = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/ratUV.dds" );
      material.specular = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/ratUV.dds" );
      material.normal = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.bump = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.shinness = 128.f;
      renderSettings.onLight = true;
      renderSettings.onRender = true;
      entity_type.value = GameType::Player;
      mesh.model = Service::Locator::Get<RM>().Get<Graphics::AnimationModel>( "Assets/Models/binary/Animation/idle.BinaryAnimated" );
    } );
    auto &entry = m_world->m_entity_manager.Get( entity );
    entry.m_name = "Player";
    Draw::OnClick( entity, *m_entity_context );
    return entity;
  },
    "Standalone Game Object" } );

  AddGameArchetype();
}

ArchetypePanel::~ArchetypePanel()
{
  m_enabled = false;
}

void ArchetypePanel::Render() noexcept
{
  if ( !m_enabled )
    return;

  ImGuiHelper::CreateImGuiWindow( m_name.c_str(), &m_enabled, [&]
  {
    if ( ImGui::IsWindowFocused( ImGuiFocusedFlags_RootAndChildWindows ) )
      ImGui::SetFocusID( ImHashStr( m_name.c_str() ), ImGui::GetCurrentWindow() );
    ImGuiHelper::Group( [&]
    {
      for ( auto &type : m_archetype_types )
      {
        if ( ImGui::CollapsingHeader( type, ( ImGuiTreeNodeFlags_FramePadding & ImGuiTreeNodeFlags_AllowItemOverlap ) | ImGuiTreeNodeFlags_DefaultOpen ) )
        {
          for ( auto &[label, name, func, group] : m_archetypes_creator )
          {
            if ( std::strcmp( type, group ) )
              continue;
            ImGuiHelper::Group( [&]
            {
              ImGui::Button( label, { 48.f,48.f } );
              ImGui::SameLine();
              ImGui::Text( name );
            } );
            if ( ImGui::IsItemClicked() )
            {
              m_create_ptr = &func;
              m_create_ptr_name = name;
            }
            if ( ImGuiHelper::IsItemHovered() )
            {
              ImVec2 m = ImGui::GetIO().MousePos;
              ImGui::SetNextWindowPos( ImVec2( m.x + 10, m.y + 10 ) );
              ImGuiHelper::Tooltip( [&]
              {
                ImGui::Text( name );
                ImGui::Spacing();
                ImGui::Text( "Component list:" );

                if ( !std::strcmp( name, "Cube" ) || !std::strcmp( name, "Sphere" ) || !std::strcmp( name, "Plane" ) || !std::strcmp( group, "Props" ) )
                  PrintComponentsName<StaticRenderType>();
                else if ( !std::strcmp( name, "Point Light" ) )
                  PrintComponentsName<LightType>();
                else if ( !std::strcmp( name, "Directional Lighting" ) )
                  PrintComponentsName<DirLightType>();
                else if ( !std::strcmp( name, "Collider Box" ) )
                  PrintComponentsName<ColliderBoxType>();
                else if ( !std::strcmp( name, "Player" ) )
                  PrintComponentsName<PlayerType>();
                else if ( !std::strcmp( name, "Animation" ) )
                  PrintComponentsName<AnimationRenderType>();
                else if ( !std::strcmp( name, "Floor" ) || !std::strcmp( name, "Goal" ) || !std::strcmp( name, "Hole Tile" ) || !std::strcmp( name, "Cheese" ) )
                  PrintComponentsName<StaticGridType>();
                else if ( !std::strcmp( name, "Pressure Plate Tile" ) || !std::strcmp( name, "Switch Tile" ) )
                  PrintComponentsName<StaticGridType::Extend<CList<Switch>>>();
                else if ( !std::strcmp( name, "Goal Tile" ) )
                  PrintComponentsName<StaticGridType::Extend<CList<GoalObj>>>();
                else if ( !std::strcmp( name, "Door Tile" ) )
                  PrintComponentsName<StaticGridType::Extend<CList<Lock>>>();
                else if ( !std::strcmp( name, "Moving Cart" ) )
                  PrintComponentsName<StaticGridType::Extend<CList<Lock, NewPosition, NewTransform>>>();
                else if ( !std::strcmp( name, "Moving Obstacle" ) || !std::strcmp( name, "Moving Platform" ) )
                  PrintComponentsName<StaticGridType::Extend<CList<Waypoints, Lock, NewPosition, NewTransform>>>();
                else if ( !std::strcmp( name, "Door" ) || !std::strcmp( name, "Switch" ) )
                  PrintComponentsName<AnimationRenderType>();
                else if ( !std::strcmp( name, "Pressure Plate" ) )
                  PrintComponentsName<AnimationRenderType::Extend<CList<PressurePlateC>>>();
                else if ( !std::strcmp( name, "Camera" ) )
                  PrintComponentsName<CameraType>();
              } );
            }
          }
        }
        ImGui::Text( "" );
      }
    } );
// Drag and drop
    ImGuiDragDropFlags src_flags = ImGuiDragDropFlags_SourceNoDisableHover |
      ImGuiDragDropFlags_SourceAllowNullID;

    if ( ImGui::BeginDragDropSource( src_flags ) )
    {
      ImGui::SetDragDropPayload( "CreateFromArchetype", m_create_ptr, sizeof( ArchetypeCreateFunc ) );
      ImGui::Text( m_create_ptr_name.c_str() );
      ImGui::EndDragDropSource();
    }
  } );
}
}
#endif