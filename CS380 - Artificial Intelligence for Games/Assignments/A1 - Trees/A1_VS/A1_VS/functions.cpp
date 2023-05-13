/*!*****************************************************************************
\file functions.cpp
\author Vadim Surov, Goh Wei Zhe
\par DP email: vsurov\@digipen.edu, weizhe.goh\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 1
\date 05-19-2021
\brief
This file has declarations and definitions that are required for submission
*******************************************************************************/

#include "functions.h"

namespace AI 
{
    /*!*************************************************************************
    \brief
    Function to convert a string to integer. 

    \param str
    The string passed in to be converted into integer.

    \return
    Return the string as integer. 
    ***************************************************************************/
    int stringToInt(std::string str)
    {
        const char* stringToInt = &str[1];
        int numChild = std::atoi(stringToInt);

        return numChild;
    }
}
