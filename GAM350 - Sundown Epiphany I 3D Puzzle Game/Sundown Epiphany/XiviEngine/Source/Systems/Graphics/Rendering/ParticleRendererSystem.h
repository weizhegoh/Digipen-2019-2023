/*****************************************************************
*\file         ParticleRendererSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "RendererSystem.h"
#include "Components/ParticleComponents.h"
namespace Xivi::Graphics
{
struct ParticleRendererSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::ChildUpdate<RendererSystem, RendererSystem::ChildEvent>
  {
    .m_id = 263,
    .m_name = "Static Renderer System"
  };
  using None = ECS::Query::None<UI, SkyBoxMaterial>;

  Ptr<Graphics::Manager> m_graphic_manager { nullptr };
  Ptr<Graphics::M_FrameBuffer> m_framebuffer { nullptr };
  void OnCreate() noexcept;
  void operator()( ParticleEmitter &emitter, ParticleSettings &settings ) noexcept;
};
}