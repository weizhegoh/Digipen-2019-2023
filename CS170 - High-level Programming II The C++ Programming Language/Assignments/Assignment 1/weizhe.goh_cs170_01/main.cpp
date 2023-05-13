/****************************************************************************
filename [forward_list.cpp] 
author [Goh Wei Zhe] 
DP email [weizhe.goh@digipen.edu]
course CS170S20-B
section B 
date [10th January 2020]
Brief Description: This file contains the 8 functions
****************************************************************************/

#include <iostream>
#include <iomanip>

#include "forward_list.h"

int main()
{
  int i;
  
  forward_list* list1 = nullptr;
  forward_list* list2 = nullptr;
  const int numbers[] =
  {
    11, 12, 13, 14, 15,
    21, 22, 23, 24, 25,
    31, 32, 33, 34, 35
  };
  
  int size = sizeof(numbers) / sizeof(*numbers);
  
  list1 = create_list();
  list2 = create_list();
  
  for (i = 0; i < size; i++)
  {
    push_back_list(list1, numbers[i]);
    push_front_list(list2, numbers[i]);
    
    std::cout << "List1 (" << std::right << std::setw(2) << count_list(list1) 
    << " nodes): "; 
    
    print_list(list1);
    std::cout << "List2 (" << std::right << std::setw(2) << count_list(list2) 
    << " nodes): "; 
    
    print_list(list2);
    std::cout << std::endl;
  }
  
  destroy_list(list2);
  list2 = nullptr;
  
  destroy_list(list1);
  list1 = nullptr;
}