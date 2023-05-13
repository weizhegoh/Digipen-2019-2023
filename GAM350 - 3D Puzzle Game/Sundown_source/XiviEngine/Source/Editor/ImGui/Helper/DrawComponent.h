/*****************************************************************
*\file         DrawComponent.h
*\author(s)    Chng Yong Quan, Goh Wei Zhe

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#ifdef XV_EDITOR
#include <imgui.h>
#include <IconsFontAwesome5.h>
#include <Service/Locator.h>

// Components
#include "Xivi/ECS/XVECS.h"
#include "Components/AudioComponents.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "Components/GUIComponents.h"
#include "Components/ParticleComponents.h"
#include "../Sundown/Source/Components/Components.h"
#include "../Sundown/Source/NComponents.h"

#include "Systems/Audio/Audio.h"
#include "Xivi/GUI/GUIManager.h"

#include "Editor/Contexts/EntityContext.h"
#include "Editor/ImGui/Helper/ImGuiHelperFunction.h"
#include "Editor/Events/EditorEvents.h"

#include "Editor/System.h"
#include "Editor/Command/Actions/UpdateComponentCommand.h"

#include "Editor/Systems/Base/ComponentUpdateSystem.h"

namespace Xivi::Editor::Draw
{
    // Default
using Default = tuplet::tuple<Position, Transform, AABB>;

//Graphics
using Graphics_Default = tuplet::tuple<Colour, Direction, RenderSettings, DebugDrawComponent, Camera_c>;
using Graphics_AnimationRendering = tuplet::tuple<AnimationState, AnimationMeshComponent, AnimatorComponent>;
using Graphics_StaticRendering = tuplet::tuple<StaticMeshComponent>;
using Graphics_MaterialRendering = tuplet::tuple<Material>;
using Graphics_SkyboxRendering = tuplet::tuple<SkyBoxMaterial>;
using Particles_Particle = tuplet::tuple<ParticleSettings, ParticleEmitter>;
//Lightings
using Graphics_LightRendering = tuplet::tuple<LightProperty, SpotLight_cutOff, QuadraticFunction>;

//GUI
using GUI_Components = tuplet::tuple<UI, GUI::UICheckBox, GUI::UIDrag, GUI::UITrigger, GUI::UIHover, GUI::UIBar, GUI::UICheese, GUI::SwitchPopupDialogue, GUI::PressurePlatePopupDialogue, GUI::CheesePopupDialogue, GUI::HolePopupDialogue, GUI::ObstaclePopupDialogue, GUI::CartPopupDialogue>;

// Logic
using Logic_Components = tuplet::tuple<PlayerControls, PlayerState, PushableObject, EntityDirection, EntityType, NewPosition, NewTransform,
  FallingFloor, Lock, Switch, Waypoints, Player, PressurePlateC, Hover>;

// Audio
using Audio_Components = tuplet::tuple</*AudioSource*/Audio2DComponent, Audio3DComponent>;

// Prefab
using Prefab_Components = tuplet::tuple<Prefab, ReferencedPrefab>;

// Grid
using Grid_Components = tuplet::tuple<GridPosition>;

template <Traits::Concepts::InvokableFunction Func>
void ShowProperties( Ptr<byte> data, const Ptr<const ECS::Component::Descriptor> desc, ImGuiTextFilter &filter, Func func ) noexcept;

void OnClick( const Entity entity, EntityContext &entity_context ) noexcept;

void PopUpDeleteComponent( Ptr<ECS::World::Instance> world,
                           ECS::Component::DescriptorManager &desc_mgr,
                           ECS::Entity entity,
                           ECS::Component::ID id,
                           Ptr<EntityContext> entity_context,
                           ECS::Signature signature,
                           const bool can_edit ) noexcept;

bool RenderEnumeration( const std::string &prop_name,
                        rttr::type &prop_type,
                        rttr::variant &value,
                        const rttr::instance &instance,
                        rttr::property &property,
                        const Ptr<const ECS::Component::Descriptor> desc,
                        const Entity entity,
                        Ptr<EntityContext> entity_context,
                        Ptr<ECS::World::Instance> world ) noexcept;

template <typename DataType>
inline void RenderAssetsProperties( const std::string &prop_name,
                                    rttr::property property,
                                    const Ptr<const ECS::Component::Descriptor> desc,
                                    Resource::Handler<DataType> &data,
                                    const Entity entity,
                                    Ptr<RM> resource_manager,
                                    Ptr<EntityContext> entity_context,
                                    Ptr<ECS::World::Instance> world ) noexcept;

template <Traits::Concepts::FuncReturnType<bool> Func>
inline void SnapshotInput( const Ptr<const ECS::Component::Descriptor> desc,
                           const Entity entity,
                           Ptr<EntityContext> entity_context,
                           Ptr<ECS::World::Instance> world,
                           Func func ) noexcept;

void RenderBaseType( const std::string &prop_name,
                     rttr::property property,
                     rttr::type &prop_type,
                     rttr::variant &value,
                     const Ptr<const ECS::Component::Descriptor> desc,
                     const Entity entity,
                     Ptr<EntityContext> entity_context,
                     Ptr<ECS::World::Instance> world ) noexcept;

void RenderClassType( const std::string &prop_name,
                      rttr::property &property,
                      rttr::type &prop_type,
                      rttr::variant &value,
                      const Ptr<const ECS::Component::Descriptor> desc,
                      const Entity entity,
                      Ptr<RM> resource_manager,
                      Ptr<EntityContext> entity_context,
                      Ptr<ECS::World::Instance> world ) noexcept;

void RenderProperties( Entity entity,
                       const rttr::instance &instance,
                       const Ptr<const ECS::Component::Descriptor> desc,
                       const ECS::Signature common_sig,
                       Ptr<RM> resource_manager,
                       Ptr<EntityContext> entity_context,
                       Ptr<ECS::World::Instance> world ) noexcept;

template <ECS::Component::Concepts::IsComponent C>
inline void DrawComponent( Ptr<byte> data,
                           Entity entity,
                           const Ptr<const ECS::Component::Descriptor> desc,
                           const ECS::Signature &signature,
                           Ptr<RM> resource_manager,
                           Ptr<EntityContext> entity_context,
                           Ptr<ECS::World::Instance> world,
                           ImGuiTextFilter &filter,
                           const ECS::Signature can_edit ) noexcept;

template <ECS::Component::Concepts::IsComponent C, Traits::Concepts::InvokableFunction Func>
inline void SpecializeDrag3FloatReset( Ptr<byte> data,
                                       Entity entity,
                                       const Ptr<const ECS::Component::Descriptor> desc,
                                       Ptr<EntityContext> entity_context,
                                       Ptr<ECS::World::Instance> world,
                                       ImGuiTextFilter &filter,
                                       Func func ) noexcept;

template <Traits::Concepts::IsTuple CTuple>
struct ComponentSection
{
  inline static void Draw( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                           Entity entity,
                           ECS::Signature signature,
                           Ptr<RM> resource_manager,
                           Ptr<EntityContext> entity_context,
                           Ptr<ECS::World::Instance> world,
                           ImGuiTextFilter &filter,
                           const ECS::Signature can_edit ) noexcept;
};

template <Traits::Concepts::InvokableFunction Func>
inline void AssignHeader( const char *name, Func func ) noexcept;

template <Traits::Concepts::IsTuple Cs>
inline ECS::Signature SetSignature() noexcept;

void DrawDefault( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                  const Entity entity,
                  ECS::Signature signature,
                  Ptr<RM> resource_manager,
                  Ptr<EntityContext> entity_context,
                  Ptr<ECS::World::Instance> world,
                  ImGuiTextFilter &filter,
                  const ECS::Signature can_edit,
                  const ECS::Signature can_draw ) noexcept;

void DrawGraphics( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                   const Entity &entity,
                   ECS::Signature signature,
                   Ptr<RM> resource_manager,
                   Ptr<EntityContext> entity_context,
                   Ptr<ECS::World::Instance> world,
                   ImGuiTextFilter &filter,
                   const ECS::Signature &can_edit,
                   const ECS::Signature &can_draw ) noexcept;

void DrawLightings( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                    const Entity entity,
                    ECS::Signature signature,
                    Ptr<RM> resource_manager,
                    Ptr<EntityContext> entity_context,
                    Ptr<ECS::World::Instance> world,
                    ImGuiTextFilter &filter,
                    const ECS::Signature can_edit,
                    const ECS::Signature can_draw ) noexcept;

//void DrawPhysics( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
//                  const Entity entity,
//                  ECS::Signature signature,
//                  Ptr<RM> resource_manager,
//                  Ptr<EntityContext> entity_context,
//                  Ptr<ECS::World::Instance> world,
//                  ImGuiTextFilter &filter,
//                  const ECS::Signature can_edit,
//                  const ECS::Signature can_draw ) noexcept;

void DrawLogic( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                const Entity entity,
                ECS::Signature signature,
                Ptr<RM> resource_manager,
                Ptr<EntityContext> entity_context,
                Ptr<ECS::World::Instance> world,
                ImGuiTextFilter &filter,
                const ECS::Signature can_edit,
                const ECS::Signature can_draw ) noexcept;

void DrawGUI( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
              const Entity entity,
              ECS::Signature signature,
              Ptr<RM> resource_manager,
              Ptr<EntityContext> entity_context,
              Ptr<ECS::World::Instance> world,
              ImGuiTextFilter &filter,
              const ECS::Signature can_edit,
              const ECS::Signature can_draw ) noexcept;

void DrawAudio( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                const Entity entity,
                ECS::Signature signature,
                Ptr<RM> resource_manager,
                Ptr<EntityContext> entity_context,
                Ptr<ECS::World::Instance> world,
                ImGuiTextFilter &filter,
                const ECS::Signature can_edit,
                const ECS::Signature can_draw ) noexcept;

void DrawPrefab( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                 const Entity &entity,
                 ECS::Signature signature,
                 Ptr<RM> resource_manager,
                 Ptr<EntityContext> entity_context,
                 Ptr<ECS::World::Instance> world,
                 ImGuiTextFilter &filter,
                 const ECS::Signature &can_edit,
                 const ECS::Signature &can_draw ) noexcept;

void DrawGridInfo( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                   const Entity &entity,
                   ECS::Signature signature,
                   Ptr<RM> resource_manager,
                   Ptr<EntityContext> entity_context,
                   Ptr<ECS::World::Instance> world,
                   ImGuiTextFilter &filter,
                   const ECS::Signature &can_edit,
                   const ECS::Signature &can_draw ) noexcept;

void DrawAllComponents( unordered_map<ECS::Component::ID, Ptr<byte>> &cached_components,
                        const Entity &entity,
                        ECS::Signature signature,
                        Ptr<RM> resource_manager,
                        Ptr<EntityContext> entity_context,
                        Ptr<ECS::World::Instance> world,
                        ImGuiTextFilter &filter,
                        const ECS::Signature &can_edit,
                        const ECS::Signature &can_draw ) noexcept;
}

#include "DrawComponent.inl"
#endif