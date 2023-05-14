#pragma once
#include "../Utils/XXH3.h"
#include <random>
#include <chrono>
#include <thread>

namespace Xivi::Details
{
struct UID
{
  i64 m_time;
  u64 m_id;
  u64 m_rand;
};

inline auto Generate() noexcept
{
  static std::mt19937_64 gen64;

  return Hash::XXH3_64( UID {
      std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() ),
      Hash::XXH3_64( std::this_thread::get_id() ),
      gen64()
                     } );
}
}

namespace Xivi
{
template <typename Tag = void>
struct GUID
{
  constexpr GUID() noexcept = default;
  constexpr GUID( std::string_view string ) noexcept : m_value( Hash::XXH3_64( string ) )
  {}
  constexpr GUID( const GUID & ) noexcept = default;
  constexpr GUID( unsigned long long value ) : m_value( value )
  {}

  constexpr auto operator<=>( const GUID & ) const = default;
  constexpr void SetFromString( std::string_view string ) noexcept
  {
    m_value = Hash::XXH3_64( string );
  }
  void Reset() noexcept
  {
    if ( std::is_constant_evaluated() ) SetNull();
    else m_value = Details::Generate();
  }
  constexpr bool Valid() const noexcept
  {
    return static_cast<bool>( m_value );
  }
  constexpr void SetNull() noexcept
  {
    m_value = 0;
  }

  friend std::ostream &operator<<( std::ostream &os, const GUID &guid )
  {
    return os << guid.m_value;
  }
  u64 m_value { 0 };
};
}

template <typename Tag>
struct std::hash<typename Xivi::GUID<Tag>>
{
  auto operator()( const typename Xivi::GUID<Tag> id ) const
  {
    return id.m_value;
  }
};