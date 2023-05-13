# XVUtils

The following repository is a collection of functions and containers that I am using for my own personal game engine. 

## To Use

As spdlog is added through submodule, please do the following before using:

git submodule sync \
git submodule update --init --recursive --remote

Include the following definition in your files before including the header "UtilitiesPackage.h"
#define XV_PLATFORM_WINDOWS

The current set-up is only geared at Windows Usage with the PinnedVector using windows VirtualAlloc

## Includes
- Core
- Log (Using spdlog)

### Utils
- Assert
- Enumerate (Python-style enumerate)
- NonCopyable 
- Singleton 
- Typename (Extracting readable typename)
- XXH3 (Compile-time variant of XXH3 based on the original XXH3 hashing library)

### Types
- Base
- GUID (Phantom typing for UUID/GUID)

#### Traits
- Function
- IsArrayType (Cover array type like std::array, std::vector, std::span and the std::is_array)
- Specialised (Identifying if template type is specialised)
- TupleType

#### Typelist
- Append
- Concatenate
- Extract
- Find
- Index
- Remove
- Unique

### Containers
- ActionList
- Buffer 
- CBitset (Compile-time variant of bitset)
- PinnedVector (Virtual allocated vector, that does not invalidates iterator on resizing... Note not all functionality implemented)
- RingBuffer
- ThreadPool
- ThreadQueue