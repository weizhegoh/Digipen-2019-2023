/*****************************************************************
*\file         PressurePlate.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
DefineDataComponent( PressurePlateC );
i32 counter { 1 };
i32 currentCounter { 0 };
EndDefine();
}

namespace Xivi::Components::PressurePlateC::Reflect
{
RTTR_REGISTRATION
{
rttr::registration::class_<Xivi::PressurePlateC>( std::string( Xivi::PressurePlateC::info.m_name ) )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::PressurePlateC>.m_id ) )
.property( "Counter", &Xivi::PressurePlateC::counter )( rttr::policy::prop::as_reference_wrapper )
;
}
}