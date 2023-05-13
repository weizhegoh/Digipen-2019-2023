/*****************************************************************
*\file         SharedFilter.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
namespace Xivi::ECS
{
struct SharedFilter
{
  static constexpr auto info = Component::Type::Data { .m_name = "Shared Filter" };

  struct Entry
  {
    Ptr<Archetype::Instance> m_archetype;
    std::vector<Ptr<Pool::Family::Instance>> m_families;
  };

  std::vector<Entry> m_entries;
};
}

namespace Xivi
{
using SharedFilter = ECS::SharedFilter;
}

namespace Xivi::ECS::Components::SharedFilter::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::SharedFilter>( "Shared Filter" )
  ( rttr::metadata( "GUID", Component::info<Xivi::SharedFilter>.m_id ) )
  .property( "Entries", &Xivi::SharedFilter::m_entries )( rttr::metadata( "NoSerialise", true ) );
}
}