/*****************************************************************
*\file         AudioPanel.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "IPanel.h"

namespace Xivi::Editor
{
struct AudioPanel : public IPanel
{
  AudioPanel( std::string name,
              Ptr<ECS::World::Instance> world,
              Ptr<Config> configs,
              Ptr<EntityContext> entity_context,
              Ptr<FileContext> file_context,
              Ptr<ToolContext> tool_context );

  ~AudioPanel();

  void Render() noexcept override;

private:
  std::set<std::string> m_sounds;
};
}
#endif