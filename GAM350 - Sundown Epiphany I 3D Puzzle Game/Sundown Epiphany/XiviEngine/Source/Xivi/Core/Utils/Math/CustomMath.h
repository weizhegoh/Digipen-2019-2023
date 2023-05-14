/*****************************************************************
*\file         CustomMath.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi::Math
{
static inline constexpr float s_PI = 3.14159265358f;
static inline constexpr float s_TORAD = s_PI / 180;
static inline constexpr float s_TODEG = 180 / s_PI;
static inline constexpr float s_EPSILON = 0.0001f;

template <typename T>
concept IsIntegral = std::is_integral<T>::value;

template <typename T>
concept IsFloatingPt = std::is_floating_point<T>::value;

// Random function for integral from min to max
template <IsIntegral integer>
__inline integer rand( const integer min, const integer max ) noexcept
{
  return min + ( std::rand() % ( max - min + 1 ) );
}

// Random function for floating point from min to max
template <IsFloatingPt floating>
__inline floating rand( const floating min, const floating max ) noexcept
{
  return min + static_cast<floating>( std::rand() ) / ( static_cast <floating> ( RAND_MAX / ( max - min ) ) );
}

template <typename T>
__inline T clamp( const T val, T min, T max )
{
  if ( min > max ) [[unlikely]]
    std::swap( min, max );
  return std::max( min, std::min( val, max ) );
}

// Factorial function
__inline constexpr int factorial( const int n )
{
  if ( n > 1 ) [[likely]]
  {
    return n * factorial( n - 1 );
  }
  else [[unlikely]]
  {
    return 1;
  }
}
}