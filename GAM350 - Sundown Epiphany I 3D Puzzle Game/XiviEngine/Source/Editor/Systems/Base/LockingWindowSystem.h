/*****************************************************************
*\file         LockingWindowSystem.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "Editor/Panels/MenuPanel.h"
#include "Editor/Panels/AssetPanelBrowser.h"

#include "Editor/System.h"

namespace Xivi::Editor
{
struct LockingWindowSystem
{
  inline static std::unordered_map<ImGuiID, Action<>> m_input_mapping;

  static void EntityShortcut( ECS::World::Instance &world, Editor::System &editor_sys ) noexcept;

  static void Init( ECS::World::Instance &world ) noexcept;

  static void Update() noexcept;
};
}
#endif