/*****************************************************************
*\file         Memory.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "pch.h"

#ifdef XV_PROFILE
//void *operator new ( std::size_t size )
//{
//  void *result = std::malloc( size );
//  TracyAlloc( result, size );
//  return result;
//}
//
//void *operator new[]( std::size_t size )
//{
//  void *result = std::malloc( size );
//  TracyAlloc( result, size );
//  return result;
//}
//
//void operator delete( void *p ) throw( )
//{
//  TracyFree( p );
//  std::free(p);
//}
//
//void operator delete[]( void *p ) throw( )
//{
//  TracyFree( p );
//  std::free( p );
//}
#endif