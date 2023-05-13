/*
BSD 2-Clause License
XXH3 - C++20 constexpr implementation of XXH3 64-bit variant of xxHash
Copyright (c) 2021, Spencer Tan
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
This file uses code from Yann Collet's xxHash implementation.
Original xxHash copyright notice:
xxHash - Extremely Fast Hash algorithm
Header File
Copyright (C) 2012-2020 Yann Collet
*/

#pragma once

#include "../Types/Base.h"
#include "../Types/Traits/IsArrayType.h"

namespace Xivi::Hash::Details
{

  struct Hash128
  {
    u64 low;
    u64 high;
  };

  using Accumulate512 = FuncPtr<void(Ptr<u64>, Ptr<const u8>, Ptr<const u8>)>;
  using ScrambleAcc = FuncPtr<void(Ptr<u64>, Ptr<const u8>)>;
  using HashLong64 = FuncPtr<u64(Ptr<const u8>, u64, Ptr<const u8>, u64, u64)>;
  using InitCustomSecret = FuncPtr<void(Ptr<u8>, u64)>;

  static constexpr u32 PRIME32_1 = 0x9E3779B1U;
  static constexpr u32 PRIME32_2 = 0x85EBCA77U;
  static constexpr u32 PRIME32_3 = 0xC2B2AE3DU;

  static constexpr u64 PRIME64_1 = 0x9E3779B185EBCA87ULL;
  static constexpr u64 PRIME64_2 = 0xC2B2AE3D27D4EB4FULL;
  static constexpr u64 PRIME64_3 = 0x165667B19E3779F9ULL;
  static constexpr u64 PRIME64_4 = 0x85EBCA77C2B2AE63ULL;
  static constexpr u64 PRIME64_5 = 0x27D4EB2F165667C5ULL;

  static constexpr u64 SECRET_DEFAULT_SIZE = 192;
  static constexpr u64 SECRET_SIZE_MIN = 136;

  static constexpr u64 STRIPE_LEN = 64;
  static constexpr u64 SECRET_CONSUME_RATE = 8;
  static constexpr u64 ACC_NB = STRIPE_LEN / sizeof(uint64_t);

  alignas(8) static constexpr u8 kSecret[SECRET_DEFAULT_SIZE]{
    0xb8, 0xfe, 0x6c, 0x39, 0x23, 0xa4, 0x4b, 0xbe, 0x7c, 0x01, 0x81, 0x2c, 0xf7, 0x21, 0xad, 0x1c,
    0xde, 0xd4, 0x6d, 0xe9, 0x83, 0x90, 0x97, 0xdb, 0x72, 0x40, 0xa4, 0xa4, 0xb7, 0xb3, 0x67, 0x1f,
    0xcb, 0x79, 0xe6, 0x4e, 0xcc, 0xc0, 0xe5, 0x78, 0x82, 0x5a, 0xd0, 0x7d, 0xcc, 0xff, 0x72, 0x21,
    0xb8, 0x08, 0x46, 0x74, 0xf7, 0x43, 0x24, 0x8e, 0xe0, 0x35, 0x90, 0xe6, 0x81, 0x3a, 0x26, 0x4c,
    0x3c, 0x28, 0x52, 0xbb, 0x91, 0xc3, 0x00, 0xcb, 0x88, 0xd0, 0x65, 0x8b, 0x1b, 0x53, 0x2e, 0xa3,
    0x71, 0x64, 0x48, 0x97, 0xa2, 0x0d, 0xf9, 0x4e, 0x38, 0x19, 0xef, 0x46, 0xa9, 0xde, 0xac, 0xd8,
    0xa8, 0xfa, 0x76, 0x3f, 0xe3, 0x9c, 0x34, 0x3f, 0xf9, 0xdc, 0xbb, 0xc7, 0xc7, 0x0b, 0x4f, 0x1d,
    0x8a, 0x51, 0xe0, 0x4b, 0xcd, 0xb4, 0x59, 0x31, 0xc8, 0x9f, 0x7e, 0xc9, 0xd9, 0x78, 0x73, 0x64,
    0xea, 0xc5, 0xac, 0x83, 0x34, 0xd3, 0xeb, 0xc3, 0xc5, 0x81, 0xa0, 0xff, 0xfa, 0x13, 0x63, 0xeb,
    0x17, 0x0d, 0xdd, 0x51, 0xb7, 0xf0, 0xda, 0x49, 0xd3, 0x16, 0x55, 0x26, 0x29, 0xd4, 0x68, 0x9e,
    0x2b, 0x16, 0xbe, 0x58, 0x7d, 0x47, 0xa1, 0xfc, 0x8f, 0xf8, 0xb8, 0xd1, 0x7a, 0xd0, 0x31, 0xce,
    0x45, 0xcb, 0x3a, 0x8f, 0x95, 0x16, 0x04, 0x28, 0xaf, 0xd7, 0xfb, 0xca, 0xbb, 0x4b, 0x40, 0x7e,
};

  inline constexpr u32 Swap32(u32 x)
  {
    return ((x << 24) & 0xff000000) |
           ((x << 8) & 0x00ff0000) |
           ((x >> 8) & 0x0000ff00) |
           ((x >> 24) & 0x000000ff);
  }

  inline constexpr u64 Swap64(u64 x)
  {
    return ((x << 56) & 0xff00000000000000ULL) |
           ((x << 40) & 0x00ff000000000000ULL) |
           ((x << 24) & 0x0000ff0000000000ULL) |
           ((x << 8) & 0x000000ff00000000ULL) |
           ((x >> 8) & 0x00000000ff000000ULL) |
           ((x >> 24) & 0x0000000000ff0000ULL) |
           ((x >> 40) & 0x000000000000ff00ULL) |
           ((x >> 56) & 0x00000000000000ffULL);
  }

  inline constexpr u32 Read32(Ptr<const u8> ptr)
  {
    return u32(u8(ptr[0])) |
           (u32(u8(ptr[1])) << 8) |
           (u32(u8(ptr[2])) << 16) |
           (u32(u8(ptr[3])) << 24);
  }

  inline constexpr u64 Read64(Ptr<const u8> ptr)
  {
    return u64(u8(ptr[0])) |
           (u64(u8(ptr[1])) << 8) |
           (u64(u8(ptr[2])) << 16) |
           (u64(u8(ptr[3])) << 24) |
           (u64(u8(ptr[4])) << 32) |
           (u64(u8(ptr[5])) << 40) |
           (u64(u8(ptr[6])) << 48) |
           (u64(u8(ptr[7])) << 56);
  }

  inline constexpr void Write64(Ptr<u8> dst, u64 v)
  {
    if (std::is_constant_evaluated())
      for (int i = 0; i < 8; ++i)
        dst[i] = u8(v >> (i * 8));
    else
      std::memcpy(dst, &v, sizeof(v));
  }

  __forceinline constexpr u64 XorShift64(u64 digest, int shift)
  {
    return digest ^ (digest >> shift);
  }

    __forceinline constexpr u64 Mult32to64(u64 x, u64 y)
  {
    return (x & 0xFFFFFFFF) * (y & 0xFFFFFFFF);
  }

  inline constexpr Hash128 Mult64to128(u64 lhs, u64 rhs)
  {
    const u64 lo_lo = Mult32to64(lhs & 0xFFFFFFFF, rhs & 0xFFFFFFFF);
    const u64 hi_lo = Mult32to64(lhs >> 32, rhs & 0xFFFFFFFF);
    const u64 lo_hi = Mult32to64(lhs & 0xFFFFFFFF, rhs >> 32);
    const u64 hi_hi = Mult32to64(lhs >> 32, rhs >> 32);

    const u64 cross = (lo_lo >> 32) + (hi_lo & 0xFFFFFFFF) + lo_hi;
    const u64 upper = (hi_lo >> 32) + (cross >> 32) + hi_hi;
    const u64 lower = (cross << 32) | (lo_lo & 0xFFFFFFFF);

    return {.low = lower, .high = upper};
  }

  inline constexpr u64 Mult128Fold64(u64 lhs, u64 rhs)
  {
    Hash128 product = Mult64to128(lhs, rhs);
    return product.low ^ product.high;
  }

  inline constexpr u64 Avalanche64(u64 digest)
  {
    digest ^= digest >> 33;
    digest *= PRIME64_2;
    digest ^= digest >> 29;
    digest *= PRIME64_3;
    digest ^= digest >> 32;
    return digest;
  }

  inline constexpr u64 Avalanche3(u64 digest)
  {
    digest = XorShift64(digest, 37);
    digest *= 0x165667919E3779F9ULL;
    digest = XorShift64(digest, 32);
    return digest;
  }

  inline constexpr u64 RRMXMX(u64 digest, u64 len)
  {
    digest ^= std::rotl(digest, 49) ^ std::rotl(digest, 24);
    digest *= 0x9FB21C651E98DF25ULL;
    digest ^= (digest >> 35) + len;
    digest *= 0x9FB21C651E98DF25ULL;
    return XorShift64(digest, 28);
  }

  __forceinline constexpr u64 Mix16(Ptr<const u8> input, Ptr<const u8> secret, u64 seed)
  {
    const u64 input_lo = Read64(input);
    const u64 input_hi = Read64(input + 8);
    return Mult128Fold64(input_lo ^ (Read64(secret) + seed), input_hi ^ (Read64(secret + 8) - seed));
  }

  __forceinline constexpr u64 Len_1to3_64b(Ptr<const u8> input, u64 len, Ptr<const u8> secret, u64 seed)
  {
    const u8 c1 = input[0];
    const u8 c2 = input[len >> 1];
    const u8 c3 = input[len - 1];

    const u32 combined = ((u32)c1 << 16) |
                         ((u32)c2 << 24) |
                         ((u32)c3 << 0) |
                         ((u32)len << 8);
    const u64 bitflip = (Read32(secret) ^ Read32(secret + 4)) + seed;
    const u64 keyed = (u64)combined ^ bitflip;
    return Avalanche64(keyed);
  }

  __forceinline constexpr u64 Len_4to8_64b(Ptr<const u8> input, u64 len, Ptr<const u8> secret, u64 seed)
  {
    seed ^= (u64)Swap32((u32)seed) << 32;
    const u32 input1 = Read32(input);
    const u32 input2 = Read32(input + len - 4);
    const u64 bitflip = (Read64(secret + 8) ^ Read64(secret + 16)) - seed;
    const u64 input64 = input2 + (((u64)input1) << 32);
    const u64 keyed = input64 ^ bitflip;
    return RRMXMX(keyed, len);
  }

  __forceinline constexpr u64 Len_9to16_64b(Ptr<const u8> input, u64 len, Ptr<const u8> secret, u64 seed)
  {
    const u64 bitflip1 = (Read64(secret + 24) ^ Read64(secret + 32)) + seed;
    const u64 bitflip2 = (Read64(secret + 40) ^ Read64(secret + 48)) - seed;
    const u64 input_lo = Read64(input) ^ bitflip1;
    const u64 input_hi = Read64(input + len - 8) ^ bitflip2;
    const u64 acc = len + Swap64(input_lo) + input_hi + Mult128Fold64(input_lo, input_hi);
    return Avalanche3(acc);
  }



  __forceinline constexpr u64 Len_0to16_64b(Ptr<const u8> input, u64 len, Ptr<const u8> secret, u64 seed)
  {
    if (len > 8)
      return Len_9to16_64b(input, len, secret, seed);
    if (len >= 4)
      return Len_4to8_64b(input, len, secret, seed);
    if (len)
      return Len_1to3_64b(input, len, secret, seed);
    return Avalanche64(seed ^ (Read64(secret + 56) ^ (Read64(secret + 64))));
  }

  __forceinline constexpr u64 Len_17to128_64b(Ptr<const u8> input, u64 len, Ptr<const u8> secret, u64 secret_len, u64 seed)
  {
    (void)secret_len;
    u64 acc = len * PRIME64_1;
    if (len > 32)
    {
      if (len > 64)
      {
        if (len > 96)
        {
          acc += Mix16(input + 48, secret + 96, seed);
          acc += Mix16(input + len - 64, secret + 112, seed);
        }
        acc += Mix16(input + 32, secret + 64, seed);
        acc += Mix16(input + len - 48, secret + 80, seed);
      }
      acc += Mix16(input + 16, secret + 32, seed);
      acc += Mix16(input + len - 32, secret + 48, seed);
    }
    acc += Mix16(input + 0, secret + 0, seed);
    acc += Mix16(input + len - 16, secret + 16, seed);
    return Avalanche3(acc);
  }

  __declspec(noinline) constexpr u64 Len_129to240_64b(Ptr<const u8> input, u64 len, Ptr<const u8> secret, u64 secret_len, u64 seed)
  {
    (void)secret_len;
    constexpr u8 STARTOFFSET = 3;
    constexpr u8 LASTOFFSET = 17;

    u64 acc = len * PRIME64_1;
    const i32 nb_rounds = (i32)len / 16;
    i32 i;
    for (i = 0; i < 8; ++i)
      acc += Mix16(input + (16 * i), secret + (16 * i), seed);
    acc = Avalanche3(acc);

    for (i = 8; i < nb_rounds; ++i)
      acc += Mix16(input + (16 * i), secret + (16 * (i - 8)) + STARTOFFSET, seed);
    acc += Mix16(input + len - 16, secret + SECRET_SIZE_MIN - LASTOFFSET, seed);
    return Avalanche3(acc);
  }

  __forceinline constexpr void Accumulate512Scalar(Ptr<u64> acc, Ptr<const u8> input, Ptr<const u8> secret)
  {
    const Ptr<u64> xacc = acc;
    const Ptr<const u8> xinput = input;
    const Ptr<const u8> xsecret = secret;
    u64 i;
    for (i = 0; i < ACC_NB; ++i)
    {
      const u64 data_val = Read64(xinput + 8 * i);
      const u64 data_key = data_val ^ Read64(xsecret + i * 8);
      xacc[i ^ i] += data_val;
      xacc[i] += Mult32to64(data_key & 0xFFFFFFFF, data_key >> 32);
    }
  }

  __forceinline constexpr void ScrambleAccScalar(Ptr<u64> acc, Ptr<const u8> secret)
  {
    const Ptr<u64> xacc = acc;
    const Ptr<const u8> xsecret = secret;
    u64 i;
    for (i = 0; i < ACC_NB; ++i)
    {
      const u64 key64 = Read64(xsecret + 8 * i);
      u64 acc64 = xacc[i];
      acc64 = XorShift64(acc64, 47);
      acc64 ^= key64;
      acc64 *= PRIME32_1;
      xacc[i] = acc64;
    }
  }

  __forceinline constexpr void InitCustomSecretScalar(Ptr<u8> secret, u64 seed)
  {
    Ptr<const u8> secret_ptr = kSecret;
    const int nb_rounds = SECRET_DEFAULT_SIZE / 16;
    int i;
    for (i = 0; i < nb_rounds; ++i)
    {
      u64 lo = Read64(secret_ptr + 16 * i) + seed;
      u64 hi = Read64(secret_ptr + 16 * i + 8) - seed;
      Write64(secret + 16 * i, lo);
      Write64(secret + 16 * i + 8, hi);
    }
  }

  __forceinline constexpr void Accmulate(Ptr<u64> acc, Ptr<const u8> input, Ptr<const u8> secret, u64 nb_stripes, Accumulate512 acc512)
  {
    u64 n;
    for (n = 0; n < nb_stripes; ++n)
    {
      const Ptr<const u8> in = input + n * STRIPE_LEN;
      acc512(acc, in, secret + n * SECRET_CONSUME_RATE);
    }
  }

  __forceinline constexpr void Long_Internal_Loop(Ptr<u64> acc, Ptr<const u8> input, u64 len, Ptr<const u8> secret, u64 secret_len, Accumulate512 acc512, ScrambleAcc scramble)
  {
    const u64 nb_stripes_per_block = (secret_len - STRIPE_LEN) / SECRET_CONSUME_RATE;
    const u64 block_len = STRIPE_LEN * nb_stripes_per_block;
    const u64 nb_blocks = (len - 1) / block_len;

    u64 n;
    for (n = 0; n < nb_blocks; ++n)
    {
      Accmulate(acc, input + n * block_len, secret, nb_stripes_per_block, acc512);
      scramble(acc, secret + secret_len - STRIPE_LEN);
    }

    const u64 nb_stripes = ((len - 1) - (block_len * nb_blocks)) / STRIPE_LEN;
    Accmulate(acc, input + nb_blocks * block_len, secret, nb_stripes, acc512);

    const Ptr<const u8> ptr = input + len - STRIPE_LEN;
    acc512(acc, ptr, secret + secret_len - STRIPE_LEN - 7);
  }

  __forceinline constexpr u64 Mix2Accs(Ptr<const u64> acc, Ptr<const u8> secret)
  {
    return Mult128Fold64(acc[0] ^ Read64(secret), acc[1] ^ Read64(secret + 8));
  }

  inline constexpr u64 MergeAcc(Ptr<const u64> acc, Ptr<const u8> secret, u64 start)
  {
    u64 result64 = start;
    u64 i = 0;
    for (i = 0; i < 4; ++i)
      result64 += Mix2Accs(acc + 2 * i, secret + 16 * i);

    return Avalanche3(result64);
  }

  __forceinline constexpr u64 Long_64b_Internal(Ptr<const u8> input, u64 len, Ptr<const u8> secret, u64 secret_len, Accumulate512 acc512, ScrambleAcc scramble)
  {
    alignas(8) u64 acc[ACC_NB]{PRIME32_3, PRIME64_1, PRIME64_2, PRIME64_3,
                               PRIME64_4, PRIME32_2, PRIME64_5, PRIME32_1};

    Long_Internal_Loop(acc, input, len, secret, secret_len, acc512, scramble);

    return MergeAcc(acc, secret + 11, len * PRIME64_1);
  }

  __declspec(noinline) constexpr u64 Long_64b(Ptr<const u8> input, u64 len, Ptr<const u8> secret, u64 secret_len, u64 seed)
  {
    (void)seed;
    (void)secret;
    (void)secret_len;
    return Long_64b_Internal(input, len, kSecret, sizeof(kSecret), Accumulate512Scalar, ScrambleAccScalar);
  }

  __forceinline constexpr u64 Long_64b_Seed_Internal(Ptr<const u8> input, u64 len,u64 seed, Accumulate512 acc512, ScrambleAcc scramble, InitCustomSecret init_custom_secret) 
  {
    if (seed == 0)
      return Long_64b_Internal(input, len, kSecret, sizeof(kSecret), acc512, scramble);
    
    alignas(8) u8 secret[SECRET_DEFAULT_SIZE];
    init_custom_secret(secret, seed);
    return Long_64b_Internal(input, len, secret, sizeof(secret), acc512, scramble);
  }

  __declspec(noinline) constexpr u64 Long_64b_Seed(Ptr<const u8> input, u64 len, Ptr<const u8> secret, u64 secret_len, u64 seed)
  {
    (void) secret; (void) secret_len;
    return Long_64b_Seed_Internal(input, len, seed, Accumulate512Scalar, ScrambleAccScalar, InitCustomSecretScalar);
  }

  __forceinline constexpr u64 XXH3_64_Internal(Ptr<const u8> input, u64 len, u64 seed, Ptr<const u8> secret, u64 secret_len, HashLong64 hash_long)
  {
    if (len <= 16)
      return Len_0to16_64b(input, len, secret, seed);
    if (len <= 128)
      return Len_17to128_64b(input, len, secret, secret_len, seed);
    if (len <= 240)
      return Len_129to240_64b(input, len, secret, secret_len, seed);
    return hash_long(input, len, secret, secret_len, seed);
  }

}

namespace Xivi::Hash
{
  // Hash any fundamental object
  // Compile-time usage
  template <typename T>
  requires(!Traits::Concepts::StringType<T> && !Traits::Concepts::ArrayType<T>) 
  inline constexpr u64 XXH3_64(const T &obj, u64 seed = 0)
  {
    if (!std::is_constant_evaluated())
      return Details::XXH3_64_Internal(std::bit_cast<Ptr<const u8>>(&obj), sizeof(T), seed, Details::kSecret, sizeof(Details::kSecret), Details::Long_64b_Seed);

    return Details::XXH3_64_Internal(std::bit_cast<std::array<u8, sizeof(T)>>(obj).data(), sizeof(T), seed, Details::kSecret, sizeof(Details::kSecret), Details::Long_64b_Seed);
  }

  // Hash any string
  // Compile-time usage
  inline constexpr u64 XXH3_64(std::string_view str, u64 seed = 0)
  {
    if (!std::is_constant_evaluated())
      return Details::XXH3_64_Internal(std::bit_cast<Ptr<const u8>>(str.data()), str.length(), seed, Details::kSecret, sizeof(Details::kSecret), Details::Long_64b_Seed);

    std::vector<u8> data;
    for (const auto &c : str)
      data.push_back(c);

    return Details::XXH3_64_Internal(data.data(), data.size(), seed, Details::kSecret, sizeof(Details::kSecret), Details::Long_64b_Seed);
  }

  // Hash any array
  // Compile-time usage
  template <typename T>
  requires(!Traits::Concepts::StringType<T> && Traits::Concepts::ArrayType<T>) 
  inline constexpr u64 XXH3_64(const T &arr, u64 seed = 0)
  {
    auto arr_view = std::span(arr);

    if (!std::is_constant_evaluated())
      return Details::XXH3_64_Internal(std::bit_cast<Ptr<const u8>>(arr_view.data()), arr_view.size_bytes(), seed, Details::kSecret, sizeof(Details::kSecret), Details::Long_64b_Seed);

    std::vector<u8> data;
    for (const auto &t : arr_view)
      for (const auto &c : std::bit_cast<std::array<u8,sizeof(t)>>(t))
        data.push_back(c);

    return Details::XXH3_64_Internal(data.data(), data.size(), seed, Details::kSecret, sizeof(Details::kSecret), Details::Long_64b_Seed);
  }

}
