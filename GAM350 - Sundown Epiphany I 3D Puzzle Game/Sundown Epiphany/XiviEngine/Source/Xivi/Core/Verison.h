/*****************************************************************
*\file         Version.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "Xivi/Core/Core.h"
#include "Xivi/Core/Utils/Types.h"
#include <sstream>

#define V_MAJOR 1
#define V_MINOR 0
#define V_PATCH 0

namespace Xivi::Version
{
namespace Details
{
constexpr u32 MinorMax { 100 };
constexpr u32 PatchMax { 100 };
}

constexpr u32 Major()
{
  return V_MAJOR;
}
constexpr u32 Minor()
{
  return V_MINOR;
}
constexpr u32 Patch()
{
  return V_PATCH;
}
constexpr u32 Version()
{
  static_assert( Minor() < Details::MinorMax, "Version Minor should be < MinorMax" );
  static_assert( Patch() < Details::PatchMax, "Version Patch should be < PatchMax" );
  return Details::PatchMax * ( Details::MinorMax * Major() + Minor() ) + Patch();
}

constexpr std::string VersionString()
{
  return std::string( "V" ) +
    XV_STRINGIFY( V_MAJOR ) +
    "." +
    XV_STRINGIFY( V_MINOR ) +
    "." +
    XV_STRINGIFY( V_PATCH );
}
}