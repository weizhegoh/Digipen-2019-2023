#pragma once

namespace Xivi::Resource
{
  enum class State : u8
  {
    NotLoaded,
    NotLoadedFallback,
    Loading,
    LoadingFallback,
    NotFound,
    NotFoundFallback,
    Mutable,
    Final
  };

  enum class Policy : u8
  {
    Persistant,
    Referenced,
    Managed
  };
}