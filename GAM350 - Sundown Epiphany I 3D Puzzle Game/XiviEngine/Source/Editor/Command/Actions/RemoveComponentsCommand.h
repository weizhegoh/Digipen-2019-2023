/*****************************************************************
*\file         RemoveComponentsCommand.h
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
#include "../DestroyInfo.h"

namespace Xivi::Editor
{
struct RemoveComponentsCommand : ICommand
{
  ECS::World::Instance &m_world;
  Entity m_entity;
  DestroyInfo m_destroy_info;

  RemoveComponentsCommand( ECS::World::Instance &world,
                           Entity &entity,
                           DestroyInfo &&destroy_info );

  static Ptr<ICommand> Create( ECS::World::Instance &world, Entity &entity, ECS::DescriptorSpan remove );

  void Execute() override;

  void Undo() override;
};
}
#endif