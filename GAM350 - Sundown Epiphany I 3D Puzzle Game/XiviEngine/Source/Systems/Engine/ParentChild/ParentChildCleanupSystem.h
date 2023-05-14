/*****************************************************************
*\file         ParentChildCleanupSystem.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct ParentChildCleanupSystem : ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::NotifyDestroy
  {
    .m_id = 102,
    .m_name = "Parent-Child Cleanup System"
  };

  unordered_map<Entity, std::vector<Entity>> m_cleanup;

  using Any = ECS::Query::Any<Parent, Child>;

  void operator()( Entity &entity, Parent *parent, Child *child )noexcept;
};
}