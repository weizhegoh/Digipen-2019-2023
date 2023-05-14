#ifndef POINT4_H
#define POINT4_H


////////////////////////////////////////////////////////////////////////////////
// DO NOT MODIFY THIS FILE FOR ASSIGNMENT 1
////////////////////////////////////////////////////////////////////////////////


struct Vector4;

struct AE_API Point4
{
  union
  {
    struct 
    {
      f32 x, y, z, w;
    };
  
    struct  
    {
      f32 r, g, b, a;
    };
  
    f32 v[4];
  };
  
  /* 
    This union lets us access the data in multiple ways
    All of these are modifying the same location in memory
  
    Point4 pnt;
    pnt.z = 1.0f;
    pnt.b = 2.0f;
    pnt.v[2] = 3.0f;
  */
  
    // Default constructor, sets x,y,z to zero and w to the defined value
  Point4(void);
  
    // Copy constructor, copies every component from the other Point4
  Point4(const Point4& rhs);
  
    // Non-Default constructor, self-explanatory
  Point4(f32 xx, f32 yy, f32 zz, f32 ww = 1.0f);
  
    // Assignment operator, copies every component from the other Point4
  Point4& operator=(const Point4& rhs);
  
    // Unary negation operator, negates every component and returns a copy
  Point4 operator-(void) const;
  
    // Binary subtraction operator, Subtract two Point4s and you get a Vector4
  Vector4 operator-(const Point4&  rhs) const;
  
    // Basic vector math operations with points, you can add a Vector4 to a Point4, or subtract
    // a Vector4 from a Point4
  Point4  operator+ (const Vector4& rhs) const;
  Point4  operator- (const Vector4& rhs) const;
  
    // Same as previous two operators, just modifies the original instead of returning a copy
  Point4& operator+=(const Vector4& rhs);
  Point4& operator-=(const Vector4& rhs);
  
    // Comparison operators which should use an epsilon defined in 
    // Utilities.h to see if the value is in a certain range
    // in which case we say they are equivalent.
  bool operator==(const Point4& rhs) const;
  bool operator!=(const Point4& rhs) const;
  
    // Sets x,y,z to zeroes, w to defined value
  void Zero(void);
  
    // Already implemented, simple print function
  void Print(void) const;
};



#endif