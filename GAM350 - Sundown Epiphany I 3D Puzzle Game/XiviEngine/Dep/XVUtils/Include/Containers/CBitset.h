#pragma once

#include <Types/Base.h>
#include <array>
#include <stdexcept>
namespace Xivi
{
  template <u64 Bits>
  class CBitset
  {
    using Type = std::conditional_t<Bits <= sizeof(u32) * CHAR_BIT, u32, u64>;
    static constexpr ptrdiff type_bits_count = CHAR_BIT * sizeof(Type);
    static constexpr ptrdiff arr_size = Bits == 0 ? 0 : (Bits - 1) / type_bits_count;

    std::array<Type, arr_size + 1> m_data{};

  public:
    // Constructors
    constexpr CBitset() noexcept = default;

    constexpr CBitset(const CBitset &rhs) noexcept = default;

    constexpr CBitset(CBitset &&rhs) noexcept = default;

    constexpr CBitset &operator=(const CBitset &rhs) noexcept = default;

    constexpr CBitset &operator=(CBitset &&rhs) noexcept = default;

    // Comparison
    friend constexpr bool operator==(const CBitset &lhs, const CBitset &rhs) noexcept
    {
      return lhs.m_data == rhs.m_data;
    }

    friend constexpr bool operator!=(const CBitset &lhs, const CBitset &rhs) noexcept
    {
      return !(lhs == rhs);
    }

    constexpr operator bool() const noexcept
    {
      return any();
    }

    // Element Access
    constexpr std::array<Type, arr_size + 1> data() const noexcept
    {
      return m_data;
    }

    constexpr bool operator[](const u64 pos) const noexcept
    {
      return m_data[pos / type_bits_count] & (Type(1) << (pos % type_bits_count));
    }

    constexpr bool test(u64 pos) const
    {
      check_out_of_range(pos);
      return operator[](pos);
    }

    constexpr bool any() const noexcept
    {
      for (auto i = 0u; i <= arr_size; ++i)
        if (m_data[i] != 0)
          return true;
      return false;
    }

    constexpr bool none() const noexcept
    {
      return !any();
    }

    constexpr bool all() const noexcept
    {
      for (auto i = 0u; i <= arr_size; ++i)
        if (m_data[i] != static_cast<Type>(-1))
          return false;
      return true;
    }

    // Capacity
    constexpr u64 size() const noexcept
    {
      return Bits;
    }

    constexpr i32 count() const noexcept
    {
      auto count = 0;
      for (auto i = 0u; i <= arr_size; ++i)
        count += std::popcount(m_data[i]);
      return count;
    }

    // Modifiers
    constexpr CBitset &set() noexcept
    {
      for (auto i = 0u; i <= arr_size; ++i)
        m_data[i] = ~Type(0);
      trim();
      return *this;
    }

    constexpr CBitset &set(const u64 pos, const bool value = true)
    {
      check_out_of_range(pos);
      return set_internal(pos, value);
    }

    constexpr CBitset &reset() noexcept
    {
      for (auto i = 0u; i <= arr_size; ++i)
        m_data[i] = Type(0);
      return *this;
    }

    constexpr CBitset &reset(const u64 pos)
    {
      check_out_of_range(pos);
      return set_internal(pos, false);
    }

    constexpr CBitset &flip(const u64 pos)
    {
      check_out_of_range(pos);
      return flip_internal(pos);
    }

    constexpr CBitset &flip() noexcept
    {
      for (auto i = 0u; i <= arr_size; ++i)
        m_data[i] = ~m_data[i];
      trim();
      return *this;
    }

    // Bits Operation
    constexpr CBitset &operator&=(const CBitset &rhs) noexcept
    {
      for (auto i = 0u; i <= arr_size; ++i)
        m_data[i] &= rhs.m_data[i];
      return *this;
    }

    constexpr CBitset &operator|=(const CBitset &rhs) noexcept
    {
      for (auto i = 0u; i <= arr_size; ++i)
        m_data[i] |= rhs.m_data[i];
      return *this;
    }

    constexpr CBitset &operator^=(const CBitset &rhs) noexcept
    {
      for (auto i = 0u; i <= arr_size; ++i)
        m_data[i] ^= rhs.m_data[i];
      return *this;
    }

    constexpr CBitset &operator<<=(const u64 shift) noexcept
    {
      const auto index_shift = static_cast<ptrdiff>(shift / type_bits_count);
      if (index_shift)
        for (ptrdiff i = arr_size; i >= 0; --i)
          m_data[i] = index_shift <= i ? m_data[i - index_shift] : 0;

      if (shift % type_bits_count)
      {
        for (ptrdiff i = arr_size; i > 0; --i)
          m_data[i] = (m_data[i] << shift) | (m_data[i - 1] >> (type_bits_count - shift));
        m_data[0] <<= shift;
      }
      trim();
      return *this;
    }

    constexpr CBitset &operator>>=(const u64 shift) noexcept
    {
      const auto index_shift = static_cast<ptrdiff>(shift / type_bits_count);
      if (index_shift)
        for (ptrdiff i = 0; i <= arr_size; ++i)
          m_data[i] = index_shift <= arr_size - i ? m_data[i + index_shift] : 0;

      if (shift % type_bits_count)
      {
        for (ptrdiff i = 0; i < arr_size; ++i)
          m_data[i] = (m_data[i] >> shift) | (m_data[i + 1] << (type_bits_count - shift));
        m_data[arr_size] >>= shift;
      }
      return *this;
    }

    constexpr CBitset operator~() const noexcept
    {
      return CBitset(*this).flip();
    }

    friend constexpr CBitset operator&(const CBitset &lhs, const CBitset &rhs) noexcept
    {
      return CBitset(lhs) &= rhs;
    }

    friend constexpr CBitset operator|(const CBitset &lhs, const CBitset &rhs) noexcept
    {
      return CBitset(lhs) |= rhs;
    }

    friend constexpr CBitset operator^(const CBitset &lhs, const CBitset &rhs) noexcept
    {
      return CBitset(lhs) ^= rhs;
    }

    friend constexpr CBitset operator<<(const CBitset &bitset, const u64 shift) noexcept
    {
      return CBitset(bitset) <<= shift;
    }

    friend constexpr CBitset operator>>(const CBitset &bitset, const u64 shift) noexcept
    {
      return CBitset(bitset) >>= shift;
    }

    // Stream Operation, output only
    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &os, const CBitset &bitset)
    {
      if constexpr (Bits > 0)
        for (auto i = Bits; i > 0;)
          os << (bitset[--i] ? "1" : "0");
      return os;
    }

  private:
    // Internal Helper Functions
    constexpr CBitset &set_internal(u64 pos, bool value = true) noexcept
    {
      if (value)
        m_data[pos / type_bits_count] |= (Type(1) << (pos % type_bits_count));
      else
        m_data[pos / type_bits_count] &= (~(Type(1) << (pos % type_bits_count)));

      return *this;
    }

    constexpr CBitset &flip_internal(u64 pos) noexcept
    {
      m_data[pos / type_bits_count] ^= (Type(1) << (pos % type_bits_count));
      return *this;
    }

    constexpr void check_out_of_range(u64 pos) const
    {
      if (pos >= Bits)
        throw std::out_of_range("CBitset: Testing bit out of range");
    }

    constexpr void trim() noexcept
    {
      if constexpr (Bits == 0 || Bits % type_bits_count != 0)
        m_data[arr_size] &= (Type(1) << Bits % type_bits_count) - 1;
    }
  };
}