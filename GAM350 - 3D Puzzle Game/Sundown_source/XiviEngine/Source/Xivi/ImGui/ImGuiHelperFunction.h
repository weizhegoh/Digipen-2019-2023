/*****************************************************************
*\file         ImGuiHelperFunction.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include <imgui.h>

namespace Xivi::ImGuiHelper
{
template <Traits::Concepts::InvokableFunction Func>
void CreateImGuiWindow( const char *name, bool *p_open, ImGuiWindowFlags flags, Func func = EmptyLambda {} )
{
  static_assert( !std::is_same_v<Func, EmptyLambda>, "No callback is passed" );
  if ( ImGui::Begin( name, p_open, flags ) )
  {
    func();
  }
  ImGui::End();
}

template <Traits::Concepts::InvokableFunction Func>
void CreateImGuiWindow( const char *name, bool *p_open, Func func = EmptyLambda {} )
{
  static_assert( !std::is_same_v<Func, EmptyLambda>, "No callback is passed" );
  if ( ImGui::Begin( name, p_open ) )
  {
    func();
  }
  ImGui::End();
}

template <Traits::Concepts::InvokableFunction Func>
void CreateImGuiWindow( const char *name, Func func = EmptyLambda {} )
{
  static_assert( !std::is_same_v<Func, EmptyLambda>, "No callback is passed" );
  if ( ImGui::Begin( name ) )
  {
    func();
  }
  ImGui::End();
}

template <Traits::Concepts::InvokableFunction Func>
void ItemDoubleClicked( Func func )
{
  if ( ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked( 0 ) )
    func();
}

__inline bool IsItemHovered()
{
  return ImGui::IsItemHovered() && ImGui::IsWindowHovered();
}

template <Traits::Concepts::InvokableFunction Func>
void Tooltip( Func func )
{
  ImGui::BeginTooltip();
  func();
  ImGui::EndTooltip();
}

template <Traits::Concepts::InvokableFunction Func>
void PopupContextItem( const char *str_id = NULL, ImGuiPopupFlags popup_flags = 1, Func func = EmptyLambda {} )
{
  static_assert( !std::is_same_v<Func, EmptyLambda>, "No callback is passed" );
  if ( ImGui::BeginPopupContextItem( str_id, popup_flags ) )
  {
    func();
    ImGui::EndPopup();
  }
}

template <Traits::Concepts::InvokableFunction Func>
void PopupContextWindow( const char *str_id, ImGuiMouseButton mb, bool over_items, Func func = EmptyLambda {} )
{
  static_assert( !std::is_same_v<Func, EmptyLambda>, "No callback is passed" );
  if ( ImGui::BeginPopupContextWindow( str_id, mb, over_items ) )
  {
    func();
    ImGui::EndPopup();
  }
}

template <Traits::Concepts::InvokableFunction Func>
void PopupModal( const char *name, bool *p_open = nullptr, ImGuiWindowFlags flags = 0, Func func = EmptyLambda {}, bool *state = nullptr )
{
  static_assert( !std::is_same_v<Func, EmptyLambda>, "No callback is passed" );
  if ( ImGui::BeginPopupModal( name, p_open, flags ) )
  {
    func();
    ImGui::EndPopup();
  }
  else if ( state )
    *state = false;
}

template <Traits::Concepts::InvokableFunction Func>
void DragDropTarget( Func func = EmptyLambda {} )
{
  static_assert( !std::is_same_v<Func, EmptyLambda>, "No callback is passed" );
  if ( ImGui::BeginDragDropTarget() )
  {
    func();
    ImGui::EndDragDropTarget();
  }
}

template <Traits::Concepts::InvokableFunction Func>
void DragDropSource( ImGuiDragDropFlags flags = 0, Func func = EmptyLambda {} )
{
  static_assert( !std::is_same_v<Func, EmptyLambda>, "No callback is passed" );
  if ( ImGui::BeginDragDropSource( flags ) )
  {
    func();
    ImGui::EndDragDropSource();
  }
}

template <Traits::Concepts::InvokableFunction Func>
void DragDropSource( Func func )
{
  if ( ImGui::BeginDragDropSource() )
  {
    func();
    ImGui::EndDragDropSource();
  }
}

template <Traits::Concepts::InvokableFunction Func>
void DropDown( const char *label, const char *preview_value, ImGuiComboFlags flags, Func func = EmptyLambda {} )
{
  static_assert( !std::is_same_v<Func, EmptyLambda>, "No callback is passed" );
  if ( ImGui::BeginCombo( label, preview_value, flags ) )
  {
    func();
    ImGui::EndCombo();
  }
}

template <Traits::Concepts::InvokableFunction Func>
void DropDown( const char *label, const char *preview_value, Func func = EmptyLambda {} )
{
  static_assert( !std::is_same_v<Func, EmptyLambda>, "No callback is passed" );
  if ( ImGui::BeginCombo( label, preview_value ) )
  {
    func();
    ImGui::EndCombo();
  }
}

template <Traits::Concepts::InvokableFunction Func>
void Menu( const char *label, bool enabled, Func func = EmptyLambda {} )
{
  static_assert( !std::is_same_v<Func, EmptyLambda>, "No callback is passed" );
  if ( ImGui::BeginMenu( label, enabled ) )
  {
    func();
    ImGui::EndMenu();
  }
}

template <Traits::Concepts::InvokableFunction Func>
void Menu( const char *label, Func func = EmptyLambda {} )
{
  static_assert( !std::is_same_v<Func, EmptyLambda>, "No callback is passed" );
  if ( ImGui::BeginMenu( label ) )
  {
    func();
    ImGui::EndMenu();
  }
}

__inline void EnumerationCombo( std::vector<Ptr<const char>> value_list, const std::string &name, int &selection ) noexcept
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

template <Traits::Concepts::InvokableFunction Func>
void ChildWindow( const char *str_id, const ImVec2 &size = ImVec2( 0, 0 ), bool border = false, ImGuiWindowFlags flags = 0, Func func = EmptyLambda {} )
{
  static_assert( !std::is_same_v<Func, EmptyLambda>, "No callback is passed" );
  if ( ImGui::BeginChild( str_id, size, border, flags ) )
  {
    func();
  }
  ImGui::EndChild();
}
template <Traits::Concepts::InvokableFunction Func>
void ChildWindow( const char *str_id, bool border = false, Func func = EmptyLambda {} )
{
  static_assert( !std::is_same_v<Func, EmptyLambda>, "No callback is passed" );
  if ( ImGui::BeginChild( str_id, ImVec2( 0, 0 ), border ) )
  {
    func();
  }
  ImGui::EndChild();
}

template <Traits::Concepts::InvokableFunction Func>
void Group( Func func )
{
  ImGui::BeginGroup();
  func();
  ImGui::EndGroup();
}

template <Traits::Concepts::InvokableFunction Func>
void Indent( float indent_w, Func func )
{
  ImGui::Indent( indent_w );
  func();
  ImGui::Unindent( indent_w );
}

template <Traits::Concepts::InvokableFunction Func>
void Indent( Func func )
{
  ImGui::Indent( ImGui::GetTreeNodeToLabelSpacing() );
  func();
  ImGui::Unindent( ImGui::GetTreeNodeToLabelSpacing() );
}
}