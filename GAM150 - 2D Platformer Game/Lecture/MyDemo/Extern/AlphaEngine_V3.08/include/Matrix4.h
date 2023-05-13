#ifndef MATRIX4_H
#define MATRIX4_H


////////////////////////////////////////////////////////////////////////////////
// DO NOT MODIFY THIS FILE FOR ASSIGNMENTS
////////////////////////////////////////////////////////////////////////////////

struct Point4;
struct Vector4;

struct AE_API Matrix4
{
  union
  {
    struct  
    {
      f32 m00, m01, m02, m03,
          m10, m11, m12, m13,
          m20, m21, m22, m23,
          m30, m31, m32, m33;
    };
  
    f32 m[4][4];
    f32 v[16];
  };
  
  /* 
    This union lets us access the data in multiple ways
    All of these are modifying the same location in memory
  
    Matrix4 mtx;
    mtx.m[2][2] = 1.0f;
    mtx.v[10] = 2.0f;
    mtx.m22 = 3.0f;
  */
  
    // Default constructor should initialize to zeroes
  Matrix4(void);
  
    // Copy constructor, copies every entry from the other matrix.
  Matrix4(const Matrix4& rhs);
  
    // Non-default constructor, self-explanatory
  Matrix4(f32 mm00, f32 mm01, f32 mm02, f32 mm03,
          f32 mm10, f32 mm11, f32 mm12, f32 mm13,
          f32 mm20, f32 mm21, f32 mm22, f32 mm23,
          f32 mm30, f32 mm31, f32 mm32, f32 mm33);
  
    // Assignment operator, does not need to handle self-assignment
  Matrix4& operator=(const Matrix4& rhs);
  
    // Multiplying a Matrix4 with a Vector4 or a Point4
  Vector4 operator*(const Vector4& rhs) const;
  Point4  operator*(const Point4& rhs) const;
  
    // Basic Matrix arithmetic operations
  Matrix4 operator+(const Matrix4& rhs) const;
  Matrix4 operator-(const Matrix4& rhs) const;
  Matrix4 operator*(const Matrix4& rhs) const;
  
    // Similar to the three above except they modify
    // the original 
  Matrix4& operator+=(const Matrix4& rhs);
  Matrix4& operator-=(const Matrix4& rhs);
  Matrix4& operator*=(const Matrix4& rhs);
  
    // Scale/Divide the entire matrix by a float
  Matrix4 operator*(const f32 rhs) const;
  Matrix4 operator/(const f32 rhs) const;
  
    // Same as previous 
  Matrix4& operator*=(const f32 rhs);
  Matrix4& operator/=(const f32 rhs);
  
    // Comparison operators which should use an epsilon defined in 
    // Utilities.h to see if the value is in a certain range
    // in which case we say they are equivalent.
  bool operator==(const Matrix4& rhs) const;
  bool operator!=(const Matrix4& rhs) const;
  
    // Zeroes out the entire matrix
  void Zero(void);
  
    // Builds the identity matrix
  void Identity(void);
  
    // Already implemented, simple print function
  void Print(void) const;
};



#endif