/*****************************************************************
*\file         ImGuiHelperFunction.cpp
*\author(s)    Chng Yong Quan, Goh Wei Zhe

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"
#ifdef XV_EDITOR
#include "ImGuiHelperFunction.h"

namespace Xivi::Editor::ImGuiHelper
{
Math::Vector2 s_mouse_pos_offsetted { 0.0f, 0.0f };
tuplet::pair<bool, Ptr<ECS::Entity>> s_picker { false, nullptr };

bool IsItemHovered() noexcept
{
  return ImGui::IsItemHovered() && ImGui::IsWindowHovered();
}

void EnumerationCombo( std::vector<Ptr<const char>> value_list, const std::string &name, int &selection ) noexcept
{
  const char *current_item = value_list[selection];
  if ( ImGui::BeginCombo( ( "##" + name ).c_str(), current_item ) )
  {
    for ( unsigned i = 0; i < value_list.size(); ++i )
    {
      bool is_selected = ( current_item == value_list[i] );
      if ( ImGui::Selectable( value_list[i], is_selected ) )
        selection = i;
      if ( is_selected )
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
}
}
#endif