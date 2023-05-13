/*****************************************************************
*\file         AddComponent.cpp
*\author(s)    Chng Yong Quan, Goh Wei Zhe

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#ifdef XV_EDITOR
#include "Xivi/ECS/XVECS.h"
#include "Components/AudioComponents.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "Components/GUIComponents.h"
#include "Components/ParticleComponents.h"
#include "../Sundown/Source/NComponents/Grid/Grid.h"
#include "Editor/Events/EditorEvents.h"

#include "Editor/System.h"
#include "Editor/Command/Actions/AddComponentsCommand.h"

#include "DrawComponent.h"
namespace Xivi::Editor
{
struct EntityContext;
}

namespace Xivi::Editor::Add
{
/**
 * @brief
 * Alias for tuple of components combination
 */
  //Particles
using Particles_Particles = tuplet::tuple<ParticleSettings, ParticleEmitter>;
//Graphics
using Graphics_Static = tuplet::tuple<StaticMeshComponent>;
using Graphics_Animation = tuplet::tuple<AnimationState, AnimatorComponent, AnimationMeshComponent>;
using Graphics_Light = tuplet::tuple<LightProperty, QuadraticFunction>;
using Graphics_SpotLight = tuplet::tuple<SpotLight_cutOff, LightProperty, QuadraticFunction>;
using Graphics_DirectionalLight = tuplet::tuple<LightProperty, QuadraticFunction>;
//Physics
//using Physics_Static = tuplet::tuple<StaticBodyTag>;
//using Physics_Dynamic = tuplet::tuple<DynamicBodyTag, LinearVelocity, Force, Mass>;
//using Physics_Kinematic = tuplet::tuple<KinematicBodyTag, DynamicBodyTag, LinearVelocity, Force, Mass>;

//Logic
using Logic_Player = tuplet::tuple<PlayerControls, PlayerState>;
using Logic_Movable = tuplet::tuple<PushableObject>;
//using Logic_Interactable = tuplet::tuple</*StaticBodyTag,*/ InteractableTag>;

//Grid
using Grid_Info = tuplet::tuple<EntityType, GridPosition>;

template <Traits::Concepts::IsTuple Cs>
bool HaveRelatedComponent( const ECS::Signature signature ) noexcept
{
  ECS::Signature sig;
  [&] <typename... Cs>( tuplet::tuple<Cs...>* )
  {
    ( [&]<typename C>( tuplet::tuple<C>* )
    {
      sig.set( ECS::Component::info<C>.m_bit_index );
    }( Traits::Tuple::null<tuplet::tuple<Cs>> ), ... );
  }( Traits::Tuple::null<Cs> );

  return ( signature & sig ).any();
}

template <Traits::Concepts::IsTuple Cs>
void GroupComponentSignature( ECS::Signature &signature ) noexcept
{
  [&] <typename... Cs>( tuplet::tuple<Cs...>* )
  {
    ( [&]<typename C>( tuplet::tuple<C>* )
    {
      if ( !signature[ECS::Component::info<C>.m_bit_index] )
        signature.set( ECS::Component::info<C>.m_bit_index );
    }( Traits::Tuple::null<tuplet::tuple<Cs>> ), ... );
  }( Traits::Tuple::null<Cs> );
}

template <Traits::Concepts::IsTuple CTuple>
struct AddComponentSection
{
  inline static void Add( Entity entity, Ptr<ECS::World::Instance> world, Ptr<EntityContext> entity_context )
  {
    [&] <typename... Cs>( tuplet::tuple<Cs...>* )
    {
      ( [&]<typename C>( tuplet::tuple<C>* )
      {
        const Ptr<const ECS::Component::Descriptor> desc = &ECS::Component::info<C>;
        System::Instance().m_command_history.Commit<AddComponentsCommand>( *world, entity, ECS::DescriptorSpan { &desc, 1 } );
      }( Traits::Tuple::null<tuplet::tuple<Cs>> ), ... );
    }( Traits::Tuple::null<CTuple> );

    Draw::OnClick( entity, *entity_context );
  }
};

ECS::Signature NotGroupComponentSignature() noexcept;

void AddGroupComponent( Ptr<ECS::World::Instance> world, ECS::Component::DescriptorManager &desc_mgr, const ECS::Entity entity, ECS::Archetype::Instance &arch, Ptr<EntityContext> entity_context, const ECS::Signature &sig ) noexcept;

void AddComponent( Ptr<ECS::World::Instance> world, ECS::Component::DescriptorManager &desc_mgr, ECS::Entity entity, ECS::Archetype::Instance &arch, Ptr<EntityContext> entity_context, const ECS::Signature component_can_add_remove ) noexcept;

void AddTag( Ptr<ECS::World::Instance> world, ECS::Component::DescriptorManager &desc_mgr, ECS::Entity entity, ECS::Archetype::Instance &arch, Ptr<EntityContext> entity_context, const ECS::Signature component_can_add_remove ) noexcept;
}
#endif