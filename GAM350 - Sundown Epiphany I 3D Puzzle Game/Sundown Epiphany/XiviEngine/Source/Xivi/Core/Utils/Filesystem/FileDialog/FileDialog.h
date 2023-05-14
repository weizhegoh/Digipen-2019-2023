/*****************************************************************
*\file         FileDialog.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include <optional>
#include <string>

namespace Xivi::FileDialogs
{
std::optional<std::string> OpenFile( const char *filter );
std::optional<std::string> SaveFile( const char *filter );
}
