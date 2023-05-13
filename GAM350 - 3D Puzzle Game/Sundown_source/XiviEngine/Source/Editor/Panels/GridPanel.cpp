/*****************************************************************
*\file         GridPanel.cpp
*\author(s)    Chng Yong Quan, Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "GridPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "Editor/ImGui/Helper/ImGuiHelperFunction.h"

#include "../Sundown/Source/NComponents/Grid/Grid.h"
#include "ArchetypePanel.h" // lazy header include

#include "Editor/Command/Actions/CreatePrefabInstanceCommand.h"
#include "Editor/Command/Actions/CreatePrefabCopyCommand.h"
#include "../Sundown/Source/Components/Components.h"
#include "Components/GraphicsComponents.h"
#include "Events/GraphicsEvents.h"
namespace Xivi::Editor
{
  /*
  static variable
  */

static Vec3f blocked_tile_offset {};
static Vec3f blocked_tile_color {};
static Vec3f floor_tile_color {};

/*
Helper
*/
void AddFloorTextureNormal( ECS::World::Instance &world )
{
  auto &m_resource_manager = Service::Locator::Get<RM>();
  world.ForEach( [&](
    Entity &entity,
    EntityType &type, Material &material )
  {
    if ( type.value == GameType::Floor || type.value == GameType::Blocked )
    {
      material.normal = m_resource_manager.Get<Graphics::Texture>( "Assets/Textures/normal.dds" );
      material.bump = m_resource_manager.Get<Graphics::Texture>( "Assets/Textures/Floor_1_Texture__Height.dds" );
      material.height_scale = 0.f;
      material.parallaxMappingOn = true;
    }
  } );
}
void CameraInit( ECS::World::Instance &world )
{
  world.ForEach( [&](
    Entity &entity,
    Camera_c &camera_c )
  {
    //camera_c.mNear = 10.f;
    //camera_c.minDistance = 80.f;
    world.AddOrRemoveComponents<CList<CameraInitalized>, CList<>>( entity );
  } );
}

void FixGrid( ECS::World::Instance &world )
{
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();
  grid.Clear();
  world.ForEach( [&]( Entity &entity, Position &pos, GridPosition &grid_pos, EntityType &type ) noexcept
  {
    if ( grid.BoundsCheck( grid_pos.value ) )
    {
      grid.UpdateCell( grid_pos.value, type.value, entity );
      pos.m_position = grid.WorldSpace( grid_pos.value ) + grid_pos.offset;
    }
  } );
}

void AddCheckFallTag( ECS::World::Instance &world )
{
  world.ForEach( [&]( Entity &entity, EntityType &type )
  {
    switch ( type.value )
    {
      case GameType::Player: [[fallthrough]];
      case GameType::Box: world.AddOrRemoveComponents<CList<CheckFall>>( entity ); break;
      case GameType::Cheese: world.AddOrRemoveComponents<CList<Hover>>( entity ); break;
    }
  } );
  world.m_archetype_manager.UpdateStructuralChanges();
}

void AddNewPosition( ECS::World::Instance &world )
{
  world.ForEach( [&]( Entity &entity, GridPosition &grid_pos )
  {
    world.AddOrRemoveComponents<CList<NewPosition>>( entity );
  } );
  world.m_archetype_manager.UpdateStructuralChanges();
}

void NormalizeMovements( ECS::World::Instance &world )
{
  world.ForEach( [&]( Entity &entity, EntityType &type, NewPosition &new_pos, Direction &direction )
  {
    switch ( type.value )
    {
      case GameType::MovingObstacle: [[fallthrough]];
      case GameType::Box: [[fallthrough]];
      case GameType::MovingPlatform:
      new_pos.speed = 4.f;
      direction.speed = 3.f;
      break;
      case GameType::Player:
      new_pos.speed = 3.5f;
      direction.speed = 4.f;
      world.GetComponent<NewTransform>( entity ).speed = 2.f;
      break;
    }
  } );
}

void UpdateBlockTilesOffset( ECS::World::Instance &world )
{
  auto &grid = world.GetOrCreateSingletonComponent<GameGrid>();

  world.ForEach( [&]( Entity &entity, EntityType &type, Position &pos, GridPosition &grid_pos )
  {
    switch ( type.value )
    {
      case GameType::Blocked:
      {
        if ( !world.HaveComponent<LeveledTile>( entity ) )
        {
          grid_pos.offset = blocked_tile_offset;
          pos.m_position = grid.WorldSpace( grid_pos.value ) + grid_pos.offset;
        }
        break;
      }
    }
  } );
}

void UpdateBlockedTileColor( ECS::World::Instance &world )
{
  world.ForEach( [&]( Entity &entity, EntityType &type, Colour &colour )
  {
    if ( type.value == GameType::Blocked )
      colour.colour = blocked_tile_color;
  } );
}

void UpdateFloorTileColor( ECS::World::Instance &world )
{
  world.ForEach( [&]( Entity &entity, EntityType &type, Colour &colour )
  {
    if ( type.value == GameType::Floor )
      colour.colour = floor_tile_color;
  } );
}
void ChangeSwitch( ECS::World::Instance &world )
{
  auto &m_resource_manager = Service::Locator::Get<RM>();
  world.ForEach( [&](
    Entity &entity,
    EntityType &type, Child &_child )
  {
    if ( type.value == GameType::Switch )
    {
      for ( auto &x : _child.m_entities )
      {
        if ( world.HaveComponent<AnimatorComponent>( x ) )
        {
          auto &ani = world.GetComponent<AnimatorComponent>( x );
          ani.speed = 5.f;
        }
      }
    }
  } );
}

void UpdateFloorForPrefab( ECS::World::Instance &world )
{
  auto &m_resource_manager = Service::Locator::Get<RM>();
  world.ForEach( [&](
    Entity &entity,
    EntityType &type, Material &material )
  {
    if ( type.value == GameType::Switch || type.value == GameType::Door || type.value == GameType::PressurePlate )
    {
      material.normal = m_resource_manager.Get<Graphics::Texture>( "Assets/Textures/normal.dds" );
      material.bump = m_resource_manager.Get<Graphics::Texture>( "Assets/Textures/Floor_1_Texture__Height.dds" );
      material.height_scale = 0.f;
      material.parallaxMappingOn = true;
    }
  } );
}

void UpdateAndFixGrid( ECS::World::Instance &world )
{
  FixGrid( world );
  AddCheckFallTag( world );
  AddNewPosition( world );
  NormalizeMovements( world );
  AddFloorTextureNormal( world );
  CameraInit( world );
  ChangeSwitch( world );
  UpdateFloorForPrefab( world );
}

/*
Grid Panel
*/

GridPanel::GridPanel( std::string name,
                      Ptr<ECS::World::Instance> world,
                      Ptr<Config> configs,
                      Ptr<EntityContext> entity_context,
                      Ptr<FileContext> file_context,
                      Ptr<ToolContext> tool_context ) :
  IPanel( name, world, configs, entity_context, file_context, tool_context )
{
  m_world->GetOrCreateSingletonComponent<GameGrid>().SetGridSize( { 5,5,5 } );
  m_editor_sys = &System::Instance();
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();

  m_recur_push = [&]( Entity ent ) -> void
  {
    m_canvas_stack->Push( ent );
    if ( m_world->HaveComponent<Child>( ent ) )
    {
      for ( auto &child : m_world->GetComponent<Child>( ent ).m_entities )
        m_recur_push( child );
    }
  };

  m_recur_update_animator = [&]( Entity ent ) -> void
  {
    if ( m_world->HaveComponent<AnimatorComponent>( ent ) )
      m_world->GetComponent<AnimatorComponent>( ent ).UpdateAnimation( 0.f );
  };
}

GridPanel::~GridPanel()
{
  m_enabled = false;
}

void GridPanel::Render() noexcept
{
  if ( !m_enabled )
    return;

  ImGuiHelper::CreateImGuiWindow( m_name.c_str(), &m_enabled, [&]
  {
    ImGuiHelper::ChildWindow( "Grid Configuration", true, [&]
    {
      ImGui::Text( "Settings" );
      ImGui::Separator();

      ImGui::Text( "Grid Size" );
      auto &grid = m_world->GetOrCreateSingletonComponent<GameGrid>();
      auto grid_size = grid.grid_size;
      if ( ImGui::DragInt3( "##Grid_size", std::bit_cast<Ptr<int>>( &grid_size ), 1.0f ) )
      {
        grid_size.x = Math::clamp( grid_size.x, 1, 500 );
        grid_size.y = Math::clamp( grid_size.y, 1, 500 );
        grid_size.z = Math::clamp( grid_size.z, 1, 500 );
        grid.SetGridSize( grid_size );
      }

      ImGui::Text( "" );

      ImGui::Text( "Cell Size" );
      ImGui::DragFloat3( "##Cell_size", std::bit_cast<Ptr<f32>>( &grid.cell_size ), 0.01f, 1.0f );

      ImGui::Text( "" );

      ImGui::Text( "Blocked Tile Offset Value" );
      if ( ImGui::DragFloat3( "##BlockTileOffset", std::bit_cast<Ptr<f32>>( &blocked_tile_offset ) ) )
        UpdateBlockTilesOffset( *m_world );

      ImGui::Text( "" );

      ImGui::Text( "Blocked Tile Color Value" );
      if ( ImGui::ColorEdit3( "##BlockedTileColor", std::bit_cast<Ptr<f32>>( &blocked_tile_color ) ) )
        UpdateBlockedTileColor( *m_world );

      ImGui::Text( "" );

      ImGui::Text( "Floor Tile Color Value" );
      if ( ImGui::ColorEdit3( "##FloorTileColor", std::bit_cast<Ptr<f32>>( &floor_tile_color ) ) )
        UpdateFloorTileColor( *m_world );

      ImGui::Text( "" );

      if ( ImGui::Button( "Fix Grid And Update Grid Entities" ) )
        UpdateAndFixGrid( *m_world );

      ImGui::Text( "" );
      ImGui::Text( "" );
      if ( ImGui::Button( "Camera Init" ) )
        CameraInit( *m_world );
      if ( ImGui::Button( "Floor Textures" ) )
        AddFloorTextureNormal( *m_world );
      if (ImGui::Button("SwitchSpeed"))
         ChangeSwitch( *m_world);
      ImGui::Text( "" );

      static int selection = 0;
      int count = 0;
      ImGui::Text( "Insert Type" );
      ImGui::Separator();
      ImGui::Text( "Archtype:" );
      ImGui::RadioButton( "Floor Tiles", &selection, count++ );
      ImGui::RadioButton( "Block Tiles", &selection, count++ );
      ImGui::RadioButton( "Hole Tiles", &selection, count++ );
      ImGui::RadioButton( "Falling Tiles", &selection, count++ );

      vector<tuplet::pair<int, Entity>> grid_prefab_entity;
      static tuplet::tuple<Entity, Vec3f, Vec3i, GameType> inst_to_create;

      if ( m_entity_context->prefab_entity.size() )
      {
        ImGui::Text( "" );
        ImGui::Text( "Prefab:" );
        for ( auto &grid_prefab : m_entity_context->prefab_entity )
        {
          if ( m_world->HaveComponent<GridPosition>( grid_prefab ) )
          {
            auto &entry = m_world->m_entity_manager.Get( grid_prefab );
            ImGui::RadioButton( entry.m_name.c_str(), &selection, count );
            grid_prefab_entity.push_back( { count++, grid_prefab } );
          }
        }
        ImGui::Text( "" );      for ( auto &grid_prefab : m_entity_context->prefab_entity )
        {
          if ( m_world->HaveComponent<GridPosition>( grid_prefab ) )
          {
            auto &entry = m_world->m_entity_manager.Get( grid_prefab );
            ImGui::RadioButton( entry.m_name.c_str(), &selection, count );
            grid_prefab_entity.push_back( { count++, grid_prefab } );
          }
        }
        ImGui::Text( "" );
      }

      ImGui::Text( "Remove:" );
      ImGui::RadioButton( "Delete Tiles", &selection, count );

      for ( auto y = 0; y < grid.grid_size.y; ++y )
      {
        ImGui::Text( "" );
        const char *level_char = ( "Level " + std::to_string( y ) ).c_str();
        ImGui::Text( level_char );
        auto column = grid.grid_size.z;
        auto cell_id = 0;
        ImGuiHelper::Table( level_char, column, ImGuiTableFlags_Borders, [&]
        {
          // To update to show all tile filling
          for ( auto z = grid.grid_size.z - 1; z >= 0; --z )
          {
            ImGui::TableNextRow();
            for ( auto x = grid.grid_size.x - 1; x >= 0; --x )
            {
              ImGui::TableNextColumn();
              // Add Button

              Vec3i cell = { x, y, z };
              if ( ImGui::Button( ( std::string( "##" ) + std::to_string( cell_id++ ) ).c_str(), ImVec2( 12.f, 12.f ) ) )
              {
                if ( selection == count && grid.GetCellType( cell ) != GameType::Empty )
                {
                  auto cell_entity = grid.GetCellEntity( cell );
                  m_world->DestroyEntity( cell_entity );
                }
                else if ( selection > 3 && grid.GetCellType( cell ) == GameType::Empty )
                {
                  for ( auto &[id, prefab_entity] : grid_prefab_entity )
                    if ( id == selection )
                    {
                      ImGui::OpenPopup( "Create Prefab Instance/Copy Grid" );
                      inst_to_create = { prefab_entity, grid.WorldSpace( cell ), cell, m_world->GetComponent<EntityType>( prefab_entity ).value };
                      break;
                    }
                }
                else if ( selection <= 3 && grid.GetCellType( cell ) == GameType::Empty )
                {
                  auto &archetype = m_world->GetOrCreateArchetype<StaticGridType>();
                  auto entity = archetype.CreateEntity( [&]( Position &pos,
                                                        Transform &xform,
                                                        Material &material,
                                                        AABB &aabb,
                                                        RenderSettings &renderSettings,
                                                        GridPosition &grid_pos )
                  {
                    pos.m_position = grid.WorldSpace( { x,y,z } );
                    grid_pos.value = { x,y,z };
                    xform.scale = Vec3f { 50.f, 50.f, 50.f };
                    aabb.AABB_mMin = 0;
                    aabb.AABB_mMax = 0;
                    aabb.type = CollisionType::AABB;
                    material.shinness = 128.f;
                    renderSettings.onLight = true;
                    renderSettings.onRender = true;
                  } );

                  std::string diffuse_str {};
                  std::string specular_str {};
                  std::string model_str {};
                  GameType entity_type;
                  Vec3f color;
                  switch ( selection )
                  {
                    case 0: // Floor
                    {
                      diffuse_str = "Assets/Textures/Floor_1_Texture_Diffuse.dds";
                      specular_str = "Assets/Textures/Floor_1_Texture_Specular.dds";
                      model_str = "Assets/Models/binary/Static/Floor_1_Model.BinaryStatic";
                      entity_type = GameType::Floor;
                      color = Vec3f { 0.471f, .8f,  .886f };

                      break;
                    }
                    case 1: // Block
                    {
                      diffuse_str = "Assets/Textures/Floor_1_Texture_Diffuse.dds";
                      specular_str = "Assets/Textures/Floor_1_Texture_Specular.dds";
                      model_str = "Assets/Models/binary/Static/Floor_1_Model.BinaryStatic";
                      entity_type = GameType::Blocked;
                      color = Vec3f { 0.f, .141f,  .224f };
                      break;
                    }
                    case 2: // Hole
                    {
                      diffuse_str = "Assets/Textures/Floor_1_Texture_Diffuse.dds";
                      specular_str = "Assets/Textures/Floor_1_Texture_Specular.dds";
                      model_str = "Assets/Models/binary/Static/Hole_1_Model.BinaryStatic";
                      entity_type = GameType::Hole;
                      color = Vec3f { 0.f, .314f, .4f };
                      break;
                    }
                    case 3: // Falling
                    {
                      diffuse_str = "Assets/Textures/FallingFloor_1_Texture_Diffuse.dds";
                      specular_str = "Assets/Textures/FallingFloor_1_Texture_Specular.dds";
                      model_str = "Assets/Models/binary/Static/Floor_1_Model.BinaryStatic";
                      entity_type = GameType::FallingFloor;
                      color = Vec3f { .224f, 1.f, .078f };
                      break;
                    }
                  }

                  m_world->GetEntity( entity, [&]( Material &material, StaticMeshComponent &mesh, EntityType &type, Colour &colour )
                  {
                    material.diffuse = Service::Locator::Get<RM>().Get<Graphics::Texture>( diffuse_str );
                    material.specular = Service::Locator::Get<RM>().Get<Graphics::Texture>( specular_str );
                    material.normal = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/default.dds" );
                    material.bump = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/default.dds" );
                    material.shinness = 128.f;
                    mesh.model = Service::Locator::Get<RM>().Get<Graphics::StaticModel>( model_str );
                    mesh.unitScales = mesh.model->unitScales;
                    type.value = entity_type;
                    colour.colour = color;
                  } );

                  grid.UpdateCell( { x,y,z }, entity_type, entity );
                }
              }

              // Block tile - 0.0, 0.141, 0.224
              // Floor tile - 0.471, 0.8, 0.886

              // Print Cell
              std::string str { "" };
              switch ( grid.GetCellType( cell ) )
              {
                case GameType::Hole: str = "H"; break;
                case GameType::Floor: str = "F"; break;
                case GameType::FallingFloor: str = "FF"; break;
                case GameType::Blocked: str = "B"; break;
                case GameType::MovingObstacle: str = "MO"; break;
                case GameType::MovingPlatform: str = "MP"; break;
                case GameType::PressurePlate: str = "PP"; break;
                case GameType::Switch: str = "S"; break;
                case GameType::Door: str = "D"; break;
                case GameType::UnlockedDoor: str = "UD"; break;
                case GameType::Box: str = "B"; break;
                case GameType::Cheese: str = "C"; break;
                case GameType::Goal: str = "G"; break;
                case GameType::Player: str = "P"; break;
                default:
                break;
              }
              ImGui::SameLine();
              ImGui::Text( str.c_str() );
            }
          }
        } );
      }
      if ( tuplet::get<0>( inst_to_create ).Valid() )
        ModalPopupInstanceCopy( tuplet::get<0>( inst_to_create ), tuplet::get<1>( inst_to_create ), tuplet::get<2>( inst_to_create ), tuplet::get<3>( inst_to_create ) );
    } );
  } );
}

void GridPanel::ModalPopupInstanceCopy( Entity &prefab, const Vec3f &pos, const Vec3i &grid_pos, const GameType &type ) noexcept
{
  auto flag = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove;
  ImGuiHelper::PopupModal( "Create Prefab Instance/Copy Grid", nullptr, flag, [&]
  {
    static bool instance = true;
    static bool copy = false;
    std::string str( "Create Prefab Instance/Copy into grid?" );
    f32 size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
    ImGui::SetNextItemWidth( size );
    ImGui::Dummy( { 1.0f,12.0f } );
    ImGui::SetCursorPosX( 15.0f ); // Bound to break!
    ImGui::Text( str.c_str() );
    ImGui::Dummy( { 1.0f,12.0f } );
    if ( ImGui::RadioButton( "Instance", instance ) )
    {
      instance = true;
      copy = false;
    }
    if ( ImGui::RadioButton( "Copy", copy ) )
    {
      copy = true;
      instance = false;
    }
    if ( ImGui::Button( ICON_FA_CHECK, { size * 0.25f + 20.0f,40.0f } ) )
    {
      Entity new_ent;
      if ( instance )
        m_editor_sys->m_command_history.Commit<CreatePrefabInstanceCommand>( *m_world,
                                                                             prefab,
                                                                             Position { pos },
                                                                             new_ent );
      if ( copy )
        m_editor_sys->m_command_history.Commit<CreatePrefabCopyCommand>( *m_world,
                                                                         prefab,
                                                                         Position { pos },
                                                                         new_ent );

      m_world->SendGlobalEvent<RelinkComponentEvent>();
      m_entity_context->ClearSelections();
      Audio::AudioManager::Instance().StopAllAudio();
      m_entity_context->selections.push_back( new_ent );
      m_entity_context->selected_entity_name = m_world->m_entity_manager.Get( new_ent ).m_name;
      Draw::OnClick( new_ent, *m_entity_context );
      m_world->GetOrCreateSingletonComponent<GameGrid>().UpdateCell( grid_pos, type, new_ent );
      m_world->GetComponent<GridPosition>( new_ent ).value = grid_pos;
      if ( m_world->HaveComponent<GUI::CanvasTag>( new_ent ) )
      {
        auto &canvas_stack = Service::Locator::Get<GUI::Manager>();
        if ( !canvas_stack.Empty() )
          canvas_stack.PopAll();
        m_recur_push( new_ent );
      }
      m_recur_update_animator( new_ent );
      prefab = Entity {};
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
      ImGui::CloseCurrentPopup();
  } );
}
}
#endif