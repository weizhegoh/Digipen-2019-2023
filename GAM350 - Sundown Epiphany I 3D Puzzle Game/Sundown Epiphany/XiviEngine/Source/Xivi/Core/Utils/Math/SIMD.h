/*****************************************************************
*\file         SIMD.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
// SIMD
#include <immintrin.h>
#include <cassert>
namespace SIMD
{
#define MakeShuffleMask(x, y, z, w)     (x | (y<<2) | (z<<4) | (w<<6))

#define VecSwizzle(vec, x, y, z, w)     _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(vec), MakeShuffleMask(x,y,z,w)))
#define VecSwizzle1(vec, x)             VecSwizzle(vec, x, x, x, x)

static const __m128 s_SIGNMASK = _mm_castsi128_ps( _mm_set1_epi32( 0x80000000 ) );
static const __m128 s_ABSMASK = _mm_castsi128_ps( _mm_set1_epi32( ~( 1 << 31 ) ) );

//--------------------------Vec3---------------------------------//
struct Vec3
{
    // Constructors
  inline Vec3() noexcept
  {
    m_SIMD = _mm_set_ps( 0, 0, 0, 0 );
  }
  inline explicit Vec3( const float *ptr ) noexcept
  {
    XV_CORE_ASSERT_MSG( ptr != nullptr, "[SIMD] Passed in nullptr" );
    m_SIMD = _mm_set_ps( 0.0f, ptr[2], ptr[1], ptr[0] );
  }
  inline explicit Vec3( const float index0, const float index1, const float index2 ) noexcept
  {
    m_SIMD = _mm_set_ps( 0, index2, index1, index0 );
  }
  inline Vec3( const Vec3 &v ) noexcept
  {
    m_SIMD = v.m_SIMD;
  }
  inline Vec3( const __m128 &simd ) noexcept
  {
    m_SIMD = simd;
  }
  inline Vec3( const float val ) noexcept
  {
    m_SIMD = _mm_set_ps( 0, val, val, val );
  }

  // Store it into ptr
  inline void store3( float *const ptr ) const noexcept
  {
    XV_CORE_ASSERT_MSG( ptr != nullptr, "[SIMD] Storing into nullptr" );
    _mm_store_ss( &ptr[0], m_SIMD );
    _mm_store_ss( &ptr[1], _mm_shuffle_ps( m_SIMD, m_SIMD, _MM_SHUFFLE( 1, 1, 1, 1 ) ) );
    _mm_store_ss( &ptr[2], _mm_shuffle_ps( m_SIMD, m_SIMD, _MM_SHUFFLE( 2, 2, 2, 2 ) ) );
  }

  inline void store4( float *const ptr ) const noexcept
  {
    XV_CORE_ASSERT_MSG( ptr != nullptr, "[SIMD] Storing into nullptr" );
    _mm_store_ps( ptr, m_SIMD );
  }

  // Set the vector
  inline void set( const float index0, const float index1, const float index2 ) noexcept
  {
    m_SIMD = _mm_set_ps( 0.0f, index2, index1, index0 );
  }

  const float *get() const
  {
    return std::bit_cast<float *>( &m_SIMD );
  }

  // Set all to zero
  inline void setToZero() noexcept
  {
    m_SIMD = _mm_setzero_ps();
  }

  // Set all to val
  inline void setAll( const float val ) noexcept
  {
    m_SIMD = _mm_set_ps( 0, val, val, val );
  }

  inline float operator[]( const size_t i ) const noexcept
  {
    XV_CORE_ASSERT_MSG( i >= 0 && i < 3, "[SIMD] Out of range" );
    return std::bit_cast<float *>( &m_SIMD )[i];
  }
  inline float &operator[]( const size_t i ) noexcept
  {
    XV_CORE_ASSERT_MSG( i >= 0 && i < 3, "[SIMD] Out of range" );
    return std::bit_cast<float *>( &m_SIMD )[i];
  }

  inline operator float *( ) noexcept
  {
    return std::bit_cast<float *>( &m_SIMD );
  }
  inline operator const float *( ) const noexcept
  {
    return std::bit_cast<float *>( &m_SIMD );
  }

  Vec3 operator=( const Vec3 &rhs ) noexcept
  {
    if ( this != &rhs )
    {
      m_SIMD = rhs.m_SIMD;
    }
    return *this;
  }

  friend inline Vec3 operator+( const Vec3 &lhs, const Vec3 &rhs ) noexcept
  {
    return _mm_add_ps( lhs.m_SIMD, rhs.m_SIMD );
  }
  friend inline Vec3 operator-( const Vec3 &lhs, const Vec3 &rhs ) noexcept
  {
    return _mm_sub_ps( lhs.m_SIMD, rhs.m_SIMD );
  }
  friend inline Vec3 operator*( const Vec3 &lhs, const Vec3 &rhs ) noexcept
  {
    return _mm_mul_ps( lhs.m_SIMD, rhs.m_SIMD );
  }
  friend inline Vec3 operator/( const Vec3 &lhs, const Vec3 &rhs ) noexcept
  {
    return _mm_div_ps( lhs.m_SIMD, rhs.m_SIMD );
  }

  friend inline Vec3 &operator+=( Vec3 &lhs, const Vec3 &rhs ) noexcept
  {
    lhs.m_SIMD = _mm_add_ps( lhs.m_SIMD, rhs.m_SIMD );
    return lhs;
  }
  friend inline Vec3 &operator-=( Vec3 &lhs, const Vec3 &rhs ) noexcept
  {
    lhs.m_SIMD = _mm_sub_ps( lhs.m_SIMD, rhs.m_SIMD );
    return lhs;
  }
  friend inline Vec3 &operator*=( Vec3 &lhs, const Vec3 &rhs ) noexcept
  {
    lhs.m_SIMD = _mm_mul_ps( lhs.m_SIMD, rhs.m_SIMD );
    return lhs;
  }
  friend inline Vec3 &operator/=( Vec3 &lhs, const Vec3 &rhs ) noexcept
  {
    lhs.m_SIMD = _mm_div_ps( lhs.m_SIMD, rhs.m_SIMD );
    return lhs;
  }

  friend inline Vec3 operator*( const Vec3 &lhs, const float rhs ) noexcept
  {
    return _mm_mul_ps( lhs.m_SIMD, _mm_set1_ps( rhs ) );
  }
  friend inline Vec3 operator*( const float lhs, const Vec3 &rhs ) noexcept
  {
    return rhs * lhs;
  }
  friend inline Vec3 operator/( const Vec3 &lhs, const float rhs ) noexcept
  {
    return _mm_div_ps( lhs.m_SIMD, _mm_set1_ps( rhs ) );
  }

  friend inline Vec3 &operator*=( Vec3 &lhs, const float rhs ) noexcept
  {
    lhs.m_SIMD = _mm_mul_ps( lhs.m_SIMD, _mm_set1_ps( rhs ) );
    return lhs;
  }
  friend inline Vec3 &operator/=( Vec3 &lhs, const float rhs ) noexcept
  {
    lhs.m_SIMD = _mm_div_ps( lhs.m_SIMD, _mm_set1_ps( rhs ) );
    return lhs;
  }

  friend inline Vec3 operator-( const Vec3 &lhs ) noexcept
  {
    return _mm_xor_ps( lhs.m_SIMD, s_SIGNMASK );
  }

  friend inline bool operator==( const Vec3 &lhs, const Vec3 &rhs ) noexcept
  {
    return ( ( ( _mm_movemask_ps( _mm_cmpneq_ps( lhs.m_SIMD, rhs.m_SIMD ) ) ) & 0x7 ) == 0 );
  }

  friend inline bool operator!=( const Vec3 &lhs, const Vec3 &rhs ) noexcept
  {
    return ( ( ( _mm_movemask_ps( _mm_cmpneq_ps( lhs.m_SIMD, rhs.m_SIMD ) ) ) & 0x7 ) != 0 );
  }

  friend inline bool operator<( const Vec3 &lhs, const Vec3 &rhs ) noexcept
  {
    return ( ( _mm_movemask_ps( _mm_cmpge_ps( lhs.m_SIMD, rhs.m_SIMD ) ) & 0x7 ) == 0 );
  }

  friend inline bool operator<=( const Vec3 &lhs, const Vec3 &rhs ) noexcept
  {
    return ( ( _mm_movemask_ps( _mm_cmpgt_ps( lhs.m_SIMD, rhs.m_SIMD ) ) & 0x7 ) == 0 );
  }

  friend inline bool operator>( const Vec3 &lhs, const Vec3 &rhs ) noexcept
  {
    return ( ( _mm_movemask_ps( _mm_cmple_ps( lhs.m_SIMD, rhs.m_SIMD ) ) & 0x7 ) == 0 );
  }

  friend inline bool operator>=( const Vec3 &lhs, const Vec3 &rhs ) noexcept
  {
    return ( ( _mm_movemask_ps( _mm_cmplt_ps( lhs.m_SIMD, rhs.m_SIMD ) ) & 0x7 ) == 0 );
  }

  friend std::ostream &operator<<( std::ostream &os, const Vec3 &lhs ) noexcept
  {
    os << "[0]:" << lhs[0] << ",\t[1]:" << lhs[1] << ",\t[2]:" << lhs[2];
    return os;
  }

  inline Vec3 abs() const noexcept
  {
    return Vec3( _mm_and_ps( m_SIMD, s_ABSMASK ) );
  }

  inline Vec3 normalize() const noexcept
  {
    return _mm_div_ps( m_SIMD, _mm_sqrt_ps( _mm_dp_ps( m_SIMD, m_SIMD, 0x77 ) ) );
  }

  inline float length() const noexcept
  {
    return _mm_cvtss_f32( _mm_sqrt_ss( _mm_dp_ps( m_SIMD, m_SIMD, 0x71 ) ) );
  }

  inline float lengthSq() const noexcept
  {
    return dot( *this );
  }

  inline float sum() const noexcept
  {
    return std::bit_cast<float *>( &m_SIMD )[0] + std::bit_cast<float *>( &m_SIMD )[1] + std::bit_cast<float *>( &m_SIMD )[2];
  }

  inline float dot( const Vec3 &rhs ) const noexcept
  {
    return _mm_cvtss_f32( _mm_dp_ps( m_SIMD, rhs.m_SIMD, 0x71 ) );
  }

  inline Vec3 cross( const Vec3 &rhs ) const noexcept
  {
    return _mm_sub_ps(
      _mm_mul_ps(
      _mm_shuffle_ps( m_SIMD, m_SIMD, _MM_SHUFFLE( 3, 0, 2, 1 ) ),
      _mm_shuffle_ps( rhs.m_SIMD, rhs.m_SIMD, _MM_SHUFFLE( 3, 1, 0, 2 ) )
    ),
      _mm_mul_ps(
      _mm_shuffle_ps( m_SIMD, m_SIMD, _MM_SHUFFLE( 3, 1, 0, 2 ) ),
      _mm_shuffle_ps( rhs.m_SIMD, rhs.m_SIMD, _MM_SHUFFLE( 3, 0, 2, 1 ) )
    )
    );
  }

  _inline float getX() const noexcept
  {
    return _mm_cvtss_f32( m_SIMD );
  }

  _inline float getY() const noexcept
  {
    return _mm_cvtss_f32( _mm_shuffle_ps( m_SIMD, m_SIMD, _MM_SHUFFLE( 1, 1, 1, 1 ) ) );
  }

  _inline float getZ() const noexcept
  {
    return _mm_cvtss_f32( _mm_shuffle_ps( m_SIMD, m_SIMD, _MM_SHUFFLE( 2, 2, 2, 2 ) ) );
  }

  inline void setX( const float x ) noexcept
  {
    m_SIMD = _mm_move_ss( m_SIMD, _mm_set_ss( x ) );
  }

  inline void setY( const float y ) noexcept
  {
    __m128 tmp = _mm_move_ss( m_SIMD, _mm_set_ss( y ) );
    tmp = _mm_shuffle_ps( tmp, tmp, _MM_SHUFFLE( 3, 2, 0, 0 ) );
    m_SIMD = _mm_move_ss( tmp, m_SIMD );
  }

  inline void setZ( const float z ) noexcept
  {
    __m128 tmp = _mm_move_ss( m_SIMD, _mm_set_ss( z ) );
    tmp = _mm_shuffle_ps( tmp, tmp, _MM_SHUFFLE( 3, 0, 1, 0 ) );
    m_SIMD = _mm_move_ss( tmp, m_SIMD );
  }

  __m128 m_SIMD;
};

inline Vec3 cross( const Vec3 &lhs, const Vec3 &rhs ) noexcept
{
  return _mm_sub_ps(
    _mm_mul_ps(
    _mm_shuffle_ps( lhs.m_SIMD, lhs.m_SIMD, _MM_SHUFFLE( 3, 0, 2, 1 ) ),
    _mm_shuffle_ps( rhs.m_SIMD, rhs.m_SIMD, _MM_SHUFFLE( 3, 1, 0, 2 ) )
  ),
    _mm_mul_ps(
    _mm_shuffle_ps( lhs.m_SIMD, lhs.m_SIMD, _MM_SHUFFLE( 3, 1, 0, 2 ) ),
    _mm_shuffle_ps( rhs.m_SIMD, rhs.m_SIMD, _MM_SHUFFLE( 3, 0, 2, 1 ) )
  )
  );
}

//--------------------------Vec4---------------------------------//
struct Vec4
{
    // Constructors
  inline Vec4() noexcept
  {
    m_SIMD = _mm_set_ps( 0, 0, 0, 0 );
  }
  inline explicit Vec4( const float *ptr ) noexcept
  {
    XV_CORE_ASSERT_MSG( ptr != nullptr, "[SIMD] Passed in nullptr" );
    m_SIMD = _mm_load_ps( ptr );
    //m_SIMD = _mm_set_ps( ptr[3], ptr[2], ptr[1], ptr[0] );
  }
  inline explicit Vec4( const float index0, const float index1, const float index2, const float index3 ) noexcept
  {
    m_SIMD = _mm_set_ps( index3, index2, index1, index0 );
  }
  inline Vec4( const Vec4 &v ) noexcept
  {
    m_SIMD = v.m_SIMD;
  }
  inline Vec4( const __m128 &simd ) noexcept
  {
    m_SIMD = simd;
  }
  inline Vec4( const float val ) noexcept
  {
    m_SIMD = _mm_set_ps( val, val, val, val );
  }

  // Set vector into ptr
  inline void store( float *const ptr ) const noexcept
  {
    XV_CORE_ASSERT_MSG( ptr != nullptr, "[SIMD] Storing into nullptr" );
    _mm_store_ps( ptr, m_SIMD );
  }

  // Set the vector
  inline void set( const float index0, const float index1, const float index2, const float index3 ) noexcept
  {
    m_SIMD = _mm_set_ps( index3, index2, index1, index0 );
  }

  const float *get() const
  {
    return std::bit_cast<float *>( &m_SIMD );
  }

  // Set all to zero
  inline void setToZero() noexcept
  {
    m_SIMD = _mm_setzero_ps();
  }

  // Set all to val
  inline void setAll( const float val ) noexcept
  {
    m_SIMD = _mm_set_ps( val, val, val, val );
  }

  inline float operator[]( const size_t i ) const noexcept
  {
    XV_CORE_ASSERT_MSG( i >= 0 && i < 4, "[SIMD] Out of range" );
    return std::bit_cast<float *>( &m_SIMD )[i];
  }
  inline float &operator[]( const size_t i ) noexcept
  {
    XV_CORE_ASSERT_MSG( i >= 0 && i < 4, "[SIMD] Out of range" );
    return std::bit_cast<float *>( &m_SIMD )[i];
  }

  inline operator float *( ) noexcept
  {
    return std::bit_cast<float *>( &m_SIMD );
  }
  inline operator const float *( ) const noexcept
  {
    return std::bit_cast<float *>( &m_SIMD );
  }

  Vec4 operator=( const Vec4 &rhs ) noexcept
  {
    if ( this != &rhs )
    {
      m_SIMD = rhs.m_SIMD;
    }
    return *this;
  }

  friend inline Vec4 operator+( const Vec4 &lhs, const Vec4 &rhs ) noexcept
  {
    return _mm_add_ps( lhs.m_SIMD, rhs.m_SIMD );
  }
  friend inline Vec4 operator-( const Vec4 &lhs, const Vec4 &rhs ) noexcept
  {
    return _mm_sub_ps( lhs.m_SIMD, rhs.m_SIMD );
  }
  friend inline Vec4 operator*( const Vec4 &lhs, const Vec4 &rhs ) noexcept
  {
    return _mm_mul_ps( lhs.m_SIMD, rhs.m_SIMD );
  }
  friend inline Vec4 operator/( const Vec4 &lhs, const Vec4 &rhs ) noexcept
  {
    return _mm_div_ps( lhs.m_SIMD, rhs.m_SIMD );
  }

  friend inline Vec4 &operator+=( Vec4 &lhs, const Vec4 &rhs ) noexcept
  {
    lhs.m_SIMD = _mm_add_ps( lhs.m_SIMD, rhs.m_SIMD );
    return lhs;
  }
  friend inline Vec4 &operator-=( Vec4 &lhs, const Vec4 &rhs ) noexcept
  {
    lhs.m_SIMD = _mm_sub_ps( lhs.m_SIMD, rhs.m_SIMD );
    return lhs;
  }
  friend inline Vec4 &operator*=( Vec4 &lhs, const Vec4 &rhs ) noexcept
  {
    lhs.m_SIMD = _mm_mul_ps( lhs.m_SIMD, rhs.m_SIMD );
    return lhs;
  }
  friend inline Vec4 &operator/=( Vec4 &lhs, const Vec4 &rhs ) noexcept
  {
    lhs.m_SIMD = _mm_div_ps( lhs.m_SIMD, rhs.m_SIMD );
    return lhs;
  }

  friend inline Vec4 operator*( const Vec4 &lhs, const float rhs ) noexcept
  {
    return _mm_mul_ps( lhs.m_SIMD, _mm_set1_ps( rhs ) );
  }
  friend inline Vec4 operator*( const float lhs, const Vec4 &rhs )
  {
    return rhs * lhs;
  }

  friend inline Vec4 operator/( const Vec4 &lhs, const float rhs ) noexcept
  {
    return _mm_div_ps( lhs.m_SIMD, _mm_set1_ps( rhs ) );
  }

  friend inline Vec4 &operator*=( Vec4 &lhs, const float rhs ) noexcept
  {
    lhs.m_SIMD = _mm_mul_ps( lhs.m_SIMD, _mm_set1_ps( rhs ) );
    return lhs;
  }
  friend inline Vec4 &operator/=( Vec4 &lhs, const float rhs ) noexcept
  {
    lhs.m_SIMD = _mm_div_ps( lhs.m_SIMD, _mm_set1_ps( rhs ) );
    return lhs;
  }

  friend inline Vec4 operator-( const Vec4 &lhs ) noexcept
  {
    return _mm_xor_ps( lhs.m_SIMD, s_SIGNMASK );
  }

  friend inline bool operator==( const Vec4 &lhs, const Vec4 &rhs ) noexcept
  {
    return ( ( ( _mm_movemask_ps( _mm_cmpneq_ps( lhs.m_SIMD, rhs.m_SIMD ) ) ) & 0xF ) == 0 );
  }

  friend inline bool operator!=( const Vec4 &lhs, const Vec4 &rhs ) noexcept
  {
    return ( ( ( _mm_movemask_ps( _mm_cmpneq_ps( lhs.m_SIMD, rhs.m_SIMD ) ) ) & 0xF ) != 0 );
  }

  friend inline bool operator<( const Vec4 &lhs, const Vec4 &rhs ) noexcept
  {
    return ( ( _mm_movemask_ps( _mm_cmpge_ps( lhs.m_SIMD, rhs.m_SIMD ) ) & 0xF ) == 0 );
  }

  friend inline bool operator<=( const Vec4 &lhs, const Vec4 &rhs ) noexcept
  {
    return ( ( _mm_movemask_ps( _mm_cmpgt_ps( lhs.m_SIMD, rhs.m_SIMD ) ) & 0xF ) == 0 );
  }

  friend inline bool operator>( const Vec4 &lhs, const Vec4 &rhs ) noexcept
  {
    return ( ( _mm_movemask_ps( _mm_cmple_ps( lhs.m_SIMD, rhs.m_SIMD ) ) & 0xF ) == 0 );
  }

  friend inline bool operator>=( const Vec4 &lhs, const Vec4 &rhs ) noexcept
  {
    return ( ( _mm_movemask_ps( _mm_cmplt_ps( lhs.m_SIMD, rhs.m_SIMD ) ) & 0xF ) == 0 );
  }

  friend std::ostream &operator<<( std::ostream &os, const Vec4 &vec ) noexcept
  {
    os << "[0]:" << vec[0] << ",\t[1]:" << vec[1] << ",\t[2]:" << vec[2] << ",\t[3]:" << vec[3];
    return os;
  }

  inline Vec4 abs() const noexcept
  {
    return Vec4( _mm_and_ps( m_SIMD, s_ABSMASK ) );
  }

  inline Vec4 normalize() const noexcept
  {
    return _mm_div_ps( m_SIMD, _mm_sqrt_ps( _mm_dp_ps( m_SIMD, m_SIMD, 0xFF ) ) );
  }

  inline float dot( const Vec4 &rhs ) const
  {
    return _mm_cvtss_f32( _mm_dp_ps( m_SIMD, rhs.m_SIMD, 0xF1 ) );
  }

  inline float length() const noexcept
  {
    return _mm_cvtss_f32( _mm_sqrt_ss( _mm_dp_ps( m_SIMD, m_SIMD, 0xF1 ) ) );
  }

  inline float lengthSq() const noexcept
  {
    return dot( *this );
  }

  inline float sum() const noexcept
  {
    return std::bit_cast<float *>( &m_SIMD )[0] + std::bit_cast<float *>( &m_SIMD )[1] + std::bit_cast<float *>( &m_SIMD )[2] + std::bit_cast<float *>( &m_SIMD )[3];
  }

  _inline float getX() const noexcept
  {
    return _mm_cvtss_f32( m_SIMD );
  }

  _inline float getY() const noexcept
  {
    return _mm_cvtss_f32( _mm_shuffle_ps( m_SIMD, m_SIMD, _MM_SHUFFLE( 1, 1, 1, 1 ) ) );
  }

  _inline float getZ() const noexcept
  {
    return _mm_cvtss_f32( _mm_shuffle_ps( m_SIMD, m_SIMD, _MM_SHUFFLE( 2, 2, 2, 2 ) ) );
  }

  _inline float getW() const noexcept
  {
    return _mm_cvtss_f32( _mm_shuffle_ps( m_SIMD, m_SIMD, _MM_SHUFFLE( 3, 3, 3, 3 ) ) );
  }

  inline void setX( const float x ) noexcept
  {
    m_SIMD = _mm_move_ss( m_SIMD, _mm_set_ss( x ) );
  }

  inline void setY( const float y ) noexcept
  {
    __m128 tmp = _mm_move_ss( m_SIMD, _mm_set_ss( y ) );
    tmp = _mm_shuffle_ps( tmp, tmp, _MM_SHUFFLE( 3, 2, 0, 0 ) );
    m_SIMD = _mm_move_ss( tmp, m_SIMD );
  }

  inline void setZ( const float z ) noexcept
  {
    __m128 tmp = _mm_move_ss( m_SIMD, _mm_set_ss( z ) );
    tmp = _mm_shuffle_ps( tmp, tmp, _MM_SHUFFLE( 3, 0, 1, 0 ) );
    m_SIMD = _mm_move_ss( tmp, m_SIMD );
  }

  inline void setW( const float w ) noexcept
  {
    __m128 tmp = _mm_move_ss( m_SIMD, _mm_set_ss( w ) );
    tmp = _mm_shuffle_ps( tmp, tmp, _MM_SHUFFLE( 0, 2, 1, 0 ) );
    m_SIMD = _mm_move_ss( tmp, m_SIMD );
  }

  __m128 m_SIMD;
};

//--------------------------Vector3/4 concept---------------------------------//
template <typename T>
concept IsSIMDVec = std::is_same<T, Vec3>::value || std::is_same<T, Vec4>::value;

//--------------------------For axis use---------------------------------//

// Same as calling v[0]
template <IsSIMDVec Vec>
_inline float getX( const Vec &v ) noexcept
{
  return _mm_cvtss_f32( v.m_SIMD );
}

// Same as calling v[0]
template <IsSIMDVec Vec>
_inline float getX( const Vec *v ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  return _mm_cvtss_f32( v->m_SIMD );
}

// Same as calling v[1]
template <IsSIMDVec Vec>
_inline float getY( const Vec &v ) noexcept
{
  return _mm_cvtss_f32( _mm_shuffle_ps( v.m_SIMD, v.m_SIMD, _MM_SHUFFLE( 1, 1, 1, 1 ) ) );
}

// Same as calling v[1]
template <IsSIMDVec Vec>
_inline float getY( const Vec *v ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  return _mm_cvtss_f32( _mm_shuffle_ps( v->m_SIMD, v->m_SIMD, _MM_SHUFFLE( 1, 1, 1, 1 ) ) );
}

// Same as calling v[2]
template <IsSIMDVec Vec>
_inline float getZ( const Vec &v ) noexcept
{
  return _mm_cvtss_f32( _mm_shuffle_ps( v.m_SIMD, v.m_SIMD, _MM_SHUFFLE( 2, 2, 2, 2 ) ) );
}

// Same as calling v[2]
template <IsSIMDVec Vec>
_inline float getZ( const Vec *v ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  return _mm_cvtss_f32( _mm_shuffle_ps( v->m_SIMD, v->m_SIMD, _MM_SHUFFLE( 2, 2, 2, 2 ) ) );
}

// Same as calling v[3]
_inline float getW( const Vec4 &v ) noexcept
{
  return _mm_cvtss_f32( _mm_shuffle_ps( v.m_SIMD, v.m_SIMD, _MM_SHUFFLE( 3, 3, 3, 3 ) ) );
}

// Same as calling v[3]
_inline float getW( const Vec4 *v ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  return _mm_cvtss_f32( _mm_shuffle_ps( v->m_SIMD, v->m_SIMD, _MM_SHUFFLE( 3, 3, 3, 3 ) ) );
}

// Same as setting v[0]
template <IsSIMDVec Vec>
inline void setX( Vec &v, const float x ) noexcept
{
  v.m_SIMD = _mm_move_ss( v.m_SIMD, _mm_set_ss( x ) );
}

// Same as setting v[0]
template <IsSIMDVec Vec>
inline void setX( Vec *v, const float x ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  v->m_SIMD = _mm_move_ss( v->m_SIMD, _mm_set_ss( x ) );
}

// Same as setting v[1]
template <IsSIMDVec Vec>
inline void setY( Vec &v, const float y ) noexcept
{
  __m128 tmp = _mm_move_ss( v.m_SIMD, _mm_set_ss( y ) );
  tmp = _mm_shuffle_ps( tmp, tmp, _MM_SHUFFLE( 3, 2, 0, 0 ) );
  v.m_SIMD = _mm_move_ss( tmp, v.m_SIMD );
}

// Same as setting v[1]
template <IsSIMDVec Vec>
inline void setY( Vec *v, const float y ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  __m128 tmp = _mm_move_ss( v->m_SIMD, _mm_set_ss( y ) );
  tmp = _mm_shuffle_ps( tmp, tmp, _MM_SHUFFLE( 3, 2, 0, 0 ) );
  v->m_SIMD = _mm_move_ss( tmp, v->m_SIMD );
}

// Same as setting v[2]
template <IsSIMDVec Vec>
inline void setZ( Vec &v, const float z ) noexcept
{
  __m128 tmp = _mm_move_ss( v.m_SIMD, _mm_set_ss( z ) );
  tmp = _mm_shuffle_ps( tmp, tmp, _MM_SHUFFLE( 3, 0, 1, 0 ) );
  v.m_SIMD = _mm_move_ss( tmp, v.m_SIMD );
}

// Same as setting v[2]
template <IsSIMDVec Vec>
inline void setZ( Vec *v, const float z ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  __m128 tmp = _mm_move_ss( v->m_SIMD, _mm_set_ss( z ) );
  tmp = _mm_shuffle_ps( tmp, tmp, _MM_SHUFFLE( 3, 0, 1, 0 ) );
  v->m_SIMD = _mm_move_ss( tmp, v->m_SIMD );
}

// Same as setting v[3]
inline void setW( Vec4 &v, const float w ) noexcept
{
  __m128 tmp = _mm_move_ss( v.m_SIMD, _mm_set_ss( w ) );
  tmp = _mm_shuffle_ps( tmp, tmp, _MM_SHUFFLE( 0, 2, 1, 0 ) );
  v.m_SIMD = _mm_move_ss( tmp, v.m_SIMD );
}

// Same as setting v[3]
inline void setW( Vec4 *v, const float w ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  __m128 tmp = _mm_move_ss( v->m_SIMD, _mm_set_ss( w ) );
  tmp = _mm_shuffle_ps( tmp, tmp, _MM_SHUFFLE( 0, 2, 1, 0 ) );
  v->m_SIMD = _mm_move_ss( tmp, v->m_SIMD );
}

//--------------------------Vec3 and SIMD4 Utils---------------------------------//

template <IsSIMDVec Vec>
inline float length( const Vec &v ) noexcept
{
  if constexpr ( std::is_same<Vec, Vec3>::value )
  {
    return _mm_cvtss_f32( _mm_sqrt_ss( _mm_dp_ps( v.m_SIMD, v.m_SIMD, 0x71 ) ) );
  }
  else
  {
    return _mm_cvtss_f32( _mm_sqrt_ss( _mm_dp_ps( v.m_SIMD, v.m_SIMD, 0xF1 ) ) );
  }
}

template <IsSIMDVec Vec>
inline float length( const Vec *v ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  if constexpr ( std::is_same<Vec, Vec3>::value )
  {
    return _mm_cvtss_f32( _mm_sqrt_ss( _mm_dp_ps( v->m_SIMD, v->m_SIMD, 0x71 ) ) );
  }
  else
  {
    return _mm_cvtss_f32( _mm_sqrt_ss( _mm_dp_ps( v->m_SIMD, v->m_SIMD, 0xF1 ) ) );
  }
}

template <IsSIMDVec Vec>
inline float dot( const Vec &lhs, const Vec &rhs )
{
  if constexpr ( std::is_same<Vec, Vec3>::value )
  {
    return _mm_cvtss_f32( _mm_dp_ps( lhs.m_SIMD, rhs.m_SIMD, 0x71 ) );
  }
  else
  {
    return _mm_cvtss_f32( _mm_dp_ps( lhs.m_SIMD, rhs.m_SIMD, 0xF1 ) );
  }
}

template <IsSIMDVec Vec>
inline float dot( const Vec *lhs, const Vec *rhs )
{
  XV_CORE_ASSERT_MSG( lhs != nullptr && rhs != nullptr, "[SIMD] Passed in nullptr" );
  if constexpr ( std::is_same<Vec, Vec3>::value )
  {
    return _mm_cvtss_f32( _mm_dp_ps( lhs->m_SIMD, rhs->m_SIMD, 0x71 ) );
  }
  else
  {
    return _mm_cvtss_f32( _mm_dp_ps( lhs->m_SIMD, rhs->m_SIMD, 0xF1 ) );
  }
}

template <IsSIMDVec Vec>
inline Vec normalize( const Vec &v ) noexcept
{
  if constexpr ( std::is_same<Vec, Vec3>::value )
  {
    return _mm_div_ps( v.m_SIMD, _mm_sqrt_ps( _mm_dp_ps( v.m_SIMD, v.m_SIMD, 0x77 ) ) );
  }
  else
  {
    return _mm_div_ps( v.m_SIMD, _mm_sqrt_ps( _mm_dp_ps( v.m_SIMD, v.m_SIMD, 0xFF ) ) );
  }
}

template <IsSIMDVec Vec>
inline void normalize( Vec *v ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  if constexpr ( std::is_same<Vec, Vec3>::value )
  {
    *v = _mm_div_ps( v->m_SIMD, _mm_sqrt_ps( _mm_dp_ps( v->m_SIMD, v->m_SIMD, 0x77 ) ) );
  }
  else
  {
    *v = _mm_div_ps( v->m_SIMD, _mm_sqrt_ps( _mm_dp_ps( v->m_SIMD, v->m_SIMD, 0xFF ) ) );
  }
}

template <IsSIMDVec Vec>
inline Vec abs( const Vec &v ) noexcept
{
  return v.abs();
}

template <IsSIMDVec Vec>
inline Vec abs( const Vec *v ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  return v->abs();
}

// Creating a vector by getting the min of both vectors
// e.g. lhs = <1,6,2,7> and rhs = <2,4,6,8>, the result will be <1,4,2,7>
template <IsSIMDVec Vec>
inline Vec min( const Vec &lhs, const Vec &rhs ) noexcept
{
  return _mm_min_ps( lhs.m_SIMD, rhs.m_SIMD );
}
// Creating a vector by getting the max of both vectors
// e.g. lhs = <1,6,2,7> and rhs = <2,4,6,8>, the result will be <2,6,6,8>
template <IsSIMDVec Vec>
inline Vec max( const Vec &lhs, const Vec &rhs ) noexcept
{
  return _mm_max_ps( lhs.m_SIMD, rhs.m_SIMD );
}

template <IsSIMDVec Vec>
inline Vec clamp( const Vec &val, const Vec &min_vec, const Vec &max_vec )
{
  return max( min( val, max_vec ), min_vec );
}

template <IsSIMDVec Vec>
inline float lengthSq( const Vec &v ) noexcept
{
  return dot( v, v );
}

template <IsSIMDVec Vec>
inline float distanceSq( const Vec &v1, const Vec &v2 ) noexcept
{
  return lengthSq( v2 - v1 );
}

template <IsSIMDVec Vec>
inline float lengthSq( const Vec *v ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  return dot( v, v );
}

template <IsSIMDVec Vec>
inline float distanceSq( const Vec *v1, const Vec *v2 ) noexcept
{
  XV_CORE_ASSERT_MSG( v1 != nullptr && v2 != nullptr, "[SIMD] Passed in nullptr" );
  return lengthSq( v2 - v1 );
}

template <IsSIMDVec Vec>
inline float distance( const Vec &v1, const Vec &v2 ) noexcept
{
  return length( v2 - v1 );
}

template <IsSIMDVec Vec>
inline Vec lerp( const Vec &a, const Vec &b, const float time_ratio ) noexcept
{
  XV_CORE_ASSERT_MSG( time_ratio >= 0.0f && time_ratio <= 1.0f, "[SIMD] Time ratio need to be in between 0 to 1. Tips: you could use clamp before passing into time_ratio" );
  return _mm_add_ps( a.m_SIMD, ( _mm_mul_ps( _mm_sub_ps( b.m_SIMD, a.m_SIMD ), _mm_set1_ps( time_ratio ) ) ) );
}

template <IsSIMDVec Vec>
inline Vec lerp( const Vec *a, const Vec *b, const float time_ratio ) noexcept
{
  XV_CORE_ASSERT_MSG( a != nullptr && b != nullptr, "[SIMD] Passed in nullptr" );
  XV_CORE_ASSERT_MSG( time_ratio >= 0.0f && time_ratio <= 1.0f, "[SIMD] Time ratio need to be in between 0 to 1. Tips: you could use clamp before passing into time_ratio" );
  return _mm_add_ps( a->m_SIMD, ( _mm_mul_ps( _mm_sub_ps( b->m_SIMD, a->m_SIMD ), _mm_set1_ps( time_ratio ) ) ) );
}

template <IsSIMDVec Vec>
inline void negate( Vec &v ) noexcept
{
  v = -v;
}

template <IsSIMDVec Vec>
inline Vec negated( const Vec &v ) noexcept
{
  return -v;
}

template <IsSIMDVec Vec>
inline void negate( Vec *v ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  *v = -( *v );
}

template <IsSIMDVec Vec>
inline Vec negated( const Vec *v ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  return -( *v );
}

template <IsSIMDVec Vec>
inline void setToZero( Vec &v ) noexcept
{
  v.m_SIMD = _mm_setzero_ps();
}

template <IsSIMDVec Vec>
inline void setToZero( Vec *v ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  v->m_SIMD = _mm_setzero_ps();
}

template <IsSIMDVec Vec>
inline void setAll( Vec &v, const float val ) noexcept
{
  if constexpr ( std::is_same<Vec, Vec3>::value )
  {
    v.m_SIMD = _mm_set_ps( 0, val, val, val );
  }
  else
  {
    v.m_SIMD = _mm_set_ps( val, val, val, val );
  }
}

template <IsSIMDVec Vec>
inline void setAll( Vec *v, const float val ) noexcept
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  if constexpr ( std::is_same<Vec, Vec3>::value )
  {
    v->m_SIMD = _mm_set_ps( 0, val, val, val );
  }
  else
  {
    v->m_SIMD = _mm_set_ps( val, val, val, val );
  }
}

template <IsSIMDVec Vec>
float attemptNormalize( Vec &v )
{
  auto lenSq = lengthSq( v );
  if ( lenSq >= s_EPSILON * s_EPSILON )
  {
    lenSq = length( v );
    v /= lenSq;
  }
  return lenSq;
}

template <IsSIMDVec Vec>
float attemptNormalize( Vec *v )
{
  XV_CORE_ASSERT_MSG( v != nullptr, "[SIMD] Passed in nullptr" );
  auto lenSq = lengthSq( *v );
  if ( lenSq >= s_EPSILON * s_EPSILON )
  {
    lenSq = length( *v );
    *v /= lenSq;
  }
  return lenSq;
}
}