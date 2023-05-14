/*****************************************************************
*\file         Matrix.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace SIMD
{
  //--------------------------Mat3x3---------------------------------//
struct Mat3x3 final
{
  __inline Mat3x3() noexcept = default;
  __inline explicit Mat3x3( const Vec3 &row1, const Vec3 &row2, const Vec3 &row3 ) noexcept
  {
    m[0] = row1;
    m[1] = row2;
    m[2] = row3;
  }
  __inline explicit Mat3x3( const float m00, const float m01, const float m02,
                            const float m10, const float m11, const float m12,
                            const float m20, const float m21, const float m22 ) noexcept
  {
    m[0] = Vec3 { m00, m01, m02 };
    m[1] = Vec3 { m10, m11, m12 };
    m[2] = Vec3 { m20, m21, m22 };
  }
  __inline explicit Mat3x3( const float *ptr ) noexcept
  {
    m[0] = Vec3 { ptr[0], ptr[1], ptr[2] };
    m[1] = Vec3 { ptr[3], ptr[4], ptr[5] };
    m[2] = Vec3 { ptr[6], ptr[7], ptr[8] };
  }
  __inline Mat3x3( const Mat3x3 &rhs ) noexcept
  {
    m[0] = rhs.m[0];
    m[1] = rhs.m[1];
    m[2] = rhs.m[2];
  }
  __inline Mat3x3( const float rhs ) noexcept
  {
    m[0].setAll( rhs );
    m[1].setAll( rhs );
    m[2].setAll( rhs );
  }

  __inline void store( float *const ptr ) const noexcept
  {
    XV_CORE_ASSERT_MSG( ptr != nullptr, "[Matrix] Storing into nullptr" );
    m[0].store3( ptr );
    m[1].store3( ptr + 3 );
    m[2].store3( ptr + 6 );
  }

  __inline void set( const Vec3 &row1, const Vec3 &row2, const Vec3 &row3 ) noexcept
  {
    m[0] = row1;
    m[1] = row2;
    m[2] = row3;
  }

  __inline Vec3 operator[]( const int i ) const noexcept
  {
    XV_CORE_ASSERT_MSG( i >= 0 && i < 3, "[Matrix] Out of range" );
    return m[i];
  }
  __inline Vec3 &operator[]( const int i ) noexcept
  {
    XV_CORE_ASSERT_MSG( i >= 0 && i < 3, "[Matrix] Out of range" );
    return m[i];
  }
  __inline float operator()( const int rowIndex, const int colIndex ) const noexcept
  {
    XV_CORE_ASSERT_MSG( rowIndex >= 0 && rowIndex < 3 && colIndex >= 0 && colIndex < 3, "[Matrix] Out of range" );
    return m[rowIndex][colIndex];
  }
  __inline float &operator()( const int rowIndex, const int colIndex ) noexcept
  {
    XV_CORE_ASSERT_MSG( rowIndex >= 0 && rowIndex < 3 && colIndex >= 0 && colIndex < 3, "[Matrix] Out of range" );
    return m[rowIndex][colIndex];
  }
  __inline Mat3x3 operator=( const Mat3x3 &rhs ) noexcept
  {
    if ( this != &rhs )
    {
      m[0] = rhs.m[0];
      m[1] = rhs.m[1];
      m[2] = rhs.m[2];
    }
    return *this;
  }

  friend __inline Mat3x3 operator*( const Mat3x3 &lhs, const float rhs ) noexcept
  {
    return Mat3x3 { lhs[0] * rhs, lhs[1] * rhs, lhs[2] * rhs };
  }

  friend __inline Mat3x3 &operator*=( Mat3x3 &lhs, const float rhs ) noexcept
  {
    lhs = lhs * rhs;
    return lhs;
  }

  friend __inline Mat3x3 operator*( const Mat3x3 &lhs, const Mat3x3 &rhs ) noexcept
  {
    auto tmp = rhs.transpose();
    return Mat3x3 { lhs[0].dot( tmp[0] ), lhs[0].dot( tmp[1] ), lhs[0].dot( tmp[2] ),
            lhs[1].dot( tmp[0] ), lhs[1].dot( tmp[1] ), lhs[1].dot( tmp[2] ),
            lhs[2].dot( tmp[0] ), lhs[2].dot( tmp[1] ), lhs[2].dot( tmp[2] ) };
  }

  friend __inline Mat3x3 &operator*=( Mat3x3 &lhs, const Mat3x3 &rhs ) noexcept
  {
    lhs = lhs * rhs;
    return lhs;
  }

  friend __inline Vec3 operator*( const Mat3x3 &lhs, const Vec3 &rhs ) noexcept
  {
    return Vec3 { lhs.m[0].dot( rhs ), lhs.m[1].dot( rhs ), lhs.m[2].dot( rhs ) };
  }

  friend std::ostream &operator<<( std::ostream &os, const Mat3x3 &mat ) noexcept
  {
    os << mat[0] << "\n" << mat[1] << "\n" << mat[2];
    return os;
  }

  friend __inline bool operator==( const Mat3x3 &lhs, const Mat3x3 &rhs ) noexcept
  {
    return	lhs.m[0] == rhs.m[0] &&
      lhs.m[1] == rhs.m[1] &&
      lhs.m[2] == rhs.m[2];
  }

  friend __inline bool operator!=( const Mat3x3 &lhs, const Mat3x3 &rhs ) noexcept
  {
    return !( lhs == rhs );
  }

  friend __inline Mat3x3 operator-( const Mat3x3 &lhs ) noexcept
  {
    return Mat3x3 { -lhs.m[0], -lhs.m[1], -lhs.m[2] };
  }

  __inline Mat3x3 concat( const Mat3x3 &rhs ) noexcept
  {
    return *this * rhs;
  }

  __inline void setToZero() noexcept
  {
    m[0].setToZero();
    m[1].setToZero();
    m[2].setToZero();
  }

  __inline void setAll( const float val ) noexcept
  {
    m[0].setAll( val );
    m[1].setAll( val );
    m[2].setAll( val );
  }

  __inline static Mat3x3 identity() noexcept
  {
    return Mat3x3 { 1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f };
  }

  friend __inline bool inverse( const Mat3x3 input, Mat3x3 &output ) noexcept
  {
    float det = input.m[0].dot( input.m[1].cross( input.m[2] ) );
    if ( det != 0.0f )
    {
      for ( auto i = 0; i < 3; i++ )
      {
        for ( auto j = 0; j < 3; j++ )
        {
          output[i][j] = ( input.m[( j + 1 ) % 3][( i + 1 ) % 3] * input.m[( j + 2 ) % 3][( i + 2 ) % 3] ) - ( input.m[( j + 1 ) % 3][( i + 2 ) % 3] * input.m[( j + 2 ) % 3][( i + 1 ) % 3] ) / det;
        }
      }

      return true;
    }

    XV_CORE_WARN( "[Matrix] Matrix not invertible" );
    return false;
  }

  __inline Mat3x3 transpose() const noexcept
  {
    return Mat3x3 { m[0][0], m[1][0], m[2][0],
            m[0][1], m[1][1], m[2][1],
            m[0][2], m[1][2], m[2][2] };
  }

  __inline Mat3x3 translate( const float x, const float y ) const noexcept
  {
    Mat3x3 result = identity();
    result[0][2] = x;
    result[1][2] = y;
    return result;
  }

  __inline Mat3x3 scale( const float x, const float y ) const noexcept
  {
    Mat3x3 result = identity();
    result[0][0] = x;
    result[1][1] = y;
    return result;
  }

  __inline Mat3x3 scale( const float val ) const noexcept
  {
    Mat3x3 result = identity();
    result[0][0] = val;
    result[1][1] = val;
    return result;
  }

  Mat3x3 rotDeg( const float angle ) const noexcept
  {
    float s = std::sinf( angle ), c = std::cosf( angle );
    Mat3x3 result = identity();
    result[0][0] = result[1][1] = c;
    result[0][1] = -s;
    result[1][1] = s;
    return result;
  }
  Mat3x3 rotRad( const float rad ) const noexcept
  {
    return rotDeg( rad * s_TODEG );
  }

  Vec3 basis( const unsigned index ) const noexcept
  {
    XV_CORE_ASSERT_MSG( index < 3, "[Matrix] Out of range" );
    return Vec3 { m[0][index], m[1][index], m[2][index] };
  }

  void setBasis( unsigned index, float x, float y, float z ) noexcept
  {
    XV_CORE_ASSERT_MSG( index < 3, "[Matrix] Out of range" );
    m[0][index] = x;
    m[1][index] = y;
    m[2][index] = z;
  }

  Vec3 getRow1() const noexcept
  {
    return m[0];
  }

  Vec3 getRow2() const noexcept
  {
    return m[1];
  }

  Vec3 getRow3() const noexcept
  {
    return m[2];
  }

  void setRow1( const Vec3 row1 ) noexcept
  {
    m[0] = row1;
  }

  void setRow2( const Vec3 row2 ) noexcept
  {
    m[1] = row2;
  }

  void setRow3( const Vec3 row3 ) noexcept
  {
    m[2] = row3;
  }

private:
  Vec3 m[3];
};

//--------------------------Mat4x4---------------------------------//
struct Mat4x4 final
{
  __inline Mat4x4() noexcept = default;
  __inline Mat4x4( const Vec4 &row1, const Vec4 &row2, const Vec4 &row3, const Vec4 &row4 ) noexcept
  {
    m[0] = row1;
    m[1] = row2;
    m[2] = row3;
    m[3] = row4;
  }
  __inline explicit Mat4x4( const float m00, const float m01, const float m02, const float m03,
                            const float m10, const float m11, const float m12, const float m13,
                            const float m20, const float m21, const float m22, const float m23,
                            const float m30, const float m31, const float m32, const float m33 ) noexcept
  {
    m[0] = Vec4 { m00, m01, m02, m03 };
    m[1] = Vec4 { m10, m11, m12, m13 };
    m[2] = Vec4 { m20, m21, m22, m23 };
    m[3] = Vec4 { m30, m31, m32, m33 };
  }
  __inline explicit Mat4x4( const float *ptr ) noexcept
  {
    m[0] = Vec4 { ptr[0], ptr[1], ptr[2], ptr[3] };
    m[1] = Vec4 { ptr[4], ptr[5], ptr[6], ptr[7] };
    m[2] = Vec4 { ptr[8], ptr[9], ptr[10], ptr[11] };
    m[3] = Vec4 { ptr[12], ptr[13], ptr[14], ptr[15] };
  }
  __inline Mat4x4( const Mat4x4 &rhs ) noexcept
  {
    m[0] = rhs.m[0];
    m[1] = rhs.m[1];
    m[2] = rhs.m[2];
    m[3] = rhs.m[3];
  }
  __inline Mat4x4( const float rhs ) noexcept
  {
    m[0].setAll( rhs );
    m[1].setAll( rhs );
    m[2].setAll( rhs );
    m[3].setAll( rhs );
  }

  __inline void store( float *const ptr ) const noexcept
  {
    XV_CORE_ASSERT_MSG( ptr != nullptr, "[Matrix] Storing into nullptr" );
    m[0].store( ptr );
    m[1].store( ptr + 4 );
    m[2].store( ptr + 8 );
    m[3].store( ptr + 12 );
  }

  __inline void set( const Vec4 &row1, const Vec4 &row2, const Vec4 &row3, const Vec4 &row4 ) noexcept
  {
    m[0] = row1;
    m[1] = row2;
    m[2] = row3;
    m[3] = row4;
  }

  __inline Vec4 operator[]( const int i ) const noexcept
  {
    XV_CORE_ASSERT_MSG( i >= 0 && i < 4, "[Matrix] Out of range" );
    return m[i];
  }
  __inline Vec4 &operator[]( const int i ) noexcept
  {
    XV_CORE_ASSERT_MSG( i >= 0 && i < 4, "[Matrix] Out of range" );
    return m[i];
  }
  __inline float operator()( const int rowIndex, const int colIndex ) const noexcept
  {
    XV_CORE_ASSERT_MSG( rowIndex >= 0 && rowIndex < 4 && colIndex >= 0 && colIndex < 4, "[Matrix] Out of range" );
    return m[rowIndex][colIndex];
  }
  __inline float &operator()( const int rowIndex, const int colIndex ) noexcept
  {
    XV_CORE_ASSERT_MSG( rowIndex >= 0 && rowIndex < 4 && colIndex >= 0 && colIndex < 4, "[Matrix] Out of range" );
    return m[rowIndex][colIndex];
  }
  __inline Mat4x4 operator=( const Mat4x4 &rhs ) noexcept
  {
    if ( this != &rhs )
    {
      m[0] = rhs.m[0];
      m[1] = rhs.m[1];
      m[2] = rhs.m[2];
      m[3] = rhs.m[3];
    }
    return *this;
  }
  friend __inline Mat4x4 operator*( const Mat4x4 &lhs, const float rhs ) noexcept
  {
    return Mat4x4 { lhs[0] * rhs, lhs[1] * rhs, lhs[2] * rhs, lhs[3] * rhs };
  }
  friend __inline Mat4x4 &operator*=( Mat4x4 &lhs, const float rhs ) noexcept
  {
    lhs = lhs * rhs;
    return lhs;
  }

  friend __inline Mat4x4 operator*( const Mat4x4 &lhs, const Mat4x4 &rhs ) noexcept
  {
    auto tmp = rhs.transpose();
    return Mat4x4 { lhs[0].dot( tmp[0] ), lhs[0].dot( tmp[1] ), lhs[0].dot( tmp[2] ), lhs[0].dot( tmp[3] ),
            lhs[1].dot( tmp[0] ), lhs[1].dot( tmp[1] ), lhs[1].dot( tmp[2] ), lhs[1].dot( tmp[3] ),
            lhs[2].dot( tmp[0] ), lhs[2].dot( tmp[1] ), lhs[2].dot( tmp[2] ), lhs[2].dot( tmp[3] ),
            lhs[3].dot( tmp[0] ), lhs[3].dot( tmp[1] ), lhs[3].dot( tmp[2] ), lhs[3].dot( tmp[3] ) };
  }

  friend __inline Mat4x4 operator^( const Mat4x4 &lhs, const Mat4x4 &rhs ) noexcept
  {
    Vec4 SrcA0 = lhs[0];
    Vec4 SrcA1 = lhs[1];
    Vec4 SrcA2 = lhs[2];
    Vec4 SrcA3 = lhs[3];

    Vec4 SrcB0 = rhs[0];
    Vec4 SrcB1 = rhs[1];
    Vec4 SrcB2 = rhs[2];
    Vec4 SrcB3 = rhs[3];;

    Mat4x4 Result;
    Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
    Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
    Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
    Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];

    return Result;
  }

  friend __inline Mat4x4 &operator*=( Mat4x4 &lhs, const Mat4x4 &rhs ) noexcept
  {
    lhs = lhs * rhs;
    return lhs;
  }

  friend __inline Vec4 operator*( const Mat4x4 &lhs, const Vec4 &rhs ) noexcept
  {
    return Vec4 { lhs.m[0].dot( rhs ), lhs.m[1].dot( rhs ), lhs.m[2].dot( rhs ), lhs.m[3].dot( rhs ) };
  }

  friend std::ostream &operator<<( std::ostream &os, const Mat4x4 &mat ) noexcept
  {
    os << mat[0] << "\n" << mat[1] << "\n" << mat[2] << "\n" << mat[3];
    return os;
  }

  friend __inline bool operator==( const Mat4x4 &lhs, const Mat4x4 &rhs ) noexcept
  {
    return	lhs.m[0] == rhs.m[0] &&
      lhs.m[1] == rhs.m[1] &&
      lhs.m[2] == rhs.m[2] &&
      lhs.m[3] == rhs.m[3];
  }

  friend __inline bool operator!=( const Mat4x4 &lhs, const Mat4x4 &rhs ) noexcept
  {
    return !( lhs == rhs );
  }

  friend __inline Mat4x4 operator-( const Mat4x4 &lhs ) noexcept
  {
    return Mat4x4 { -lhs.m[0], -lhs.m[1], -lhs.m[2], -lhs.m[3] };
  }

  __inline Mat4x4 concat( const Mat4x4 &rhs ) noexcept
  {
    return *this * rhs;
  }

  __inline void setToZero() noexcept
  {
    m[0].setToZero();
    m[1].setToZero();
    m[2].setToZero();
    m[3].setToZero();
  }

  __inline void setAll( const float val ) noexcept
  {
    m[0].setAll( val );
    m[1].setAll( val );
    m[2].setAll( val );
    m[3].setAll( val );
  }

  __inline static Mat4x4 identity() noexcept
  {
    return Mat4x4 { 1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f };
  }

  __inline Mat4x4 transpose() const noexcept
  {
    return Mat4x4 { m[0][0], m[1][0], m[2][0], m[3][0],
            m[0][1], m[1][1], m[2][1], m[3][1],
            m[0][2], m[1][2], m[2][2], m[3][2],
            m[0][3], m[1][3], m[2][3], m[3][3] };
  }

  __inline Mat4x4 translate( const float x, const float y, const float z ) const noexcept
  {
    Mat4x4 result = identity();
    result[0][3] = x;
    result[1][3] = y;
    result[2][3] = z;
    return result;
  }

  __inline Mat4x4 scale( const float x, const float y, const float z ) const noexcept
  {
    Mat4x4 result = identity();
    result[0][0] = x;
    result[1][1] = y;
    result[2][2] = z;
    return result;
  }

  __inline Mat4x4 scale( const float val ) const noexcept
  {
    Mat4x4 result = identity();
    result[0][0] = val;
    result[1][1] = val;
    result[2][2] = val;
    return result;
  }

  __inline __m128 mat2Mul( const Vec4 &lhs, const Vec4 &rhs ) const noexcept
  {
    return	_mm_add_ps( _mm_mul_ps( lhs.m_SIMD, VecSwizzle( rhs.m_SIMD, 0, 3, 0, 3 ) ),
                        _mm_mul_ps( VecSwizzle( lhs.m_SIMD, 1, 0, 3, 2 ), VecSwizzle( rhs.m_SIMD, 2, 1, 2, 1 ) ) );
  }

  __inline __m128 mat2AdjMul( const Vec4 &lhs, const Vec4 &rhs ) const noexcept
  {
    return	_mm_sub_ps( _mm_mul_ps( VecSwizzle( lhs.m_SIMD, 3, 3, 0, 0 ), rhs.m_SIMD ),
                        _mm_mul_ps( VecSwizzle( lhs.m_SIMD, 1, 1, 2, 2 ), VecSwizzle( rhs.m_SIMD, 2, 3, 0, 1 ) ) );
  }

  __inline __m128 mat2MulAdj( const Vec4 &lhs, const Vec4 &rhs ) const noexcept
  {
    return	_mm_sub_ps( _mm_mul_ps( lhs.m_SIMD, VecSwizzle( rhs.m_SIMD, 3, 0, 3, 0 ) ),
                        _mm_mul_ps( VecSwizzle( lhs.m_SIMD, 1, 0, 3, 2 ), VecSwizzle( rhs.m_SIMD, 2, 1, 2, 1 ) ) );
  }

  friend __inline bool inverse( const Mat4x4 input, Mat4x4 &output ) noexcept
  {
    output = {
      _mm_movelh_ps( input.m[0].m_SIMD, input.m[1].m_SIMD ),
      _mm_movehl_ps( input.m[1].m_SIMD, input.m[0].m_SIMD ),
      _mm_movelh_ps( input.m[2].m_SIMD, input.m[3].m_SIMD ),
      _mm_movehl_ps( input.m[3].m_SIMD, input.m[2].m_SIMD )
    };
    // Find determinant
    Vec4 det128 = _mm_sub_ps(
      _mm_mul_ps( _mm_shuffle_ps( input.m[0].m_SIMD, input.m[2].m_SIMD, 0x88 ), _mm_shuffle_ps( input.m[1].m_SIMD, input.m[3].m_SIMD, 0xDD ) ),
      _mm_mul_ps( _mm_shuffle_ps( input.m[0].m_SIMD, input.m[2].m_SIMD, 0xDD ), _mm_shuffle_ps( input.m[1].m_SIMD, input.m[3].m_SIMD, 0x88 ) )
    );
    Mat4x4 det {
      VecSwizzle1( det128.m_SIMD, 0 ),
      VecSwizzle1( det128.m_SIMD, 1 ),
      VecSwizzle1( det128.m_SIMD, 2 ),
      VecSwizzle1( det128.m_SIMD, 3 )
    };

    Vec4 WZ = input.mat2AdjMul( output[3], output[2] );
    Vec4 XY = input.mat2AdjMul( output[0], output[1] );

    // |M| = |A|*|D| + |B|*|C|
    det128 = _mm_add_ps( _mm_mul_ps( det[0].m_SIMD, det[3].m_SIMD ), _mm_mul_ps( det[1].m_SIMD, det[2].m_SIMD ) );

    __m128 tr = _mm_mul_ps( XY.m_SIMD, VecSwizzle( WZ.m_SIMD, 0, 2, 1, 3 ) );
    tr = _mm_hadd_ps( tr, tr );
    tr = _mm_hadd_ps( tr, tr );

    // |M| = |A|*|D| + |B|*|C| - tr
    det128 = _mm_sub_ps( det128.m_SIMD, tr );
    if ( det128[0] == 0.0f || det128[1] == 0.0f || det128[2] == 0.0f || det128[3] == 0.0f )
    {
      XV_CORE_WARN( "[Matrix] Matrix not invertible" );
      return false;
    }

    // (1/|M|, -1/|M|, -1/|M|, 1/|M|)
    det128 = _mm_div_ps( _mm_setr_ps( 1.f, -1.f, -1.f, 1.f ), det128.m_SIMD );

    Mat4x4 tmp2 {
      _mm_sub_ps( _mm_mul_ps( det[3].m_SIMD, output[0].m_SIMD ), input.mat2Mul( output[1], WZ ) ),
      _mm_sub_ps( _mm_mul_ps( det[1].m_SIMD, output[2].m_SIMD ), input.mat2MulAdj( output[3], XY ) ),
      _mm_sub_ps( _mm_mul_ps( det[2].m_SIMD, output[1].m_SIMD ), input.mat2MulAdj( output[0], WZ ) ),
      _mm_sub_ps( _mm_mul_ps( det[0].m_SIMD, output[3].m_SIMD ), input.mat2Mul( output[2], XY ) )
    };

    tmp2 = {
      _mm_mul_ps( tmp2[0].m_SIMD, det128.m_SIMD ),
      _mm_mul_ps( tmp2[1].m_SIMD, det128.m_SIMD ),
      _mm_mul_ps( tmp2[2].m_SIMD, det128.m_SIMD ),
      _mm_mul_ps( tmp2[3].m_SIMD, det128.m_SIMD )
    };

    output = {
       _mm_shuffle_ps( tmp2[0].m_SIMD, tmp2[1].m_SIMD, 0x77 ),
       _mm_shuffle_ps( tmp2[0].m_SIMD, tmp2[1].m_SIMD, 0x22 ),
       _mm_shuffle_ps( tmp2[2].m_SIMD, tmp2[3].m_SIMD, 0x77 ),
       _mm_shuffle_ps( tmp2[2].m_SIMD, tmp2[3].m_SIMD, 0x22 )
    };

    return true;
  }

  __inline Mat4x4 rotXDeg( const float angle ) const noexcept
  {
    float s = std::sinf( angle ), c = std::cosf( angle );
    Mat4x4 result = identity();
    result[1][1] = result[2][2] = c;
    result[2][1] = -s;
    result[1][2] = s;
    return result;
  }
  __inline Mat4x4 rotXRad( const float rad ) const noexcept
  {
    return rotXDeg( rad * s_TODEG );
  }

  __inline Mat4x4 rotYDeg( const float angle ) const noexcept
  {
    float s = std::sinf( angle ), c = std::cosf( angle );
    Mat4x4 result = identity();
    result[0][0] = result[2][2] = c;
    result[0][2] = -s;
    result[2][0] = s;
    return result;
  }
  __inline Mat4x4 rotYRad( const float rad ) const noexcept
  {
    return rotYDeg( rad * s_TODEG );
  }

  __inline Mat4x4 rotZDeg( const float angle ) const noexcept
  {
    float s = std::sinf( angle ), c = std::cosf( angle );
    Mat4x4 result = identity();
    result[0][0] = result[1][1] = c;
    result[1][0] = -s;
    result[0][1] = s;
    return result;
  }
  __inline Mat4x4 rotZRad( const float rad ) const noexcept
  {
    return rotZDeg( rad * s_TODEG );
  }

  __inline Mat4x4 rotDeg( const float angle ) const noexcept
  {
    return rotZDeg( angle ) * rotYDeg( angle ) * rotXDeg( angle );
  }
  __inline Mat4x4 rotRad( const float rad ) const noexcept
  {
    return rotZRad( rad ) * rotYRad( rad ) * rotXRad( rad );
  }

  __inline Vec4 basis( const unsigned index ) const noexcept
  {
    XV_CORE_ASSERT_MSG( index < 4, "[Matrix] Out of range" );
    return Vec4 { m[0][index], m[1][index], m[2][index], m[3][index] };
  }

  __inline void setBasis( unsigned index, float x, float y, float z, float w ) noexcept
  {
    XV_CORE_ASSERT_MSG( index < 4, "[Matrix] Out of range" );
    m[0][index] = x;
    m[1][index] = y;
    m[2][index] = z;
    m[3][index] = w;
  }

  __inline void decompose( Vec3 *scale, Mat3x3 *rotate, Vec3 *translate ) const noexcept
  {
    Vec3 shear;
    decompose( scale, &shear, rotate, translate );
  }

  __inline void decompose( Vec3 *scale, Vec3 *shear, Mat3x3 *rotate, Vec3 *translate ) const noexcept
  {
    XV_CORE_ASSERT_MSG( scale != nullptr && shear != nullptr && rotate != nullptr && translate != nullptr, "[Matrix] Passed nullptr as param" );

    //Translation is the last basis vector
    setX( translate, m[0][3] );
    setY( translate, m[1][3] );
    setZ( translate, m[2][3] );

    for ( auto i = 0; i < 3; ++i )
    {
      for ( auto j = 0; j < 3; ++j )
      {
        ( *rotate )[i][j] = m[i][j];
      }
    }
    //ScaleX is the magnitude of X'
    Vec3 tmp { m[0][0], m[1][0], m[2][0] };
    setX( scale, tmp.length() );
    //X' is normalized
    for ( auto i = 0; i < 3; ++i )
    {
      ( *rotate )[i][0] /= getX( scale );
    }

    //ShearXY is the dot product of X' and Y'
    tmp = Vec3 { ( *rotate )[0][0], ( *rotate )[1][0], ( *rotate )[2][0] };
    Vec3 tmp2 { ( *rotate )[0][1], ( *rotate )[1][1], ( *rotate )[2][1] };

    //ShearXY is the dot product of X' and Y'
    setZ( shear, dot( tmp, tmp2 ) );

    //Make Y' orthogonal to X' by " Y' = Y' - (ShearXY * X') "
    for ( auto i = 0; i < 3; ++i )
    {
      ( *rotate )[i][1] -= getZ( shear ) * ( *rotate )[i][0];
    }

    //ScaleY is the magnitude of the modified Y'
    tmp = Vec3 { m[0][1], m[1][1], m[2][1] };
    setY( scale, tmp.length() );
    //Y' is normalized
    for ( auto i = 0; i < 3; ++i )
    {
      ( *rotate )[i][1] /= getY( scale );
    }

    //ShearXY is divided by ScaleY to get it's final value
    setZ( shear, getZ( shear ) / getY( scale ) );
    //ShearYZ is the dot product of Y' and Z'
    tmp2 = Vec3 { m[0][2], m[1][2], m[2][2] };
    setX( shear, dot( tmp, tmp2 ) );
    //ShearXZ is the dot product of X' and Z'
    tmp = Vec3 { m[0][0], m[1][0], m[2][0] };
    setY( shear, dot( tmp, tmp2 ) );

    //Make Z' orthogonal to X' by " Z' = Z' - (ShearXZ * X') "
    for ( auto i = 0; i < 3; ++i )
    {
      ( *rotate )[i][2] -= getY( shear ) * ( *rotate )[i][0];
    }

    //Make Z' orthogonal to Y' by " Z' = Z' - (ShearYZ * Y') "
    for ( auto i = 0; i < 3; ++i )
    {
      ( *rotate )[i][2] -= getX( shear ) * ( *rotate )[i][1];
    }

    //ScaleZ is the magnitude of the modified Z'
    setZ( *scale, tmp2.length() );
    //Z' is normalized
    for ( auto i = 0; i < 3; ++i )
    {
      ( *rotate )[i][2] /= getZ( *scale );
    }

    //ShearXZ is divided by ScaleZ to get it's final value
    setY( shear, getY( shear ) / getZ( scale ) );

    //ShearYZ is divided by ScaleZ to get it's final value
    setX( shear, getX( shear ) / getZ( scale ) );
    tmp = Vec3 { ( *rotate )[1][1] * ( *rotate )[2][2] - ( *rotate )[2][1] * ( *rotate )[1][2],
        ( *rotate )[2][1] * ( *rotate )[0][2] - ( *rotate )[0][1] * ( *rotate )[2][2],
        ( *rotate )[0][1] * ( *rotate )[1][2] - ( *rotate )[1][1] * ( *rotate )[0][2] };
    tmp2 = Vec3 { ( *rotate )[0][0], ( *rotate )[1][0], ( *rotate )[2][0] };
    if ( dot( tmp, tmp2 ) < 0.0f )
    {
      ( *rotate ) = -( *rotate );
      negate( scale );
    }
  }

  Vec4 getRow1() const noexcept
  {
    return m[0];
  }

  Vec4 getRow2() const noexcept
  {
    return m[1];
  }

  Vec4 getRow3() const noexcept
  {
    return m[2];
  }

  Vec4 getRow4() const noexcept
  {
    return m[3];
  }

  void setRow1( const Vec4 row1 ) noexcept
  {
    m[0] = row1;
  }

  void setRow2( const Vec4 row2 ) noexcept
  {
    m[1] = row2;
  }

  void setRow3( const Vec4 row3 ) noexcept
  {
    m[2] = row3;
  }

  void setRow4( const Vec4 row4 ) noexcept
  {
    m[3] = row4;
  }

private:
  Vec4 m[4];
};

//--------------------------Mat3x3 and Mat4x4---------------------------------//
template <typename T>
concept IsMat = std::is_same<T, Mat3x3>::value || std::is_same<T, Mat4x4>::value;

template <IsMat Mat>
__inline Mat transpose( const Mat &m ) noexcept
{
  return m.transpose();
}

template <IsMat Mat>
__inline Mat transpose( const Mat *m ) noexcept
{
  XV_ASSERT_MSG( m != nullptr, "[Matrix] Passed in nullptr" );
  return m->transpose();
}

template <IsMat Mat>
__inline Mat identity( const Mat &m ) noexcept
{
  return m.identity();
}

template <IsMat Mat>
__inline Mat identity( const Mat *m ) noexcept
{
  XV_ASSERT_MSG( m != nullptr, "[Matrix] Passed in nullptr" );
  return m->identity();
}

template <IsMat Mat>
__inline Mat concat( const Mat &lhs, const Mat &rhs ) noexcept
{
  return lhs * rhs;
}

template <IsMat Mat>
__inline Mat concat( const Mat *lhs, const Mat *rhs ) noexcept
{
  XV_ASSERT_MSG( lhs != nullptr && rhs != nullptr, "[Matrix] Passed in nullptr" );
  return ( *lhs ) * ( *rhs );
}

template <IsMat Mat>
__inline void setToZero( Mat &m ) noexcept
{
  m.setToZero();
}

template <IsMat Mat>
__inline void setToZero( Mat *m ) noexcept
{
  XV_ASSERT_MSG( m != nullptr, "[Matrix] Passed in nullptr" );
  m->setToZero();
}

template <IsMat Mat>
__inline void setAll( Mat &m, const float val ) noexcept
{
  m.setAll( val );
}

template <IsMat Mat>
__inline void setAll( Mat *m, const float val ) noexcept
{
  XV_ASSERT_MSG( m != nullptr, "[Matrix] Passed in nullptr" );
  m->setAll( val );
}

template <IsMat Mat>
__inline Mat rotDeg( const Mat &m, const float angle ) noexcept
{
  return m.rotRad( angle );
}

template <IsMat Mat>
__inline Mat rotDeg( const Mat *m, const float angle ) noexcept
{
  XV_ASSERT_MSG( m != nullptr, "[Matrix] Passed in nullptr" );
  return m->rotRad( angle );
}

template <IsMat Mat>
__inline Mat rotRad( const Mat &m, const float rad ) noexcept
{
  return m.rotRad( rad );
}

template <IsMat Mat>
__inline Mat rotRad( const Mat *m, const float rad ) noexcept
{
  XV_ASSERT_MSG( m != nullptr, "[Matrix] Passed in nullptr" );
  return m->rotRad( rad );
}

__inline Mat3x3 translate( const Mat3x3 &m, const float x, const float y ) noexcept
{
  Mat3x3 result = identity( m );
  result[0][2] = x;
  result[1][2] = y;
  return result;
}

__inline Mat3x3 translate( const Mat3x3 *m, const float x, const float y ) noexcept
{
  XV_ASSERT_MSG( m != nullptr, "[Matrix] Passed in nullptr" );
  Mat3x3 result = identity( m );
  result[0][2] = x;
  result[1][2] = y;
  return result;
}

__inline Mat4x4 translate( const Mat4x4 &m, const float x, const float y, const float z ) noexcept
{
  Mat4x4 result = identity( m );
  result[0][3] = x;
  result[1][3] = y;
  result[2][3] = z;
  return result;
}

__inline Mat4x4 translate( const Mat4x4 *m, const float x, const float y, const float z ) noexcept
{
  XV_ASSERT_MSG( m != nullptr, "[Matrix] Passed in nullptr" );
  Mat4x4 result = identity( m );
  result[0][3] = x;
  result[1][3] = y;
  result[2][3] = z;
  return result;
}

__inline Mat3x3 scale( const Mat3x3 &m, const float x, const float y ) noexcept
{
  Mat3x3 result = identity( m );
  result[0][0] = x;
  result[1][1] = y;
  return result;
}

__inline Mat3x3 scale( const Mat3x3 *m, const float x, const float y ) noexcept
{
  XV_ASSERT_MSG( m != nullptr, "[Matrix] Passed in nullptr" );
  Mat3x3 result = identity( m );
  result[0][0] = x;
  result[1][1] = y;
  return result;
}

__inline Mat4x4 scale( const Mat4x4 &m, const float x, const float y, const float z ) noexcept
{
  Mat4x4 result = identity( m );
  result[0][0] = x;
  result[1][1] = y;
  result[2][2] = z;
  return result;
}

__inline Mat4x4 scale( const Mat4x4 *m, const float x, const float y, const float z ) noexcept
{
  XV_ASSERT_MSG( m != nullptr, "[Matrix] Passed in nullptr" );
  Mat4x4 result = identity( m );
  result[0][0] = x;
  result[1][1] = y;
  result[2][2] = z;
  return result;
}

template <IsMat Mat>
__inline Mat scale( const Mat &m, const float val ) noexcept
{
  return m.scale( val );
}

template <IsMat Mat>
__inline Mat scale( const Mat *m, const float val ) noexcept
{
  XV_ASSERT_MSG( m != nullptr, "[Matrix] Passed in nullptr" );
  return m->scale( val );
}

__inline Vec3 transposedTransform( const Mat3x3 &matrix, const Vec3 &vector ) noexcept
{
  return Vec3 { dot( matrix.basis( 0 ), vector ), dot( matrix.basis( 1 ), vector ), dot( matrix.basis( 2 ), vector ) };
}

__inline Mat3x3 toMat3x3( const Mat4x4 &m ) noexcept
{
  Mat3x3 result { 0.0f };
  for ( auto i = 0; i < 3; ++i )
  {
    for ( auto j = 0; j < 3; ++j )
    {
      result[i][j] = m[i][j];
    }
  }
  return result;
}

__inline void toMat3x3( const Mat4x4 &m4x4, Mat3x3 &m3x3 ) noexcept
{
  for ( auto i = 0; i < 3; ++i )
  {
    for ( auto j = 0; j < 3; ++j )
    {
      m3x3[i][j] = m4x4[i][j];
    }
  }
}

__inline void toMat3x3( const Mat4x4 &m4x4, Mat3x3 *m3x3 ) noexcept
{
  XV_ASSERT_MSG( m3x3 != nullptr, "[Matrix] Passed in nullptr" );
  for ( auto i = 0; i < 3; ++i )
  {
    for ( auto j = 0; j < 3; ++j )
    {
      ( *m3x3 )[i][j] = m4x4[i][j];
    }
  }
}

template <IsMat Mat>
__inline void negate( Mat &m ) noexcept
{
  m = -m;
}

template <IsMat Mat>
__inline Mat negated( const Mat &m ) noexcept
{
  return -m;
}

template <IsMat Mat>
__inline void negate( Mat *m ) noexcept
{
  XV_ASSERT_MSG( m != nullptr, "[Matrix] Passed in nullptr" );
  *m = -( *m );
}

template <IsMat Mat>
__inline Mat negated( const Mat *m ) noexcept
{
  XV_ASSERT_MSG( m != nullptr, "[Matrix] Passed in nullptr" );
  return -( *m );
}

__inline Vec3 transformPoint( const Mat4x4 &m4x4, const Vec3 &normal )
{
  auto tmp = toMat3x3( m4x4 );
  return Vec3 { dot( tmp[0], normal ) + m4x4[0][3], dot( tmp[1], normal ) + m4x4[1][3], dot( tmp[2], normal ) + m4x4[2][3] };
}

__inline Vec3 transformNormal( const Mat4x4 &m4x4, const Vec3 &normal )
{
  auto tmp = toMat3x3( m4x4 );
  return Vec3 { dot( tmp[0], normal ), dot( tmp[1], normal ), dot( tmp[2], normal ) };
}

bool inverse( const Mat3x3 input, Mat3x3 &output ) noexcept;
bool inverse( const Mat4x4 input, Mat4x4 &output ) noexcept;
}