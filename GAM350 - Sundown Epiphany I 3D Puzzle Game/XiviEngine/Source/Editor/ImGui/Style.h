/*****************************************************************
*\file         Stlye.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
void WindowStyle()
{
  auto &style = ImGui::GetStyle();

  // Main Style
  style.WindowPadding = { 12.f, 12.f };
  style.FramePadding = { 2.f, 2.f };
  style.CellPadding = { 8.f, 8.f };
  style.ItemSpacing = { 6.f, 6.f };
  style.ItemInnerSpacing = { 6.f, 6.f };
  style.TouchExtraPadding = { 2.f, 2.f };
  style.IndentSpacing = 24.f;
  style.ScrollbarSize = 16.f;
  style.GrabMinSize = 16.f;

  // Borders
  style.WindowBorderSize = 1.f;
  style.ChildBorderSize = 1.f;
  style.PopupBorderSize = 1.f;
  style.FrameBorderSize = 1.f;
  style.TabBorderSize = 1.f;

  // Rounding
  style.WindowRounding = 6.f;
  style.ChildRounding = 6.f;
  style.FrameRounding = 6.f;
  style.PopupRounding = 6.f;
  style.ScrollbarRounding = 6.f;
  style.GrabRounding = 6.f;
  style.LogSliderDeadzone = 6.f;
  style.TabRounding = 6.f;

  // Alignment
  style.WindowTitleAlign = { 0.00f, 0.5f };
  style.WindowMenuButtonPosition = ImGuiDir_Right;
  style.ColorButtonPosition = ImGuiDir_Left;
  style.ButtonTextAlign = { 0.55f, 0.5f };
  style.SelectableTextAlign = { 0.5f, 0.5f };

  // Safe Area Padding
  style.DisplaySafeAreaPadding = { 16.f, 0.f };
}

void ThemeStyle()
{
  ImVec4 *colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_Text] = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
  colors[ImGuiCol_TextDisabled] = ImVec4( 0.69f, 0.69f, 0.69f, 1.00f );
  colors[ImGuiCol_WindowBg] = ImVec4( 0.04f, 0.02f, 0.02f, 1.00f );
  colors[ImGuiCol_ChildBg] = ImVec4( 0.09f, 0.07f, 0.07f, 1.00f );
  colors[ImGuiCol_PopupBg] = ImVec4( 0.09f, 0.07f, 0.07f, 1.00f );
  colors[ImGuiCol_Border] = ImVec4( 0.31f, 0.09f, 0.14f, 1.00f );
  colors[ImGuiCol_BorderShadow] = ImVec4( 0.18f, 0.06f, 0.08f, 1.00f );
  colors[ImGuiCol_FrameBg] = ImVec4( 0.07f, 0.04f, 0.05f, 1.00f );
  colors[ImGuiCol_FrameBgHovered] = ImVec4( 0.16f, 0.06f, 0.08f, 0.75f );
  colors[ImGuiCol_FrameBgActive] = ImVec4( 0.38f, 0.12f, 0.17f, 0.63f );
  colors[ImGuiCol_TitleBg] = ImVec4( 0.12f, 0.07f, 0.07f, 1.00f );
  colors[ImGuiCol_TitleBgActive] = ImVec4( 0.07f, 0.00f, 0.00f, 1.00f );
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4( 0.21f, 0.14f, 0.14f, 1.00f );
  colors[ImGuiCol_MenuBarBg] = ImVec4( 0.00f, 0.00f, 0.00f, 0.88f );
  colors[ImGuiCol_ScrollbarBg] = ImVec4( 0.00f, 0.00f, 0.00f, 0.63f );
  colors[ImGuiCol_ScrollbarGrab] = ImVec4( 0.67f, 0.16f, 0.27f, 0.25f );
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4( 0.67f, 0.16f, 0.27f, 1.00f );
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4( 0.67f, 0.16f, 0.27f, 0.75f );
  colors[ImGuiCol_CheckMark] = ImVec4( 0.67f, 0.16f, 0.27f, 1.00f );
  colors[ImGuiCol_SliderGrab] = ImVec4( 0.67f, 0.16f, 0.27f, 0.25f );
  colors[ImGuiCol_SliderGrabActive] = ImVec4( 0.67f, 0.16f, 0.27f, 0.75f );
  colors[ImGuiCol_Button] = ImVec4( 0.67f, 0.16f, 0.27f, 0.25f );
  colors[ImGuiCol_ButtonHovered] = ImVec4( 0.67f, 0.16f, 0.27f, 1.00f );
  colors[ImGuiCol_ButtonActive] = ImVec4( 0.67f, 0.16f, 0.27f, 0.75f );
  colors[ImGuiCol_Header] = ImVec4( 0.67f, 0.16f, 0.27f, 0.25f );
  colors[ImGuiCol_HeaderHovered] = ImVec4( 0.67f, 0.16f, 0.27f, 1.00f );
  colors[ImGuiCol_HeaderActive] = ImVec4( 0.67f, 0.16f, 0.27f, 0.75f );
  colors[ImGuiCol_Separator] = ImVec4( 0.67f, 0.16f, 0.27f, 0.25f );
  colors[ImGuiCol_SeparatorHovered] = ImVec4( 0.67f, 0.16f, 0.27f, 1.00f );
  colors[ImGuiCol_SeparatorActive] = ImVec4( 0.67f, 0.16f, 0.27f, 0.75f );
  colors[ImGuiCol_ResizeGrip] = ImVec4( 0.67f, 0.16f, 0.27f, 0.25f );
  colors[ImGuiCol_ResizeGripHovered] = ImVec4( 0.67f, 0.16f, 0.27f, 1.00f );
  colors[ImGuiCol_ResizeGripActive] = ImVec4( 0.67f, 0.16f, 0.27f, 0.75f );
  colors[ImGuiCol_Tab] = ImVec4( 0.67f, 0.16f, 0.27f, 0.25f );
  colors[ImGuiCol_TabHovered] = ImVec4( 0.67f, 0.16f, 0.27f, 1.00f );
  colors[ImGuiCol_TabActive] = ImVec4( 0.67f, 0.16f, 0.27f, 0.75f );
  colors[ImGuiCol_TabUnfocused] = ImVec4( 0.53f, 0.05f, 0.15f, 0.13f );
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4( 0.53f, 0.05f, 0.15f, 0.75f );
  colors[ImGuiCol_DockingPreview] = ImVec4( 0.77f, 0.20f, 0.31f, 1.00f );
  colors[ImGuiCol_DockingEmptyBg] = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );
  colors[ImGuiCol_PlotLines] = ImVec4( 0.61f, 0.61f, 0.61f, 1.00f );
  colors[ImGuiCol_PlotLinesHovered] = ImVec4( 1.00f, 0.43f, 0.35f, 1.00f );
  colors[ImGuiCol_PlotHistogram] = ImVec4( 0.90f, 0.70f, 0.00f, 1.00f );
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4( 1.00f, 0.60f, 0.00f, 1.00f );
  colors[ImGuiCol_TableHeaderBg] = ImVec4( 0.19f, 0.19f, 0.20f, 1.00f );
  colors[ImGuiCol_TableBorderStrong] = ImVec4( 0.31f, 0.31f, 0.35f, 1.00f );
  colors[ImGuiCol_TableBorderLight] = ImVec4( 0.23f, 0.23f, 0.25f, 1.00f );
  colors[ImGuiCol_TableRowBg] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
  colors[ImGuiCol_TableRowBgAlt] = ImVec4( 1.00f, 1.00f, 1.00f, 0.06f );
  colors[ImGuiCol_TextSelectedBg] = ImVec4( 0.26f, 0.59f, 0.98f, 0.35f );
  colors[ImGuiCol_DragDropTarget] = ImVec4( 0.94f, 0.23f, 0.37f, 1.00f );
  colors[ImGuiCol_NavHighlight] = ImVec4( 0.26f, 0.59f, 0.98f, 1.00f );
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4( 1.00f, 1.00f, 1.00f, 0.70f );
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4( 0.80f, 0.80f, 0.80f, 0.20f );
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4( 0.80f, 0.80f, 0.80f, 0.35f );
}