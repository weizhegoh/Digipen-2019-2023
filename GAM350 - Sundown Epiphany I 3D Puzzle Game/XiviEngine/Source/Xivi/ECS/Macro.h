/*****************************************************************
*\file         Macro.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

// Component Define
#define DefineDataComponent(COMPONENT) struct COMPONENT { static constexpr auto info = Xivi::ECS::Component::Type::Data{.m_name = #COMPONENT}
#define DefineDataComponent_Inherit(COMPONENT, ...) struct COMPONENT : public __VA_ARGS__ { static constexpr auto info = Xivi::ECS::Component::Type::Data{.m_name = #COMPONENT}
#define DefineDataComponent_ID(COMPONENT, ID) struct COMPONENT { static constexpr auto info = Xivi::ECS::Component::Type::Data{.m_id = ID, .m_name = #COMPONENT}
#define DefineDataComponent_ID_Inherit(COMPONENT, ID, ...) struct COMPONENT : public __VA_ARGS__ { static constexpr auto info = Xivi::ECS::Component::Type::Data{.m_id = ID, .m_name = #COMPONENT}

#define DefineSharedComponent(COMPONENT) struct COMPONENT { static constexpr auto info = Xivi::ECS::Component::Type::Shared{.m_name = #COMPONENT}
#define DefineSharedComponent_Inherit(COMPONENT, ...) struct COMPONENT : public __VA_ARGS__ { static constexpr auto info = Xivi::ECS::Component::Type::Shared{.m_name = #COMPONENT}
#define DefineSharedComponent_ID(COMPONENT, ID) struct COMPONENT { static constexpr auto info = Xivi::ECS::Component::Type::Shared{.m_id = ID, .m_name = #COMPONENT}
#define DefineSharedComponent_ID_Inherit(COMPONENT, ID, ...) struct COMPONENT : public __VA_ARGS__ { static constexpr auto info = Xivi::ECS::Component::Type::Shared{.m_id = ID, .m_name = #COMPONENT}

#define DefineTagComponent(COMPONENT) struct COMPONENT { static constexpr auto info = Xivi::ECS::Component::Type::Tag{.m_name = #COMPONENT};}
#define DefineTagComponent_ID(COMPONENT, ID) struct COMPONENT { static constexpr auto info = Xivi::ECS::Component::Type::Tag{.m_id = ID, .m_name = #COMPONENT};}

#define DefineExclusiveComponent(COMPONENT) struct COMPONENT { static constexpr auto info = Xivi::ECS::Component::Type::Exclusive{.m_name = #COMPONENT};}
#define DefineExclusiveComponent_ID(COMPONENT, ID) struct COMPONENT { static constexpr auto info = Xivi::ECS::Component::Type::Exclusive{.m_id = ID, .m_name = #COMPONENT};}

#define DefineSingletonComponent(COMPONENT) struct COMPONENT { static constexpr auto info = Xivi::ECS::Component::Type::Singleton{.m_name = #COMPONENT}
#define DefineSingletonComponent_ID(COMPONENT, ID) struct COMPONENT { static constexpr auto info = Xivi::ECS::Component::Type::Singleton{.m_id = ID, .m_name = #COMPONENT}

// System Define
#define DefineUpdateSystem(SYSTEM) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::Update{.m_name = #SYSTEM}

#define DefineUpdateSystem_ID(SYSTEM, ID) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::Update{.m_id = ID, .m_name = #SYSTEM}

#define DefineChildUpdateSystem(SYSTEM, PARENT_SYSTEM, PARENT_EVENT) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::ChildUpdate<PARENT_SYSTEM, PARENT_EVENT>{.m_name = #SYSTEM}

#define DefineChildUpdateSystem_ID(SYSTEM, ID, PARENT_SYSTEM, PARENT_EVENT) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::ChildUpdate<PARENT_SYSTEM, PARENT_EVENT>{.m_id = ID, .m_name = #SYSTEM}

#define DefineGlobalEventSystem(SYSTEM, GLOBAL_EVENT) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::GlobalEvent<GLOBAL_EVENT>{.m_name = #SYSTEM}

#define DefineGlobalEventSystem_ID(SYSTEM, ID, GLOBAL_EVENT) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::GlobalEvent<GLOBAL_EVENT>{.m_id = ID, .m_name = #SYSTEM}

#define DefineSystemEventSystem(SYSTEM, PARENT_SYSTEM, PARENT_EVENT) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::SystemEvent<PARENT_SYSTEM, PARENT_EVENT>{.m_name = #SYSTEM}

#define DefineSystemEventSystem_ID(SYSTEM, ID, PARENT_SYSTEM, PARENT_EVENT) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::SystemEvent<PARENT_SYSTEM, PARENT_EVENT>{.m_id = ID, .m_name = #SYSTEM}

#define DefineNotifyCreateSystem(SYSTEM) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::NotifyCreate{.m_name = #SYSTEM}

#define DefineNotifyCreateSystem_ID(SYSTEM, ID) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::NotifyCreate{.m_id = ID, .m_name = #SYSTEM}

#define DefineNotifyDestroySystem(SYSTEM) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::NotifyDestroy{.m_name = #SYSTEM}

#define DefineNotifyDestroySystem_ID(SYSTEM, ID) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::NotifyDestroy{.m_id = ID, .m_name = #SYSTEM}

#define DefineNotifyMoveInSystem(SYSTEM) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::NotifyMoveIn{.m_name = #SYSTEM}

#define DefineNotifyMoveInSystem_ID(SYSTEM, ID) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::NotifyMoveIn{.m_id = ID, .m_name = #SYSTEM}

#define DefineNotifyMoveOutSystem(SYSTEM) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::NotifyMoveOut{.m_name = #SYSTEM}

#define DefineNotifyMoveOutSystem_ID(SYSTEM, ID) struct SYSTEM : public Xivi::ECS::System::Instance { \
  static constexpr auto info = Xivi::ECS::System::Type::NotifyMoveOut{.m_id = ID, .m_name = #SYSTEM}

#define EndDefine() }