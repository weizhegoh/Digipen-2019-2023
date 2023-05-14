/*****************************************************************
*\file         LightTransformSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi::Graphics
{
class Manager;
class M_FrameBuffer;
struct LightTransformSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = Xivi::ECS::System::Type::Update
  {
    .m_id = 270,
    .m_name = "Light Transform System"
  };
  Ptr<Graphics::Manager> m_graphic_manager { nullptr };
  Ptr<Graphics::M_FrameBuffer> m_framebuffer { nullptr };

  void OnCreate() noexcept;
  void OnUpdate() noexcept;
};
}