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
#include <cstdlib>
#include <iomanip>

#include "forward_list.h"

/*****************************************************************************
 * DATA TYPES
*****************************************************************************/
struct node
{
  int number;        /* data portion */
  node* next; /* pointer portion */
};

struct forward_list
{
  node* head;
};

/*****************************************************************************
 * HELPER FUNCTIONS
*****************************************************************************/
node* create_node(int number, node* next)
{
  node* result = (node*)malloc(sizeof(node));
  result->number = number;
  result->next = next;
  return result;
}

void destroy_node(node* const that)
{
  free(that);
}

/*****************************************************************************
 * FUNCTIONS OF A FORWARD LIST
*****************************************************************************/

forward_list* create_list()
{
  return (forward_list*)calloc(1, sizeof(forward_list));
}

void destroy_list(forward_list* const that)
{
  node*current = that->head;
  
  while(current)
  {
    node*next = current->next;
    free(current);
    current = next;
  }
  free(that);
}

unsigned int count_list(const forward_list* const that)
{
  unsigned int count = 0;
  node*current = that->head;
  while (current)
  {
    count++;
    current = current->next;
  }
  return count;
}

void push_front_list(forward_list* const that, int value)
{
  if(that)
  {
    that->head = create_node(value, that->head);
  }
}

void push_back_list(forward_list* const that, int value)
{
  node*current = that->head;
  if(current)
  {
    while(current->next)
    {
      current = current->next;
    }
    
    current->next = create_node(value, nullptr);
  }
  else
  {
    that->head = create_node(value, nullptr);
  }
}

void print_list(const forward_list* const that)
{
  node*current = that->head;
  while (current)
  {
    std::cout << std::left << std::setw(6) << current->number; 
    current = current->next;
  }
  std::cout << std::endl;
}