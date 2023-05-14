/*****************************************************************
*\file         UIBarFillSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "UIBarFillSystem.h"
#include "Components/GUI/UIBar.h"
#include "Systems/Audio/Audio.h"
#include "Xivi/GUI/GUIManager.h"
#include "Components/GraphicsComponents.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"

namespace Xivi
{
void UIBarFillSystem::OnCreate() noexcept
{
  m_audio_manager = &Audio::AudioManager::Instance();
  m_graphic_manager = &Service::Locator::Get<Graphics::Manager>();
}
void UIBarFillSystem::OnEvent( bool &left, bool &right, Entity &hovered_entity ) noexcept
{
  if ( !hovered_entity.Valid() )
    return;

  if ( HaveComponent<GUI::VolumeBarTag>( hovered_entity ) )
  {
    if ( left && m_audio_manager->GetGUIvol() != 0 )
    {
      m_audio_manager->decreaseMasterVol();
      m_audio_manager->playSFX( "SFX_UIHOVER.ogg", false );
    }

    else if ( right && m_audio_manager->GetGUIvol() != 10 )
    {
      m_audio_manager->increaseMasterVol();
      m_audio_manager->playSFX( "SFX_UIHOVER.ogg", false );
    }
  }
  else if ( HaveComponent<GUI::GammaBarTag>( hovered_entity ) )
  {
    //auto& brightContrast = GetOrCreateSingletonComponent<BrightnessContrast>();
    if ( left && m_graphic_manager->m_cur_gamma > 0 )
    {
      m_graphic_manager->contrast += 0.2f;
      --m_graphic_manager->m_cur_gamma;
      m_audio_manager->playSFX( "SFX_UIHOVER.ogg", false );
    }
    else if ( right && m_graphic_manager->m_cur_gamma < 10 )
    {
      m_graphic_manager->contrast -= 0.2f;
      ++m_graphic_manager->m_cur_gamma;
      m_audio_manager->playSFX( "SFX_UIHOVER.ogg", false );
    }
  }

  ForEach( [&]( Entity &entity, GUI::UIBar &bar, RenderSettings &render_setting )
  {
    if ( HaveComponent<GUI::VolumeBarTag>( GetComponent<Parent>( entity ).m_entity ) )
    {
      if ( bar.m_id < m_audio_manager->GetGUIvol() && render_setting.transparent != 1.0f )
        render_setting.transparent = 1.0f;
      else if ( bar.m_id >= m_audio_manager->GetGUIvol() && render_setting.transparent != 0.0f )
        render_setting.transparent = 0.0f;
    }
    else if ( HaveComponent<GUI::GammaBarTag>( GetComponent<Parent>( entity ).m_entity ) )
    {
      if ( bar.m_id < m_graphic_manager->m_cur_gamma && render_setting.transparent != 1.0f )
        render_setting.transparent = 1.0f;
      else if ( bar.m_id >= m_graphic_manager->m_cur_gamma && render_setting.transparent != 0.0f )
        render_setting.transparent = 0.0f;
    }
  } );
}
}