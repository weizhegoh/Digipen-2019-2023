/*****************************************************************
*\file         CreatePrefabInstanceCommand.h
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
struct CreatePrefabInstanceCommand : ICommand
{
  const Position m_position;
  ECS::World::Instance &m_world;
  const Entity m_create_from;
  Entity &m_return_ref;
  Entity m_created;
  std::vector<Ptr<Entity::Descriptor>> m_entries;
  std::function<void( Entity )> m_recorder;

  CreatePrefabInstanceCommand( ECS::World::Instance &,
                               const Entity &,
                               const Position &,
                               Entity & );

  static Ptr<ICommand> Create( ECS::World::Instance &,
                               const Entity &,
                               const Position &,
                               Entity & );

  void Execute() override;
  void Undo() override;
};
}
#endif