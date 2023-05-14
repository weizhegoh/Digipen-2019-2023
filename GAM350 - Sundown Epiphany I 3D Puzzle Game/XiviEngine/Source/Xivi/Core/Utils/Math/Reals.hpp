/*****************************************************************
*\file         Reals.hpp
*\author(s)    Lee Jun Jie

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi::Math
{
//a pointer of the given type
typedef float* RealPointer;
//a const pointer of the given type
typedef const float* ConstRealPointer;
//a reference of the given type
typedef float& RealRef;
//a const reference of the given type
typedef const float& ConstRealRef;

const unsigned cX = 0;
const unsigned cY = 1;
const unsigned cZ = 2;
const unsigned cW = 3;

//these cannot be constants
extern const float& cInfinite;

//Golden ratio!
const float cGoldenRatio = 1.6180339887498948482045868343656f;

float DebugEpsilon();
float PositiveMax();
float PositiveMin();
bool DebugIsZero(float val);
bool IsNegative(float number);
bool IsPositive(float number);
bool LessThan(float lhs, float rhs);
bool LessThanOrEqual(float lhs, float rhs);
bool GreaterThan(float lhs, float rhs);
bool GreaterThanOrEqual(float lhs, float rhs);
float Sqrt(float val);
float Rsqrt(float val);
float Sq(float sqrt);
float Pow(float base, float exp);
float Log(float val);
float Abs(float val);
int Abs(int val);
float FMod(float dividend, float divisor);
float GetSign(float val);
float Cos(float val);
float Sin(float val);
float Tan(float angle);
float ArcCos(float angle);
float ArcSin(float angle);
float ArcTan(float angle);
float ArcTan2(float y, float x);
float RadToDeg(float radians);
float DegToRad(float degrees);
float Round(float val);
float Ceil(float val);
float Floor(float val);
bool IsValid(float val);

template <typename T>
inline T Max(const T lhs, const T rhs)
{
  return lhs > rhs ? lhs : rhs;
}

template <typename T>
inline T Min(const T lhs, const T rhs) 
{
  return lhs > rhs ? rhs : lhs;
}

template <typename T>
inline T Clamp(const T x, const T xMin, const T xMax)
{
  return Max(xMin, Min(x, xMax));
}

template <typename T>
inline T Clamp(const T value) 
{
  return Clamp(value, T(0), T(1));
}

/// Clamps between min and max but it sets a bool saying whether or not a value was clamped.
template <typename T>
inline T DebugClamp(const T x, const T xMin, const T xMax, bool& wasClamped)
{
  wasClamped = true;
  if(x < xMin)  
    return xMin;
  if(x > xMax)
    return xMax;
  wasClamped = false;
  return x;
}

template <typename T>
inline T ClampIfClose(const T x, const T xMin, const T xMax, const T epsilon)
{
  float value = x < xMin && x > (xMin - epsilon) ? xMin : x;
  value = value > xMax && value < (xMax + epsilon) ? xMax : value;
  return value;
}

template <typename T>
inline float InverseLerp(const T x, const T start, const T end)
{
  if(end == start)
  {
    return 1.0f;
  }

  return (x - start) / (end - start);
}

template <typename T>
inline float InverseLerpClamped(const T x, const T start, const T end)
{
  return Clamp(InverseLerp(x, start, end));
}

///Checks to see if x is within the interval of [xMin, xMax]
template <typename T>
inline bool InRange(const T x, const T xMin, const T xMax)
{
    return ((xMin <= x) && (x <= xMax));
}

///Checks to see if x is within the interval of (xMin, xMax)
template <typename T>
inline bool InBounds(const T x, const T xMin, const T xMax)
{
    return ((xMin < x) && (x < xMax));
}

template <typename T>
inline T Wrap(const T x, const T xMin, const T xMax)
{
    return (x < xMin) ? (x + (xMax - xMin)) : 
           ((x > xMax) ? (x - (xMax - xMin)) : x);
}

template <typename T>
inline void Swap(T& a, T& b)
{
  T temp(a);
  a = b;
  b = temp;
}

template <typename Data, typename T>
inline Data Lerp(const Data& start, const Data& end, T interpolationValue)
{
  return (T(1.0) - interpolationValue) * start + interpolationValue * end;
}

}// namespace Math
