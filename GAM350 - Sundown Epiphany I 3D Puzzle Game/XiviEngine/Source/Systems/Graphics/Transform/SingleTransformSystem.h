/*****************************************************************
*\file         SingleTransformSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "TransformSystem.h"
namespace Xivi::Graphics
{
struct SingleTransformSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::ChildUpdate<TransformSystem, TransformSystem::ChildEvent>
  {
    .m_id = 215,
    .m_name = "Single Transform System"
  };
  using All = ECS::Query::All<Transform>;
  using None = ECS::Query::None<Root, Parent, Child, UI>;
  void operator()(
    ECS::Entity &entity,
    Position &pos,
    Transform &transform ) noexcept;
};
}