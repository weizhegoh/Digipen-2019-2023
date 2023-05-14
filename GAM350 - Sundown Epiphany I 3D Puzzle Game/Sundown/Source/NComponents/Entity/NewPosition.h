/*****************************************************************
*\file         NewPosition.h
*\author(s)    Tan tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "Xivi/Core/Utils/Math.h"

namespace Xivi
{
DefineDataComponent( NewPosition );
Vec3f value;
Vec3f bezier_point0;
Vec3f bezier_point1;
f32 speed { 1.0f };
f32 ratio { 0.0f };
EndDefine();
}

namespace Xivi::Components::NewPosition::Reflect
{
RTTR_REGISTRATION
{
rttr::registration::class_<Xivi::NewPosition>( std::string( Xivi::NewPosition::info.m_name ) )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::NewPosition>.m_id ) )
//.property( "New Position", &Xivi::NewPosition::value )( rttr::metadata( "NoEditor", true ) )
.property( "Speed", &Xivi::NewPosition::speed )( ( rttr::policy::prop::as_reference_wrapper ) );
}
}