/*****************************************************************
*\file         CameraSystem.h
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
struct CameraSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = Xivi::ECS::System::Type::Update
  {
    .m_id = 202,
    .m_name = "Camera System"
  };

  struct ChildEvent : ECS::Event::Instance<>
  {};
  using Events = tuplet::tuple<ChildEvent>;
  ECS::Query::Instance m_query;

  Ptr<Graphics::Manager> m_graphic_manager;
  Ptr<Graphics::M_FrameBuffer> m_framebuffer;
  ECS::Query::Instance cameraInitQuery;
  ECS::Query::Instance setTargetQuery;
  ECS::Query::Instance autoOrbitQuery;
  ECS::Query::Instance goalQuery;
  ECS::Query::Instance playerQuery;

  void OnCreate() noexcept;
  void OnResume() noexcept;
  void OnUpdate() noexcept;
  //ECS::Entity* TraverseGetCamera(Child& child);
};
}
