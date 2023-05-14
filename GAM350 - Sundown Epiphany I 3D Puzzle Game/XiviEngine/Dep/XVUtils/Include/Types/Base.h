#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <functional>

namespace Xivi
{
  using byte = std::byte;
  using size_t = std::size_t;

  using u8 = uint8_t;
  using u16 = uint16_t;
  using u32 = uint32_t;
  using u64 = uint64_t;

  static constexpr u8 u8_max = UINT8_MAX;
  static constexpr u16 u16_max = UINT16_MAX;
  static constexpr u32 u32_max = UINT32_MAX;
  static constexpr u64 u64_max = UINT64_MAX;

  using i8 = int8_t;
  using i16 = int16_t;
  using i32 = int32_t;
  using i64 = int64_t;

  static constexpr i8 i8_min = INT8_MIN;
  static constexpr i8 i8_max = INT8_MAX;
  static constexpr i16 i16_min = INT16_MIN;
  static constexpr i16 i16_max = INT16_MAX;
  static constexpr i32 i32_min = INT32_MIN;
  static constexpr i32 i32_max = INT32_MAX;
  static constexpr i64 i64_min = INT64_MIN;
  static constexpr i64 i64_max = INT64_MAX;

  using f32 = float;
  using f64 = double;

  using uptr = uintptr_t;
  using iptr = intptr_t;
  using ptrdiff = ptrdiff_t;

  template<typename T = void>
  using Ptr = std::add_pointer_t<T>;

  template <typename T>
  using FuncPtr = std::add_pointer_t<T>;

  template <typename T>
  using UniquePtr = std::unique_ptr<T>;

  template <typename T, typename... Args>
  inline UniquePtr<T>
  MakeUnique(Args &&...args)
  {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  inline UniquePtr<T>
  MakeUniqueOverwrite(size_t size)
  {
    return std::make_unique_for_overwrite<T>(size);
  }

  template <typename T>
  using SharedPtr = std::shared_ptr<T>;

  template <typename T, typename... Args>
  inline SharedPtr<T>
  MakeShared(Args &&...args)
  {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  using WeakPtr = std::weak_ptr<T>;
  
  template <typename... Args>
  using Action = std::function<void(Args...)>;

  template <typename T>
  using Func = std::function<T>;

  using EmptyLambda = decltype([]() {});

  template <typename T>
  using Raw = std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>;
}

namespace Xivi::Concepts
{
template <typename T>
concept IsPointer = std::is_pointer_v<T>;

template <typename T>
concept IsReference = std::is_reference_v<T>;
}