/*****************************************************************
*\file         ParentChildLinkSystem.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct ParentChildLinkSystem : ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::GlobalEvent<ParentChildLink>
  {
    .m_name = "Parent-Child Link System"
  };

  void OnEvent( Entity &child, Entity &parent );
};
}