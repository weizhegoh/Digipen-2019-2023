/*****************************************************************
*\file         FamilyTransformSystem.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "TransformSystem.h"
#include "Components/GraphicsComponents.h"
namespace Xivi::Graphics
{
struct FamilyTransformSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::ChildUpdate<TransformSystem, TransformSystem::ChildEvent>
  {
    .m_id = 213,
    .m_name = "Parent Transform System"
  };

  using All = ECS::Query::All<Root, Transform>;
  using None = ECS::Query::None<UI>;

  void operator()(
    Entity &entity,
    Position &pos,
    Transform &transform,
    Child &child ) noexcept;
  void Traverse( Child &child );
};
}