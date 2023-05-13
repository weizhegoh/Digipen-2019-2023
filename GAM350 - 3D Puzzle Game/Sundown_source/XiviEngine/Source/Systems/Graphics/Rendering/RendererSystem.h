/*****************************************************************
*\file         RendererSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "Xivi/Core/Declaration.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
namespace Xivi::Graphics
{
class M_FrameBuffer;
class Manager;
struct RendererSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = Xivi::ECS::System::Type::Update
  {
    .m_id = 207,
    .m_name = "Renderer System"
  };

  struct ChildEvent : ECS::Event::Instance<>
  {};
  struct WireFrameChildEvent : ECS::Event::Instance<>
  {};
  struct RendererUI : ECS::Event::Instance<>
  {};
  struct DebugDrawEvent : ECS::Event::Instance<>
  {};
  struct GridRenderEvent : ECS::Event::Instance<>
  {};
  struct ParticleEvent : ECS::Event::Instance<>
  {};
  struct BuildShadowEvent : ECS::Event::Instance<>
  {};
  struct SkyBoxEvent : ECS::Event::Instance<>
  {};
  using Events = tuplet::tuple<SkyBoxEvent, ChildEvent, WireFrameChildEvent, RendererUI, DebugDrawEvent, GridRenderEvent, ParticleEvent, BuildShadowEvent>;

  Ptr<RM> m_resource_manager { nullptr };
  Ptr< Graphics::M_FrameBuffer> m_framebuffer { nullptr };
  Ptr< Graphics::Manager> m_graphic_manager { nullptr };
  //Mesh quad;
  void OnCreate() noexcept;
  void OnPreUpdate() noexcept;
  void OnUpdate() noexcept;
  void OnPostUpdate() noexcept;
};
}
