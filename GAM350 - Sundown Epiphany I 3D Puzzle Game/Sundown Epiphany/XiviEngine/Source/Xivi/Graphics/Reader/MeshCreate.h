/*****************************************************************
*\file         MeshCreate.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "../GraphicTypes.h"

namespace Xivi::Graphics
{
class Mesh;
class StaticModel;
using Vector3 = Vec3f;
using Vector2 = Vec2f;
class VertexBuffer;
class IndexBuffer;

void CreatePrimative( Model::StaticType type, std::vector<Mesh> &mesh );
Mesh CreateCube();
Mesh CreatePlane( int width, int height, const Vector3 &normal );
Mesh CreateSkyBox();
Mesh CreateSquare();
Mesh QUAD();
void ComputeTangentsBitangents( VertexBuffer &vertices, const IndexBuffer &indices );
}