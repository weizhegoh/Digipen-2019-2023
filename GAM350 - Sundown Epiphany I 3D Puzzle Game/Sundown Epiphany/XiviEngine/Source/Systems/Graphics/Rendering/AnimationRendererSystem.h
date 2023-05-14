/*****************************************************************
*\file         AnimationRendererSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "RendererSystem.h"

namespace Xivi::Graphics
{
class GLSLShader;
struct AnimationRendererSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::ChildUpdate<RendererSystem, RendererSystem::ChildEvent>
  {
    .m_id = 203,
    .m_name = "Animation Renderer System"
  };
  using None = ECS::Query::None<UI>;

  Ptr<Graphics::Manager> m_graphic_manager;
  Ptr<GLSLShader> m_shader_manager;
  void OnCreate() noexcept;

  void operator()(
    Entity &entity,
    Position &pos,
    Transform &transform,
    AnimationMeshComponent &mesh,
    AnimatorComponent &animator,
    Material &material,
    Colour &colour,
    RenderSettings &renderSettings,
    LightData &lightData ) noexcept;
};
}