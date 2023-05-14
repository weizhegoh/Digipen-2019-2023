/*****************************************************************
*\file         GraphicsEvents.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct RelinkComponentEvent : ECS::Event::Instance<>
{};

using Graphics_EList = EList<RelinkComponentEvent>;
}