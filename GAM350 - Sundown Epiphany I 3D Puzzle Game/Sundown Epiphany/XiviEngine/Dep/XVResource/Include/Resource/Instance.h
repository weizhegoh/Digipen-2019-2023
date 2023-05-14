#pragma once

#include <Utils/Assert.h>

namespace Xivi::Resource
{
template <typename Base>
struct Instance
{
  using ReferenceCount = u32;
  UniquePtr<Base> m_data { nullptr };
  std::string m_file {};
  ReferenceCount m_reference_count { 0 };
  Resource::State m_state { Resource::State::Final };
  Resource::Policy m_policy { Resource::Policy::Persistant };

  explicit Instance() = default;
  explicit Instance( const std::string& file, Ptr<Base> data, Resource::State state, Resource::Policy policy ) :
    m_file(file),
    m_data( data ),
    m_state( state ),
    m_policy( policy )
  {}

// No Copy / Move
  Instance( const Instance & ) = delete;
  Instance &operator=( const Instance & ) = delete;
  Instance( Instance &&rhs ) = delete;
  Instance &operator=( Instance && ) = delete;

  ~Instance()
  {
    XV_CORE_ASSERT_MSG( m_reference_count == 0, "[Data] Existing reference exists" );
  }
};
} // namespace Xivi