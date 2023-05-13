/*****************************************************************
*\file         EventCreateSkyBox.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "Xivi/ECS/XVECS.h"
#include "Components/EngineComponents.h"
#include "Xivi/Core/Declaration.h"

namespace Xivi::Editor
{
struct EventCreateSkyBoxSystem
{
  static void Init( ECS::World::Instance &world ) noexcept;
  static void OnCall( ECS::World::Instance &world, Ptr<RM> resource_manager ) noexcept;

private:
  inline static Ptr<ECS::Archetype::Instance> m_skybox_archetype;
};
}
#endif