/*****************************************************************
*\file         ParentChildUnlinkSystem.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct ParentChildUnlinkSystem : ECS::System::Instance
{
  static constexpr auto info = ECS::System::Type::GlobalEvent<ParentChildUnlink>
  {
    .m_name = "Parent-Child Unlink System"
  };

  void OnEvent( Entity &child );
};
}