/*****************************************************************
*\file         NewTransform.h
*\author(s)    Tan tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "Xivi/Core/Utils/Math.h"

namespace Xivi
{
DefineDataComponent( NewTransform );
Vec3f scale;
Vec3f prevScale;
f32 speed;
f32 ratio;
EndDefine();
}

namespace Xivi::Components::NewTransform::Reflect
{
RTTR_REGISTRATION
{
rttr::registration::class_<Xivi::NewTransform>( std::string( Xivi::NewTransform::info.m_name ) )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::NewTransform>.m_id ) )
.property( "Scale Speed", &Xivi::NewTransform::speed )( rttr::policy::prop::as_reference_wrapper );
;
}
}