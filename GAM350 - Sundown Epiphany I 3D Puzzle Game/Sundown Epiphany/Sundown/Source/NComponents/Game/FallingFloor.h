/*****************************************************************
*\file         FallingFloor.h
*\author(s)    Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
DefineDataComponent( FallingFloor );
i32 counter { 0 };
EndDefine();
}

namespace Xivi::Components::FallingFloor::Reflect
{
RTTR_REGISTRATION
{
rttr::registration::class_<Xivi::FallingFloor>( std::string( Xivi::FallingFloor::info.m_name ) )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::FallingFloor>.m_id ) )
.property( "Counter", &Xivi::FallingFloor::counter )( rttr::policy::prop::as_reference_wrapper )
;
}
}