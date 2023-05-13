/*****************************************************************
*\file         EngineEvents.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
// Default
struct CloneEntity : ECS::Event::Instance<Entity & >
{};

// Parent-Child
struct ParentChildLink : ECS::Event::Instance<Entity &, Entity &>
{};
struct ParentChildUnlink : ECS::Event::Instance<Entity & >
{};
struct ParentChildRemove : ECS::Event::Instance<const Entity & >
{};
struct ParentChildLinkCallbacks : ECS::Event::Instance<Entity &, Entity &>
{};
struct ParentChildUnlinkCallbacks : ECS::Event::Instance<Entity &, Entity &>
{};

// Prefab
struct CreatePrefab : ECS::Event::Instance<Entity &, const std::string &>
{};
struct CreatePrefabInstanceInternal : ECS::Event::Instance<Entity &>
{};
struct SerialisePrefab : ECS::Event::Instance<Entity &, const std::string &>
{};
struct DeserialisePrefab : ECS::Event::Instance<Entity &, const std::string &>
{};

// Default List
using Default_EList = EList<
  CloneEntity>;

// Parent-Child
using ParentChild_EList = EList<
  ParentChildLink,
  ParentChildUnlink,
  ParentChildRemove,
  ParentChildLinkCallbacks,
  ParentChildUnlinkCallbacks>;

// Prefab
using Prefab_EList = EList<
  CreatePrefab,
  CreatePrefabInstanceInternal,
  SerialisePrefab,
  DeserialisePrefab>;

using Engine_EList = Default_EList::Extend<ParentChild_EList>::Extend<Prefab_EList>;
}
