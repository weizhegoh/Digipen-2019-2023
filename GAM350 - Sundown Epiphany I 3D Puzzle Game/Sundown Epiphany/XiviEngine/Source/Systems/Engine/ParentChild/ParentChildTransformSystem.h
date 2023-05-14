/*****************************************************************
*\file         ParentChildTransformSystem.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
DefineGlobalEventSystem( ParentChildTransformSystem, ParentChildLinkCallbacks );

void OnEvent( const Entity &child, const Entity &parent );

EndDefine();
}