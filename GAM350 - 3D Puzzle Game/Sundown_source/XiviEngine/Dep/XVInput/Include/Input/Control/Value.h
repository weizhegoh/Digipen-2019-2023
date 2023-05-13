#pragma once

#include "Base.h"

namespace Xivi::Intput::Control
{

template <typename T>
requires ( std::integral<T> || std::floating_point<T>)
class Value : public Base 
{
  T m_value{} ;
public:
  Value() = default;
  
  Value(Input::Device::Instance& device, T& value ) : 
  Base(device),
  m_value(value) {}
};


}