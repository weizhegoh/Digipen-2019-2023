/*****************************************************************
*\file         SettingsPanel.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "IPanel.h"

namespace Xivi
{
struct TimerEngine;
}

namespace Xivi::Editor
{
struct SettingsPanel : public IPanel
{
  SettingsPanel( std::string name,
                 Ptr<ECS::World::Instance> world,
                 Ptr<Config> configs,
                 Ptr<EntityContext> entity_context,
                 Ptr<FileContext> file_context,
                 Ptr<ToolContext> tool_context );

  ~SettingsPanel();

  void Render() noexcept override;

private:
  Ptr<TimerEngine> m_timer_engine { nullptr };
};
}
#endif