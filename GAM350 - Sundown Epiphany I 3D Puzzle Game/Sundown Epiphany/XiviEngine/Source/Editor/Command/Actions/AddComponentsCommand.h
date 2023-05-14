/*****************************************************************
*\file         AddComponentsCommand.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "../ICommand.h"
#include "Xivi/ECS/XVECS.h"
namespace Xivi::Editor
{
struct AddComponentsCommand : ICommand
{
  ECS::World::Instance &m_world;
  Entity m_entity;
  std::vector<Ptr<const ECS::Component::Descriptor>> m_add;

  AddComponentsCommand( ECS::World::Instance &world,
                        Entity &entity,
                        ECS::DescriptorSpan add );

  static Ptr<ICommand> Create( ECS::World::Instance &, Entity &, ECS::DescriptorSpan );
  void Execute() override;
  void Undo() override;
};
}
#endif