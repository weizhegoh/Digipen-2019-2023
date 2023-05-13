/*****************************************************************
*\file         CreateEntityFromAssetCommand.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "CreateEntityFromAssetCommand.h"

namespace Xivi::Editor
{
CreateEntityFromAssetCommand::CreateEntityFromAssetCommand( ECS::World::Instance &world,
                                                            const AssetCreateFunc &create_func,
                                                            const Position &position,
                                                            const std::string &asset,
                                                            Entity &return_ref ) : m_world( world ),
  m_create_func( create_func ),
  m_asset( asset ),
  m_position( position ),
  m_return_ref( return_ref )
{}

Ptr<ICommand> CreateEntityFromAssetCommand::Create( ECS::World::Instance &world,
                                                    const AssetCreateFunc &create_func,
                                                    const Position &pos,
                                                    const std::string &asset,
                                                    Entity &ref_entity )
{
  auto command = new CreateEntityFromAssetCommand( world, create_func, pos, asset, ref_entity );
  command->Execute();
  return command;
}
void CreateEntityFromAssetCommand::Execute()
{
  m_return_ref = m_created = m_create_func( m_position, m_asset );
  m_world.m_archetype_manager.UpdateStructuralChanges();
}

void CreateEntityFromAssetCommand::Undo()
{
  auto &entry = m_world.m_entity_manager.Get( m_created );
  m_world.DestroyEntity( m_created );
  m_world.m_archetype_manager.UpdateStructuralChanges();
  --entry.m_validation.m_generation;
  m_return_ref = m_created = Entity {};
}
}
#endif