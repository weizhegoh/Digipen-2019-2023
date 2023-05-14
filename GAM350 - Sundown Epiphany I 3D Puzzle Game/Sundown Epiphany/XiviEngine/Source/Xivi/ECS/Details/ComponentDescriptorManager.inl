/*****************************************************************
*\file         ComponentDescriptorManager.inl
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS::Component::Details
{
inline std::string_view Typename( Component::Type::ID id )
{
  switch ( id )
  {
    case Component::Type::ID::Data:
    return "Data";
    break;
    case Component::Type::ID::Shared:
    return "Shared";
    break;
    case  Component::Type::ID::Tag:
    return "Tag";
    case  Component::Type::ID::Exclusive:
    return "Exclusive";
    case Component::Type::ID::Singleton:
    return "Singleton";
    default:
    break;
  }
  XV_CORE_ASSERT_MSG( false, "[ComponentDescriptorManager] Invalid Component ID" );
  return "";
}
}

namespace Xivi::ECS::Component
{
template <Concepts::IsComponent C>
void DescriptorManager::Register() noexcept
{
  if ( m_lock )
    return;

  if ( info<C>.m_bit_index == Descriptor::invalid_bit_index )
  {
    if constexpr ( info<C>.m_type == Type::ID::Shared )
    {
      C tmp {};
      info<C>.m_shared_key = info<C>.m_generate_key( std::bit_cast <Ptr<const byte>>( &tmp ) );
    }

    info<C>.m_bit_index = Descriptor::processing_bit_index;
    m_descriptor_array.emplace_back( &info<C> );
    ++m_types_count;
    //m_descriptor_array[m_types_count++] = &info<C>;
  }
}

void DescriptorManager::Lock() noexcept
{
  if ( m_lock )
    return;

  m_lock = true;

  std::sort( m_descriptor_array.begin(), m_descriptor_array.begin() + m_types_count, CompareDescriptor );

  for ( u16 i = 0u; i < m_types_count; ++i )
  {
    XV_CORE_ASSERT_MSG( m_descriptor_array[i]->m_bit_index == Descriptor::processing_bit_index, "[ComponentDescriptorManager] Invalid bit index" );

    m_descriptor_array[i]->m_bit_index = i;

    m_descriptor_map.emplace( std::pair { m_descriptor_array[i]->m_id,m_descriptor_array[i] } );

    switch ( m_descriptor_array[i]->m_type )
    {
      case Type::ID::Tag:
      m_tag_bits.set( i );
      break;
      case Type::ID::Data:
      m_data_bits.set( i );
      break;
      case Type::ID::Shared:
      m_shared_bits.set( i );
      break;
      case Type::ID::Exclusive:
      m_tag_bits.set( i );
      m_exclusive_tag_bits.set( i );
      break;
    }
  }

  for ( auto i = 0u; i < m_types_count; ++i )
  {
    const auto &desc = m_descriptor_array[i];
    XV_CORE_TRACE( "[ComponentDescriptorManager] Component - {}, Type - {}, ID - {}, Bit - {}",
                   desc->m_name,
                   Details::Typename( desc->m_type ),
                   desc->m_id.m_value,
                   desc->m_bit_index );
  }
}

bool DescriptorManager::IsLocked() const noexcept
{
  return m_lock;
}

std::vector<Ptr<const Descriptor>> DescriptorManager::DescriptorVector( const Signature &signature ) const noexcept
{
  std::vector<Ptr<const Descriptor>> result;
  for ( u32 i = 0; i < m_types_count; ++i )
    if ( signature[m_descriptor_array[i]->m_bit_index] )
      result.push_back( m_descriptor_array[i] );
  return result;
}

u32 DescriptorManager::CountDataComponent( const Signature &signature ) const noexcept
{
  auto sig = signature & m_data_bits;
  return sig.count();
}

u32 DescriptorManager::CountSharedComponent( const Signature &signature ) const noexcept
{
  auto sig = signature & m_shared_bits;
  return sig.count();
}

u32 DescriptorManager::CountTagComponent( const Signature &signature ) const noexcept
{
  auto sig = signature & m_tag_bits;
  return sig.count();
}

u32 DescriptorManager::CountExclusiveTagomponent( const Signature &signature ) const noexcept
{
  auto sig = signature & m_exclusive_tag_bits;
  return sig.count();
}

Ptr<const Descriptor> DescriptorManager::Get( Component::ID id ) const noexcept
{
  auto it = m_descriptor_map.find( id );
  if ( it != m_descriptor_map.end() )
    return it->second;
  return nullptr;
}
}