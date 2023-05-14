/*****************************************************************
*\file		 AI.h
*\brief		 Components for AI

*\author(s)   Yip Ling Kit Justin	11 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct AI
{
    enum TURN_TYPE turn_type { TURN_TYPE::TURN_LEFT };          //Which direction the AI turns when it hits a wall
    enum TURN_TYPE corner_turn{ TURN_TYPE::TURN_RIGHT };        //Which direction the AI checks to turn corners
    enum ObjectDirection start_dir { ObjectDirection::RIGHT };  //AI's starting direction
    float turn_delay_max{ 3.0f };                               //How long before the AI checks corner turning again
    float turn_delay{ 0.f };                                    //Initial Delay before the AI checks corner turning
    float lifetime;

    float time{ 0.f };
};