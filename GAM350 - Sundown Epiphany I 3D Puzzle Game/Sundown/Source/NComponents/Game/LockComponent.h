/*****************************************************************
*\file         LockComponenet.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
DefineDataComponent( Lock );
bool activated { false };
EndDefine();
}

namespace Xivi::Components::Lock::Reflect
{
RTTR_REGISTRATION
{
rttr::registration::class_<Xivi::Lock>( std::string( Xivi::Lock::info.m_name ) )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::Lock>.m_id ) )
.property( "Activated", &Xivi::Lock::activated )( rttr::policy::prop::as_reference_wrapper )
;
}
}