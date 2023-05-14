/*****************************************************************
*\file         StaticRendererSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "RendererSystem.h"
namespace Xivi::Graphics
{
struct StaticRendererSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::ChildUpdate<RendererSystem, RendererSystem::ChildEvent>
  {
    .m_id = 203,
    .m_name = "Static Renderer System"
  };
  using None = ECS::Query::None<SkyBoxMaterial, UI>;

  Ptr<Graphics::Manager> m_graphic_manager { nullptr };
  Ptr<Graphics::M_FrameBuffer> m_framebuffer { nullptr };

  void OnCreate() noexcept;

  void operator()(
    Entity &entity,
    Position &pos,
    Transform &transform,
    StaticMeshComponent &mesh,
    Material &material,
    Colour &colour,
    RenderSettings &renderSettings,
    LightData &lightData ) noexcept;
};
}