/*****************************************************************
*\file         GridRendererSystem.h
*\author(s)    Lee Jun Jie

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "../Sundown/Source/NComponents/Grid/GameGrid.h"

namespace Xivi::Graphics
{
struct GridRendererSystem : Xivi::ECS::System::Instance
{
  constexpr static auto info = ECS::System::Type::ChildUpdate<RendererSystem, RendererSystem::DebugDrawEvent>
  {
    .m_id = 218,
    .m_name = "Debug Draw Renderer System"
  };
  using None = ECS::Query::None<UI>;

  Ptr<Graphics::Manager> m_graphic_manager;

  void OnCreate() noexcept
  {
    m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
  }

  void OnPreUpdate() noexcept
  {
  }
  void OnUpdate() noexcept
  {
    if ( !m_graphic_manager->debugRender )
      return;
    auto &frameBuffer = m_graphic_manager->frameBuffer;
    auto &camera = m_graphic_manager->camera;
    auto &debugDrawer = m_graphic_manager->debugdrawer;

    //auto& shdrpgm1 = m_graphic_manager->GetShaders(ShaderOption::LINE);
    //shdrpgm1.Use();
    //
    //glEnable(GL_DEPTH_TEST);
    ////glDisable(GL_DEPTH_TEST);

    {
      /*float minX, minY, maxX, maxY;
      minX  = -camera->GetViewportX()/2 + camera->CameraEye().getX();
      minY = -camera->GetViewportY() / 2 + camera->CameraEye().getY();
      maxX = camera->GetViewportX()/2 + camera->CameraEye().getX();
      maxY = camera->GetViewportY()/2 + camera->CameraEye().getY();*/

      auto &grid = GetOrCreateSingletonComponent<GameGrid>();

      float minX, minY, maxX, maxY;

      minX = 0.0f;
      minY = 0.0f;
      maxX = grid.grid_size.x * grid.cell_size.getX();
      maxY = grid.grid_size.z * grid.cell_size.getZ();

      for ( int i = static_cast<int>( Math::Round( minY ) ); i <= maxY; i += 50 )
      {
        Vec3f start = Vec3f( minX, 0.0f, static_cast<float>( i ) );
        Vec3f end = Vec3f( maxX, 0.0f, static_cast<float>( i ) );
        debugDrawer.DrawLine( Shapes::LineSegment( start, end ) ).Color( Vec3f( 0.f, 1.f, 0.f ) );
        //Graphics::Gfx::Renderer::DrawLine(start, end, Vec3f(1.0,0.0,0.0), 5.f);
      }

      for ( int i = static_cast<int>( Math::Round( minX ) ); i <= maxX; i += 50 )
      {
        Vec3f start = Vec3f( static_cast<float>( i ), 0.0f, minY );
        Vec3f end = Vec3f( static_cast<float>( i ), 0.0f, maxY );
        debugDrawer.DrawLine( Shapes::LineSegment( start, end ) ).Color( Vec3f( 0.f, 1.f, 0.f ) );
        //Graphics::Gfx::Renderer::DrawLine(start, end, Vec3f(1.0, 0.0, 0.0), 5.f);
      }
    }

    debugDrawer.DrawPoint( Vec3f( 0.f, 0.f, 0.f ) ).Color( Vec3f( 1.f, 0.f, 0.f ) );

    //shdrpgm1.UnUse();

    //auto& shapesList = debugDrawer.ShapeList();
    //for (size_t i = 0; i < shapesList.size(); ++i)
    //{
    //	DebugShape& shape = shapesList[i];
    //	if ((shape.mMask & debugDrawer.isActive()) == 0)
    //		continue;
    //	if (shape.mOnTop)
    //		glDisable(GL_DEPTH_TEST);

      //for (size_t j = 0; j < shape.mSegments.size(); ++j)
      //{
      //	Shapes::LineSegment& segment = shape.mSegments[j];
      //	Math::Helper::MathFunction::clamp(shape.mColor, 0.f, 1.f);
      //	shape.mTransform = Math::Mat4x4().identity();
      //	Gfx::Renderer::SetTransformLine(
      //		camera->ViewMat_3D(),
      //		camera->ProjMat_3D(),
      //		shdrpgm1);

      //	//		//debugDrawer.DrawLine(Shapes::LineSegment(start, end));

      //	Graphics::Gfx::Renderer::DrawLine(segment.mStart, segment.mEnd, shape.mColor);

      //}

    //	{
    //		float minX, minY, maxX, maxY;
    //		minX = minY = 0;
    //		maxX = camera->GetViewportX();
    //		maxY = camera->GetViewportY();

    //		i = Math::Round(minY);
    //		for (int i = 0; i <= maxY; ++i)
    //		{
    //			Vec3f start = Vec3f(minX, 0.0f, static_cast<float>(i));
    //			Vec3f end = Vec3f(maxX, 0.0f, static_cast<float>(i));
    //			//Graphics::Gfx::Renderer::DrawLine(start, end, shape.mColor);
    //			debugDrawer.DrawLine(Shapes::LineSegment(start, end));

    //		}
    //		i = Math::Round(minX);
    //		for (; i <= maxX; ++i)
    //		{
    //			Vec3f start = Vec3f(static_cast<float>(i), 0.0f, minY);
    //			Vec3f end = Vec3f(static_cast<float>(i), 0.0f, maxY);
    //			//Graphics::Gfx::Renderer::DrawLine(start, end, shape.mColor);
    //			debugDrawer.DrawLine(Shapes::LineSegment(start, end));

    //		}
    //	}
    //

    //	if (shape.mOnTop)
    //		glEnable(GL_DEPTH_TEST);
    //}
    //shdrpgm1.UnUse();

    /*auto& timer = Service::Locator::Get<TimerEngine>();
    debugDrawer.Update(timer.GetDeltaTime(), camera->CameraEye(), camera->CameraDirection());*/
  }

  void OnPostUpdate() noexcept
  {}
};
}