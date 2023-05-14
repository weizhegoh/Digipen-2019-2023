/*****************************************************************
*\file         EditorConfig.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
enum class EditorState : u8
{
  Stop = 1,
  RunningPaused = 5,
  Running = 6,
};

enum class EditorBuffer : u8
{
  LevelEditor = 1,
  UIEditor = 2
};

struct Config
{
  Ptr<ImFont> m_fonts;
  EditorState m_cur_state = EditorState::Stop;
  EditorBuffer m_cur_buffer = EditorBuffer::LevelEditor;
};
}