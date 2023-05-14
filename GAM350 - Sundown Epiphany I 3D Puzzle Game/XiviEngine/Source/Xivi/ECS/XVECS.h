/*****************************************************************
*\file         XVECS.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

/**
 * Files
 * * Declaration
 */

 // Core
#include <string_view>
#include <utility>
#include <algorithm>
#include <STL/span.h>
#include <STL/array.h>
#include <STL/vector.h>
#include <Containers/CBitset.h>
#include <Containers/PinnedVector.h>

// Base
#include <Core.h>
#include <Log.h>

// Types
#include <Types/Base.h>
//#include <STL/memory.h>
//#include <STL/functional.h>
#include <Types/GUID.h>
#include <Types/Traits/Specialised.h>
#include <Types/Traits/Function.h>
#include <Types/Traits/Tuple.h>
#include <Types/Typelist/Typelist.h>

// Utils
#include <Utils/Assert.h>
#include <Utils/Enumerate.h>
#include <Utils/NonCopyable.h>
#include <Utils/Singleton.h>
#include <Utils/Typename.h>
#include <Utils/XXH3.h>

#include <Event/Manager.h>

// Reflection
#include <rttr/registration.h>
#include "Xivi/Core/Utils/Json/JsonFile.h"
#include "Xivi/Core/Utils/Json/JsonDeserialise.h"
#include "Xivi/Core/Utils/Json/JsonSerialise.h"

namespace Xivi
{
// Alias
template <typename... Cs>
using CList = Typelist<Cs...>;

template <typename... Cs>
using SList = Typelist<Cs...>;

template <typename... Cs>
using EList = Typelist<Cs...>;
}

// XVECS
#include "Settings.h" // Settings
#include "Declarations.h" // Forward Declarations
#include "Component.h" // Components
#include "Details/Component.inl"
#include "Concepts.h" // Concepts
#include "Components/ECSComponents.h"
#include "Events/ECSEvents.h"
#include "ComponentDescriptorManager.h"
#include "EntityManager.h" // Entity Manager
#include "Pool.h" // Archetypes
#include "Family.h"
#include "Archetype.h"
#include "ArchetypeManager.h"
#include "Query.h" // Queries
#include "QueryIterator.h"
#include "System.h" // Systems
#include "SystemManager.h"
#include "World.h" // World

/**
 * Inline Files
 * * Definitions
 */
#include "Details/ComponentDescriptorManager.inl"
#include "Details/EntityManager.inl" // Entity Manager
#include "Details/Pool.inl" // Archetypes
#include "Details/Family.inl"
#include "Details/Archetype.inl"
#include "Details/ArchetypeManager.inl"
#include "Details/Query.inl" // Queries
#include "Details/QueryIterator.inl"
#include "Details/System.inl" // Systems
#include "Details/SystemManager.inl"
#include "Details/World.inl" // World

 /**
 * Files
 * * Systems
 */
#include "Systems/ECSSystems.h"

#include "Macro.h"