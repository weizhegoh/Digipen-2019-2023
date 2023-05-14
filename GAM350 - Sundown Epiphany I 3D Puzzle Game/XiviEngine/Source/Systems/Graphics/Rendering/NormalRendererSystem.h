/*****************************************************************
*\file         NormalRendererSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "RendererSystem.h"
namespace Xivi::Graphics
{
struct NormalRendererSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::ChildUpdate<RendererSystem, RendererSystem::ChildEvent>
  {
    .m_id = 206,
    .m_name = "Normal Renderer System"
  };

  Ptr<Graphics::Manager> m_graphic_manager;
  Ptr<GLSLShader> m_shader;
  using None = ECS::Query::None<UI>;

  void OnCreate() noexcept;
  void operator()(
    Position &pos,
    Transform &transform,
    StaticMeshComponent &mesh,
    Colour &colour,
    DebugDrawComponent &debugDraw,
    RenderSettings &renderSettings ) noexcept;
};
}