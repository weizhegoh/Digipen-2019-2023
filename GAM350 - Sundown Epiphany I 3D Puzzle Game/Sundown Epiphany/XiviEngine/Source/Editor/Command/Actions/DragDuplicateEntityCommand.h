/*****************************************************************
*\file         DragDuplicateEntityCommand.h
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
struct DragDuplicateEntityCommand : ICommand
{
  ECS::World::Instance &m_world;
  const Entity m_clone_from;
  const ECS::Component::Descriptor &m_descriptor;
  UniquePtr<byte[]> m_data;
  Entity &m_return_ref;
  Entity m_created;
  std::vector<Ptr<Entity::Descriptor>> m_entries;
  std::function<void( Entity )> m_recorder;

  DragDuplicateEntityCommand( ECS::World::Instance &world,
                              const Entity &clone_from,
                              const ECS::Component::Descriptor &descriptor,
                              UniquePtr<byte[]> &&data,
                              Entity &return_ref );

  ~DragDuplicateEntityCommand();

  static Ptr<ICommand> Create( ECS::World::Instance &world, const Entity &duplicate_from, const ECS::Component::Descriptor &descriptor, byte &data, Entity &return_ref );

  void Execute() override;

  void Undo() override;
};
}
#endif