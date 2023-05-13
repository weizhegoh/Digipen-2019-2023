/*****************************************************************
*\file         Component.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS::Component
{
template <Concepts::IsComponent C>
consteval ID GenerateID()
{
  if constexpr ( std::is_same_v<Entity, C> )
    return ID();

  if constexpr ( C::info.m_id.m_value )
    return C::info.m_id;
  else
    return ID( __FUNCSIG__ );
}
namespace Details
{
template <Concepts::IsComponent C>
consteval Descriptor CreateDescriptor() noexcept
{
  Descriptor result_desc;

  // ID
  result_desc.m_id = GenerateID<C>();

  // Name
  if constexpr ( C::info.m_name.empty() )
    result_desc.m_name = Typename<C>();
  else
    result_desc.m_name = C::info.m_name;

  // Bit Index
  result_desc.m_bit_index = Descriptor::invalid_bit_index;

  // Size
  result_desc.m_size = static_cast<u16>( sizeof( C ) );

  // Type
  result_desc.m_type = C::info.type;

  // Exclusive Tag
  if constexpr ( C::info.type == Type::ID::Tag )
    result_desc.m_exclusive = C::info.exclusive;
  else
    result_desc.m_exclusive = false;

  // Global Scope
  if constexpr ( C::info.type == Type::ID::Shared )
    result_desc.m_global_scoped = C::info.m_global_scoped;
  else
    result_desc.m_global_scoped = false;

  // Build Filter
  if constexpr ( C::info.type == Type::ID::Shared )
    result_desc.m_build_filter = C::info.m_build_filter;
  else
    result_desc.m_build_filter = false;

  if constexpr ( Concepts::TagComponent<C> )
    return result_desc;

  // Constructor
  if constexpr ( std::is_trivially_constructible_v<C> )
    result_desc.m_constructor = nullptr;
  else
    result_desc.m_constructor = []( Ptr<byte> p ) noexcept
  {
    new ( p ) C;
  };

  // Destructor
  if constexpr ( std::is_trivially_destructible_v<C> )
    result_desc.m_destructor = nullptr;
  else
    result_desc.m_destructor = []( Ptr<byte> p ) noexcept
  {
    std::destroy_at( std::bit_cast<Ptr<C>>( p ) );
  };

  // Copy
  if constexpr ( std::is_trivially_copy_assignable_v<C> )
    result_desc.m_copy = nullptr;
  else
    result_desc.m_copy = []( Ptr<byte> dest, Ptr<const byte> src ) noexcept
  {
    *std::bit_cast<Ptr<C>>( dest ) = *std::bit_cast<Ptr<const C>>( src );
  };

  // Move
  if constexpr ( std::is_trivially_move_assignable_v<C> )
    result_desc.m_move = nullptr;
  else
    result_desc.m_move = []( Ptr<byte> dest, Ptr<byte> src ) noexcept
  {
    *std::bit_cast<Ptr<C>>( dest ) = std::move( *std::bit_cast<Ptr<C>>( src ) );
  };

  result_desc.m_reflect = []( Ptr<byte> p ) noexcept
  {
    return rttr::instance( *std::bit_cast<Ptr<C>>( p ) );
  };

  if constexpr ( C::info.type != Type::ID::Shared )
    result_desc.m_generate_key = nullptr;
  else if constexpr ( C::info.m_generator )
    result_desc.m_generate_key = []( const byte *p ) constexpr noexcept->Type::Shared::Key
  {
    constexpr auto id = GenerateID<C>();
    return { C::info.m_generator( p ).m_value + id.m_value };
  };
  else
    result_desc.m_generate_key = []( const byte *p ) constexpr noexcept->Type::Shared::Key
  {
    constexpr auto id = GenerateID<C>();
    return { XXH64( *std::bit_cast<C *>( p ) ) + id.m_value };
  };

  if constexpr ( C::info.type == Type::ID::Shared )
    result_desc.m_shared_key.m_value = XXH64( C() );
  return result_desc;
}
}

constexpr bool CompareDescriptor( Ptr<const Descriptor> lhs, Ptr<const Descriptor> rhs ) noexcept
{
  if ( lhs->m_type == rhs->m_type )
    return lhs->m_id < rhs->m_id;
  return static_cast<u32>( lhs->m_type ) < static_cast<u32>( rhs->m_type );
}

constexpr Type::Shared::Key GenerateSharedKey( Archetype::ID id, const Descriptor &desc, Ptr<const byte> data ) noexcept
{
  if ( data )
  {
    auto key = desc.m_generate_key( data );
    if ( desc.m_global_scoped )
      return key;
    return { id.m_value + key.m_value };
  }
  else if ( desc.m_global_scoped )
    return desc.m_shared_key;
  return { id.m_value + desc.m_shared_key.m_value };
}

template <Concepts::IsComponent... Cs>
constexpr Signature GenerateSignature() noexcept
{
  Signature signature;
  ( signature.set( info<Cs>.m_bit_index ), ... );
  return signature;
}
}