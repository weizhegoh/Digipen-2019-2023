/*****************************************************************
*\file         UITransformSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "TransformSystem.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
namespace Xivi::Graphics
{
class M_FrameBuffer;
struct UITransformSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::ChildUpdate<TransformSystem, TransformSystem::ChildEvent>
  {
    .m_id = 217,
    .m_name = "Single Transform System"
  };
  using All = ECS::Query::All<Transform, UI>;

  Ptr<Graphics::M_FrameBuffer> m_framebuffer;

  void OnCreate() noexcept;

  void operator()(
    Entity &entity,
    Position &pos,
    Transform &transform,
    AABB &aabb ) noexcept;
};
}