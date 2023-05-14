/**********************************************************************************
* \file   JsonFile.h
* \brief  This file contains the definition of the Json File interface for
*         serialization and deserialization using RTTR
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/prettywriter.h> // for stringify JSON
#include <rapidjson/filewritestream.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rttr/type>

namespace Xivi
{
class JsonFile
{
  char *m_buffer { nullptr };
  FILE *m_fp { nullptr };
  rapidjson::FileWriteStream *m_wstream { nullptr };
  rapidjson::FileReadStream *m_rstream { nullptr };
  rapidjson::PrettyWriter<rapidjson::FileWriteStream> *m_writer { nullptr };
  rapidjson::Document *m_doc { nullptr };

public:

  // Writing Operations
  JsonFile &StartWriter( const std::string &filename );
  JsonFile &EndWriter();
  JsonFile &StartObject();
  JsonFile &EndObject();
  JsonFile &StartArray();
  JsonFile &EndArray();
  JsonFile &WriteKey( const std::string &str );
  JsonFile &WriteString( const std::string &str );
  JsonFile &WriteBool( bool b );
  JsonFile &WriteU32( u32 );
  JsonFile &WriteU64( u64 );
  JsonFile &WriteI32( i32 );
  JsonFile &WriteF64( f64 );
  JsonFile &WriteObject( rttr::instance object );
  JsonFile &WriteArray( rttr::variant_sequential_view view );
  JsonFile &WriteAssociative( rttr::variant_associative_view view );

  // Reading Operations
  JsonFile &StartReader( const std::string &filename );
  JsonFile &EndReader();
  JsonFile &LoadArray( rttr::variant_sequential_view &view, rapidjson::Value &json_array_value );
  JsonFile &LoadAssociative( rttr::variant_associative_view &view, rapidjson::Value &json_array_value );
  JsonFile &LoadObject( rttr::instance object );

  rapidjson::Value::MemberIterator FindMember( const std::string &str );
  bool HasMember( const std::string &str );
};
}
