/*****************************************************************
*\file         ResetPrefabInstanceCommand.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "../ICommand.h"
#include "Xivi/ECS/XVECS.h"
#include "Components/EngineComponents.h"

namespace Xivi::Editor
{
struct System; // Forward declared
struct ResetPrefabInstanceCommand : ICommand
{
  ECS::World::Instance &m_world;
  Entity m_old_prefab_inst;
  Entity &m_new_prefab_inst;

  Ptr<System> m_editor_sys;

  ResetPrefabInstanceCommand( ECS::World::Instance &world,
                              Entity prefab_inst,
                              Entity &new_inst );

  static Ptr<ICommand> Create( ECS::World::Instance &world, Entity prefab_inst,
                               Entity &new_inst );

  void Execute() override;

  void Undo() override;
};
}
#endif