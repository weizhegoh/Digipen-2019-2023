/*****************************************************************
*\file         Vector2D.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include <array>
#include <cassert>

namespace CMath
{
template <typename T>
struct Vec2
{
    // * methods
  __inline static constexpr std::uint32_t size() noexcept
  {
    return 2;
  }
  __inline const T *get() const noexcept
  {
    return arr.data();
  }
  __inline Vec2<T> &set( const T *rhs ) noexcept
  {
    XV_CORE_ASSERT_MSG( rhs != nullptr, "[Vector2D] Storing into nullptr" );
    x = rhs[0];
    y = rhs[1];
    return *this;
  }
  __inline Vec2<T> &set( const T pX, const T pY ) noexcept
  {
    x = pX;
    y = pY;
    return *this;
  }
  __inline void store( float *const ptr ) const noexcept
  {
    XV_CORE_ASSERT_MSG( ptr != nullptr, "[Vector2D] Storing into nullptr" );
    ptr[0] = arr[0];
    ptr[1] = arr[1];
  }

  // * Constructors
  __inline Vec2( const T &scalar = T() ) noexcept :
    x( scalar ), y( scalar )
  {
  }
  __inline Vec2( const T *datas ) noexcept :
    x( datas[0] ), x( datas[1] )
  {
  }
  __inline Vec2( const T x_, const T y_ ) noexcept :
    x( x_ ), y( y_ )
  {
  }

  // * Operators
  __inline T &operator[]( const int index ) noexcept
  {
    XV_CORE_ASSERT_MSG( index >= 0 && index < 2, "[Vector2D] Out of range" );
    return arr[index];
  }
  __inline const T &operator[]( const int index ) const noexcept
  {
    XV_CORE_ASSERT_MSG( index >= 0 && index < 2, "[Vector2D] Out of range" );
    return arr[index];
  }

  __inline operator T *( ) noexcept
  {
    return arr.data();
  }
  __inline operator const T *( ) const noexcept
  {
    return arr.data();
  }

  friend __inline Vec2<T> operator*( const T lhs_scalar, const Vec2<T> &rhs ) noexcept
  {
    return Vec2<T>{ lhs_scalar *rhs.x, lhs_scalar *rhs.y };
  }

  friend __inline Vec2<T> operator*( const Vec2<T> &lhs, const Vec2<T> &rhs ) noexcept
  {
    return Vec2<T>{ lhs.x *rhs.x, lhs.y *rhs.y };
  }

  friend __inline Vec2<T> &operator*=( Vec2<T> &lhs, const T rhs_scalar ) noexcept
  {
    lhs = lhs * rhs_scalar;
    return lhs;
  }

  friend __inline Vec2<T> &operator*=( Vec2<T> &lhs, const Vec2<T> &rhs ) noexcept
  {
    lhs = lhs * rhs;
    return lhs;
  }

  friend __inline Vec2<T> operator*( const Vec2<T> &lhs, const T rhs_scalar ) noexcept
  {
    return rhs_scalar * lhs;
  }

  friend __inline Vec2<T> operator/( const Vec2<T> &lhs, const T rhs_scalar ) noexcept
  {
    return Vec2<T>{ lhs.x / rhs_scalar, lhs.y / rhs_scalar };
  }

  friend __inline Vec2<T> operator/( const Vec2<T> &lhs, const Vec2<T> &rhs ) noexcept
  {
    return Vec2<T>{ lhs.x / rhs.x, lhs.y / rhs.y };
  }

  friend __inline Vec2<T> &operator/=( Vec2<T> &lhs, const T rhs_scalar ) noexcept
  {
    lhs = lhs / rhs_scalar;
    return lhs;
  }

  friend __inline Vec2<T> &operator/=( Vec2<T> &lhs, const Vec2<T> &rhs ) noexcept
  {
    lhs = lhs / rhs;
    return lhs;
  }

  friend __inline Vec2<T> operator+( const Vec2<T> &lhs, const Vec2<T> &rhs ) noexcept
  {
    return Vec2<T>{ lhs.x + rhs.x, lhs.y + rhs.y };
  }

  friend __inline Vec2<T> &operator+=( Vec2<T> &lhs, const Vec2<T> &rhs ) noexcept
  {
    lhs = lhs + rhs;
    return lhs;
  }

  friend __inline Vec2<T> operator-( const Vec2<T> &lhs, const Vec2<T> &rhs ) noexcept
  {
    return Vec2<T>{ lhs.x - rhs.x, lhs.y - rhs.y };
  }

  friend __inline Vec2<T> &operator-=( Vec2<T> &lhs, const Vec2<T> &rhs ) noexcept
  {
    lhs = lhs - rhs;
    return lhs;
  }

  friend __inline Vec2<T> operator-( const Vec2<T> &lhs ) noexcept
  {
    Vec2<T> v;
    v.x = -lhs.x;
    v.y = -lhs.y;
    return v;
  }

  // * Comparison
  friend __inline bool operator==( const Vec2<T> &lhs, const Vec2<T> &rhs ) noexcept
  {
    bool b { true };
    b &= lhs.x == rhs.x;
    b &= lhs.y == rhs.y;
    return b;
  }

  friend __inline bool operator!=( const Vec2<T> &lhs, const Vec2<T> &rhs ) noexcept
  {
    return !( lhs == rhs );
  }

  friend __inline std::ostream &operator<<( std::ostream &os, const Vec2<T> &vec ) noexcept
  {
    os << "[0]:" << vec[0] << ",\t[1]:" << vec[1];
    return os;
  }

  __inline T dot( const Vec2<T> &rhs ) noexcept
  {
    T result { 0 };
    for ( auto i = 0u; i < size(); ++i )
    {
      result += arr[i] * rhs.arr[i];
    }
    return result;
  }

  __inline T lengthSq() noexcept
  {
    return dot( *this );
  }

  __inline T length() noexcept
  {
    return std::sqrt( lengthSq() );
  }

  __inline Vec2<T> normalize() noexcept
  {
    return *this / length( *this );
  }

  __inline void setToZero() noexcept
  {
    for ( auto i = 0u; i < size(); ++i )
    {
      arr[i] = static_cast<T>( 0 );
    }
  }

  __inline void setAll( const float val ) noexcept
  {
    for ( auto i = 0u; i < size(); ++i )
    {
      arr[i] = static_cast<T>( val );
    }
  }

  using value_type = T;

  #pragma warning(disable : 4201)
          // * Data Structure
  union
  {
    struct
    {
      T x, y;
    };
    std::array<T, 2> arr;
  };
  #pragma warning(default : 4201)
};

template <typename T>
__inline auto dot( const Vec2<T> &lhs, const Vec2<T> &rhs ) noexcept -> Vec2<T>::value_type
{
  typename Vec2<T>::value_type result { 0 };
  for ( auto i = 0u; i < Vec2<T>::size(); ++i )
  {
    result += lhs.arr[i] * rhs.arr[i];
  }
  return result;
}

template <typename T>
__inline auto lengthSq( const Vec2<T> &v ) noexcept -> Vec2<T>::value_type
{
  return dot( v, v );
}

template <typename T>
__inline auto length( const Vec2<T> &v ) noexcept -> Vec2<T>::value_type
{
  return std::sqrt( lengthSq( v ) );
}

template <typename T>
__inline auto normalize( const Vec2<T> &v ) noexcept -> Vec2<T>
{
  return v / length( v );
}

// Creating a vector by getting the min of both vectors
// e.g. lhs = <1,6> and rhs = <2,4>, the result will be <1,4>
template <typename T>
__inline auto min( const Vec2<T> &lhs, const Vec2<T> &rhs ) noexcept -> Vec2<T>
{
  Vec2<T> result;
  for ( auto i = 0u; i < Vec2<T>::size(); ++i )
  {
    result[i] = std::min( lhs[i], rhs[i] );
  }
  return result;
}
// Creating a vector by getting the max of both vectors
// e.g. lhs = <1,6> and rhs = <2,4>, the result will be <2,6>
template <typename T>
__inline auto max( const Vec2<T> &lhs, const Vec2<T> &rhs ) noexcept -> Vec2<T>
{
  Vec2<T> result;
  for ( auto i = 0u; i < Vec2<T>::size(); ++i )
  {
    result[i] = std::max( lhs[i], rhs[i] );
  }
  return result;
}

template <typename T>
__inline auto distanceSq( const Vec2<T> &v1, const Vec2<T> &v2 ) noexcept -> Vec2<T>::value_type
{
  return lengthSq( v2 - v1 );
}

template <typename T>
__inline auto distance( const Vec2<T> &v1, const Vec2<T> &v2 ) noexcept -> Vec2<T>::value_type
{
  return length( v2 - v1 );
}

template <typename T>
__inline auto lerp( const Vec2<T> &start, const Vec2<T> &end, const float alphatime ) noexcept -> Vec2<T>
{
  Vec2<T> result;
  for ( auto i = 0u; i < Vec2<T>::size(); ++i )
  {
    result[i] = start[i] + ( end[i] - start[i] ) * alphatime;
  }
  return result;
}

template <typename T>
__inline auto negate( Vec2<T> &v ) noexcept -> void
{
  v = -v;
}

template <typename T>
__inline auto negated( const Vec2<T> &v ) noexcept -> Vec2<T>
{
  return -v;
}

template <typename T>
__inline auto setToZero( Vec2<T> &v ) noexcept -> void
{
  for ( auto i = 0u; i < Vec2<T>::size(); ++i )
  {
    v[i] = static_cast<T>( 0 );
  }
}

template <typename T>
__inline auto setAll( Vec2<T> &v, const T val ) noexcept -> void
{
  for ( auto i = 0u; i < Vec2<T>::size(); ++i )
  {
    v[i] = val;
  }
}

template <typename T>
__inline auto clamp( const Vec2<T> &v, const Vec2<T> &min, const Vec2<T> &max ) noexcept -> Vec2<T>
{
  return Vec2<T>{ Math::clamp( v.x, min.x, max.x ), std::clamp( v.y, min.y, max.y ) };
}
}