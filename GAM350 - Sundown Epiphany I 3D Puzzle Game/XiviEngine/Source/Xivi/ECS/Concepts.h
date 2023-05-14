/*****************************************************************
*\file         Concepts.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi::ECS::Concepts
{
template <typename C>
concept ComponentReference = Xivi::Concepts::IsReference<C> && Component::Concepts::IsDataComponent<C>;

template <typename C>
concept ComponentPointer = Xivi::Concepts::IsPointer<C> && Component::Concepts::IsDataComponent<C>;

template <typename Func>
concept SystemCallback = ( std::is_same_v< typename Traits::Function::Type<Func>::ReturnType, void > ||
                           std::is_same_v< typename Traits::Function::Type<Func>::ReturnType, bool > ) &&
  [] <Component::Concepts::IsDataComponent... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
{
  return true;
}
( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> );

template <typename Func>
concept SetterCallback = Traits::Concepts::FuncReturnType<Func, void> &&
[] <Component::Concepts::IsDataComponent... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
{
  static_assert( ( ( false == std::is_same_v<Raw<Cs>, Entity> )&&... ), "No Entity component is allowed" );
  static_assert( ( ( std::is_same_v<Cs, std::remove_const_t<Cs>> ) && ... ), "No const in parameters allowed" );
  static_assert( ( ( std::is_reference_v<Cs> )&&... ), "Only references is allowed" );
  return true;
}  ( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> );

template <typename Func>
concept HasSharedParameters = [] <Component::Concepts::IsDataComponent... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
{
  return ( ( Component::info<Cs>.m_type == Component::Type::ID::Shared ) || ... );
}
( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> );

template <typename Func>
concept NoSharedParameters = [] <Component::Concepts::IsDataComponent... Cs>( tuplet::tuple<Cs...>* ) constexpr noexcept
{
  return ( ( Component::info<Cs>.m_type != Component::Type::ID::Shared ) && ... );
}
( Traits::Tuple::null<typename Traits::Function::Type<Func>::ArgsTuple> );
}
