/*****************************************************************
*\file         Profiler.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#ifdef XV_PROFILE
#include <Tracy.hpp>

#define XV_PROFILE_FRAMEMARKER() FrameMark
#define XV_PROFILE_FRAMEMARKER_NAME(name) FrameMarkNamed(name)
#define XV_PROFILE_FRAMEMARKER_START(name) FrameMarkStart(name)
#define XV_PROFILE_FRAMEMARKER_END(name) FrameMarkEnd(name)

#define XV_PROFILE_ZONESCOPED() ZoneScoped
#define XV_PROFILE_ZONESCOPED_N(name) ZoneScopedN(name)
#define XV_PROFILE_ZONESCOPED_C(color) ZoneScopedC(color)
#define XV_PROFILE_ZONESCOPED_NC(name, color) ZoneScopedNC(name, color)

#define XV_PROFILE_LOCK(type, var, desc) TracyLockable(type,var,desc)
#define XV_PROFILE_LOCKMARKER(var) LockMark(var)
#define XV_PROFILE_THREAD(name) tracy::SetThreadName(name)
#else
#define XV_PROFILE_FRAMEMARKER()
#define XV_PROFILE_FRAMEMARKER_NAME(name)
#define XV_PROFILE_FRAMEMARKER_START(name)
#define XV_PROFILE_FRAMEMARKER_END(name)

#define XV_PROFILE_ZONESCOPED()
#define XV_PROFILE_ZONESCOPED_N(name)
#define XV_PROFILE_ZONESCOPED_C(color)
#define XV_PROFILE_ZONESCOPED_NC(name, color)

#define XV_PROFILE_LOCK(type, var, desc)
#define XV_PROFILE_LOCKMARKER(var)
#define XV_PROFILE_THREAD(name)
#endif