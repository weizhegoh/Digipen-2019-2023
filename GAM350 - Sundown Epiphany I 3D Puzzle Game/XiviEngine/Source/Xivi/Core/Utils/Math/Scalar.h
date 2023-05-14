/*****************************************************************
*\file         Scalar.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include <Types/Base.h>

namespace Xivi
{
using scalar = f32;
static constexpr scalar scalar_epsilon = FLT_EPSILON;
static constexpr scalar scalar_min = FLT_MIN;
static constexpr scalar scalar_max = FLT_MAX;

struct Scalar
{
  scalar value;

  operator scalar &( )
  {
    return value;
  }

  operator scalar() const
  {
    return value;
  }
};
}