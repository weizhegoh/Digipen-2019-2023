/*****************************************************************
*\file         Filesystem.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include <filesystem>

namespace Xivi::Filesystem
{
namespace fs = std::filesystem;

void root( fs::path path );
fs::path root();
fs::path temp_dir();

bool create_dir( fs::path dir_name );
void copy_dir( fs::path source, fs::path dest );
bool delete_dir( fs::path dir_name );
size_t delete_all_dir( fs::path dir_name );
bool dir_exists( fs::path dir_name );
std::vector<fs::directory_entry> dir_list( fs::path dir = fs::current_path() );
std::vector<fs::directory_entry> recursive_dir_list( fs::path dir = fs::current_path() );

template <typename Func>
void resursive_dir_callback( fs::path dir, Func &&func )
{
  for ( auto &ref : fs::recursive_directory_iterator( dir ) )
    func( ref );
}

fs::path create_file( fs::path filename );
bool delete_file( fs::path filename );
void copy_file( fs::path source, fs::path dest );
bool file_exists( fs::path filename );
size_t file_size( fs::path filename );
std::vector<fs::path> file_list( fs::path dir = fs::current_path() );

// TBC
// Open / Close files
// Read / Write stream for files
}