/*****************************************************************
*\file         PrefabInstanceOverrideSystem.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "Xivi/ECS/XVECS.h"
#include "Components/EngineComponents.h"

namespace Xivi::Editor
{
struct PrefabInstanceOverrideSystem
{
  static void OnCall( ECS::World::Instance &world, const Entity &entity, const ECS::Component::Descriptor &descriptor ) noexcept;
};
}
#endif