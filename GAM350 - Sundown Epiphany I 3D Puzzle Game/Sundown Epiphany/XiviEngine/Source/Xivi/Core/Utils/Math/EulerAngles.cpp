///////////////////////////////////////////////////////////////////////////////
///
///  \file EulerAngles.cpp
///  Implementation of the EulerAngle structure, design referenced from
///  Insomniac Games.
///
///  Authors: Benjamin Strukus
///  Copyright 2010-2012, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "EulerAngles.hpp"
#include "MathFunctions.hpp"

namespace Xivi::Math
{
//----------------------------------------------------------------- Euler Angles
EulerAngles::EulerAngles( EulerOrderParam order )
  : Order( order )
{
  //
}

EulerAngles::EulerAngles( Vec3Param xyzRotation, EulerOrderParam order )
  : Angles( xyzRotation ), Order( order )
{
  //
}

EulerAngles::EulerAngles( float xRotation, float yRotation, float zRotation,
                          EulerOrderParam order )
  : Angles( xRotation, yRotation, zRotation ), Order( order )
{
  //
}

EulerAngles::EulerAngles( Mat3Param matrix, EulerOrderParam order )
  : Order( order )
{
  ToEulerAngles( matrix, this );
}

EulerAngles::EulerAngles( Mat4Param matrix, EulerOrderParam order )
  : Order( order )
{
  ToEulerAngles( matrix, this );
}

EulerAngles::EulerAngles( QuatParam quaternion, EulerOrderParam order )
  : Order( order )
{
  ToEulerAngles( quaternion, this );
}

float EulerAngles::operator [] ( unsigned index ) const
{
  return Angles[index];
}

float &EulerAngles::operator [] ( unsigned index )
{
  return Angles[index];
}

float EulerAngles::I() const
{
  return Angles[Order.I()];
}

float EulerAngles::J() const
{
  return Angles[Order.J()];
}

float EulerAngles::K() const
{
  return Angles[Order.K()];
}

float EulerAngles::H() const
{
  return Angles[Order.H()];
}

void EulerAngles::I( float i )
{
  Angles[Order.I()] = i;
}

void EulerAngles::J( float j )
{
  Angles[Order.J()] = j;
}

void EulerAngles::K( float k )
{
  Angles[Order.K()] = k;
}

void EulerAngles::H( float h )
{
  Angles[Order.H()] = h;
}

void EulerAngles::Reorder( EulerOrderParam newOrder )
{
  Order = newOrder;
}
}// namespace Xivi::Math