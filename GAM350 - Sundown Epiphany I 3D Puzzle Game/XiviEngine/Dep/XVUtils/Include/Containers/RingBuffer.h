#pragma once
#include <Types/Base.h>
#include <Utils/NonCopyable.h>

namespace Xivi
{
  // Power of 2 for N only
  template <typename T, size_t N>
  class RingBuffer : NonCopyable
  {
    UniquePtr<T[]> buffer;

    size_t mask;
    unsigned head;
    unsigned tail;

  public:
    RingBuffer() : buffer(MakeUnique<T[]>(N + 1)), mask(N), head(0), tail(0) {}

    bool IsFull() const
    {
      return ((tail + 1) & mask) == head;
    }

    bool IsEmpty() const
    {
      return head == tail;
    }

    size_t Capacity() const
    {
      return N;
    }

    size_t Count() const
    {
      size_t count = tail - head;
      if (count < 0)
        count += N + 1;
      return static_cast<size_t>(count);
    }

    void Clear()
    {
      head = tail;
    }

    bool Enqueue(const T &data)
    {
      size_t t = (tail + 1) & mask;
      if (t != head)
      {
        buffer[tail] = data;
        tail = t;
        return true;
      }
      return false;
    }

    bool Enqueue(const T &&data)
    {
      size_t t = (tail + 1) & mask;
      if (t != head)
      {
        buffer[tail] = std::move(data);
        tail = t;
        return true;
      }
      return false;
    }

    bool Peek(T &output) const
    {
      if (head != tail)
      {
        output = buffer[head];
        return true;
      }
      return false;
    }

    const T *Peek() const
    {
      if (head != tail)
        return buffer[head];
      return nullptr;
    }

    bool Dequeue(T &output)
    {
      if (head != tail)
      {
        output = std::move(buffer[head]);
        head = (head + 1) & mask;

        return true;
      }
      return false;
    }

    bool Dequeue()
    {
      if (head != tail)
      {
        head = (head + 1) & mask;
        return true;
      }
      return false;
    }
  };

}