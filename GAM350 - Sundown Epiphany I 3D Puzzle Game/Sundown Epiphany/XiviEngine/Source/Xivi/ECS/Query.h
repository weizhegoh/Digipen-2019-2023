/*****************************************************************
*\file         Query.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi::ECS::Query
{
template <Component::Concepts::IsComponent... Cs>
struct All final
{};

template <Component::Concepts::IsComponent... Cs>
struct Any final
{};

template <Component::Concepts::IsComponent... Cs>
struct None final
{};

namespace Concepts
{
template <typename T>
concept IsAll = Traits::IsSpecialised<All, T>();

template <typename T>
concept IsAny = Traits::IsSpecialised<Any, T>();

template <typename T>
concept IsNone = Traits::IsSpecialised<None, T>();
}

struct Instance final
{
  Signature m_all;
  Signature m_any;
  Signature m_none;

  template <ECS::Concepts::ComponentReference C>
  inline void FunctionSet() noexcept;

  template <ECS::Concepts::ComponentPointer C>
  inline void FunctionSet() noexcept;

public:
  Instance() noexcept = default;
  Instance( const Instance &rhs ) = default;
  Instance &operator=( const Instance &rhs ) = default;

  inline bool Compare( const Signature &signature ) const noexcept;

  inline bool Compare( const Signature &signature, const Signature &exclusive ) const noexcept;

  template <Concepts::IsAll AllComponents,
    Concepts::IsAny AnyComponents,
    Concepts::IsNone NoneComponents>
    inline Instance &Set() noexcept;

  inline Instance &Set( Instance q ) noexcept;

  template <Component::Concepts::IsComponent... Cs>
  inline Instance &AllOf() noexcept;

  template <Component::Concepts::IsComponent... Cs>
  inline Instance &AnyOf() noexcept;

  template <Component::Concepts::IsComponent... Cs>
  inline Instance &NoneOf() noexcept;

  template <Traits::Concepts::InvokableFunction F>
  inline void SetQueryFromFunction() noexcept;

  template <Traits::Concepts::InvokableFunction F>
  inline void SetQueryFromFunction( F && ) noexcept;

  inline u64 GenerateID() const noexcept;
};
}
