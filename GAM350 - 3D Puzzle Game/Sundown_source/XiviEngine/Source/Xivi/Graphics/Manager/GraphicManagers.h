/*****************************************************************
*\file         GraphicManagers.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "Xivi/ECS/XVECS.h"
#include "../Shader/ShaderManager.h"
#include "../Camera/Camera3D.h"
#include "../Frustum/Frustum.h"
#include "../Gfx/Gfx.h"
#include "../Collision/Geometry.h"
#include "Components/Graphics/AABB.h"
#include "../FrameBuffer/Framebuffers.h"
#include "../FrameBuffer/Framebuffer.h"
#include "../Reader/MeshCreate.h"
#include "../DebugDraw/DebugDraw.h"
#include "Xivi/Graphics/Helper/GraphicHelper.h"
//#include "Xivi/Graphics/CutScene/CutSceneList.h"

namespace Xivi::Graphics
{
class M_FrameBuffer :
  public FrameBuffers
{
public:

  void InitManager( float width, float height )
  {
    #ifdef XV_EDITOR
    FrameBuffers::Create( width, height, RenderList::Game, false );
    FrameBuffers::Create( width, height, RenderList::Editor, false );
    SetActiveList( FrameBuffers::RenderList::Editor );

    #else
    FrameBuffers::Create( width, height, RenderList::Game, false );
    SetActiveList( FrameBuffers::RenderList::Game );
    #endif
    FrameBuffers::CreateUI( width, height );
  }

  void SetCameraPosition( const size_t index, const size_t cameraIndex, const Vec3f position )
  {
    Camera( index, cameraIndex ).SetCameraEye( position );
  }

  bool GetPickingAABBCheck(
    float mouseX, float mouseY,
    float width, float height,
    AABB aabb, float t )
  {
    //std::cout << Camera(GetActiveFB(), 0).CameraEye() << std::endl;
    return
      Math::Helper::MathFunction::RayAabb(
      Camera( GetActiveFB(), 0 ).CameraEye(),
      Camera( GetActiveFB(), 0 ).MouseTo3DWorldDriection( mouseX, mouseY, width, height ),
      aabb.AABB_mMin, aabb.AABB_mMax,
      t );
  }
};

struct ShadowParameter
{
  Vec3f lightPos;
  Mat4x4 lightView, lightSpaceMatrix;
};

class Manager : private ShaderManager
{
public:
  void InitManager( float width, float height )
  {
    InitAllShader();
    debugdrawer.InitLineBuffer();
    #ifdef XV_EDITOR
    shadowRender = false;
    #else
    shadowRender = true;
    #endif
  }

  void InitAllShader()
  {
    AddShader( ShaderOption::QUAD,
               "Assets/Shaders/3D/FrameBuffer2DQuad.vert",
               "Assets/Shaders/3D/FrameBuffer2DQuad.frag" );

    AddShader( ShaderOption::TEXTURE,
               "Assets/Shaders/3D/Texture_Light.vert",
               "Assets/Shaders/3D/Texture_Light.frag" );
    AddShader( ShaderOption::NORMAL,
               "Assets/Shaders/3D/normalVert.vert",
               "Assets/Shaders/3D/normalFrag.frag" );

    AddShader( ShaderOption::ANIMATION,
               "Assets/Shaders/3D/AnimationModel.vert",
               "Assets/Shaders/3D/AnimationModel.frag" );

    AddShader( ShaderOption::AnimationWireFrame,
               "Assets/Shaders/3D/AnimationModel.vert",
               "Assets/Shaders/3D/normalFrag.frag" );

    AddShader( ShaderOption::SKYBOX_SHADER,
               "Assets/Shaders/3D/Skybox_shader.vert",
               "Assets/Shaders/3D/Skybox_shader.frag" );

    AddShader( ShaderOption::DEPTH_RTT,
               "Assets/Shaders/3D/DepthRTT.vert",
               "Assets/Shaders/3D/DepthRTT.frag" );

    AddShader( ShaderOption::SHADOW_MAPPING,
               "Assets/Shaders/3D/ShadowMapping.vert",
               "Assets/Shaders/3D/ShadowMapping.frag" );

    AddShader( ShaderOption::UI,
               "Assets/Shaders/3D/textureNormal.vert",
               "Assets/Shaders/3D/textureNormal.frag" );

    AddShader( ShaderOption::LINE,
               "Assets/Shaders/3D/line.vert",
               "Assets/Shaders/3D/line.frag" );
  }

  GLSLShader &GetShaders( ShaderOption option )
  {
    return GetShader( option );
  }
//SpotLight spotLight;
  bool wireFrameMode = false;
  bool shadowRender = false;
  bool debugRender = false;
  bool enableAnimation = false;
  bool enableFreeCam = false;
  bool picked = false;
  FrameBuffer *frameBuffer;
  Camera3D *camera;
  ShadowParameter shadowParameter;
  //CutSceneFunc cutSceneFunc;
  int win = 0;
  int gearCount = 0;
  DebugDrawer debugdrawer;
  bool gamma = true;

  float bright = 1.f;
  float contrast = 1.f;
  Math::SIMD::Vec4 AverageLuminance = Math::SIMD::Vec4( 0.5, 0.5, 0.5, 1.0 );
  i32 m_cur_gamma = 10;

  //bool cutscene = false;
  #ifndef XV_EDITOR
  Mesh Quad = Xivi::Graphics::QUAD();
  #endif
  DebugDrawer &GetDebugDraw()
  {
    return debugdrawer;
  }
};
}