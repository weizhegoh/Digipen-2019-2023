/*****************************************************************
*\file         Waypoints.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "Xivi/Core/Utils/Math.h"

namespace Xivi
{
DefineDataComponent( Waypoints );
vector<Vec3i> moving_waypoints;
i32 current_index;
bool reverse;

Vec3i GetNextWaypoint()
{
  if ( moving_waypoints.empty() )
    return Vec3i( 0, 0, 0 );

  reverse ? --current_index : ++current_index;

  if ( reverse && current_index < 0 )
  {
    current_index += 2;
    reverse = false;
  }
  else if ( !reverse && current_index >= moving_waypoints.size() )
  {
    current_index -= 2;
    reverse = true;
  }

  return moving_waypoints[current_index];
}

Vec3i GetNextDirection()
{
  i32 index( 0 );
  if ( reverse )
  {
    index = current_index - 1;
    if ( index < 0 )
      index += 2;
  }
  else
  {
    index = current_index + 1;
    if ( index >= moving_waypoints.size() )
      index -= 2;
  }

  return moving_waypoints[index] - moving_waypoints[current_index];
}

EndDefine();
}

namespace Xivi::Components::Waypoints::Reflect
{
RTTR_REGISTRATION
{
rttr::registration::class_<Xivi::Waypoints>( std::string( Xivi::Waypoints::info.m_name ) )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::Waypoints>.m_id ) )
.property( "Moving Waypoints", &Xivi::Waypoints::moving_waypoints )
.property( "Current Index", &Xivi::Waypoints::current_index )( rttr::policy::prop::as_reference_wrapper )
.property( "Reverse", &Xivi::Waypoints::reverse )( rttr::policy::prop::as_reference_wrapper )
;
}
}
