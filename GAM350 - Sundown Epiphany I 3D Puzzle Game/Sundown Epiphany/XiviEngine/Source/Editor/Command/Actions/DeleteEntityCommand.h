/*****************************************************************
*\file         DeleteEntityCommand.h
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
struct DeleteEntityCommand : ICommand
{
  static Ptr<ICommand> Create( ECS::World::Instance &world, Entity &entity );
};
}
#endif