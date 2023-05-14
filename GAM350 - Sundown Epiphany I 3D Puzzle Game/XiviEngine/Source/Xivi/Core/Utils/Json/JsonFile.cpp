/**********************************************************************************
* \file   JsonFile.cpp
* \brief  This file contains the implementation of the Json File interface for
*         serialization and deserialization using RTTR
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "pch.h"
#include "JsonFile.h"
#include "JsonSerialise.h"
#include "JsonDeserialise.h"
#include <string>
using namespace rttr;
using namespace rapidjson;

namespace Xivi
{
JsonFile &JsonFile::StartWriter( const std::string &filename )
{
  XV_CORE_ASSERT_MSG( m_fp == nullptr, "[JsonFile] Another file is currently opened, close current operation" );
  fopen_s( &m_fp, filename.c_str(), "wb" );
  m_buffer = new char[65536] {};
  m_wstream = new FileWriteStream( m_fp, m_buffer, 65536 );
  m_writer = new PrettyWriter<FileWriteStream>( *m_wstream );
  return *this;
}

JsonFile &JsonFile::EndWriter()
{
  delete m_writer;
  delete m_wstream;
  delete m_buffer;
  fclose( m_fp );
  m_fp = nullptr;
  return *this;
}

JsonFile &JsonFile::StartObject()
{
  m_writer->StartObject();
  return *this;
}

JsonFile &JsonFile::EndObject()
{
  m_writer->EndObject();
  return *this;
}

JsonFile &JsonFile::StartArray()
{
  m_writer->StartArray();
  return *this;
}

JsonFile &JsonFile::EndArray()
{
  m_writer->EndArray();
  return *this;
}

JsonFile &JsonFile::WriteKey( const std::string &str )
{
  m_writer->Key( str );
  return *this;
}

JsonFile &JsonFile::WriteString( const std::string &str )
{
  m_writer->String( str );
  return *this;
}

JsonFile &JsonFile::WriteBool( bool b )
{
  m_writer->Bool( b );
  return *this;
}

JsonFile &JsonFile::WriteU32( u32 v )
{
  m_writer->Uint( v );
  return *this;
}

JsonFile &JsonFile::WriteU64( u64 v )
{
  m_writer->Uint64( v );
  return *this;
}

JsonFile &JsonFile::WriteI32( i32 v )
{
  m_writer->Int( v );
  return *this;
}

JsonFile &JsonFile::WriteF64( f64 v )
{
  m_writer->Double( v );
  return *this;
}

JsonFile &JsonFile::WriteObject( instance object )
{
  Serialise::WriteObject( object, *m_writer );
  return *this;
}

JsonFile &JsonFile::WriteArray( variant_sequential_view view )
{
  Serialise::WriteArray( view, *m_writer );
  return *this;
}

JsonFile &JsonFile::WriteAssociative( variant_associative_view view )
{
  Serialise::WriteObject( view, *m_writer );
  return *this;
}

JsonFile &JsonFile::StartReader( const std::string &filename )
{
  XV_CORE_ASSERT_MSG( m_fp == nullptr, "[JsonFile] Another file is currently opened, close current operation" );
  fopen_s( &m_fp, filename.c_str(), "rb" );
  m_buffer = new char[65536];
  m_rstream = new FileReadStream( m_fp, m_buffer, 65536 );
  m_doc = new Document();
  m_doc->ParseStream( *m_rstream );
  XV_CORE_ASSERT_MSG( !m_doc->HasParseError(), "[JsonFile] Document parsing have failed" );
  return *this;
}

JsonFile &JsonFile::EndReader()
{
  delete m_doc;
  delete m_rstream;
  delete m_buffer;
  fclose( m_fp );
  m_fp = nullptr;
  return *this;
}

JsonFile &JsonFile::LoadArray( variant_sequential_view &view, Value &json_array_value )
{
  Deserialise::ReadArray( view, json_array_value );
  return *this;
}

JsonFile &JsonFile::LoadAssociative( variant_associative_view &view, Value &json_array_value )
{
  Deserialise::ReadAssociative( view, json_array_value );
  return *this;
}

JsonFile &JsonFile::LoadObject( instance object )
{
  Deserialise::ReadObject( object, *m_doc );
  return *this;
}

rapidjson::Value::MemberIterator JsonFile::FindMember( const std::string &str )
{
  rapidjson::Value::MemberIterator it = m_doc->FindMember( str );
  XV_CORE_ASSERT_MSG( it != m_doc->MemberEnd(), "[JsonFile] Member cannot be found!" );
  return it;
}

bool JsonFile::HasMember( const std::string &str )
{
  return m_doc->HasMember( str );
}
}