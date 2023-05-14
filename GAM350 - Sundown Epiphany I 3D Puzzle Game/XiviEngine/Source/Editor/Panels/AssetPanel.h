/*****************************************************************
*\file         AssetPanel.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "IPanel.h"

namespace Xivi::Editor
{
struct AssetPanel : public IPanel
{
  Func<bool( const Utils::Directory &,
             std::filesystem::directory_entry &,
             Ptr<const std::vector<Utils::Directory>> &,
             Ptr < const std::vector<Filesystem::fs::path>> & )> m_sync;

  AssetPanel( std::string name,
              Ptr<ECS::World::Instance> world,
              Ptr<Config> configs,
              Ptr<EntityContext> entity_context,
              Ptr<FileContext> file_context,
              Ptr<ToolContext> tool_context );

  ~AssetPanel();

  void Render() noexcept override;
};
}
#endif