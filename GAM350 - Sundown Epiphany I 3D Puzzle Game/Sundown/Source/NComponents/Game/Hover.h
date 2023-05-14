/*****************************************************************
*\file         Hover.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
enum class HoverType
{
  EaseIn,
  EaseOut,
  EaseInOut,
  Normal
};

DefineDataComponent( Hover );
Vec3f start;
Vec3f end;
Vec3f rotate_dir;
f32 linear_speed;
f32 rotate_speed;
f32 rotate_speed_start;
f32 rotate_speed_end;
f32 linear_ratio;
f32 rotate_ratio;
HoverType linear_type;
HoverType rotate_type;
EndDefine();
}

namespace Xivi::Components::Hover::Reflect
{
RTTR_REGISTRATION
{
rttr::registration::enumeration<HoverType>( "Hover Type" )
(
rttr::value( "Ease In", HoverType::EaseIn ),
rttr::value( "Ease Out", HoverType::EaseOut ),
rttr::value( "Ease In Out", HoverType::EaseInOut ),
rttr::value( "Normal", HoverType::Normal )
);

rttr::registration::class_<Xivi::Hover>( std::string( Xivi::Hover::info.m_name ) )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::Hover>.m_id ) )
.property( "Start", &Xivi::Hover::start )( rttr::policy::prop::as_reference_wrapper )
.property( "End", &Xivi::Hover::end )( rttr::policy::prop::as_reference_wrapper )
.property( "Rotate Direction", &Xivi::Hover::rotate_dir )( rttr::policy::prop::as_reference_wrapper )
.property( "Linear Speed", &Xivi::Hover::linear_speed )( rttr::policy::prop::as_reference_wrapper )
.property( "Linear Type", &Xivi::Hover::linear_type )
.property( "Rotate Speed", &Xivi::Hover::rotate_speed )( rttr::policy::prop::as_reference_wrapper )
.property( "Rotate Speed Start", &Xivi::Hover::rotate_speed_start )( rttr::policy::prop::as_reference_wrapper )
.property( "Rotate Speed End", &Xivi::Hover::rotate_speed_end )( rttr::policy::prop::as_reference_wrapper )
.property( "Rotate Type", &Xivi::Hover::rotate_type )
;
}
}
