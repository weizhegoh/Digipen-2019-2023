#include "4_Classes_Part2.h"

//g++ -Wall -Wextra -Werror -Wconversion -pedantic -std=c++11 -o Test.exe Test.cpp

/*
#include <iostream>
#include <cstdlib>

namespace List
{
    struct Node
    {
        int data;
        Node* next;
    };

    void push_back(Node** list, int value)
    {
        Node* newNode = new Node;
        Node* current = nullptr;

        if (list == nullptr)
        {
            return;
        }

        newNode->data = value;
        newNode->next = nullptr;

        if (*list == nullptr)
        {
            *list = newNode;
            return;
        }

        current = *list;

        while (current->next)
        {
            current = current->next;
        }

        current->next = newNode;
    }

    void push_front(Node** list, int value)
    {
        Node* newNode = new Node;

        if (list == nullptr)
        {
            return;
        }

        newNode->data = value;
        newNode->next = *list;

        *list = newNode;
    }

    void print(Node* list)
    {
        Node* current = list;

        if (list == nullptr)
        {
            std::cout << std::endl;
            return;
        }

        while (current)
        {
            if (current->next)
            {
                std::cout << current->data << ",";
            }
            else
            {
                std::cout << current->data;
            }
            current = current->next;
        }

        std::cout << std::endl;

    }

    void clear(Node** list)
    {
        Node* deleteNode = nullptr;
        Node* current = nullptr;

        if (list == nullptr)
        {
            return;
        }

        current = *list;

        while (current)
        {
            deleteNode = current;
            current = current->next;
            delete deleteNode;
        }

        *list = nullptr;
    }
}

//int main() {}

 int main() {
    List::Node *pHead = nullptr;
    List::print(pHead);
    List::push_front(&pHead, 1);
    List::print(pHead);  //1
    List::push_front(&pHead, 2);
    List::print(pHead);  //2,1
    List::push_back(&pHead, 3);
    List::print(pHead);  //2,1,3
    List::push_back(&pHead, 4);
    List::print(pHead);  //2,1,3,4
    // Remove all nodes from the list
    List::clear(&pHead);
    List::print(pHead);
    return 0;
}
 */
