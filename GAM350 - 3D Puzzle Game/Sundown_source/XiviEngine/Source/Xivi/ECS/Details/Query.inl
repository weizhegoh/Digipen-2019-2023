/*****************************************************************
*\file         Query.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS::Query
{
template <ECS::Concepts::ComponentReference C>
void Instance::FunctionSet() noexcept
{
  m_all.set( Component::info<C>.m_bit_index );
}

template <ECS::Concepts::ComponentPointer C>
void Instance::FunctionSet() noexcept
{
  m_any.set( Component::info<C>.m_bit_index );
}

bool Instance::Compare( const Signature &signature ) const noexcept
{
  if ( m_none & signature ) return false;

  if ( ( ( m_all & signature ) ^ m_all ) ) return false;

  if ( m_any & signature || !m_any.any() ) return true;

  return false;
}

bool Instance::Compare( const Signature &signature, const Signature &exclusive ) const noexcept
{
  if ( m_none & signature ) return false;

  if ( ( ( m_all & signature ) ^ m_all ) ) return false;

  if ( ( ( m_all | m_any ) & exclusive ) ^ exclusive ) return false;

  if ( m_any & signature || !m_any.any() ) return true;

  return false;
}

template <Concepts::IsAll AllComponents,
  Concepts::IsAny AnyComponents,
  Concepts::IsNone NoneComponents>
  Instance &Instance::Set() noexcept
{
  [&] <Component::Concepts::IsComponent... Cs>( All<Cs...> )
  {
    ( m_all.set( Component::info<Cs>.m_bit_index ), ... );
  }
  ( AllComponents() );

  [&] <Component::Concepts::IsComponent... Cs>( Any<Cs...> )
  {
    ( m_any.set( Component::info<Cs>.m_bit_index ), ... );
  }
  ( AnyComponents() );

  [&] <Component::Concepts::IsComponent... Cs>( None<Cs...> )
  {
    ( m_none.set( Component::info<Cs>.m_bit_index ), ... );
  }
  ( NoneComponents() );
  return *this;
}

Instance &Instance::Set( Instance q ) noexcept
{
  *this = q;
  return *this;
}

template <Component::Concepts::IsComponent... Cs>
Instance &Instance::AllOf() noexcept
{
  ( m_all.set( Component::info<Cs>.m_bit_index ), ... );
  return *this;
}

template <Component::Concepts::IsComponent... Cs>
Instance &Instance::AnyOf() noexcept
{
  ( m_any.set( Component::info<Cs>.m_bit_index ), ... );
  return *this;
}

template <Component::Concepts::IsComponent... Cs>
Instance &Instance::NoneOf() noexcept
{
  ( m_none.set( Component::info<Cs>.m_bit_index ), ... );
  return *this;
}

template <Traits::Concepts::InvokableFunction F>
void Instance::SetQueryFromFunction() noexcept
{
  [&] <Component::Concepts::IsComponent... Cs>( tuplet::tuple<Cs...>* )
  {
    ( FunctionSet<Cs>(), ... );
  }( Traits::Tuple::null<Traits::Function::Type<F>::ArgsTuple> );
}

template <Traits::Concepts::InvokableFunction F>
void Instance::SetQueryFromFunction( F && ) noexcept
{
  SetQueryFromFunction<F>();
}

inline u64 Instance::GenerateID() const noexcept
{
  return Hash::XXH3_64( *this );
}
}