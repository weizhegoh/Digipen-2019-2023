/*****************************************************************
*\file         WireframeRendererSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "RendererSystem.h"
namespace Xivi::Graphics
{
struct WireFrameRendererSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::ChildUpdate<RendererSystem, RendererSystem::WireFrameChildEvent>
  {
    .m_id = 212,
    .m_name = "Wire Frame Renderer System"
  };
  using None = ECS::Query::None<UI>;

  Ptr<Graphics::Manager> m_graphic_manager;

  void OnCreate() noexcept;
  void OnPreUpdate() noexcept;
  void OnUpdate() noexcept;
  void OnPostUpdate() noexcept;
};
}