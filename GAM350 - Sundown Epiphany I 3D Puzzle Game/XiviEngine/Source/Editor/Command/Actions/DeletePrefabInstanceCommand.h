/*****************************************************************
*\file         DeletePrefabInstanceCommand.h
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
#include "../CacheInfo.h"

namespace Xivi::Editor
{
struct DeletePrefabInstanceCommand : ICommand
{
  ECS::World::Instance &m_world;
  const Entity m_entity;
  std::vector<CacheInfo> m_cached;

  DeletePrefabInstanceCommand( ECS::World::Instance &world,
                               Entity &entity,
                               std::vector<CacheInfo> &&cached );

  static Ptr<ICommand> Create( ECS::World::Instance &world, Entity &entity, std::vector<CacheInfo> &&cache );

  void Execute() override;

  void Undo() override;
};
}
#endif