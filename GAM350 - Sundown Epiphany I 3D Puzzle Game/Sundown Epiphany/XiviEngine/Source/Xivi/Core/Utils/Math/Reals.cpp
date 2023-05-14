/*****************************************************************
*\file         Reals.cpp
*\author(s)    Lee Jun Jie

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#include "Reals.hpp"

#include <cmath>
#include <limits>
#include <cstdio>
#include <cstdlib>
#include <cfloat>

namespace Xivi::Math
{
namespace
{
const float cTemp = -0.0f;
const unsigned cSignBit = *reinterpret_cast<unsigned const *>( &cTemp );
const float cNegative = -1.0f;
const float cPositive = 1.0f;

float gZeroForInf = 0.0f;
float gInfinite = 1.0f / gZeroForInf;
}

const float &cInfinite = gInfinite;

float DebugEpsilon()
{
  return 0.00000001f;
}

float PositiveMax()
{
  return FLT_MAX;
}

float PositiveMin()
{
  return FLT_MIN;
}

//bool Equal(float lhs, float rhs)
//{
//  return Abs(lhs - rhs) <= Epsilon() * (Abs(lhs) + abs(rhs) + 1.0f);
//}
//
//bool NotEqual(float lhs, float rhs)
//{
//  return !Equal(lhs, rhs);
//}

bool DebugIsZero( float val )
{
  return Abs( val ) <= DebugEpsilon();
}

bool IsNegative( float number )
{
  return GetSign( number ) == cNegative;
}

bool IsPositive( float number )
{
  return GetSign( number ) == cPositive;
}

bool LessThan( float lhs, float rhs )
{
  return lhs < rhs;
}

bool LessThanOrEqual( float lhs, float rhs )
{
  return lhs <= rhs;
}

bool GreaterThan( float lhs, float rhs )
{
  return lhs > rhs;
}

bool GreaterThanOrEqual( float lhs, float rhs )
{
  return lhs >= rhs;
}

float Sqrt( float val )
{
  return std::sqrt( val );
}

float Rsqrt( float val )
{
  return 1.0f / std::sqrt( val );
}

float Sq( float sqrt )
{
  return sqrt * sqrt;
}

float Pow( float base, float exp )
{
  return std::pow( base, exp );
}

float Log( float val )
{
  return std::log( val );
}

float Abs( float val )
{
  return std::abs( val );
}

int Abs( int val )
{
  return std::abs( val );
}

float FMod( float dividend, float divisor )
{
  return fmod( dividend, divisor );
}

float GetSign( float val )
{
  return ( *reinterpret_cast<unsigned *>( &val ) & cSignBit ) != 0 ? cNegative
    : cPositive;
  //return lhs >= 0.0f ? 1.0f : -1.0f;
}

float Cos( float val )
{
  return std::cos( val );
}

float Sin( float val )
{
  return std::sin( val );
}

float Tan( float angle )
{
  return std::tan( angle );
}

float ArcCos( float angle )
{
  angle = ClampIfClose( angle, -1.0f, 1.0f, 0.00001f );
  return std::acos( angle );
}

float ArcSin( float angle )
{
  angle = ClampIfClose( angle, -1.0f, 1.0f, 0.00001f );
  return std::asin( angle );
}

float ArcTan( float angle )
{
  angle = ClampIfClose( angle, -1.0f, 1.0f, 0.00001f );
  return std::atan( angle );
}

float ArcTan2( float y, float x )
{
  return std::atan2( y, x );
}

float RadToDeg( float radians )
{
  return ( 180.0f / s_PI ) * radians;
}

float DegToRad( float degrees )
{
  return ( s_PI / 180.0f ) * degrees;
}

bool IsValid( float val )
{
  #ifdef _MSC_VER
  return _finite( val ) != 0;
  #else
  return val == val;
  #endif
}

float Round( float val )
{
  return std::floor( val + 0.5f );
}

float Ceil( float val )
{
  return std::ceil( val );
}

float Floor( float val )
{
  return std::floor( val );
}
}