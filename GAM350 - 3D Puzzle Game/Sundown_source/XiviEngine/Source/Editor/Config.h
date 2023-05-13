/*****************************************************************
*\file         Config.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#ifdef XV_EDITOR
#include <Types/Base.h>
struct ImFont;

namespace Xivi::Editor
{
enum class State : u8
{
  Stop = 1,
  RunningPaused = 5,
  Running = 6,
};

enum class Buffer : u8
{
  LevelEditor = 1,
  UIEditor = 2
};

struct Config
{
  Ptr<ImFont> fonts;
  Editor::State current_state = Editor::State::Stop;
  Editor::Buffer current_buffer = Editor::Buffer::LevelEditor;
};
}
#endif