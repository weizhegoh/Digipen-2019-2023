/*****************************************************************
*\file         DestroyInfo.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#ifdef XV_EDITOR
#include <vector>
#include "Types/Base.h"
#include "Xivi/ECS/Component.h"

namespace Xivi::Editor
{
struct DestroyInfo
{
  std::vector<Ptr<const ECS::Component::Descriptor>> m_descriptors;
  std::vector<UniquePtr<byte[]>> m_components_cache;
};
}
#endif