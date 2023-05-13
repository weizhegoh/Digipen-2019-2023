/*****************************************************************
*\file         ButtonPanel.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include "IPanel.h"

namespace Xivi::Audio
{
class AudioManager;
}

namespace Xivi::Graphics
{
class Manager;
class M_FrameBuffer;
}

namespace Xivi::GUI
{
class Manager;
}

namespace Xivi::Editor
{
struct ButtonPanel : public IPanel
{
  using Label = const char *;
  using Name = const char *;
  using Mask = u32;
  using GraphicsSettingFunction = std::function<bool( void )>;
  using ToolsetDetails = tuplet::tuple<Label, Name, Mask, Action<>>;
  using ControlDetails = tuplet::tuple<Label, Name, Mask, Action<>>;
  using Buffer = tuplet::tuple<Label, Name, Mask, Action<>>;
  using GraphicsSettingDetails = tuplet::tuple<Name, bool, GraphicsSettingFunction>;
  using SnappingList = unordered_map<const char *, f32>;

  std::string m_tmp_file_path;
  std::vector<ToolsetDetails> m_toolset;
  std::vector<ControlDetails> m_controls;
  std::vector<Buffer> m_editor_buffer;
  std::vector<GraphicsSettingDetails> m_graphics_setting;
  Ptr<Audio::AudioManager> m_audio_manager { nullptr };
  Ptr<Graphics::Manager> m_graphic_manager { nullptr };
  Ptr<Graphics::M_FrameBuffer> m_framebuffer { nullptr };
  Ptr<GUI::Manager> m_canvas_stack { nullptr };

  bool m_for_cam_sensitivity { false };

  SnappingList m_trans_snap_vals {
      {"1", 1.0f},
      {"2", 2.0f},
      {"5", 5.0f},
      {"10", 10.0f},
      {"50", 50.0f},
      {"100", 100.0f},
      {"500", 500.0f},
      {"1000", 1000.0f},
      {"5000", 5000.0f},
      {"10000", 10000.0f}
  };

  SnappingList m_rot_snap_vals
  {
      {"1", 1.0f},
      {"5", 5.0f},
      {"10", 10.0f},
      {"15", 15.0f},
      {"30", 30.0f},
      {"45", 45.0f},
      {"60", 60.0f},
      {"90", 90.0f},
      {"120", 120.0f},
      {"150", 150.0f},
      {"180", 180.0f}
  };
  SnappingList m_scale_snap_vals
  {
      {"0.03125", 0.03125f},
      {"0.0625", 0.0625f},
      {"0.125", 0.125f},
      {"0.25", 0.25f},
      {"0.5", 0.5f},
      {"1", 1.0f},
      {"2", 2.0f},
      {"10", 10.0f},
      {"20", 20.0f}
  };

  ButtonPanel( std::string name,
               Ptr<ECS::World::Instance> world,
               Ptr<Config> configs,
               Ptr<EntityContext> entity_context,
               Ptr<FileContext> file_context,
               Ptr<ToolContext> tool_context );

  ~ButtonPanel();

  void Render() noexcept override;
  void SnappingGizmo() noexcept;
  void SnappingGizmo( const std::string name ) noexcept;
};
}
#endif