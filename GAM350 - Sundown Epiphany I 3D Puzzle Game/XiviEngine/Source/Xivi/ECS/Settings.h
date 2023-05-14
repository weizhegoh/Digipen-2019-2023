/*****************************************************************
*\file         Settings.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi::ECS::Settings
{
  // Pool
static constexpr auto page_size = 4096;
static constexpr auto max_entities = 16383;
static constexpr auto max_components = 192;
static constexpr auto max_data_components_per_entity = 32;
static constexpr auto max_shared_components_per_entity = 8;
static constexpr auto max_tag_components_per_entity = 32;
}