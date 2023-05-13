/*****************************************************************
*\file         LocalToParentUpdateSystem.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
namespace Xivi
{
struct LocalToParentUpdateSystem : ECS::System::Instance
{
  static constexpr auto info = ECS::System::Type::Update { .m_id = 101, .m_name = "Local To Parent Update System" };

  using All = ECS::Query::All<Root, Child>;
  using None = ECS::Query::None<Parent, UI>;

  std::function<void( Position &pos, Child &child )> m_updates;

  void OnCreate() noexcept;

  void operator()( Position &pos, Child &child ) noexcept;
};
}