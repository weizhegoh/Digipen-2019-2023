/*****************************************************************
*\file         PropertyInspectorPanel.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "PropertyInspectorPanel.h"
#include <imgui_internal.h>
#include "Editor/ImGui/Helper/AddComponent.h"

#include "Events/EngineEvents.h"

#include "Systems/Audio/Audio.h"

#include "Editor/Command/Actions/RemoveComponentsCommand.h"
#include "Editor/Command/Actions/ResetPrefabInstanceCommand.h"
#include "Editor/Panels/WorldPanel.h"

namespace Xivi::Editor
{
PropertyInspectorPanel::PropertyInspectorPanel( std::string name,
                                                Ptr<ECS::World::Instance> world,
                                                Ptr<Config> configs,
                                                Ptr<EntityContext> entity_context,
                                                Ptr<FileContext> file_context,
                                                Ptr<ToolContext> tool_context ) :
  IPanel( name, world, configs, entity_context, file_context, tool_context )
{
  m_enabled = true;
  m_audio_manager = &Audio::AudioManager::Instance();
  m_editor_sys = &System::Instance();

  ECS::Signature cannot_add_remove;

  using NoAddRemoveList = CList<ShareAsData, SharedFilter, ECS::ReferenceCount, Parent, Child, Root, Prefab, PrefabTag, ReferencedPrefab, Position, Transform, SkyBoxMaterial, UI, GUI::CanvasTag>;

  [&] <ECS::Component::Concepts::IsComponent... Cs>( CList<Cs...> ) noexcept
  {
    ( cannot_add_remove.set( ECS::Component::info<Cs>.m_bit_index ), ... );
  }( NoAddRemoveList() );

  m_component_can_add_remove = ~cannot_add_remove;

  #ifdef XV_DEBUG
  m_component_can_show = ~m_component_can_show;
  #else
  ECS::Signature dont_show;
  using DontShowList = CList<ShareAsData, SharedFilter, ECS::ReferenceCount, Parent, Child, Root, Prefab, PrefabTag, ReferencedPrefab>;
  [&] <ECS::Component::Concepts::IsComponent... Cs>( CList<Cs...> ) noexcept
  {
    ( dont_show.set( ECS::Component::info<Cs>.m_bit_index ), ... );
  }( DontShowList() );
  m_component_can_show = ~dont_show;
  #endif

  m_prefab_instance_sig.set( ECS::Component::info<ReferencedPrefab>.m_bit_index );
  m_resource_manager = &Service::Locator::Get<RM>();

  m_get_root = [&]( Entity entity ) noexcept -> Entity
  {
    if ( m_world->HaveComponent<Root>( entity ) )
      return entity;
    if ( !m_world->HaveComponent<Parent>( entity ) )
      return Entity {};
    return m_get_root( m_world->GetComponent<Parent>( entity ).m_entity );
  };
}

PropertyInspectorPanel::~PropertyInspectorPanel()
{
  m_enabled = false;
}

void PropertyInspectorPanel::Render() noexcept
{
  if ( !m_enabled )
    return;

  m_resource_manager = &Service::Locator::Get<RM>();

  ImGuiHelper::CreateImGuiWindow( m_name.c_str(), &m_enabled, [&]
  {
    if ( ImGui::IsWindowHovered( ImGuiHoveredFlags_ChildWindows ) && ImGui::IsMouseClicked( 1 ) )
      ImGui::SetWindowFocus();

    if ( ImGui::IsWindowFocused( ImGuiFocusedFlags_RootAndChildWindows ) )
      ImGui::SetFocusID( ImHashStr( m_name.c_str() ), ImGui::GetCurrentWindow() );

  //auto& editor_sys = GetSystem<Editor::System>();
  //auto& entity_context = editor_sys.m_entity_context;
    if ( m_entity_context->selections.empty() )
    {
      ImGui::Text( "Select an entity/prefab to view its properties" );
      return;
    }

    ECS::Signature common_sig = m_world->GetArchetype( m_entity_context->selections[0] ).GetSignature();
    for ( auto i = 1; i < m_entity_context->selections.size(); ++i )
    {
      auto &entry = m_world->m_entity_manager.Get( m_entity_context->selections[i] );
      auto pool = entry.m_pool;
      auto pool_index = entry.m_pool_index;
      if ( pool->GetComponent<ECS::Entity>( pool_index ) != m_entity_context->selections[i] )
        return;
      common_sig &= pool->m_family->m_archetype->GetSignature();
    }

    if ( common_sig.none() )
    {
      ImGui::Text( "Multiple entities selected with no common components." );
      return;
    }

    //for (auto& selected_entity : entity_context.m_selections)
    //{
        // Get entry
    auto &entry = m_world->m_entity_manager.Get( m_entity_context->LastSelectedEntity() );
    auto pool = entry.m_pool;
    auto pool_index = entry.m_pool_index;

    // Check pool & global referenced is the same
    if ( pool->GetComponent<ECS::Entity>( pool_index ) != m_entity_context->LastSelectedEntity() )
      return;

    auto &arch = *pool->m_family->m_archetype;

    if ( m_entity_context->selections.size() == 1 )
    {
      if ( arch.GetSignature()[ECS::Component::info<ReferencedPrefab>.m_bit_index] )
      {
        if ( ImGui::Button( ICON_FA_RECYCLE, { 30.0f, 30.0f } ) )
        {
          Entity new_ent;
          m_editor_sys->m_command_history.Commit<ResetPrefabInstanceCommand>( *m_world,
                                                                              m_entity_context->selections[0],
                                                                              new_ent );
          m_entity_context->ClearSelections();
          m_entity_context->selections.push_back( new_ent );
          Draw::OnClick( new_ent, *m_entity_context );
        }
        if ( ImGuiHelper::IsItemHovered() )
        {
          ImVec2 m = ImGui::GetIO().MousePos;
          ImGui::SetNextWindowPos( ImVec2( m.x - 10, m.y ) );
          ImGuiHelper::Tooltip( [&]
          {
            ImGui::Text( "Revert value to prefab" );
          } );
        }
      }

      SaveButton( m_entity_context->LastSelectedEntity(), arch );

      EntityName( m_world, entry );
    }
    else
      ImGui::Text( "Multiple entities selected\n" );

    auto &desc_mgr = ECS::Component::DescriptorManager::Instance();

    if ( !arch.GetSignature()[ECS::Component::info<Prefab>.m_bit_index] /*&& !arch.GetSignature()[ECS::Component::info<ReferencedPrefab>.m_bit_index]*/ )
    {
      Add::AddComponent( m_world, desc_mgr, m_entity_context->LastSelectedEntity(), arch, m_entity_context, m_component_can_add_remove );

      Add::AddTag( m_world, desc_mgr, m_entity_context->LastSelectedEntity(), arch, m_entity_context, m_component_can_add_remove );

      Add::AddGroupComponent( m_world, desc_mgr, m_entity_context->LastSelectedEntity(), arch, m_entity_context, common_sig );

      // EntityTag( m_world, desc_mgr, m_entity_context->LastSelectedEntity(), arch );

      if ( m_remove_component )
      {
        m_remove_component = false;
        return;
      }
      SearchBarEntity( m_world, desc_mgr, pool, pool_index, common_sig, m_entity_context->LastSelectedEntity(), m_resource_manager, m_entity_context, arch );
    }
    else
    {
      Entity ent = m_entity_context->LastSelectedEntity();
      while ( m_world->HaveComponent<Parent>( ent ) )
      {
        ent = m_world->GetComponent<Parent>( ent ).m_entity;
      }
      auto &root_entry = m_world->m_entity_manager.Get( ent );
      SearchBarPrefab( m_world, desc_mgr, root_entry.m_name, ent, common_sig, m_resource_manager, m_entity_context );
    }
    //}
  } );
}

void PropertyInspectorPanel::SaveButton( const Entity selected_entity, const ECS::Archetype::Instance &arch ) noexcept
{
  if ( arch.GetSignature()[ECS::Component::info<Prefab>.m_bit_index] &&
       ImGui::Button( ICON_FA_SAVE, { 30.0f, 30.0f } ) )
  {
    bool found = false;
    for ( auto &[entity, path_str] : m_file_context->prefab_path )
    {
      auto root = m_get_root( selected_entity );
      if ( root.Valid() && entity == root )
      {
        m_world->SendGlobalEvent<SerialisePrefab>( root, path_str );
        found = true;
        break;
      }
    }
    if ( !found )
    {
      auto root = m_get_root( selected_entity );
      if ( root.Valid() )
      {
        auto pathname = m_editor_sys->GetPanel<WorldPanel>( "World" )->SaveEntityAsPrefab( root, false, true );
        if ( !pathname.empty() )
          m_file_context->prefab_path.push_back( { root, pathname } );
      }
    }
  }

  if ( ImGuiHelper::IsItemHovered() )
  {
    ImVec2 m = ImGui::GetIO().MousePos;
    ImGui::SetNextWindowPos( ImVec2( m.x - 10, m.y ) );
    ImGuiHelper::Tooltip( [&]
    {
      ImGui::Text( "Save prefab into file" );
    } );
  }
}

void PropertyInspectorPanel::SearchBarPrefab( Ptr<ECS::World::Instance> world,
                                              ECS::Component::DescriptorManager &desc_mgr,
                                              const std::string ent_name,
                                              Entity entity,
                                              const ECS::Signature common_sig,
                                              Ptr<RM> resource_manager,
                                              Ptr<EntityContext> entity_context ) noexcept
{
  ImGuiHelper::ChildWindow( "Prefab Property Details", { 0, 0 }, true, ImGuiWindowFlags_AlwaysAutoResize, [&]
  {
    static ImGuiTextFilter prop_filter;
    ImGui::Text( "Search: " );
    ImGui::SameLine();
    prop_filter.Draw( "##Property Filter" );
    ImGui::Text( "" );

    auto &entry = m_world->m_entity_manager.Get( entity );

    RecurRenderPrefabComponents( world, desc_mgr, entity, entry.m_name, prop_filter );
    RecurCheckForSelection( world, desc_mgr, entity, common_sig, resource_manager, entity_context );
  } );
}

void PropertyInspectorPanel::SearchBarEntity( Ptr<ECS::World::Instance> world,
                                              ECS::Component::DescriptorManager &desc_mgr,
                                              Ptr<ECS::Pool::Instance> pool,
                                              ECS::Pool::Index pool_index,
                                              const ECS::Signature common_sig,
                                              const Entity entity,
                                              Ptr<RM> resource_manager,
                                              Ptr<EntityContext> entity_context,
                                              ECS::Archetype::Instance &arch ) noexcept
{
  ImGuiHelper::ChildWindow( "Components Details", { 0, 0 }, true, ImGuiWindowFlags_AlwaysAutoResize, [&]
  {
    static ImGuiTextFilter prop_filter;
    ImGui::Text( "Search: " );
    ImGui::SameLine();
    prop_filter.Draw( "##PropertyFilter" );
    ImGui::Text( "" );

    auto can_edit = pool->m_family->m_archetype->GetSignature() & m_component_can_add_remove;
    auto can_show = pool->m_family->m_archetype->GetSignature() & m_component_can_show;

    EntityTag( world, desc_mgr, entity, arch, prop_filter );
    Draw::DrawAllComponents( m_entity_context->cached_map, entity, m_entity_context->selected_signature, resource_manager, entity_context, world, prop_filter, can_edit, can_show );

    /*for ( const auto& [i, desc] : Enumerate(pool->m_descriptors ) )
    {
      std::string name = std::string{ desc->m_name };
      ImGui::PushID( name.c_str() );
      for ( auto& selected_entitiy : m_entity_context->m_selections )
      {
        if (can_edit[desc->m_bit_index])
          Draw::PopUpDeleteComponent( world, desc_mgr, selected_entitiy, desc->m_id, m_entity_context );
      }
      ImGui::PopID();
    }*/

    // auto can_edit = pool->m_family->m_archetype->GetSignature() & m_component_can_add_remove;

    // Reflect properties
    //for ( const auto &[i, desc] : Enumerate( pool->m_descriptors ) )
    //{
        // Skip entity
      //if ( i == 0 )
      //  continue;

      // Skip non-common component among selected entities
      //if ( !common_sig[desc->m_bit_index] )
      //  continue;

      //rttr::instance instance = desc->m_reflect( &pool->m_components[i][pool_index.m_value * desc->m_size] );
      //auto instance_name = instance.get_type().get_name().to_string();

      //if ( prop_filter.PassFilter( instance_name.c_str() ) )
      //{
        // ImGui::PushID( instance_name.c_str() );

        //DrawAllComponents(m_entity_context->m_cached_map, entity, m_entity_context->m_selected_signature, resource_manager, entity_context, world, prop_filter);
        //RenderProperties( entity, instance, desc, common_sig, resource_manager, entity_context, world);

        /*for ( auto &selected_entitiy : m_entity_context->m_selections )
        {
          if ( can_edit[desc->m_bit_index] )
            PopUpDeleteComponent( world, desc_mgr, selected_entitiy, instance_name );
        }*/

        /*if ( m_remove_component )
        {
          m_remove_component = false;
          ImGui::PopID();
          return;
        }

        ImGui::PopID();*/
      //}
      /*else
        ImGui::Text( "No matching search found" );*/
    //}
  } );
}

void PropertyInspectorPanel::PrefabName( Ptr<ECS::World::Instance> world, ECS::Entity::Descriptor &entry ) noexcept
{
  char buffer_text[64] {};

  for ( auto i = 0; i < entry.m_name.size(); ++i )
  {
    buffer_text[i] = entry.m_name[i];
  }
  buffer_text[entry.m_name.size()] = NULL;

  static std::string dup_name;

  ImGui::Text( "Name:" );
  ImGui::SameLine();
  if ( ImGui::InputText( "##buffer_text", buffer_text, 64, ImGuiInputTextFlags_EnterReturnsTrue ) )
  {
    std::string str = RemoveExtraSpace( buffer_text );

    for ( auto &prefab : m_entity_context->prefab_entity )
    {
      auto entry2 = world->m_entity_manager.Get( prefab );
      if ( entry2.m_name == std::string_view { str } )
      {
        dup_name = str;
        break;
      }
    }
    if ( str.empty() )
    {
      ImGui::OpenPopup( "Prefab Name Empty" );
    }
    if ( !dup_name.empty() )
    {
      ImGui::OpenPopup( "Prefab Name Existed" );
    }
    else if ( !str.empty() && entry.m_name != str )
    {
      entry.m_name = std::string { str };
    }
  }

  auto flag = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove;

  ImGuiHelper::PopupModal( "Prefab Name Existed", nullptr, flag, [&]
  {
    std::string str( "ERROR: An prefab with the name \"" + dup_name + "\" existed" );
    f32 size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
    ImGui::SetNextItemWidth( size );
    ImGui::Dummy( { 1.0f,12.0f } );
    ImGui::SetCursorPosX( 10.0f ); // Bound to break!
    ImGui::Text( str.c_str() );
    ImGui::Dummy( { 1.0f,12.0f } );
    ImGui::SameLine();
    if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
    {
      ImGui::CloseCurrentPopup();
      dup_name.clear();
    }
  } );

  ImGuiHelper::PopupModal( "Prefab Name Empty", nullptr, flag, [&]
  {
    std::string str( "ERROR: Prefab name cannot be empty or space" );
    f32 size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
    ImGui::SetNextItemWidth( size );
    ImGui::Dummy( { 1.0f,12.0f } );
    ImGui::SetCursorPosX( 10.0f ); // Bound to break!
    ImGui::Text( str.c_str() );
    ImGui::Dummy( { 1.0f,12.0f } );
    ImGui::SameLine();
    if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
      ImGui::CloseCurrentPopup();
  } );
}

void PropertyInspectorPanel::EntityName( Ptr<ECS::World::Instance> world, ECS::Entity::Descriptor &entry ) noexcept
{
  char buffer_text[64] {};

  for ( auto i = 0; i < entry.m_name.size(); ++i )
  {
    buffer_text[i] = entry.m_name[i];
  }
  buffer_text[entry.m_name.size()] = NULL;

  static std::string dup_name;

  ImGui::Text( "Name:" );
  ImGui::SameLine();
  if ( ImGui::InputText( "##buffer_text", buffer_text, 64, ImGuiInputTextFlags_EnterReturnsTrue ) )
  {
    std::string str = RemoveExtraSpace( buffer_text );
    for ( const auto &[i, entry2] : Enumerate( world->m_entity_manager.m_entities ) )
    {
      if ( entry2.m_name == std::string_view { str } )
      {
        dup_name = str;
        break;
      }
    }
    if ( str.empty() )
    {
      ImGui::OpenPopup( "Entity Name Empty" );
    }
    if ( !dup_name.empty() )
    {
      ImGui::OpenPopup( "Entity Name Existed" );
    }
    else if ( !str.empty() && entry.m_name != str )
    {
      entry.m_name = std::string { str };
    }
  }

  auto flag = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove;
  ImGuiHelper::PopupModal( "Entity Name Existed", nullptr, flag, [&]
  {
    std::string str( "ERROR: An entity with the name \"" + dup_name + "\" existed" );
    f32 size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
    ImGui::SetNextItemWidth( size );
    ImGui::Dummy( { 1.0f,12.0f } );
    ImGui::SetCursorPosX( 10.0f ); // Bound to break!
    ImGui::Text( str.c_str() );
    ImGui::Dummy( { 1.0f,12.0f } );
    ImGui::SameLine();
    if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
    {
      ImGui::CloseCurrentPopup();
      dup_name.clear();
    }
  } );
  ImGuiHelper::PopupModal( "Entity Name Empty", nullptr, flag, [&]
  {
    std::string str( "ERROR: Entity name cannot be empty or space" );
    f32 size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
    ImGui::SetNextItemWidth( size );
    ImGui::Dummy( { 1.0f,12.0f } );
    ImGui::SetCursorPosX( 10.0f ); // Bound to break!
    ImGui::Text( str.c_str() );
    ImGui::Dummy( { 1.0f,12.0f } );
    ImGui::SameLine();
    if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
      ImGui::CloseCurrentPopup();
  } );
}

void PropertyInspectorPanel::EntityTag( Ptr<ECS::World::Instance> world,
                                        ECS::Component::DescriptorManager &desc_mgr,
                                        ECS::Entity entity,
                                        ECS::Archetype::Instance &arch,
                                        ImGuiTextFilter &filter ) noexcept
{
  auto tag_signature = arch.GetSignature() & ( desc_mgr.m_tag_bits | desc_mgr.m_exclusive_tag_bits );

  auto descriptors = desc_mgr.DescriptorVector( tag_signature );
  if ( ImGui::CollapsingHeader( "Tags" ) )
  {
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf;
    auto can_remove = tag_signature & m_component_can_add_remove;
    auto can_show = tag_signature & m_component_can_show;
    for ( const auto &desc : descriptors )
    {
      if ( !can_show[desc->m_bit_index] || !filter.PassFilter( std::string( desc->m_name ).c_str() ) )
        continue;

      bool node_open = false;
      if ( ImGui::TreeNodeEx( ( void * )static_cast<intptr_t>( desc->m_bit_index ), node_flags, std::string { desc->m_name }.c_str() ) )
      {
        node_open = true;
        if ( can_remove[desc->m_bit_index] )
        {
          ImGuiHelper::PopupContextItem( "remove tag", ImGuiPopupFlags_MouseButtonRight, [&]
          {
            if ( ImGui::Selectable( "Remove Tag" ) )
            {
              m_remove_component = true;
              m_editor_sys->m_command_history.Commit<RemoveComponentsCommand>( *world, entity, ECS::DescriptorSpan { &desc, 1 } );
            }
          } );
        }
      }
      if ( node_open )
        ImGui::TreePop();
    }
  }
}

void PropertyInspectorPanel::RenderPrefabEntity( Ptr<ECS::World::Instance> world,
                                                 ECS::Component::DescriptorManager &desc_mgr,
                                                 Entity entity,
                                                 std::string entity_name ) noexcept
{
  ImGui::PushItemWidth( ImGui::GetWindowWidth() - 30.0f );
  ImGui::BeginGroup();

  auto &archetype = m_world->GetArchetype( entity );
  ECS::Signature prefab_root_sig;
  prefab_root_sig.set( ECS::Component::info<Root>.m_bit_index );
  bool is_prefab_root = ( archetype.GetSignature() & prefab_root_sig ).any();
  bool has_parent = m_world->HaveComponent<Parent>( entity );
  if ( has_parent && !is_prefab_root )
  {
    ImGui::EndGroup();
    ImGui::PopItemWidth();
    return;
  }

  if ( has_parent && is_prefab_root && !m_entity_context->EntityIsSelected( entity ) )
  {
    ImGui::EndGroup();
    ImGui::PopItemWidth();
    return;
  }

  bool has_child = m_world->HaveComponent<Child>( entity );
  ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None;
  if ( m_entity_context->EntityIsSelected( entity ) )
    node_flags |= ImGuiTreeNodeFlags_Selected;
  if ( has_child )
    node_flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
  else
    node_flags |= ImGuiTreeNodeFlags_Leaf;

  bool node_open = false;
  if ( ImGui::TreeNodeEx( ( void * )static_cast<intptr_t>( entity.m_global_index ), node_flags, entity_name.c_str() ) )
  {
    node_open = true;
    if ( ImGui::IsItemClicked() )
    {
      if ( Service::Locator::Get<Input::Manager>().Keyboard()->LeftControl().Pressed() )
      {
        if ( m_entity_context->EntityIsSelected( entity ) )
          m_entity_context->RemoveFromSelectedEntity( entity );
        else if ( !m_entity_context->selections.empty() )
        {
          auto &arch = m_world->GetArchetype( m_entity_context->LastSelectedEntity() );
          if ( !arch.GetSignature()[ECS::Component::info<Prefab>.m_bit_index] )
          {
            m_entity_context->selections.push_back( entity );
            Draw::OnClick( entity, *m_entity_context );
          }
          else
          {
            m_entity_context->ClearSelections();
            m_audio_manager->StopAllAudio();
            m_entity_context->selections.push_back( entity );
            Draw::OnClick( entity, *m_entity_context );
            auto &entry = m_world->m_entity_manager.Get( entity );
            m_entity_context->selected_entity_name = entry.m_name;
          }
        }
      }
      else
      {
        m_entity_context->ClearSelections();
        m_audio_manager->StopAllAudio();
        m_entity_context->selections.push_back( entity );
        Draw::OnClick( entity, *m_entity_context );
        auto &entry = m_world->m_entity_manager.Get( entity );
        m_entity_context->selected_entity_name = entry.m_name;
      }
    }
  }
  if ( node_open )
  {
    if ( has_child )
    {
      auto &child = m_world->GetComponent<Child>( entity );
      RecurChildProperties( world, child );
    }
    ImGui::TreePop();
  }

  ImGui::EndGroup();
  ImGui::PopItemWidth();
}

void PropertyInspectorPanel::RenderPrefabProperties( Ptr<ECS::World::Instance> world, Entity entity ) noexcept
{
  bool has_child = m_world->HaveComponent<Child>( entity );

  ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None;
  if ( m_entity_context->EntityIsSelected( entity ) )
    node_flags |= ImGuiTreeNodeFlags_Selected;
  if ( has_child )
    node_flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
  else
    node_flags |= ImGuiTreeNodeFlags_Leaf;

  auto &archetype = m_world->GetArchetype( entity );
  ECS::Signature prefab_tag;
  prefab_tag.set( ECS::Component::info<PrefabTag>.m_bit_index );
  bool is_not_normal_ent = ( archetype.GetSignature() & ( /*m_prefab_instance_sig |*/ prefab_tag ) ).any();

  bool node_open = false;
  auto &entry = m_world->m_entity_manager.Get( entity );
  if ( is_not_normal_ent && ImGui::TreeNodeEx( ( void * )static_cast<intptr_t>( entity.m_global_index ), node_flags, entry.m_name.c_str() ) )
  {
    node_open = true;
    if ( ImGui::IsItemClicked() )
    {
      if ( Service::Locator::Get<Input::Manager>().Keyboard()->LeftControl().Pressed() )
      {
        if ( m_entity_context->EntityIsSelected( entity ) )
          m_entity_context->RemoveFromSelectedEntity( entity );
        else if ( !m_entity_context->selections.empty() )
        {
          auto &arch = m_world->GetArchetype( m_entity_context->LastSelectedEntity() );
          if ( !arch.GetSignature()[ECS::Component::info<Prefab>.m_bit_index] )
          {
            m_entity_context->selections.push_back( entity );
            Draw::OnClick( entity, *m_entity_context );
          }
          else
          {
            m_entity_context->ClearSelections();
            m_audio_manager->StopAllAudio();
            m_entity_context->selections.push_back( entity );
            Draw::OnClick( entity, *m_entity_context );
            auto &entry = m_world->m_entity_manager.Get( entity );
            m_entity_context->selected_entity_name = entry.m_name;
          }
        }
      }
      else
      {
        m_entity_context->ClearSelections();
        m_audio_manager->StopAllAudio();
        m_entity_context->selections.push_back( entity );
        Draw::OnClick( entity, *m_entity_context );
        auto &entry = m_world->m_entity_manager.Get( entity );
        m_entity_context->selected_entity_name = entry.m_name;
      }
    }
  }
  if ( node_open )
  {
    if ( has_child )
    {
      auto &child = m_world->GetComponent<Child>( entity );
      RecurChildProperties( world, child );
    }
    ImGui::TreePop();
  }
}

void PropertyInspectorPanel::RecurChildProperties( Ptr<ECS::World::Instance> world, Child &child ) noexcept
{
  ImGuiHelper::Indent( 1.0f, [&]
  {
    for ( auto &c : child.m_entities )
    {
      auto &archetype = m_world->GetArchetype( c );
      ECS::Signature root_prefab_tag;
      root_prefab_tag.set( ECS::Component::info<Root>.m_bit_index );
      bool is_prefab_root = ( archetype.GetSignature() & root_prefab_tag ).any();
      if ( is_prefab_root )
        return;
      RenderPrefabProperties( world, c );
    }
  } );
}

std::string PropertyInspectorPanel::Trim( const std::string &str ) noexcept
{
  const auto str_begin = str.find_first_not_of( " " );
  if ( str_begin == std::string::npos )
    return "";

  const auto str_end = str.find_last_not_of( " " );

  return str.substr( str_begin, str_end - str_begin + 1 );
}

std::string PropertyInspectorPanel::RemoveExtraSpace( const std::string &str ) noexcept
{
  auto result = Trim( str );

  auto begin_space = result.find_first_of( " " );
  while ( begin_space != std::string::npos )
  {
    const auto end_space = result.find_first_not_of( " ", begin_space );

    result.replace( begin_space, end_space - begin_space, " " );

    begin_space = result.find_first_of( " ", begin_space + 1 );
  }

  return result;
}

bool PropertyInspectorPanel::RecursiveSearch( Entity entity, const ImGuiTextFilter &world_filter ) noexcept
{
  auto &entry = m_world->m_entity_manager.Get( entity );
  if ( world_filter.PassFilter( entry.m_name.c_str() ) )
    return true;
  if ( m_world->HaveComponent<Child>( entity ) )
  {
    auto childs = m_world->GetComponent<Child>( entity );
    for ( auto &child : childs.m_entities )
    {
      if ( RecursiveSearch( child, world_filter ) )
        return true;
    }
  }
  return false;
}

void PropertyInspectorPanel::RecurRenderPrefabComponents( Ptr<ECS::World::Instance> world,
                                                          ECS::Component::DescriptorManager &desc_mgr,
                                                          Entity entity,
                                                          std::string entity_name,
                                                          const ImGuiTextFilter prop_filter,
                                                          bool ignore_indentation ) noexcept
{
  if ( !ignore_indentation )
  {
    ImGui::Indent( 1.0f );
  }

  if ( prop_filter.PassFilter( entity_name.c_str() ) )
  {
    ImGuiHelper::ChildWindow( "##Entities Details", { 0.0f, 50.f }, true, /*ImGuiWindowFlags_None*/ImGuiWindowFlags_AlwaysAutoResize, [&]
    {
      RenderPrefabEntity( world, desc_mgr, entity, entity_name );
    } );
  }
  else
    ImGui::Text( "No matching search found" );

  if ( m_world->HaveComponent<Child>( entity ) )
  {
    auto &childs = m_world->GetComponent<Child>( entity );

    for ( auto &child : childs.m_entities )
    {
      auto &child_entry = world->m_entity_manager.Get( child );
      RecurRenderPrefabComponents( world, desc_mgr, child, child_entry.m_name, prop_filter, false );
    }
  }

  if ( !ignore_indentation )
  {
    ImGui::Unindent( 1.0f );
  }
}

bool PropertyInspectorPanel::RecurCheckForSelection( Ptr<ECS::World::Instance> world,
                                                     ECS::Component::DescriptorManager &desc_mgr,
                                                     Entity entity,
                                                     const ECS::Signature common_sig,
                                                     Ptr<RM> resource_manager,
                                                     Ptr<EntityContext> entity_context ) noexcept
{
  if ( m_world->HaveComponent<Child>( entity ) )
  {
    auto childs = m_world->GetComponent<Child>( entity );
    for ( auto &child : childs.m_entities )
    {
      if ( RecurCheckForSelection( world, desc_mgr, child, common_sig, resource_manager, entity_context ) )
        return true;
      /*if ( RecurCheckForSelection( world, desc_mgr, child, common_sig, resource_manager, entity_context ) )
        return true;*/
    }
  }
  bool selected = m_entity_context->EntityIsSelected( entity );
  if ( selected )
  {
    ImGuiHelper::ChildWindow( "Prefab Entity Properties", true, [&]
    {
      auto &entry = m_world->m_entity_manager.Get( entity );
      auto pool = entry.m_pool;
      auto pool_index = entry.m_pool_index;
      auto &arch = *pool->m_family->m_archetype;
      ImGui::Dummy( { 0.0f, 8.0f } );

      EntityName( world, entry );

      Add::AddComponent( world, desc_mgr, entity, arch, m_entity_context, m_component_can_add_remove );

      Add::AddTag( world, desc_mgr, entity, arch, m_entity_context, m_component_can_add_remove );

      Add::AddGroupComponent( m_world, desc_mgr, m_entity_context->LastSelectedEntity(), arch, m_entity_context, common_sig );

      if ( m_remove_component )
      {
        m_remove_component = false;
        return;
      }

      ImGuiHelper::ChildWindow( "Components Details", { 0, 0 }, true, ImGuiWindowFlags_AlwaysAutoResize, [&]
      {
        static ImGuiTextFilter prop_filter;
        ImGui::Text( "Search: " );
        ImGui::SameLine();
        prop_filter.Draw( "##Property Filter" );
        ImGui::Text( "" );

        auto can_edit = pool->m_family->m_archetype->GetSignature() & m_component_can_add_remove;
        auto can_show = pool->m_family->m_archetype->GetSignature() & m_component_can_show;

        EntityTag( world, desc_mgr, entity, arch, prop_filter );
        //if (m_entity_context->m_selections.size() == 1)
        //{
        Draw::DrawAllComponents( m_entity_context->cached_map, entity, m_entity_context->selected_signature, resource_manager, entity_context, world, prop_filter, can_edit, can_show );

        /*for ( const auto& [i, desc] : Enumerate( pool->m_descriptors ) )
        {
          std::string name = std::string{ desc->m_name };
          ImGui::PushID( name.c_str() );
          for ( auto& selected_entitiy : m_entity_context->m_selections )
          {
            if ( can_edit[desc->m_bit_index] )
              Draw::PopUpDeleteComponent(world, desc_mgr, selected_entitiy, desc->m_id, m_entity_context);
          }
          ImGui::PopID();
        }*/
      //}
      //else
      //{
      //  Draw::DrawAllComponents( m_entity_context->m_cached_map, entity, m_entity_context->m_selected_signature, resource_manager, entity_context, world, prop_filter );

      //  /*for ( auto &selected_entitiy : m_entity_context->m_selections )
      //  {
      //    for ( const auto& [i, desc] : Enumerate( pool->m_descriptors ) )
      //    {
      //      std::string name = std::string{ desc->m_name };
      //      ImGui::PushID( name.c_str() );
      //      for ( auto& selected_entitiy : m_entity_context->m_selections )
      //      {
      //        if ( can_edit[desc->m_bit_index] )
      //          PopUpDeleteComponent( world, desc_mgr, selected_entitiy, desc->m_id );
      //      }
      //      ImGui::PopID();
      //    }
      //  }*/
      //}

      //auto can_edit = pool->m_family->m_archetype->GetSignature() & m_component_can_add_remove;

      //for ( const auto &[i, desc] : Enumerate( pool->m_descriptors ) )
      //{
      //    // Skip entity
      //  if ( i == 0 )
      //    continue;

      //  rttr::instance instance = desc->m_reflect( &pool->m_components[i][pool_index.m_value * desc->m_size] );
      //  auto instance_name = instance.get_type().get_name().to_string();

      //  if ( prop_filter.PassFilter( instance_name.c_str() ) )
      //  {
      //    ImGui::PushID( instance_name.c_str() );

      //    if (m_entity_context->m_selections.size() == 1 )
      //    {
      //      DrawAllComponents(m_entity_context->m_cached_map, entity, m_entity_context->m_selected_signature, resource_manager, entity_context, world, prop_filter);
      //      //RenderProperties( entity, instance, desc, common_sig, resource_manager, entity_context, world);

      //      if ( can_edit[desc->m_bit_index] )
      //        PopUpDeleteComponent( world, desc_mgr, entity, instance_name );
      //    }
      //    else
      //    {
      //      DrawAllComponents(m_entity_context->m_cached_map, entity, m_entity_context->m_selected_signature, resource_manager, entity_context, world, prop_filter);
      //      //RenderProperties( entity, instance, desc, common_sig, resource_manager, entity_context, world );

      //      for ( auto &selected_entitiy : m_entity_context->m_selections )
      //      {
      //        if ( can_edit[desc->m_bit_index] )
      //          PopUpDeleteComponent( world, desc_mgr, selected_entitiy, instance_name );
      //      }
      //    }
      //    if ( m_remove_component )
      //    {
      //      m_remove_component = false;
      //      ImGui::PopID();
      //      return;
      //    }

      //    ImGui::PopID();
      //  }
      //  else
      //    ImGui::Text( "No matching search found" );
      //}
      } );

      ImGui::Dummy( { 0.0f, 8.0f } );
    } );
  }
  return selected;
}
}
#endif