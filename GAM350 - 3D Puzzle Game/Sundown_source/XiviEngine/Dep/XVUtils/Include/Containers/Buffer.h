#pragma once
#include <Types/Base.h>
#include <Utils/Assert.h>
namespace Xivi
{

  class Buffer
  {
    Ptr<byte> m_data{nullptr};
    u32 m_size{0};

  public:
    Buffer() = default;

    Buffer(u32 size)
    {
      Allocate(size);
    }

    Buffer(const byte *data, u32 size)
    {
      Allocate(size);
      if (m_data)
        std::memcpy(m_data, data, size);
    }

    template <typename T>
    Buffer(const T &obj)
    {
      Allocate(sizeof(obj));
      if (m_data)
        std::memcpy(m_data, &obj, sizeof(T));
    }

    Buffer(const Buffer &rhs)
    {
      Allocate(rhs.m_size);
      if (m_data)
        std::memcpy(m_data, rhs.m_data, rhs.m_size);
    }

    Buffer &operator=(const Buffer &rhs)
    {
      if (this != &rhs)
      {
        Allocate(rhs.m_size);
        std::memcpy(m_data, rhs.m_data, rhs.m_size);
      }
      return *this;
    }

    Buffer(Buffer &&rhs) noexcept : m_data(rhs.m_data), m_size(rhs.m_size)
    {
      rhs.m_data = nullptr;
      rhs.m_size = 0;
    }

    Buffer &operator=(Buffer &&rhs) noexcept
    {
      if (this != &rhs)
      {
        delete[] m_data;
        m_data = rhs.m_data;
        m_size = rhs.m_size;

        rhs.m_data = nullptr;
        rhs.m_size = 0;
      }
      return *this;
    }

    ~Buffer()
    {
      if (m_data)
        delete[] m_data;
    }

    static Buffer CreateFrom(const byte *data, u32 size)
    {
      return Buffer(data, size);
    }

    template <typename T>
    static Buffer CreateFrom(const T &obj)
    {
      return Buffer(obj);
    }

    void Allocate(u32 size)
    {
      if (size)
      {
        delete[] m_data;
        m_data = nullptr;

        m_data = new byte[size];
        m_size = size;
      }
    }

    void Free()
    {
      delete[] m_data;
      m_data = nullptr;
      m_size = 0;
    }

    void Initialise()
    {
      if (m_data)
        memset(m_data, 0, m_size);
    }

    template <typename T>
    T &Read(u32 offset = 0)
    {
      return *std::bit_cast<T *>(m_data + offset);
    }

    template <typename T>
    void Write(const T &obj, u32 offset = 0)
    {
      XV_ASSERT_MSG(sizeof(T) + offset <= m_size, "[Buffer] Out of Bound");
      memcpy(m_data + offset, &obj, sizeof(T));
    }

    void Write(byte *data, u32 size, u32 offset = 0)
    {
      XV_ASSERT_MSG(size + offset <= m_size, "[Buffer] Out of Bound");
      memcpy(m_data + offset, data, size);
    }

    u32 Size() const
    {
      return m_size;
    }

    operator bool() const
    {
      return m_data;
    }

    byte &operator[](int index)
    {
      return m_data[index];
    }

    byte operator[](int index) const
    {
      return m_data[index];
    }
  };

}