/*****************************************************************
*\file         Component.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi::ECS::Component::Type
{
enum class ID : u8
{
  Data,
  Shared,
  Tag,
  Exclusive,
  Singleton
};

struct Data final
{
  static constexpr auto max_size = Settings::page_size;
  static constexpr auto type = ID::Data;

  Component::ID m_id;
  std::string_view m_name {};
};

struct Shared final
{
  struct Key final
  {
    auto operator<=>( const Key & ) const = default;
    u64 m_value { 0u };
  };

  using GenerateKey = FuncPtr<Key( Ptr<const byte> ) noexcept>;

  static constexpr auto max_size = Settings::page_size;
  static constexpr auto type = ID::Shared;

  Component::ID m_id;
  std::string_view m_name {};
  GenerateKey m_generator { nullptr };
  bool m_global_scoped { true };
  bool m_build_filter { false };
};

struct Tag final
{
  static constexpr auto max_size = 1u;
  static constexpr auto type = ID::Tag;
  static constexpr auto exclusive = false;

  Component::ID m_id;
  std::string_view m_name {};
};

struct Exclusive final
{
  static constexpr auto max_size = 1u;
  static constexpr auto type = ID::Exclusive;
  static constexpr auto exclusive = true;

  Component::ID m_id;
  std::string_view m_name {};
};

struct Singleton final
{
  static constexpr auto type = ID::Singleton;
  Component::ID m_id;
  std::string_view m_name {};
};
}

template <>
struct std::hash<Xivi::ECS::Component::Type::Shared::Key>
{
  auto operator()( const Xivi::ECS::Component::Type::Shared::Key key ) const
  {
    return Xivi::Hash::XXH3_64( key.m_value );
  }
};

namespace Xivi::ECS::Component::Concepts
{
template <typename C>
concept DataComponent = Raw<C>::info.type == Type::ID::Data &&
sizeof( Raw<C> ) <= Type::Data::max_size;

template <typename C>
concept SharedComponent = Raw<C>::info.type == Type::ID::Shared &&
sizeof( Raw<C> ) <= Type::Shared::max_size;

template <typename C>
concept TagComponent = Raw<C>::info.type == Type::ID::Tag &&
Raw<C>::info.exclusive == false &&
sizeof( Raw<C> ) <= Type::Tag::max_size;

template <typename C>
concept ExclusiveTagComponent = Raw<C>::info.type == Type::ID::Exclusive &&
Raw<C>::info.exclusive == true &&
sizeof( Raw<C> ) <= Type::Tag::max_size;

template <typename C>
concept SingletonComponent = Raw<C>::info.type == Type::ID::Singleton;

template <typename C>
concept IsDataComponent = DataComponent<C> || SharedComponent<C>;

template <typename C>
concept IsComponent = TagComponent<C> || ExclusiveTagComponent<C> || DataComponent<C> || SharedComponent<C> || SingletonComponent<C>;

template <typename List>
concept IsComponentList = IsTypelist<List> &&
[]<IsComponent... Cs>( Typelist<Cs...> ) constexpr noexcept
{
  return true;
}( List() );
}

namespace Xivi::ECS::Component
{
struct Descriptor final
{
  static constexpr auto invalid_bit_index = 0xffff;
  static constexpr auto processing_bit_index = invalid_bit_index - 1;

  using Constructor = FuncPtr<void( Ptr<byte> ) noexcept>;
  using Destructor = FuncPtr<void( Ptr<byte> ) noexcept>;
  using Copy = FuncPtr<void( Ptr<byte>, Ptr<const byte> ) noexcept>;
  using Move = FuncPtr<void( Ptr<byte>, Ptr<byte> ) noexcept>;
  using Reflect = FuncPtr<rttr::instance( Ptr<byte> )noexcept>;
  using GenerateKey = Type::Shared::GenerateKey;

  Constructor m_constructor { nullptr };
  Destructor m_destructor { nullptr };
  Copy m_copy { nullptr };
  Move m_move { nullptr };
  Reflect m_reflect { nullptr };
  GenerateKey m_generate_key { nullptr };
  mutable Type::Shared::Key m_shared_key;

  ID m_id;
  std::string_view m_name;
  mutable u16 m_bit_index;
  u16 m_size;
  Type::ID m_type;
  bool m_global_scoped;
  bool m_build_filter;
  bool m_exclusive;
};

namespace Details
{
template <Concepts::IsComponent C>
inline consteval Descriptor CreateDescriptor() noexcept;

template <typename C>
inline constexpr auto desc = CreateDescriptor<C>();
}

template <typename C>
inline constexpr auto &info = Details::desc<Raw<C>>;

inline constexpr bool CompareDescriptor( Ptr<const Descriptor> lhs, Ptr<const Descriptor> rhs ) noexcept;

constexpr Type::Shared::Key GenerateSharedKey( Archetype::ID id, const Descriptor &desc, Ptr<const byte> data = nullptr ) noexcept;

template <Concepts::IsComponent... Cs>
inline constexpr Signature GenerateSignature() noexcept;
}

namespace Xivi::ECS
{
using SharedKeySpan = span<const Component::Type::Shared::Key>;
}

namespace Xivi::ECS
{
namespace Details
{
template <typename A, typename B>
struct SmallerComponent
{
  static constexpr bool value = CompareDescriptor( &Component::info<A>, &Component::info<B> );
};
}

template <typename Tuple>
using SharedOnlyTuple =
std::invoke_result_t<decltype( []<typename... Ts>( tuplet::tuple<Ts...> * )
  -> Traits::Tuple::CatType<std::conditional_t<
                               Component::info<Ts>.m_type == Component::Type::ID::Shared, tuplet::tuple<Ts>, tuplet::tuple<>
  >...>
{} ), Tuple * > ;

template <typename Tuple>
using DataOnlyTuple =
std::invoke_result_t<decltype( []<typename... Ts>( tuplet::tuple<Ts...> * )
  -> Traits::Tuple::CatType<std::conditional_t<
                               Component::info<Ts>.m_type == Component::Type::ID::Data, tuplet::tuple<Ts>, tuplet::tuple<>
  >...>
{} ), Tuple * > ;

template <typename Tuple>
using SortComponentsTuple = Traits::Tuple::SortType<Details::SmallerComponent, Tuple>;
}