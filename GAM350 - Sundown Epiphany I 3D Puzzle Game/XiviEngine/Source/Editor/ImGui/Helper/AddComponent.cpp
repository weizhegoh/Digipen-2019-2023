/*****************************************************************
*\file         AddComponent.cpp
*\author(s)    Chng Yong Quan, Goh Wei Zhe

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"

#ifdef XV_EDITOR
#include "AddComponent.h"
#include "DrawComponent.h"
#include "Editor/Events/EditorEvents.h"
#include <imgui.h>
#include "ImGuiHelperFunction.h"
namespace Xivi::Editor::Add
{
ECS::Signature NotGroupComponentSignature() noexcept
{
  ECS::Signature sig;

  // Graphics
  GroupComponentSignature<Graphics_Static>( sig );
  GroupComponentSignature<Graphics_Animation>( sig );
  GroupComponentSignature<Graphics_Light>( sig );
  GroupComponentSignature<Graphics_SpotLight>( sig );
  GroupComponentSignature<Graphics_DirectionalLight>( sig );
  GroupComponentSignature<Graphics_Static>( sig );
  GroupComponentSignature<Particles_Particles>( sig );

  // Physics
  //GroupComponentSignature<Physics_Static>(sig);
  //GroupComponentSignature<Physics_Dynamic>(sig);
  //GroupComponentSignature<Physics_Kinematic>(sig);

  // Logic
  GroupComponentSignature<Logic_Player>( sig );
  GroupComponentSignature<Logic_Movable>( sig );
  //GroupComponentSignature<Logic_Interactable>(sig);

  // Grid
  GroupComponentSignature<Grid_Info>( sig );

  sig = ~sig;
  return sig;
}

void AddGroupComponent( Ptr<ECS::World::Instance> world, ECS::Component::DescriptorManager &desc_mgr, const ECS::Entity entity, ECS::Archetype::Instance &arch, Ptr<EntityContext> entity_context, const ECS::Signature &sig ) noexcept
{
  const char *com_current_item = " +\tAdd Group Components";
  ImGuiHelper::DropDown( "##add_grp_comp", com_current_item, [&]
  {
    // Graphics
    if ( !HaveRelatedComponent<Graphics_Animation>( sig ) && !HaveRelatedComponent<Graphics_Static>( sig ) && ImGui::Selectable( "Graphics Animation" ) )
      AddComponentSection<Graphics_Animation>::Add( entity, world, entity_context );
    else if ( !HaveRelatedComponent<Graphics_Static>( sig ) && !HaveRelatedComponent<Graphics_Animation>( sig ) && ImGui::Selectable( "Graphics Static Rendering" ) )
      AddComponentSection<Graphics_Static>::Add( entity, world, entity_context );
    else if ( !HaveRelatedComponent<Graphics_Light>( sig ) && ImGui::Selectable( "Graphics Light Rendering" ) )
      AddComponentSection<Graphics_Light>::Add( entity, world, entity_context );
    else if ( !HaveRelatedComponent<Graphics_SpotLight>( sig ) && ImGui::Selectable( "Graphics SpotLight Rendering" ) )
      AddComponentSection<Graphics_SpotLight>::Add( entity, world, entity_context );
    else if ( !HaveRelatedComponent<Particles_Particles>( sig ) && ImGui::Selectable( "Particles Rendering" ) )
      AddComponentSection<Particles_Particles>::Add( entity, world, entity_context );

    // Physics
    //else if (!HaveRelatedComponent<Physics_Static>(sig) && ImGui::Selectable("Physics Static"))
    //  AddComponentSection<Physics_Static>::Add(entity, world, entity_context);
    //else if (!HaveRelatedComponent<Physics_Dynamic>(sig) && ImGui::Selectable("Physics Dynamic"))
    //  AddComponentSection<Physics_Dynamic>::Add(entity, world, entity_context);
    //else if (!HaveRelatedComponent<Physics_Kinematic>(sig) && ImGui::Selectable("Physics Kinematic"))
    //  AddComponentSection<Physics_Kinematic>::Add(entity, world, entity_context);

    // Logic
    else if ( !HaveRelatedComponent<Logic_Player>( sig ) && ImGui::Selectable( "Player" ) )
      AddComponentSection<Logic_Player>::Add( entity, world, entity_context );
    else if ( !HaveRelatedComponent<Logic_Movable>( sig ) && ImGui::Selectable( "Pushable Objects" ) )
      AddComponentSection<Logic_Movable>::Add( entity, world, entity_context );
    //else if (!HaveRelatedComponent<Logic_Interactable>(sig) && ImGui::Selectable("Interactables"))
    //  AddComponentSection<Logic_Interactable>::Add(entity, world, entity_context);

    // Grid
    else if ( !HaveRelatedComponent<Grid_Info>( sig ) && ImGui::Selectable( "Grid Info" ) )
      AddComponentSection<Grid_Info>::Add( entity, world, entity_context );
  } );
}

void AddComponentOrTag( Ptr<ECS::World::Instance> world, ECS::Component::DescriptorManager &desc_mgr, ECS::Entity entity, ECS::Archetype::Instance &arch, Ptr<EntityContext> entity_context, const ECS::Signature component_can_add_remove, std::string com_current_item, bool data_comp ) noexcept
{
  ImGuiHelper::DropDown( ( "##" + com_current_item ).c_str(), com_current_item.c_str(), [&]
  {
    auto add_signature = data_comp ?
      ~arch.GetSignature() & NotGroupComponentSignature() & ( desc_mgr.m_data_bits | desc_mgr.m_shared_bits ) :
      ~arch.GetSignature() & NotGroupComponentSignature() & ( desc_mgr.m_tag_bits | desc_mgr.m_exclusive_tag_bits );

    // Print dropdown list
    auto descriptors = desc_mgr.DescriptorVector( add_signature );

    auto can_add = add_signature & component_can_add_remove;

    using ListOfDisplayComponent = tuplet::pair<std::string, Ptr<const ECS::Component::Descriptor>>;
    std::vector<ListOfDisplayComponent> ordered_by_name;

    for ( const auto &desc : descriptors )
      if ( can_add[desc->m_bit_index] )
        ordered_by_name.push_back( { std::string{ desc->m_name }, desc } );

    std::sort( ordered_by_name.begin(), ordered_by_name.end(), [=]( ListOfDisplayComponent &lhs, ListOfDisplayComponent &rhs )
    {
      return lhs.first < rhs.first;
    } );

    for ( const auto &[name, desc] : ordered_by_name )
      if ( ImGui::Selectable( name.c_str() ) )
      {
        System::Instance().m_command_history.Commit<AddComponentsCommand>( *world, entity, ECS::DescriptorSpan { &desc, 1 } );
        Draw::OnClick( entity, *entity_context );
        break;
      }
  } );
}

void AddComponent( Ptr<ECS::World::Instance> world, ECS::Component::DescriptorManager &desc_mgr, ECS::Entity entity, ECS::Archetype::Instance &arch, Ptr<EntityContext> entity_context, const ECS::Signature component_can_add_remove ) noexcept
{
  AddComponentOrTag( world, desc_mgr, entity, arch, entity_context, component_can_add_remove, " +\tAdd Component", true );
}

void AddTag( Ptr<ECS::World::Instance> world, ECS::Component::DescriptorManager &desc_mgr, ECS::Entity entity, ECS::Archetype::Instance &arch, Ptr<EntityContext> entity_context, const ECS::Signature component_can_add_remove ) noexcept
{
  AddComponentOrTag( world, desc_mgr, entity, arch, entity_context, component_can_add_remove, " +\tAdd Tag", false );
}
}
#endif