/*****************************************************************
*\file         CommandHistory.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#ifdef XV_EDITOR
#include "ICommand.h"
#include <Types/Base.h>
#include <STL/stack.h>
namespace Xivi::Editor
{
class CommandHistory
{
  stack<UniquePtr<ICommand>> undo_history {};
  stack<UniquePtr<ICommand>> redo_history {};

public:
  template <Concepts::IsCommand Command, typename... Args>
  void Commit( Args&&... args )
  {
    undo_history.push( UniquePtr<ICommand>( Command::Create( std::forward<Args>( args )... ) ) );
    while ( !redo_history.empty() )
      redo_history.pop();
  }

  void Undo()
  {
    if ( !undo_history.empty() )
    {
      UniquePtr<ICommand> command( undo_history.top().release() );
      undo_history.pop();
      command->Undo();
      redo_history.push( std::move( command ) );
    }
  }

  void Redo()
  {
    if ( !redo_history.empty() )
    {
      UniquePtr<ICommand> command( redo_history.top().release() );
      redo_history.pop();
      command->Execute();
      undo_history.push( std::move( command ) );
    }
  }

  void Clear()
  {
    while ( !redo_history.empty() )
      redo_history.pop();
    while ( !undo_history.empty() )
      undo_history.pop();
  }
};
}
#endif