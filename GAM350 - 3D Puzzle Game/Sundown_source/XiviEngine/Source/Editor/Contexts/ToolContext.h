/*****************************************************************
*\file         ToolContext.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include <imgui.h>
#include <ImGuizmo.h>
#include <Types/Base.h>
#include <string>

namespace Xivi::Editor
{
struct TranslationSnap
{
  std::string str_value;
  f32 value;
  bool surface_snap;
  bool grid_snap;
};

struct RotationSnap
{
  std::string str_value;
  f32 value;
  bool grid_snap;
};

struct ScaleSnap
{
  std::string str_value;
  f32 value;
  bool grid_snap;
};

struct ToolContext
{
  // Gizmo snapping
  TranslationSnap m_translation_snap { "1", 1.0f, false, false };
  RotationSnap m_rotation_snap { "5", 5.0f, false };
  ScaleSnap m_scale_snap { "0.25", 0.25f, false };

  // Current gizmo mode
  ImGuizmo::OPERATION m_gizmo_type { ImGuizmo::OPERATION::TRANSLATE };

  // Camera
  i32 m_camera_scalar { 5 };
  f32 m_camera_speed { 1.0f };
};
}
#endif