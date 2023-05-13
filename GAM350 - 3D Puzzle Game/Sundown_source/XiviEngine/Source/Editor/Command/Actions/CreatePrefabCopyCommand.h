/*****************************************************************
*\file         CreatePrefabCopyCommand.h
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
struct CreatePrefabCopyCommand : ICommand
{
  const Position m_position;
  ECS::World::Instance &m_world;
  const Entity m_create_from;
  Entity &m_return_ref;
  Entity m_created;
  std::vector<Ptr<Entity::Descriptor>> m_entries;
  std::function<void( Entity )> m_recorder;
  std::function<void( Entity/*, Ptr<const ECS::Component::Descriptor>, Ptr<const ECS::Component::Descriptor>*/ )> m_recur_remove_prefab;
  std::function<Entity( Entity )> m_recur_create;
  ECS::Signature m_prefab_sig;

  CreatePrefabCopyCommand( ECS::World::Instance &,
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