#include "4_Classes_Part1.h"

//g++ -Wall -Wextra -Werror -Wconversion -pedantic -std=c++11 -o Test.exe Test.cpp
//std:c++11: cl /W4 /WX /Za /nologo /EHa /Femain.exe main.cpp  

/*
#include <iostream>
#include <cstdlib>

namespace List
{
  struct Node
  {
    int data;
    Node *next;
  };

  void print(Node *list)
  {
    Node* current = list;

    if(list == nullptr)
    {
      return;
    }

    while(current)
    {
      if(current->next)
      {
        std::cout <<current->data <<",";
      }
      else
      {
        std::cout << current->data;
      }
      current = current->next;
    }
  }
}

using namespace List;

int main()
{
  Node *head = nullptr;
  head = new Node;
  Node *node1 = new Node;
  Node *node2 = new Node;

  head->data = 1;
  node1->data = 2;
  node2->data = 3;

  head->next = node1;
  node1->next = node2;
  node2->next = NULL;

  print(head);

  delete node2;
  delete node1;
  delete head;

  return 0;
}
*/