/*****************************************************************
*\file         InitFuncMapping.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "InitFuncMapping.h"
#include "Xivi/ECS/XVECS.h"
#include "NComponents.h"
#include "NEvents.h"
#include "GameGUIHelperFunction.h"
#include "NSystems/GameWorld/Pause/PauseUnpauseSystem.h"

namespace Xivi::FuncMapping
{
void Unhover( ECS::World::Instance &world ) noexcept
{
  auto hover_entity = &world.GetSystem<UIButtonMovementSystem>().m_hovered_button;
  if ( world.HaveComponent<GUI::UIHover>( *hover_entity ) )
    world.GetComponent<GUI::UIHover>( *hover_entity ).m_callback_unhover( *hover_entity );
  *hover_entity = Entity {};
}

void ChangeTexture( ECS::World::Instance &world, RM &resource_manager, Entity &entity, const std::string texure ) noexcept
{
  if ( world.HaveComponent<Material>( entity ) )
  {
    auto &material = world.GetComponent<Material>( entity );
    material.diffuse = resource_manager.Get<Graphics::Texture>( texure );
  }
}

void StartButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager, Audio::AudioManager &audio_mgr ) noexcept
{
  canvas_stack.AddFunc( "Normal Start", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover Start", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger Start", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_selected.dds" );
    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();
    #ifndef XV_EDITOR
    auto &gsm = Service::Locator::Get<GSM::GSManager>();
    gsm.ChangeState( GSM::TUTORIAL_1 );
    #endif
    Unhover( world );
  }
  );
}

void InstructionButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal Instruction", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover Instruction", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger Instruction", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_selected.dds" );
    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();

    Helper::PushInstructionGUI( world, canvas_stack );
    Unhover( world );
  }
  );
}

void SettingsButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal Settings", [&]( Entity entity )
  {
    bool paused = world.GetSystem<PauseUnpauseSystem>().m_paused;
    std::string str = paused ?
      "Assets/Textures/PauseBtn.dds" :
      "Assets/Textures/btn.dds";
    ChangeTexture( world, resource_manager, entity, str );
  }
  );
  canvas_stack.AddFunc( "Hover Settings", [&]( Entity entity )
  {
    bool paused = world.GetSystem<PauseUnpauseSystem>().m_paused;
    std::string str = paused ?
      "Assets/Textures/PauseBtn_Hover.dds" :
      "Assets/Textures/btn_hover.dds";
    ChangeTexture( world, resource_manager, entity, str );
  }
  );
  canvas_stack.AddFunc( "Trigger Settings", [&]( Entity entity )
  {
    bool paused = world.GetSystem<PauseUnpauseSystem>().m_paused;
    std::string str = paused ?
      "Assets/Textures/PauseBtn_Selected.dds" :
      "Assets/Textures/btn_selected.dds";
    ChangeTexture( world, resource_manager, entity, str );
    Unhover( world );
    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();
    ECS::Query::Instance setting_canvas;
    setting_canvas.AllOf<GUI::SettingsCanvasTag>();
    world.ForEach( setting_canvas, [&]( Entity &entity )
    {
      Helper::RecursivePushGUI( world, entity );
    } );
  }
  );
}

void HowToPlayButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal HowToPlay", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/PauseBtn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover HowToPlay", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/PauseBtn_Hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger HowToPlay", [&]( Entity entity )
  {
    ;
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/PauseBtn_Selected.dds" );
    Unhover( world );
    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();
    ECS::Query::Instance how_to_play_canvas;
    how_to_play_canvas.AllOf<GUI::HowToPlayCanvasTag>();
    world.ForEach( how_to_play_canvas, [&]( Entity &entity )
    {
      Helper::RecursivePushGUI( world, entity );
    } );
  }
  );
}

void PausedInstructionButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal Paused Instruction", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/PauseBtn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover Paused Instruction", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/PauseBtn_Hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger Paused Instruction", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/PauseBtn_Selected.dds" );
    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();

    Helper::PushInstructionGUI( world, canvas_stack );
    Unhover( world );
  }
  );
}

void PauseSettingsButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal Pause Settings", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/PauseBtn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover Pause Settings", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/PauseBtn_Hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger Pause Settings", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/PauseBtn_Selected.dds" );
    Unhover( world );
    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();
    ECS::Query::Instance setting_canvas;
    setting_canvas.AllOf<GUI::SettingsCanvasTag>();
    world.ForEach( setting_canvas, [&]( Entity &entity )
    {
      Helper::RecursivePushGUI( world, entity );
    } );
  }
  );
}

void CreditsButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager, Audio::AudioManager &audio_mgr ) noexcept
{
  canvas_stack.AddFunc( "Normal Credits", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover Credits", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger Credits", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_selected.dds" );
    Unhover( world );
    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();
    ECS::Query::Instance credits_canvas;
    credits_canvas.AllOf<GUI::CreditsCanvasTag>();
    world.ForEach( credits_canvas, [&]( Entity &entity )
    {
      Helper::RecursivePushGUI( world, entity );
    } );
    audio_mgr.StopAllLoop();
    audio_mgr.playSFX( "SFX_CREDITS.ogg", false );
  }
  );
}

void ExitGameButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal ExitGame", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover ExitGame", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger ExitGame", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_selected.dds" );
    Unhover( world );
    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();
    ECS::Query::Instance popup_canvas;
    popup_canvas.AllOf<GUI::ExitGamePopupCanvasTag>();
    world.ForEach( popup_canvas, [&]( Entity &entity )
    {
      Helper::RecursivePushGUI( world, entity );
    } );
  }
  );
}

void ReturnButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal Return", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover Return", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger Return", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_selected.dds" );
    Unhover( world );

    bool found = false;
    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();

    if ( world.GetSystem<PauseUnpauseSystem>().m_paused )
    {
      Helper::PushPauseGUI( world, canvas_stack );
      return;
    }

    ECS::Query::Instance popup_canvas;
    popup_canvas.AllOf<GUI::GoToMainMenuPopupCanvasTag>();
    world.ForEach( popup_canvas, [&]( Entity &entity )
    {
      Helper::RecursivePushGUI( world, entity );
      found = true;
      return true;
    } );

    if ( found )
      return;

    ECS::Query::Instance instruction_canvas;
    instruction_canvas.AllOf<GUI::InstructionCanvasTag>();
    world.ForEach( instruction_canvas, [&]( Entity &entity )
    {
      Helper::PushMainMenuGUI( world, canvas_stack );
      return true;
    } );
  }
  );
}

void NextLevelButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal NextLevel", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover NextLevel", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger NextLevel", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_selected.dds" );
    Unhover( world );
    #ifndef XV_EDITOR
    auto &gsm = Service::Locator::Get<GSM::GSManager>();
    if ( gsm.GetPreviousState() != GSM::LEVEL2_STAGE2 )
    {
      auto next_state = gsm.GetNextStage( gsm.GetPreviousState() );
      gsm.ChangeState( next_state );
    }
    else
      gsm.ChangeState( GSM::END );
    #endif
  }
  );
}

void RetryButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal Retry", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover Retry", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger Retry", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_selected.dds" );
    Unhover( world );
    #ifndef XV_EDITOR
    auto &gsm = Service::Locator::Get<GSM::GSManager>();
    gsm.ChangeState( gsm.GetPreviousState() );
    gsm.retry = true;
    #endif
  }
  );
}

void SettingsBackButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager, Audio::AudioManager &audio_mgr ) noexcept
{
  canvas_stack.AddFunc( "Normal Settings Back", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover Settings Back", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger Settings Back", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_selected.dds" );
    Unhover( world );

    auto &state = world.GetOrCreateSingletonComponent<GameWorldState>();
    if ( world.GetSystem<PauseUnpauseSystem>().m_paused )
    {
      if ( !canvas_stack.Empty() )
        canvas_stack.PopAll();
      Helper::PushPauseGUI( world, canvas_stack );
    }
    else
    {
      bool is_in_credit = false;
      for ( const auto &ui : *canvas_stack.Get() )
      {
        if ( world.HaveComponent<GUI::CreditsCanvasTag>( ui ) )
        {
          is_in_credit = true;
          break;
        }
      }
      if ( is_in_credit )
      {
        audio_mgr.StopAllSFX();
        audio_mgr.playLoop( "LOOP_MAINMENU.ogg", false );
        audio_mgr.playSFX( "SFX_UITRIGGER.ogg", false );
      }

      if ( !canvas_stack.Empty() )
        canvas_stack.PopAll();
      Helper::PushMainMenuGUI( world, canvas_stack );
    }
  }
  );
}

void ArrowButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal Arrow", [&]( Entity entity )
  {
    if ( world.HaveComponent<RenderSettings>( entity ) )
    {
      auto &render_settings = world.GetComponent<RenderSettings>( entity );
      render_settings.transparent = 0.0f;
    }
  }
  );
  canvas_stack.AddFunc( "Hover Arrow", [&]( Entity entity )
  {
    if ( world.HaveComponent<RenderSettings>( entity ) )
    {
      auto &render_settings = world.GetComponent<RenderSettings>( entity );
      render_settings.transparent = 1.0f;
    }
  }
  );
  canvas_stack.AddFunc( "Trigger Arrow", [&]( Entity entity )
  {
  }
  );
}

void PauseButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal Pause", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/Pause_1.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover Pause", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/Pause_2.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger Pause", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/Pause_3.dds" );
    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();
    Helper::RecurPush( world, canvas_stack, "PauseScreen_instance" );
    world.SendGlobalEvent<PauseUnpauseEvent>();
  }
  );
}

void QuitToMainMenuButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager, Audio::AudioManager &audio_mgr ) noexcept
{
  canvas_stack.AddFunc( "Normal Quit To Main Menu", [&]( Entity entity )
  {
    ECS::Query::Instance summary_canvas;
    summary_canvas.AllOf<GUI::SummaryCanvasTag>();
    bool found = false;
    world.ForEach( summary_canvas, [&]
    {
      found = true;
      return true;
    } );
    std::string str = found ?
      "Assets/Textures/btn.dds" :
      "Assets/Textures/PauseBtn.dds";
    ChangeTexture( world, resource_manager, entity, str );
  }
  );
  canvas_stack.AddFunc( "Hover Quit To Main Menu", [&]( Entity entity )
  {
    ECS::Query::Instance summary_canvas;
    summary_canvas.AllOf<GUI::SummaryCanvasTag>();
    bool found = false;
    world.ForEach( summary_canvas, [&]
    {
      found = true;
      return true;
    } );
    std::string str = found ?
      "Assets/Textures/btn_hover.dds" :
      "Assets/Textures/PauseBtn_Hover.dds";
    ChangeTexture( world, resource_manager, entity, str );
  }
  );
  canvas_stack.AddFunc( "Trigger Quit To Main Menu", [&]( Entity entity )
  {
    ECS::Query::Instance summary_canvas;
    summary_canvas.AllOf<GUI::SummaryCanvasTag>();
    bool found = false;
    world.ForEach( summary_canvas, [&]
    {
      found = true;
      return true;
    } );
    std::string str = found ?
      "Assets/Textures/btn_selected.dds" :
      "Assets/Textures/PauseBtn_Selected.dds";
    ChangeTexture( world, resource_manager, entity, str );

    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();
    Unhover( world );
    if ( found )
    {
      #ifndef XV_EDITOR
      auto &gsm = Service::Locator::Get<GSM::GSManager>();
      gsm.ChangeState( GSM::MAINMENU_NOLOGO );
      world.SendGlobalEvent<PauseUnpauseEvent>();
      #endif
    }
    else
    {
      ECS::Query::Instance popup_canvas;
      popup_canvas.AllOf<GUI::GoToMainMenuPopupCanvasTag>();
      world.ForEach( popup_canvas, [&]( Entity &entity )
      {
        Helper::RecursivePushGUI( world, entity );
        return true;
      } );
    }
  }
  );
}

void ContinueButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal Continue", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/PauseBtn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover Continue", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/PauseBtn_Hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger Continue", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/PauseBtn_Selected.dds" );
    world.SendGlobalEvent<PauseUnpauseEvent>();
    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();
    Helper::RecurPush( world, canvas_stack, "MoveCountText_instance" );
    Helper::RecurPush( world, canvas_stack, "MoveCountNumberOnes_instance" );
    Helper::RecurPush( world, canvas_stack, "MoveCountNumberTens_instance" );
    Helper::RecurPush( world, canvas_stack, "MoveCountNumberHundreds_instance" );
    Helper::RecurPush( world, canvas_stack, "CollectableCountText_instance" );
    Helper::RecurPush( world, canvas_stack, "CollectableCountNumber_instance" );
  }
  );
}

void RestartButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal Restart", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/PauseBtn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover Restart", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/PauseBtn_Hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger Restart", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/PauseBtn_Selected.dds" );
    world.SendGlobalEvent<PauseUnpauseEvent>();
    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();
    Unhover( world );
    #ifndef XV_EDITOR
    auto &gsm = Service::Locator::Get<GSM::GSManager>();
    gsm.ChangeState( GSM::GS_RESTART );
    #endif
  }
  );
}

void ChangeStateToMainMenuButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal Change State To MainMenu", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover Change State To MainMenu", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger Change State To MainMenu", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_selected.dds" );
    Unhover( world );
    #ifndef XV_EDITOR
    auto &gsm = Service::Locator::Get<GSM::GSManager>();
    gsm.ChangeState( GSM::MAINMENU_NOLOGO );
    #endif
  }
  );
}

void YesExitButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal Yes Exit", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover Yes Exit", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger Yes Exit", [&]( Entity entity )
  {
    ECS::Query::Instance mainmenu_canvas;
    mainmenu_canvas.AllOf<GUI::MainMenuCanvasTag>();
    bool found = false;
    world.ForEach( mainmenu_canvas, [&]
    {
      found = true;
      return true;
    } );
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_selected.dds" );
    Unhover( world );
    #ifndef XV_EDITOR
    auto &gsm = Service::Locator::Get<GSM::GSManager>();
    if ( found )
      gsm.ChangeState( GSM::GS_QUIT );
    else
    {
      gsm.ChangeState( GSM::MAINMENU_NOLOGO );
      world.SendGlobalEvent<PauseUnpauseEvent>();
    }
    #endif
  }
  );
}

void NoExitButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal No Exit", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover No Exit", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger No Exit", [&]( Entity entity )
  {
    ECS::Query::Instance mainmenu_canvas;
    mainmenu_canvas.AllOf<GUI::MainMenuCanvasTag>();
    bool found = false;
    world.ForEach( mainmenu_canvas, [&]
    {
      found = true;
      return true;
    } );
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_selected.dds" );
    Unhover( world );
    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();

    if ( found )
      Helper::PushMainMenuGUI( world, canvas_stack );
    else
    {
      if ( world.GetSystem<PauseUnpauseSystem>().m_paused )
        Helper::PushPauseGUI( world, canvas_stack );
      else
      {
        bool win = false;
        ECS::Query::Instance win_canvas;
        win_canvas.AllOf<GUI::WinCanvasTag>();
        world.ForEach( win_canvas, [&]( Entity &entity )
        {
          win = true;
          Helper::RecursivePushGUI( world, entity );
          return true;
        } );

        if ( win )
          return;

        ECS::Query::Instance lose_canvas;
        lose_canvas.AllOf<GUI::LoseCanvasTag>();
        world.ForEach( lose_canvas, [&]( Entity &entity )
        {
          Helper::RecursivePushGUI( world, entity );
          return true;
        } );
      }
    }
  }
  );
}

void LevelSelectorButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal Level Selector", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover Level Selector", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger Level Selector", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_selected.dds" );
    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();

    ECS::Query::Instance level_selector_canvas;
    level_selector_canvas.AllOf<GUI::LevelSelectorCanvasTag>();
    world.ForEach( level_selector_canvas, [&]( Entity &entity )
    {
      Helper::RecursivePushGUI( world, entity );
      return true;
    } );

    Unhover( world );
  }
  );
}

void LevelSelectorLevelsButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager ) noexcept
{
  canvas_stack.AddFunc( "Normal Level Selector Levels", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover Level Selector Levels", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_hover.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger Level Selector Levels", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/btn_selected.dds" );
    if ( !canvas_stack.Empty() )
      canvas_stack.PopAll();

    LevelToGo<GUI::L1_1Tag>( world, GSM::TUTORIAL_1 );
    LevelToGo<GUI::L1_2Tag>( world, GSM::TUTORIAL_2 );
    LevelToGo<GUI::L1_3Tag>( world, GSM::LEVEL1_STAGE3 );
    LevelToGo<GUI::L1_4Tag>( world, GSM::LEVEL1_STAGE4 );
    LevelToGo<GUI::L2_1Tag>( world, GSM::LEVEL2_STAGE1 );
    LevelToGo<GUI::L2_2Tag>( world, GSM::LEVEL2_STAGE2 );
    Unhover( world );
  }
  );
}

void SFXButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager, Audio::AudioManager &audio_mgr ) noexcept
{
  canvas_stack.AddFunc( "Normal SFX", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/SFX_1.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover SFX", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/SFX_2.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger SFX", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/SFX_3.dds" );

    audio_mgr.setMuteSFX( true );

    auto &hover = world.GetComponent<GUI::UIHover>( entity );
    auto &trigger = world.GetComponent<GUI::UITrigger>( entity );

    trigger.m_key_to_func = "Mute Trigger SFX";
    hover.m_key_to_func = "Mute Hover SFX";
    hover.m_key_to_func_unhover = "Mute Normal SFX";

    auto &func_map = canvas_stack.GetFuncMap();
    trigger.m_callback = func_map[trigger.m_key_to_func];
    hover.m_callback = func_map[hover.m_key_to_func];
    hover.m_callback_unhover = func_map[hover.m_key_to_func_unhover];

    hover.m_callback( entity );
  }
  );

  canvas_stack.AddFunc( "Mute Normal SFX", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/SFX_4.dds" );
  }
  );
  canvas_stack.AddFunc( "Mute Hover SFX", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/SFX_5.dds" );
  }
  );
  canvas_stack.AddFunc( "Mute Trigger SFX", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/SFX_6.dds" );

    audio_mgr.setMuteSFX( false );

    auto &hover = world.GetComponent<GUI::UIHover>( entity );
    auto &trigger = world.GetComponent<GUI::UITrigger>( entity );

    trigger.m_key_to_func = "Trigger SFX";
    hover.m_key_to_func = "Hover SFX";
    hover.m_key_to_func_unhover = "Normal SFX";

    auto &func_map = canvas_stack.GetFuncMap();
    trigger.m_callback = func_map[trigger.m_key_to_func];
    hover.m_callback = func_map[hover.m_key_to_func];
    hover.m_callback_unhover = func_map[hover.m_key_to_func_unhover];

    hover.m_callback( entity );
  }
  );
}

void BGMButtonFunc( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager, Audio::AudioManager &audio_mgr ) noexcept
{
  canvas_stack.AddFunc( "Normal BGM", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/BGM_1.dds" );
  }
  );
  canvas_stack.AddFunc( "Hover BGM", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/BGM_2.dds" );
  }
  );
  canvas_stack.AddFunc( "Trigger BGM", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/BGM_3.dds" );

    auto &hover = world.GetComponent<GUI::UIHover>( entity );
    auto &trigger = world.GetComponent<GUI::UITrigger>( entity );

    audio_mgr.setMuteBGM( true );

    trigger.m_key_to_func = "Mute Trigger BGM";
    hover.m_key_to_func = "Mute Hover BGM";
    hover.m_key_to_func_unhover = "Mute Normal BGM";

    auto &func_map = canvas_stack.GetFuncMap();
    trigger.m_callback = func_map[trigger.m_key_to_func];
    hover.m_callback = func_map[hover.m_key_to_func];
    hover.m_callback_unhover = func_map[hover.m_key_to_func_unhover];

    hover.m_callback( entity );
  }
  );
  canvas_stack.AddFunc( "Mute Normal BGM", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/BGM_4.dds" );
  }
  );
  canvas_stack.AddFunc( "Mute Hover BGM", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/BGM_5.dds" );
  }
  );
  canvas_stack.AddFunc( "Mute Trigger BGM", [&]( Entity entity )
  {
    ChangeTexture( world, resource_manager, entity, "Assets/Textures/BGM_6.dds" );

    audio_mgr.setMuteBGM( false );

    auto &hover = world.GetComponent<GUI::UIHover>( entity );
    auto &trigger = world.GetComponent<GUI::UITrigger>( entity );

    trigger.m_key_to_func = "Trigger BGM";
    hover.m_key_to_func = "Hover BGM";
    hover.m_key_to_func_unhover = "Normal BGM";

    auto &func_map = canvas_stack.GetFuncMap();
    trigger.m_callback = func_map[trigger.m_key_to_func];
    hover.m_callback = func_map[hover.m_key_to_func];
    hover.m_callback_unhover = func_map[hover.m_key_to_func_unhover];

    hover.m_callback( entity );
  }
  );
}

void InitFuncMapping( ECS::World::Instance &world, GUI::Manager &canvas_stack, RM &resource_manager, Audio::AudioManager &audio_mgr ) noexcept
{
  StartButtonFunc( world, canvas_stack, resource_manager, audio_mgr );
  InstructionButtonFunc( world, canvas_stack, resource_manager );
  SettingsButtonFunc( world, canvas_stack, resource_manager );
  HowToPlayButtonFunc( world, canvas_stack, resource_manager );
  CreditsButtonFunc( world, canvas_stack, resource_manager, audio_mgr );
  ExitGameButtonFunc( world, canvas_stack, resource_manager );
  ReturnButtonFunc( world, canvas_stack, resource_manager );
  NextLevelButtonFunc( world, canvas_stack, resource_manager );
  RetryButtonFunc( world, canvas_stack, resource_manager );
  PauseButtonFunc( world, canvas_stack, resource_manager );
  ChangeStateToMainMenuButtonFunc( world, canvas_stack, resource_manager );
  YesExitButtonFunc( world, canvas_stack, resource_manager );
  NoExitButtonFunc( world, canvas_stack, resource_manager );
  SettingsBackButtonFunc( world, canvas_stack, resource_manager, audio_mgr );
  ArrowButtonFunc( world, canvas_stack, resource_manager );
  QuitToMainMenuButtonFunc( world, canvas_stack, resource_manager, audio_mgr );
  ContinueButtonFunc( world, canvas_stack, resource_manager );
  RestartButtonFunc( world, canvas_stack, resource_manager );
  PausedInstructionButtonFunc( world, canvas_stack, resource_manager );
  PauseSettingsButtonFunc( world, canvas_stack, resource_manager );
  LevelSelectorButtonFunc( world, canvas_stack, resource_manager );
  LevelSelectorLevelsButtonFunc( world, canvas_stack, resource_manager );
  SFXButtonFunc( world, canvas_stack, resource_manager, audio_mgr );
  BGMButtonFunc( world, canvas_stack, resource_manager, audio_mgr );
  canvas_stack.SetAllFuncByKey();
}
}