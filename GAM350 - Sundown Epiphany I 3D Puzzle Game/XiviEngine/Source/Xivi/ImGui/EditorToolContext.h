/*****************************************************************
*\file         EditorToolContext.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include <ImGuizmo.h>

namespace Xivi
{
struct TranslationSnap
{
  f32 value;
  bool surface_snap;
  bool grid_snap;
};

struct RotationSnap
{
  f32 value;
  bool grid_snap;
};

struct ScaleSnap
{
  f32 value;
  bool grid_snap;
};

struct ToolContext
{
  // Gizmo snapping
  TranslationSnap m_translation_snap { 1.0f, false, false };
  RotationSnap m_rotation_snap { 1.0f, false };
  ScaleSnap m_scale_snap { 0.25f, false };

  // Current gizmo mode
  ImGuizmo::OPERATION m_gizmo_type { ImGuizmo::OPERATION::TRANSLATE };

  // Camera
  f32 m_camera_speed { 4.0f };
  f32 m_camera_scalar { 1.0f };
};
}