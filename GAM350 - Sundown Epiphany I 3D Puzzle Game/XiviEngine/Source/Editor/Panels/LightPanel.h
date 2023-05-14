/*****************************************************************
*\file         LightPanel.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "IPanel.h"

namespace Xivi::Editor
{
struct LightPanel : public IPanel
{
  LightPanel( std::string name,
              Ptr<ECS::World::Instance> world,
              Ptr<Config> configs,
              Ptr<EntityContext> entity_context,
              Ptr<FileContext> file_context,
              Ptr<ToolContext> tool_context );

  ~LightPanel();

  void Render() noexcept override;
};
}
#endif