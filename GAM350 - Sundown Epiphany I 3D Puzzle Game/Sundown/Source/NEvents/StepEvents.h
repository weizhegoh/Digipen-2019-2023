/*****************************************************************
*\file         StepEvents.h
*\author(s)    Tan tong Wee, Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
struct PlayerInputEvent : ECS::Event::Instance<>
{};
struct ProcessPlayerEvent : ECS::Event::Instance<>
{};

struct UpdateGUIMoveEvent : ECS::Event::Instance<const i32 &>
{};
struct UpdateGUICollectableEvent : ECS::Event::Instance<const i32 &>
{};
struct UpdateGUISummaryEvent : ECS::Event::Instance<>
{};
struct UpdateGUIBackToMenuEvent : ECS::Event::Instance<>
{};
struct UpdateGUIBackToWinLoseEvent : ECS::Event::Instance<>
{};
struct UpdateGUIInstructionPageEvent : ECS::Event::Instance<const bool &, const bool &>
{};
struct UpdateGUIHowToPlayPageEvent : ECS::Event::Instance<const bool &, const bool &>
{};
struct UIButtonMovementEvent : ECS::Event::Instance<>
{};
struct UIBarFillEvent : ECS::Event::Instance<bool &, bool &, Entity &>
{};
struct LevelSelectorEvent : ECS::Event::Instance<Entity &>
{};
struct PauseUnpauseEvent : ECS::Event::Instance<>
{};

// UI Dialogue
struct ActivateSwitchPopupDialogue : ECS::Event::Instance<>
{};
struct CloseSwitchPopupDialogue : ECS::Event::Instance<>
{};

struct ActivatePressurePlatePopupDialogue : ECS::Event::Instance<>
{};
struct ClosePressurePlatePopupDialogue : ECS::Event::Instance<>
{};

struct ActivateCheesePopupDialogue : ECS::Event::Instance<>
{};
struct CloseCheesePopupDialogue : ECS::Event::Instance<>
{};

struct ActivateObstaclePopupDialogue : ECS::Event::Instance<>
{};
struct CloseObstaclePopupDialogue : ECS::Event::Instance<>
{};

struct ActivateHolePopupDialogue : ECS::Event::Instance<>
{};
struct CloseHolePopupDialogue : ECS::Event::Instance<>
{};

struct ActivateCartPopupDialogue : ECS::Event::Instance<>
{};
struct CloseCartPopupDialogue : ECS::Event::Instance<>
{};
}