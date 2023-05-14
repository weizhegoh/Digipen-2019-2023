/*****************************************************************
*\file         UpdateComponentCommand.h
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
struct UpdateComponentCommand : ICommand
{
  ECS::World::Instance &m_world;
  const Entity m_entity;
  const ECS::Component::Descriptor &m_descriptor;
  const UniquePtr<byte[]> m_component_initial_snapshot;
  const UniquePtr<byte[]> m_component_update_snapshot;

  UpdateComponentCommand( ECS::World::Instance &world,
                          Entity entity,
                          const ECS::Component::Descriptor &descriptor,
                          UniquePtr<byte[]> &&component_initial_snapshot,
                          UniquePtr<byte[]> &&component_update_snapshot );

  ~UpdateComponentCommand();

  static Ptr<ICommand> Create( ECS::World::Instance &world, Entity &entity, const ECS::Component::Descriptor &descriptor, byte &component_initial_snapshot );

  void Execute() override;

  void Undo() override;
};
}
#endif