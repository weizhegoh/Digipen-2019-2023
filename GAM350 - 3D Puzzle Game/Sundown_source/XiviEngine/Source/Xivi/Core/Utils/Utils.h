/*****************************************************************
*\file         Utils.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

// STD Core
#include <type_traits>
#include <cstdint>
#include <climits>
#include <cmath>
#include <functional>
#include <future>
#include <memory>
#include <bit>
#include <chrono>
#include <thread>
#include <random>
#include <concepts>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <filesystem>
#include <atomic>
#include <fstream>
#include <iostream>
#include <chrono>
#include <sstream>

// STD Debug
#include <stdexcept>
#include <cassert>

// STD Containers
#include <array>
#include <vector>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <queue>
#include <list>
#include <iterator>
#include <tuple>

// Core
#include "Memory/Memory.h"
#include "Math.h"// Math

// Filesystem
#include "Filesystem/Filesystem.h"
#include "Filesystem/VFS/VirtualFilesystem.h"
#include "Filesystem/Watcher/IFileWatcherListener.h"
#include "Filesystem/Watcher/FileWatcher.h"
