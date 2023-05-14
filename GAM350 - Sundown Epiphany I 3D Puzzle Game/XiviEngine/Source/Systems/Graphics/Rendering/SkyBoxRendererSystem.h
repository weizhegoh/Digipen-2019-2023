/*****************************************************************
*\file         SkyBoxRendererSystem.h
*\author(s)    Lee Jun Jie

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "RendererSystem.h"
namespace Xivi::Graphics
{
struct SkyBoxRendererSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::ChildUpdate<RendererSystem, RendererSystem::SkyBoxEvent>
  {
    .m_id = 209,
    .m_name = "SkyBox Renderer System"
  };
  using None = ECS::Query::None<UI>;

  Ptr<Graphics::Manager> m_graphic_manager;
  Ptr<GLSLShader> m_skybox_shader;

  void OnCreate() noexcept;

  void operator()(
    Transform &transform,
    StaticMeshComponent &mesh,
    SkyBoxMaterial &material ) noexcept;
};
}