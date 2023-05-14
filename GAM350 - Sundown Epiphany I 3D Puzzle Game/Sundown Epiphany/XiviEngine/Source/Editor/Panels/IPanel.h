/*****************************************************************
*\file         IPanel.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include <string>
#include <Types/Base.h>
#include "Editor/Config.h"
#include "Editor/Contexts.h"

namespace Xivi::ECS::World
{
struct Instance;
}

namespace Xivi::Editor
{
struct System;
struct Config;
struct EntityContext;
struct FileContext;
struct ToolContext;

struct IPanel
{
  IPanel( std::string name,
          Ptr<ECS::World::Instance> world,
          Ptr<Config> configs,
          Ptr<EntityContext> entity_context,
          Ptr<FileContext> file_context,
          Ptr<ToolContext> tool_context ) :
    m_world { world },
    m_configs { configs },
    m_entity_context { entity_context },
    m_file_context { file_context },
    m_tool_context { tool_context },
    m_enabled { true },
    m_name { name }
  {
  }

  virtual ~IPanel() noexcept = default;
  virtual void Render() noexcept = 0;

  std::string GetName() const noexcept
  {
    return m_name;
  }

  void ToggleEnable() noexcept
  {
    m_enabled = !m_enabled;
  }

  bool IsEnabled() const noexcept
  {
    return m_enabled;
  }

protected:
  Ptr<ECS::World::Instance> m_world;
  Ptr<Config> m_configs;
  Ptr<EntityContext> m_entity_context;
  Ptr<FileContext> m_file_context;
  Ptr<ToolContext> m_tool_context;
  bool m_enabled;
  std::string m_name;
};
}
#endif