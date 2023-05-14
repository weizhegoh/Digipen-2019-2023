/*****************************************************************
*\file         LevelSelectorUpdateSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "XiviEngine.h"
#include "LevelSelectorUpdateSystem.h"

namespace Xivi
{
void LevelSelectorUpdateSystem::OnCreate() noexcept
{
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();
  m_audio_manager = &Audio::AudioManager::Instance();
  m_resource_manager = &Service::Locator::Get<RM>();
}

void LevelSelectorUpdateSystem::OnEvent( Entity &hovered_entity ) noexcept
{
  // Return if it is not Level Selector page
  ECS::Query::Instance level_selector_cavnas;
  bool found = false;
  level_selector_cavnas.AllOf<GUI::LevelSelectorCanvasTag>();
  ForEach( level_selector_cavnas, [&]
  {
    found = true;
    return true;
  } );
  if ( !found )
    return;

  UpdateScoreAndRat<GUI::L1_1Tag>( hovered_entity, GSM::LEVEL1_STAGE1 );
  UpdateScoreAndRat<GUI::L1_2Tag>( hovered_entity, GSM::LEVEL1_STAGE2 );
  UpdateScoreAndRat<GUI::L1_3Tag>( hovered_entity, GSM::LEVEL1_STAGE3 );
  UpdateScoreAndRat<GUI::L1_4Tag>( hovered_entity, GSM::LEVEL1_STAGE4 );
  UpdateScoreAndRat<GUI::L2_1Tag>( hovered_entity, GSM::LEVEL2_STAGE1 );
  UpdateScoreAndRat<GUI::L2_2Tag>( hovered_entity, GSM::LEVEL2_STAGE2 );
}
}