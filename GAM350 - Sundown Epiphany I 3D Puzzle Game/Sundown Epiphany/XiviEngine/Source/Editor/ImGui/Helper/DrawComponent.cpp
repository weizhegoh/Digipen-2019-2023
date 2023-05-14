/*****************************************************************
*\file         DrawComponent.cpp
*\author(s)    Chng Yong Quan, Goh Wei Zhe

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"

#ifdef XV_EDITOR
#include "DrawComponent.h"
#include "Events/GraphicsEvents.h"
#include "Events/EngineEvents.h"

#include "Editor/System.h"
#include "Editor/Command/Actions/RemoveComponentsCommand.h"
#include "Editor/Command/Actions/UpdateComponentCommand.h"
#include "Editor/Systems/Base/LocalCoordsUpdateSystem.h"

namespace Xivi::Editor::Draw
{
void OnClick( const Entity entity, EntityContext &entity_context ) noexcept
{
  entity_context.cached_map.clear();

  Ptr<ECS::World::Instance> world = &Service::Locator::Get<ECS::World::Instance>();
  entity_context.selected_signature = world->GetArchetype( entity ).GetSignature();

  auto descriptor_vec = ECS::Component::DescriptorManager::Instance().DescriptorVector( entity_context.selected_signature );
  auto data_vec = world->GetComponents( entity, descriptor_vec );

  for ( auto i = 0, j = 0; i < descriptor_vec.size(); ++i )
  {
    if ( descriptor_vec[i]->m_type != ECS::Component::Type::ID::Data )
      continue;

    entity_context.cached_map[descriptor_vec[i]->m_id] = data_vec[j++];
  }
}

void PopUpDeleteComponent( Ptr<ECS::World::Instance> world,
                           ECS::Component::DescriptorManager &desc_mgr,
                           ECS::Entity entity,
                           ECS::Component::ID id,
                           Ptr<EntityContext> entity_context,
                           ECS::Signature signature,
                           const bool can_edit ) noexcept
{
  // Block non-editable component
  if ( !can_edit )
    return;

  ImGuiHelper::PopupContextItem( "delete component", ImGuiPopupFlags_MouseButtonRight, [&]
  {
    if ( ImGui::Selectable( "Delete Component" ) )
    {
      auto descriptors = desc_mgr.DescriptorVector( signature );
      for ( const auto &desc : descriptors )
      {
        auto comp_name = std::string { desc->m_name };
        if ( id == desc->m_id )
        {
          // m_remove_component = true;
          System::Instance().m_command_history.Commit<RemoveComponentsCommand>( *world, entity, ECS::DescriptorSpan { &desc, 1 } );
          OnClick( entity, *entity_context );
          break;
        }
      }
    }
  } );
}

bool RenderEnumeration( const std::string &prop_name,
                        rttr::type &prop_type,
                        rttr::variant &value,
                        const rttr::instance &instance,
                        rttr::property &property,
                        const Ptr<const ECS::Component::Descriptor> desc,
                        const Entity entity,
                        Ptr<EntityContext> entity_context,
                        Ptr<ECS::World::Instance> world ) noexcept
{
  auto enumeration = prop_type.get_enumeration();
  auto enum_list = enumeration.get_names();
  std::vector<const char *> value_list;
  for ( auto &e_num : enum_list )
    value_list.push_back( e_num.data() );
  auto selection = value.get_value<int>();
  auto initial = selection;
  auto current_item = value_list[initial];

  bool changes = false;
  if ( ImGui::BeginCombo( ( "##" + prop_name ).c_str(), current_item ) )
  {
    for ( unsigned i = 0; i < value_list.size(); ++i )
    {
      bool is_selected = ( current_item == value_list[i] );
      if ( ImGui::Selectable( value_list[i], is_selected ) )
      {
        selection = i;
        auto result = enumeration.name_to_value( enumeration.value_to_name( selection ) );
        entity_context->SetComponentSnapshot( *world, desc, property, result );
        System::Instance().m_command_history.Commit<UpdateComponentCommand>( *world,
                                                                             entity_context->LastSelectedEntity(), *desc, entity_context->snapshots[0][0] );
        changes = true;
      }

      if ( is_selected )
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
  return changes;
}

void RenderBaseType( const std::string &prop_name,
                     rttr::property property,
                     rttr::type &prop_type,
                     rttr::variant &value,
                     const Ptr<const ECS::Component::Descriptor> desc,
                     const Entity entity,
                     Ptr<EntityContext> entity_context,
                     Ptr<ECS::World::Instance> world ) noexcept
{
  if ( prop_type == rttr::type::get<std::reference_wrapper<f32>>() )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      if ( ImGui::DragFloat( ( "##" + prop_name ).c_str(), ( &value.get_value<std::reference_wrapper<f32>>().get() ), 0.01f ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<f32>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<f32>>().get() = new_val;
          }
        }
        return true;
      }
      return false;
    } );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<i32>>() )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      if ( ImGui::InputInt( ( "##" + prop_name ).c_str(), ( &value.get_value<std::reference_wrapper<i32>>().get() ), 1 ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<i32>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<i32>>().get() = new_val;
          }
        }
        return true;
      }

      return false;
    } );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<u32>>() )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      if ( ImGui::InputInt( ( "##" + prop_name ).c_str(), std::bit_cast<Ptr<i32>>( &value.get_value<std::reference_wrapper<u32>>().get() ), 1, 0 ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<u32>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<u32>>().get() = new_val;
          }
        }
        return true;
      }

      return false;
    } );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<bool>>() )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      if ( ImGui::Checkbox( ( "##" + prop_name ).c_str(), &value.get_value<std::reference_wrapper<bool>>().get() ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<bool>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<bool>>().get() = new_val;
          }
        }
        return true;
      }

      return false;
    } );
  }
}

void RenderClassType( const std::string &prop_name,
                      rttr::property &property,
                      rttr::type &prop_type,
                      rttr::variant &value,
                      const Ptr<const ECS::Component::Descriptor> desc,
                      const Entity entity,
                      Ptr<RM> resource_manager,
                      Ptr<EntityContext> entity_context,
                      Ptr<ECS::World::Instance> world ) noexcept
{
  if ( prop_type == rttr::type::get<std::reference_wrapper<Vec3f>>() )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      if ( ImGui::DragFloat3( ( "##" + prop_name ).c_str(), std::bit_cast<Ptr<f32>>( &value.get_value<std::reference_wrapper<Vec3f>>().get() ), 0.01f ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<Vec3f>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<Vec3f>>().get() = new_val;
            if ( desc2->m_id == ECS::Component::info<Position>.m_id && world->HaveComponent<Parent>( ent ) )
              LocalCoordsUpdateSystem::OnCall( *world, ent );
          }
        }
        return true;
      }
      return false;
    } );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<glm::vec3>>() )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      if ( ImGui::DragFloat3( ( "##" + prop_name ).c_str(), std::bit_cast<Ptr<f32>>( &value.get_value<std::reference_wrapper<glm::vec3>>().get() ), 0.01f ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<glm::vec3>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<glm::vec3>>().get() = new_val;
          }
        }
        return true;
      }
      return false;
    } );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<Vec4f>>() )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      if ( ImGui::DragFloat4( ( "##" + prop_name ).c_str(), std::bit_cast<Ptr<f32>>( &value.get_value<std::reference_wrapper<Vec4f>>().get() ), 0.01f ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<Vec4f>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<Vec4f>>().get() = new_val;
          }
        }
        return true;
      }
      return false;
    } );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<glm::vec4>>() )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      if ( ImGui::DragFloat4( ( "##" + prop_name ).c_str(), std::bit_cast<Ptr<f32>>( &value.get_value<std::reference_wrapper<glm::vec4>>().get() ), 0.01f ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<glm::vec4>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<glm::vec4>>().get() = new_val;
          }
        }
        return true;
      }
      return false;
    } );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<Vec3i>>() )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      if ( ImGui::DragInt3( ( "##" + prop_name ).c_str(), std::bit_cast<Ptr<i32>>( &value.get_value<std::reference_wrapper<Vec3i>>().get() ), 0.01f ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<Vec3i>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<Vec3i>>().get() = new_val;
            if ( desc2->m_id == ECS::Component::info<Position>.m_id && world->HaveComponent<Parent>( ent ) )
              LocalCoordsUpdateSystem::OnCall( *world, ent );
          }
        }
        return true;
      }
      return false;
    } );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<Entity>>() )
  {
    //SnapshotInput(desc, entity, entity_context, world, [&]
    {
      /*bool changes = false;
      ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
      auto& ent = value.get_value<std::reference_wrapper<Entity>>().get();
      if ( ImGui::TreeNodeEx( ( void * )static_cast<intptr_t>( entity.m_global_index ), node_flags, "Entity" ) )
      {
        ImGuiHelper::DragDropTarget([&]
          {
            if (const ImGuiPayload* dragged_entity = ImGui::AcceptDragDropPayload("Attach To Property Inspector"); dragged_entity)
            {
              ent = *static_cast<Ptr<Entity>>(dragged_entity->Data);
              changes = true;
            }
          });
        if (ent.Valid())
        {
          auto& entry = world->m_entity_manager.Get(ent);
          ImGui::Text(entry.m_name.c_str());
        }
        ImGui::TreePop();
      }
      return changes;*/

      auto &ent = value.get_value<std::reference_wrapper<Entity>>().get();
      ImGuiHelper::Table( ( "##entity_picker" + prop_name + std::to_string( entity.m_global_index ) ).c_str(), 2, ImGuiTableFlags_None, [&]
      {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        if ( ent.Valid() )
        {
          auto &entry = world->m_entity_manager.Get( ent );
          ImGui::Text( entry.m_name.c_str() );
        }
        else
          ImGui::Text( "- None -" );

        /*ImGui::SameLine();
        ImGui::Text("  ");*/

        ImGui::TableNextColumn();

        bool prev_picked = ImGuiHelper::s_picker.first;
        if ( prev_picked )
          ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 1.00f, 0.55f, 0.33f, 0.90f ) );
        if ( ImGui::Button( ICON_FA_EYE_DROPPER, { 20.f, 20.f } ) )
        {
          if ( !ImGuiHelper::s_picker.first )
          {
            ImGuiHelper::s_picker.first = true;
            ImGuiHelper::s_picker.second = &ent;
          }
          else
            ImGuiHelper::s_picker.first = false;
        }
        if ( prev_picked )
          ImGui::PopStyleColor();
        if ( ImGuiHelper::IsItemHovered() )
        {
          ImVec2 m = ImGui::GetIO().MousePos;
          ImGui::SetNextWindowPos( ImVec2( m.x + 10, m.y + 10 ) );
          ImGuiHelper::Tooltip( [&]
          {
            ImGui::Text( "Pick Entity" );
          } );
        }
      } );
    }//);
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<Resource::Handler<Graphics::Texture>>>() )
  {
    auto &tex = value.get_value<std::reference_wrapper<Resource::Handler<Graphics::Texture>>>().get();
    RenderAssetsProperties( prop_name, property, desc, tex, entity, resource_manager, entity_context, world );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<Resource::Handler<Graphics::StaticModel>>>() )
  {
    auto &s_model = value.get_value<std::reference_wrapper<Resource::Handler<Graphics::StaticModel>>>().get();
    RenderAssetsProperties( prop_name, property, desc, s_model, entity, resource_manager, entity_context, world );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<Resource::Handler<Graphics::AnimationModel>>>() )
  {
    auto &a_model = value.get_value<std::reference_wrapper<Resource::Handler<Graphics::AnimationModel>>>().get();
    RenderAssetsProperties( prop_name, property, desc, a_model, entity, resource_manager, entity_context, world );
    world->SendGlobalEvent<RelinkComponentEvent>();
  }
  else if ( prop_type == rttr::type::get <std::vector<Entity>>() )
  {
    auto vec = value.get_value<std::vector<Entity>>();
    for ( auto e : vec )
      ImGui::Text( std::to_string( e.m_global_index ).c_str() );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<std::string>>() )
  {
    auto &str = value.get_value<std::reference_wrapper<std::string>>().get();
    char buffer[256] {};
    std::copy( str.begin(), str.end(), buffer );
    if ( ImGui::InputText( ( "##" + prop_name ).c_str(), buffer, sizeof( buffer ), ImGuiInputTextFlags_EnterReturnsTrue ) )
      str = std::string( buffer );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<InBETWN_Vec3>>() )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      auto &first = value.get_value<std::reference_wrapper<InBETWN_Vec3>>().get().first;
      auto &second = value.get_value<std::reference_wrapper<InBETWN_Vec3>>().get().second;
      bool change = false;
      if ( ImGui::DragFloat3( ( "##" + prop_name + "First" ).c_str(), std::bit_cast<Ptr<f32>>( &first ), 0.01f ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<Vec3f>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<Vec3f>>().get() = new_val;
          }
        }
        change = true;
      }
      if ( ImGui::DragFloat3( ( "##" + prop_name + "Second" ).c_str(), std::bit_cast<Ptr<f32>>( &second ), 0.01f ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<Vec3f>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<Vec3f>>().get() = new_val;
          }
        }
        change = true;
      }
      return change;
    } );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<InBETWN_Vec2>>() )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      auto &first = value.get_value<std::reference_wrapper<InBETWN_Vec2>>().get().first;
      auto &second = value.get_value<std::reference_wrapper<InBETWN_Vec2>>().get().second;
      bool change = false;
      if ( ImGui::DragFloat2( ( "##" + prop_name + "First" ).c_str(), std::bit_cast<Ptr<f32>>( &first ), 0.01f ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<Vec2f>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<Vec2f>>().get() = new_val;
          }
        }
        change = true;
      }
      if ( ImGui::DragFloat2( ( "##" + prop_name + "Second" ).c_str(), std::bit_cast<Ptr<f32>>( &second ), 0.01f ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<Vec2f>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<Vec2f>>().get() = new_val;
          }
        }
        change = true;
      }
      return change;
    } );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<InBETWN_Float>>() )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      auto &first = value.get_value<std::reference_wrapper<InBETWN_Float>>().get().first;
      auto &second = value.get_value<std::reference_wrapper<InBETWN_Float>>().get().second;
      bool change = false;
      if ( ImGui::DragFloat( ( "##" + prop_name + "First" ).c_str(), std::bit_cast<Ptr<f32>>( &first ), 0.01f ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<f32>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<f32>>().get() = new_val;
          }
        }
        change = true;
      }
      if ( ImGui::DragFloat( ( "##" + prop_name + "Second" ).c_str(), std::bit_cast<Ptr<f32>>( &second ), 0.01f ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<f32>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<f32>>().get() = new_val;
          }
        }
        change = true;
      }
      return change;
    } );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<InBETWN_Int>>() )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      auto &first = value.get_value<std::reference_wrapper<InBETWN_Int>>().get().first;
      auto &second = value.get_value<std::reference_wrapper<InBETWN_Int>>().get().second;
      if ( ImGui::DragInt( ( "##" + prop_name ).c_str(), std::bit_cast<Ptr<int>>( &first ), 0.01f ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<int>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<int>>().get() = new_val;
          }
        }
        return true;
      }
      if ( ImGui::DragInt( ( "##" + prop_name ).c_str(), std::bit_cast<Ptr<int>>( &second ), 0.01f ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<int>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<int>>().get() = new_val;
          }
        }
        return true;
      }
      return false;
    } );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<FollowEntity>>() )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      auto &first = value.get_value<std::reference_wrapper<FollowEntity>>().get().first;
      auto &second = value.get_value<std::reference_wrapper<FollowEntity>>().get().second;
      bool change = false;
      if ( ImGui::Checkbox( ( "##" + prop_name + "First" ).c_str(), std::bit_cast<Ptr<bool>>( &first ) ) )
      {
        auto &new_val = value.get_value<std::reference_wrapper<bool>>().get();
        for ( auto &selected_entity : entity_context->selections )
        {
          Entity ent = selected_entity;
          auto &entry = world->m_entity_manager.Get( ent );
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for ( const auto &[j, desc2] : Enumerate( pool->m_descriptors ) )
          {
            if ( desc->m_bit_index != desc2->m_bit_index )
              continue;
            auto selected_instance = desc2->m_reflect( &pool->m_components[j][pool_index.m_value * desc2->m_size] );
            rttr::variant setter = property.get_value( selected_instance );

            setter.get_value<std::reference_wrapper<bool>>().get() = new_val;
          }
        }
      }
      ImGui::Text( std::to_string( second.m_global_index ).c_str() );
      return change;
    } );
  }
  else if ( prop_type == rttr::type::get<std::reference_wrapper<vector<Vec3i>>>() )
  {
    SnapshotInput( desc, entity, entity_context, world, [&]
    {
      /*auto& first = value.get_value<std::reference_wrapper<FollowEntity>>().get().first;
      auto& second = value.get_value<std::reference_wrapper<FollowEntity>>().get().second;
      bool change = false;
      if (ImGui::Checkbox(("##" + prop_name + "First").c_str(), std::bit_cast<Ptr<bool>>(&first)))
      {
        auto& new_val = value.get_value<std::reference_wrapper<bool>>().get();
        for (auto& selected_entity : entity_context->selections)
        {
          Entity ent = selected_entity;
          auto& entry = world->m_entity_manager.Get(ent);
          auto pool = entry.m_pool;
          auto pool_index = entry.m_pool_index;
          for (const auto& [j, desc2] : Enumerate(pool->m_descriptors))
          {
            if (desc->m_bit_index != desc2->m_bit_index)
              continue;
            auto selected_instance = desc2->m_reflect(&pool->m_components[j][pool_index.m_value * desc2->m_size]);
            rttr::variant setter = property.get_value(selected_instance);

            setter.get_value<std::reference_wrapper<bool>>().get() = new_val;
          }
        }
      }
      ImGui::Text(std::to_string(second.m_global_index).c_str());*/
      bool change = false;
      auto &vecs = value.get_value<std::reference_wrapper<vector<Vec3i>>>().get();

      ImGuiHelper::Table( ( "##vectors_of_vec3i" + std::to_string( entity.m_global_index ) ).c_str(), 2, ImGuiTableFlags_SizingStretchProp, [&]
      {
        int i = 0;
        for ( auto it = vecs.begin(); it != vecs.end(); ++it, ++i )
        {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          if ( ImGui::DragInt3( ( "##" + prop_name + std::to_string( i ) ).c_str(), std::bit_cast<Ptr<i32>>( &( *it ) ), 0.01f ) )
          {
            it->x = it->x < 0 ? 0 : it->x;
            it->y = it->y < 0 ? 0 : it->y;
            it->z = it->z < 0 ? 0 : it->z;
            change = true;
          }

          ImGui::TableNextColumn();
          if ( ImGui::Button( ICON_FA_MINUS, { 20.f, 20.f } ) )
          {
            vecs.erase( it );
            change = true;
            break;
          }
        }
      } );

      if ( ImGui::Button( ICON_FA_PLUS, { 20.f, 20.f } ) )
        vecs.push_back( { 0, 0, 0 } );

      return change;
    } );
  }
}

void RenderProperties( Entity entity,
                       const rttr::instance &instance,
                       const Ptr<const ECS::Component::Descriptor> desc,
                       const ECS::Signature common_sig,
                       Ptr<RM> resource_manager,
                       Ptr<EntityContext> entity_context,
                       Ptr<ECS::World::Instance> world ) noexcept
{
  auto instance_name = instance.get_type().get_name().to_string();
  ImGui::PushItemWidth( ImGui::GetWindowWidth() - 30.0f );
  ImGui::BeginGroup();
  if ( ImGui::CollapsingHeader( instance_name.c_str(), ImGuiTreeNodeFlags_FramePadding & ImGuiTreeNodeFlags_AllowItemOverlap ) )
  {
    ImGui::Dummy( { 0.0f, 8.0f } );
    for ( auto property : instance.get_type().get_properties() )
    {
      if ( property.get_metadata( "NoEditor" ) )
        continue;

      auto prop_type = property.get_type();
      auto prop_name = property.get_name().to_string();

      ImGui::Text( prop_name.c_str() );
      prop_name.assign( prop_name + instance_name );

      rttr::variant value = property.get_value( instance );

      // for(auto& entity : editor_sys.m_entity_context.m_selections)
      if ( prop_type.is_enumeration() )
        RenderEnumeration( prop_name, prop_type, value, instance, property, desc, entity, entity_context, world );
      else if ( prop_type.get_wrapped_type().is_arithmetic() )
        RenderBaseType( prop_name, property, prop_type, value, desc, entity, entity_context, world );
      else if ( prop_type.is_class() )
        RenderClassType( prop_name, property, prop_type, value, desc, entity, resource_manager, entity_context, world );
    }
    ImGui::Dummy( { 0.0f, 8.0f } );
  }
  ImGui::EndGroup();
  ImGui::PopItemWidth();
}

void DrawDefault( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                  const Entity entity,
                  ECS::Signature signature,
                  Ptr<RM> resource_manager,
                  Ptr<EntityContext> entity_context,
                  Ptr<ECS::World::Instance> world,
                  ImGuiTextFilter &filter,
                  const ECS::Signature can_edit,
                  const ECS::Signature can_draw ) noexcept
{
  ECS::Signature default_sig = SetSignature<Default>();

  if ( ( default_sig & signature & can_draw ).any() )
    AssignHeader( "Default", [&]
  {
    ComponentSection<Default>::Draw( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit );
  } );
}

void DrawGraphics( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                   const Entity &entity,
                   ECS::Signature signature,
                   Ptr<RM> resource_manager,
                   Ptr<EntityContext> entity_context,
                   Ptr<ECS::World::Instance> world,
                   ImGuiTextFilter &filter,
                   const ECS::Signature &can_edit,
                   const ECS::Signature &can_draw ) noexcept
{
  ECS::Signature default_sig = SetSignature<Graphics_Default>();
  ECS::Signature animation_sig = SetSignature<Graphics_AnimationRendering>();
  ECS::Signature static_sig = SetSignature<Graphics_StaticRendering>();
  ECS::Signature material_sig = SetSignature<Graphics_MaterialRendering>();
  ECS::Signature skybox_sig = SetSignature<Graphics_SkyboxRendering>();
  ECS::Signature particle_sig = SetSignature<Particles_Particle>();

  if ( ( ( default_sig | animation_sig | static_sig | material_sig | skybox_sig ) & signature & can_draw ).any() )
    AssignHeader( "Graphics", [&]
  {
    if ( ( default_sig & signature & can_draw ).any() )
      ComponentSection<Graphics_Default>::Draw( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit );

    if ( ( animation_sig & signature & can_draw ).any() )
      ComponentSection<Graphics_AnimationRendering>::Draw( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit );

    if ( ( static_sig & signature & can_draw ).any() )
      ComponentSection<Graphics_StaticRendering>::Draw( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit );

    if ( ( material_sig & signature & can_draw ).any() )
      ComponentSection<Graphics_MaterialRendering>::Draw( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit );

    if ( ( particle_sig & signature & can_draw ).any() )
      ComponentSection<Particles_Particle>::Draw( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit );

    if ( ( skybox_sig & signature & can_draw ).any() )
      ComponentSection<Graphics_SkyboxRendering>::Draw( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit );
  } );
}

void DrawLightings( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                    const Entity entity,
                    ECS::Signature signature,
                    Ptr<RM> resource_manager,
                    Ptr<EntityContext> entity_context,
                    Ptr<ECS::World::Instance> world,
                    ImGuiTextFilter &filter,
                    const ECS::Signature can_edit,
                    const ECS::Signature can_draw ) noexcept
{
  ECS::Signature light_sig = SetSignature<Graphics_LightRendering>();

  if ( ( light_sig & signature & can_draw ).any() )
    AssignHeader( "Lighting", [&]
  {
    if ( ( light_sig & signature & can_draw ).any() )
      ComponentSection<Graphics_LightRendering>::Draw( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit );
  } );
}

//void DrawPhysics( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
//									const Entity entity,
//									ECS::Signature signature,
//									Ptr<RM> resource_manager,
//									Ptr<EntityContext> entity_context,
//									Ptr<ECS::World::Instance> world,
//									ImGuiTextFilter &filter,
//									const ECS::Signature can_edit,
//									const ECS::Signature can_draw ) noexcept
//{
//	ECS::Signature physics_sig = SetSignature<Physics_Components>();
//
//	if ( ( physics_sig & signature & can_draw ).any() )
//		AssignHeader( "Physics", [&]
//	{
//		ComponentSection<Physics_Components>::Draw( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit );
//	} );
//}

void DrawLogic( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                const Entity entity,
                ECS::Signature signature,
                Ptr<RM> resource_manager,
                Ptr<EntityContext> entity_context,
                Ptr<ECS::World::Instance> world,
                ImGuiTextFilter &filter,
                const ECS::Signature can_edit,
                const ECS::Signature can_draw ) noexcept
{
  ECS::Signature logic_sig = SetSignature<Logic_Components>();

  if ( ( logic_sig & signature & can_draw ).any() )
    AssignHeader( "Logic", [&]
  {
    ComponentSection<Logic_Components>::Draw( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit );
  } );
}

void DrawGUI( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
              const Entity entity,
              ECS::Signature signature,
              Ptr<RM> resource_manager,
              Ptr<EntityContext> entity_context,
              Ptr<ECS::World::Instance> world,
              ImGuiTextFilter &filter,
              const ECS::Signature can_edit,
              const ECS::Signature can_draw ) noexcept
{
  ECS::Signature gui_sig = SetSignature<GUI_Components>();

  if ( ( gui_sig & signature & can_draw ).any() )
    AssignHeader( "GUI", [&]
  {
    ComponentSection<GUI_Components>::Draw( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit );
  } );
}

void DrawAudio( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                const Entity entity,
                ECS::Signature signature,
                Ptr<RM> resource_manager,
                Ptr<EntityContext> entity_context,
                Ptr<ECS::World::Instance> world,
                ImGuiTextFilter &filter,
                const ECS::Signature can_edit,
                const ECS::Signature can_draw ) noexcept
{
  ECS::Signature audio_sig = SetSignature<Audio_Components>();

  if ( ( audio_sig & signature & can_draw ).any() )
    AssignHeader( "Audio", [&]
  {
    ComponentSection<Audio_Components>::Draw( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit );
  } );
}

void DrawPrefab( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                 const Entity &entity,
                 ECS::Signature signature,
                 Ptr<RM> resource_manager,
                 Ptr<EntityContext> entity_context,
                 Ptr<ECS::World::Instance> world,
                 ImGuiTextFilter &filter,
                 const ECS::Signature &can_edit,
                 const ECS::Signature &can_draw ) noexcept
{
  ECS::Signature prefab_sig = SetSignature<Prefab_Components>();

  if ( ( prefab_sig & signature & can_draw ).any() )
    AssignHeader( "Prefab", [&]
  {
    ComponentSection<Prefab_Components>::Draw( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit );
  } );
}

void DrawGridInfo( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                   const Entity &entity,
                   ECS::Signature signature,
                   Ptr<RM> resource_manager,
                   Ptr<EntityContext> entity_context,
                   Ptr<ECS::World::Instance> world,
                   ImGuiTextFilter &filter,
                   const ECS::Signature &can_edit,
                   const ECS::Signature &can_draw ) noexcept
{
  ECS::Signature grid_sig = SetSignature<Grid_Components>();

  if ( ( grid_sig & signature & can_draw ).any() )
    AssignHeader( "Grid Info", [&]
  {
    ComponentSection<Grid_Components>::Draw( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit );
  } );
}

void DrawAllComponents( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                        const Entity &entity,
                        ECS::Signature signature,
                        Ptr<RM> resource_manager,
                        Ptr<EntityContext> entity_context,
                        Ptr<ECS::World::Instance> world,
                        ImGuiTextFilter &filter,
                        const ECS::Signature &can_edit,
                        const ECS::Signature &can_draw ) noexcept
{
  DrawDefault( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit, can_draw );
  DrawGraphics( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit, can_draw );
  DrawLightings( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit, can_draw );
  //DrawPhysics( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit, can_draw );
  DrawLogic( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit, can_draw );
  DrawGUI( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit, can_draw );
  DrawAudio( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit, can_draw );
  DrawPrefab( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit, can_draw );
  DrawGridInfo( cached_components, entity, signature, resource_manager, entity_context, world, filter, can_edit, can_draw );
}
}
#endif