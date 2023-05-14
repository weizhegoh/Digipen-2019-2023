/*****************************************************************
*\file         CreateEntityFromArchetypeCommand.h
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

namespace Xivi
{
using ArchetypeCreateFunc = Func<Entity( const Position & )>;
}

namespace Xivi::Editor
{
struct CreateEntityFromArchetypeCommand : ICommand
{
  const Position m_position;
  ECS::World::Instance &m_world;
  const ArchetypeCreateFunc m_create_func;
  Entity &m_return_ref;
  Entity m_created;

  CreateEntityFromArchetypeCommand( ECS::World::Instance &world,
                                    const ArchetypeCreateFunc &create_func,
                                    const Position &position,
                                    Entity &return_ref );

  static Ptr<ICommand> Create( ECS::World::Instance &,
                               const ArchetypeCreateFunc &,
                               const Position &,
                               Entity & );
  void Execute() override;
  void Undo() override;
};
}
#endif