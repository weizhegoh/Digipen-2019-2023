/*****************************************************************
*\file         BoneInfo.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <pch.h>

namespace Xivi::Graphics
{
struct BoneInfo
{
  int id;
//#ifdef Test1
  Math::Matrix4 offset;
//#else
//		glm::mat4 offset;
//#endif
};
}