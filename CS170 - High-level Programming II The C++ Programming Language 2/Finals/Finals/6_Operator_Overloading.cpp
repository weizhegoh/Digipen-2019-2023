#include "6_Operator_Overloading.h"

//g++ -Wall -Wextra -Werror -Wconversion -pedantic -std=c++11 -o Test.exe Test.cpp
//std:c++11: cl /W4 /WX /Za /nologo /EHa /Femain.exe main.cpp  

/*
#include <iostream>

struct Node
{
    int data;
    Node* next;
};

class List
{
private:
    Node* head;

public:
    List() :head(nullptr) {}

    List(const List& src) :head(nullptr)
    {
        *this = src;
    }

    List& operator=(const List& src)
    {
        if (this->head != nullptr)
        {
            this->clear();
        }

        Node* current = src.head;

        while (current)
        {
            this->push_back(current->data);
            current = current->next;
        }
        return *this;
    }

    void push_back(int value)
    {
        Node* newNode = new Node;
        Node* current = nullptr;

        newNode->data = value;
        newNode->next = nullptr;

        if (head == nullptr)
        {
            head = newNode;
            return;
        }

        current = head;

        while (current->next)
        {
            current = current->next;
        }
        current->next = newNode;
    }

    void clear()
    {
        Node* deleteNode = nullptr;
        Node* current = nullptr;

        current = head;

        while (current)
        {
            deleteNode = current;
            current = current->next;
            delete deleteNode;
        }

        head = nullptr;
    }

    ~List()
    {
        clear();
    }

    friend std::ostream& operator << (std::ostream& os, const List& list);
};

std::ostream& operator << (std::ostream& os, const List& list)
{
    Node* current = nullptr;

    if (list.head == nullptr)
    {
        os << std::endl;
        return os;
    }

    current = list.head;

    while (current)
    {
        if (current->next)
        {
            os << current->data << ",";
        }
        else
        {
            os << current->data;
        }
        current = current->next;
    }
    os << std::endl;

    return os;
}

int main()
{
    List src;
    src.push_back(1);
    src.push_back(2);
    src.push_back(3);

    List dst1(src);

    src.clear();

    List dst2;
    dst2 = dst1;

    std::cout << dst2;
    return 0;
}
*/