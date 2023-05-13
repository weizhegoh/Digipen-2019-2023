/*****************************************************************
*\file         CacheInfo.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#ifdef XV_EDITOR
#include "DestroyInfo.h"
#include "Xivi/ECS/Components/Base/Entity.h"

namespace Xivi::Editor
{
struct CacheInfo
{
  Ptr<ECS::Archetype::Instance> m_archetype;
  DestroyInfo m_destroy;
  Entity::Descriptor m_entry;
  u32 m_global_index;
};
}
#endif