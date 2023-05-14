/*****************************************************************
*\file         CreateEntityFromAssetCommand.h
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
using AssetCreateFunc = Func<Entity( const Position &, const std::string & )>;
}

namespace Xivi::Editor
{
struct CreateEntityFromAssetCommand : ICommand
{
  const Position m_position;
  ECS::World::Instance &m_world;
  const AssetCreateFunc m_create_func;
  const std::string m_asset;
  Entity &m_return_ref;
  Entity m_created;

  CreateEntityFromAssetCommand( ECS::World::Instance &,
                                const AssetCreateFunc &,
                                const Position &,
                                const std::string &,
                                Entity & );

  static Ptr<ICommand> Create( ECS::World::Instance &,
                               const AssetCreateFunc &,
                               const Position &,
                               const std::string &,
                               Entity & );
  void Execute() override;
  void Undo() override;
};
}
#endif