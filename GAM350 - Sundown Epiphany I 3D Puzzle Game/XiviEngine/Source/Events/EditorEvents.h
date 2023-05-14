/*****************************************************************
*\file         EditorEvents.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
// Editor Commands
struct AddComponents : ECS::Event::Instance<Entity &, ECS::DescriptorSpan>
{};
struct RemoveComponents : ECS::Event::Instance<Entity &, ECS::DescriptorSpan>
{};
struct UpdateComponent : ECS::Event::Instance<Entity &, const ECS::Component::Descriptor &, byte &>
{};

struct CreateEntityFromArchetype : ECS::Event::Instance<const ArchetypeCreateFunc &, const Position &, Entity &>
{};
struct CreateEntityFromAsset : ECS::Event::Instance<const AssetCreateFunc &, const Position &, const std::string &, Entity &>
{};
struct CreatePrefabInstance : ECS::Event::Instance<const Entity &, const Position &, Entity &>
{};
struct DeleteEntity : ECS::Event::Instance<Entity &>
{};
struct DuplicateEntity : ECS::Event::Instance<Entity &, Entity &>
{};
struct DragDuplicateEntity : ECS::Event::Instance<const Entity &, const ECS::Component::Descriptor &, byte &, Entity &>
{};

struct LinkParent : ECS::Event::Instance<Entity &, Entity &>
{};
struct UnlinkParent : ECS::Event::Instance<Entity &>
{};

struct Undo : ECS::Event::Instance<>
{};
struct Redo : ECS::Event::Instance<>
{};

// Editor Event List
using EditorBase_EList = EList<>;

// Command Event List
using EditorCommand_EList = EList<
  AddComponents,
  RemoveComponents,
  UpdateComponent,
  CreateEntityFromArchetype,
  CreateEntityFromAsset,
  CreatePrefabInstance,
  DeleteEntity,
  DuplicateEntity,
  DragDuplicateEntity,
  LinkParent,
  UnlinkParent,
  Undo,
  Redo>;

using Editor_EList = EditorBase_EList::Extend<EditorCommand_EList>;
}
