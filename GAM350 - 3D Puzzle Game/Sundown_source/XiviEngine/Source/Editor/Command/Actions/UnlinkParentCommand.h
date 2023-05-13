/*****************************************************************
*\file         UnlinkParentCommand.h
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
struct UnlinkParentCommand : ICommand
{
  ECS::World::Instance &m_world;
  Entity m_child;
  Entity m_parent;

  UnlinkParentCommand( ECS::World::Instance &world,
                       Entity &child,
                       Entity &parent );

  static Ptr<ICommand> Create( ECS::World::Instance &world, Entity &child );

  void Execute() override;

  void Undo() override;
};
}
#endif