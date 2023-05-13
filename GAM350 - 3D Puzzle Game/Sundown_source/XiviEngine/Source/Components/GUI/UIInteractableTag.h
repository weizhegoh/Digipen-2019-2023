/*****************************************************************
*\file         UIInteractableTag.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

namespace Xivi::GUI
{
struct UIInteractableTag
{
  constexpr static auto info = Xivi::ECS::Component::Type::Tag
  {
    .m_name = "UI Interactable Tag"
  };
};
};

namespace Xivi::Components::UIInteractableTag::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::GUI::UIInteractableTag>( "UI Interactable Tag" )
  ( rttr::metadata( "GUID", ECS::Component::info<Xivi::GUI::UIInteractableTag>.m_id ) );
}
}