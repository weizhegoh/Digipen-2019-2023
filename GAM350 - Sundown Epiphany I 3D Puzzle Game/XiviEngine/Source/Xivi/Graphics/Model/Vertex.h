/*****************************************************************
*\file         Vertex.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"

#define MAX_BONE_INFLUENCE 10
namespace Xivi::Graphics
{
struct Vertex
{
  Vec3f position;
  Vec3f normal;
  Vec2f texture_coord;
  Vec3f tan;
  Vec3f bit;
  int  jointID[MAX_BONE_INFLUENCE];
  float weights[MAX_BONE_INFLUENCE];
};
}