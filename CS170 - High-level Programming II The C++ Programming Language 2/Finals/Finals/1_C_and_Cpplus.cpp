        #include "1_C_and_Cpplus.h"

//g++ -Wall -Wextra -Werror -Wconversion -pedantic -std=c++11 -o Test.exe Test.cpp
//std:c++11: cl /W4 /WX /Za /nologo /EHa /Femain.exe main.cpp  

/*
#include <iostream>

struct Point
{
    int x;
    int y;
};

struct AABB
{
    Point min;
    Point max;
};

AABB aabb(Point pp[], int size)
{
    AABB aabb;

    aabb.min.x = pp[0].x;
    aabb.max.x = pp[0].x;
    aabb.min.y = pp[0].y;
    aabb.max.y = pp[0].y;

    for (int i = 1;i < size;i++)
    {
        if (pp[i].x <= aabb.min.x)
        {
            aabb.min.x = pp[i].x;
        }

        if (pp[i].x >= aabb.max.x)
        {
            aabb.max.x = pp[i].x;
        }

        if (pp[i].y <= aabb.min.y)
        {
            aabb.min.y = pp[i].y;
        }

        if (pp[i].y >= aabb.max.y)
        {
            aabb.max.y = pp[i].y;
        }
    }
    return aabb;
}

// Do not edit below this line
int main()
{
    Point pp[] = { {1,4},{-1,8},{4,-7},{0,3} };
    AABB value = aabb(pp, 4);

    std::cout << "(" << value.min.x << ","
              << value.min.y << ")";

    std::cout << "(" << value.max.x << ","
              << value.max.y << ")";
    return 0;
}
*/