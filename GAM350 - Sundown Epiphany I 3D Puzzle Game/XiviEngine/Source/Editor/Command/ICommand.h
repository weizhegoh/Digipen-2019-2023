/*****************************************************************
*\file         ICommand.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#ifdef XV_EDITOR
#include <Types/Base.h>
namespace Xivi::Editor
{
struct ICommand
{
  virtual ~ICommand() = default;
  virtual void Execute()
  {};
  virtual void Undo()
  {};
};

namespace Concepts
{
template <typename Command>
concept IsCommand = std::derived_from<Command, ICommand>;
}
}
#endif