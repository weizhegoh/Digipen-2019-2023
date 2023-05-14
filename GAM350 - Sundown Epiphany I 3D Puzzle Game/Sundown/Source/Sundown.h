/*****************************************************************
*\file         Sundown.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

//#ifdef EXPORTDLL
//#define SUNDOWN_DECLSPEC    __declspec(dllexport)
//#else
//#define SUNDOWN_DECLSPEC    __declspec(dllimport)
//#endif

struct /*SUNDOWN_DECLSPEC*/ SundownEXE
{
  static void Launch();
};