/*****************************************************************
*\file		 Character.h
*\brief		 Header file declarations for Character.

*\author(s)   Lee Liang Ping	6 lines x 100% Code contribution

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>

struct Character 
{
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
   // glm::ivec2   startUV;
    //glm::ivec2   endUV;

    //glm::vec2 advance;
    //glm::vec2 dimensions;
    //glm::vec2 bearing;
    //float texOffset;

};
//struct Character 
//{
//    glm::ivec2   startUV;
//    glm::ivec2   endUV;
//
//    glm::vec2 advance;
//    glm::vec2 dimensions;
//    glm::vec2 bearing;
//    float texOffset;
//
//};