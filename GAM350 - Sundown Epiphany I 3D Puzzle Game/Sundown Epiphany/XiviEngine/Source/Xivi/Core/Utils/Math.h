/*****************************************************************
*\file         Math.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include <rttr/registration.h>
#include <rttr/type.h>
#include <Types/Base.h>
#include <Utils/Assert.h>
#include "Math/CustomMath.h"
#include "Math/Scalar.h"

namespace Xivi::Math
{
#include "Math/SIMD.h"
#include "Math/Matrix.h"
#include "Math/Vector2D.h"

using namespace SIMD;
using namespace CMath;

struct Vec3i
{
  i32 x, y, z;
  constexpr Vec3i() = default;
  constexpr Vec3i( i32 i, i32 j, i32 k ) : x( i ), y( j ), z( k )
  {}

  Vec3i( const Math::SIMD::Vec3 &v ) :
    x( static_cast<i32>( std::nearbyint( v.getX() ) ) ),
    y( static_cast<i32>( std::nearbyint( v.getY() ) ) ),
    z( static_cast<i32>( std::nearbyint( v.getZ() ) ) )
  {}

  auto operator<=>( const Vec3i & ) const = default;
  Vec3i operator+=( const Vec3i &rhs )
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
  Vec3i operator+( const Vec3i &rhs ) const
  {
    Vec3i tmp( *this );
    return tmp += rhs;
  }

  Vec3i operator-=( const Vec3i &rhs )
  {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }
  Vec3i operator-( const Vec3i &rhs ) const
  {
    Vec3i tmp( *this );
    return tmp -= rhs;
  }

  Vec3i operator-()
  {
    return { -x,-y,-z };
  }
};
}

// Good to have alias over types so you can swap anytime
namespace Xivi
{
using Vec2f = Math::CMath::Vec2<f32>;
using Vec3f = Math::SIMD::Vec3;
using Vec4f = Math::SIMD::Vec4;
using Mat3x3 = Math::SIMD::Mat3x3;
using Mat4x4 = Math::SIMD::Mat4x4;
using Vec3i = Math::Vec3i;

static inline auto vec2_zero = Vec2f { 0,0 };
static inline auto vec2_one = Vec2f { 1,1 };
static inline auto vec2_x = Vec2f { 1,0 };
static inline auto vec2_y = Vec2f { 0,1 };
static inline auto vec2_min = Vec2f { scalar_min, scalar_min };
static inline auto vec2_max = Vec2f { scalar_max, scalar_max };

static inline auto vec3_zero = Vec3f { 0,0,0 };
static inline auto vec3_one = Vec3f { 1,1,1 };
static inline auto vec3_x = Vec3f { 1,0,0 };
static inline auto vec3_y = Vec3f { 0,1,0 };
static inline auto vec3_z = Vec3f { 0,0,1 };
static inline auto vec3_min = Vec3f { scalar_min, scalar_min, scalar_min };
static inline auto vec3_max = Vec3f { scalar_max, scalar_max, scalar_max };

static inline auto vec4_zero = Vec4f { 0,0,0,0 };
static inline auto vec4_one = Vec4f { 1,1,1,1 };
static inline auto vec4_x = Vec4f { 1,0,0,0 };
static inline auto vec4_y = Vec4f { 0,1,0,0 };
static inline auto vec4_z = Vec4f { 0,0,1,0 };
static inline auto vec4_w = Vec4f { 0,0,0,1 };
static inline auto vec4_min = Vec4f { scalar_min, scalar_min, scalar_min, scalar_min };
static inline auto vec4_max = Vec4f { scalar_max, scalar_max, scalar_max, scalar_max };
}

#include "../Utils/Math/MathFunctions.hpp"

namespace Xivi::Math::Reflect
{
RTTR_REGISTRATION
{
// SIMD Vec3
rttr::registration::class_<SIMD::Vec3>( "SIMDVector3" )
.property( "x", &SIMD::Vec3::getX, &SIMD::Vec3::setX )
.property( "y", &SIMD::Vec3::getY, &SIMD::Vec3::setY )
.property( "z", &SIMD::Vec3::getZ, &SIMD::Vec3::setZ );

// SIMD Vec4
rttr::registration::class_<SIMD::Vec4>( "SIMDVector4" )
.property( "x", &SIMD::Vec4::getX, &SIMD::Vec4::setX )
.property( "y", &SIMD::Vec4::getY, &SIMD::Vec4::setY )
.property( "z", &SIMD::Vec4::getZ, &SIMD::Vec4::setZ )
.property( "w", &SIMD::Vec4::getW, &SIMD::Vec4::setW );

// SIMD Matrix 3x3
rttr::registration::class_<SIMD::Mat3x3>( "SIMDMatrix3x3" )
.property( "row1", &SIMD::Mat3x3::getRow1, &SIMD::Mat3x3::setRow1 )
.property( "row2", &SIMD::Mat3x3::getRow2, &SIMD::Mat3x3::setRow2 )
.property( "row3", &SIMD::Mat3x3::getRow3, &SIMD::Mat3x3::setRow3 );

// SIMD Matrix 4x4
rttr::registration::class_<SIMD::Mat4x4>( "SIMDMatrix4x4" )
.property( "row1", &SIMD::Mat4x4::getRow1, &SIMD::Mat4x4::setRow1 )
.property( "row2", &SIMD::Mat4x4::getRow2, &SIMD::Mat4x4::setRow2 )
.property( "row3", &SIMD::Mat4x4::getRow3, &SIMD::Mat4x4::setRow3 )
.property( "row4", &SIMD::Mat4x4::getRow4, &SIMD::Mat4x4::setRow4 );

// GLM Vec3
rttr::registration::class_<glm::vec3>( "Vector3" )
.property( "x", &glm::vec3::x )
.property( "y", &glm::vec3::y )
.property( "z", &glm::vec3::z );

// GLM Vec4
rttr::registration::class_<glm::vec4>( "Vector4" )
.property( "x", &glm::vec4::x )
.property( "y", &glm::vec4::y )
.property( "z", &glm::vec4::z )
.property( "w", &glm::vec4::w );

rttr::registration::class_<Xivi::Vec3i>( "Vector3_i" )
.property( "x", &Xivi::Vec3i::x )
.property( "z", &Xivi::Vec3i::z )
.property( "y", &Xivi::Vec3i::y );
}
}