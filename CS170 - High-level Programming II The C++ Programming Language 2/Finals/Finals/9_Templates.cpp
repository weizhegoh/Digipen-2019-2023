 #include "9_Templates.h"

//g++ -Wall -Wextra -Werror -Wconversion -pedantic -std=c++11 -o Test.exe Test.cpp
//std:c++11: cl /W4 /WX /Za /nologo /EHa /Femain.exe main.cpp  

/*
#include <iostream>

template <typename T>
struct Node
{
    T data;
    Node* next;
};

template <typename T>
class List
{
private:
    Node<T>* head;

public:
    List() :head(nullptr) {}

    void push_front(T value)
    {
        Node<T>* newNode = new Node<T>;

        newNode->data = value;
        newNode->next = head;

        head = newNode;
    }

    void push_back(T value)
    {
        Node<T>* newNode = new Node<T>;
        Node<T>* current = nullptr;

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

    List<T>& operator=(const List<T>& rhs)
    {
        if (this->head == rhs.head)
        {
            return *this;
        }

        if (this->head)
        {
            clear();
        }

        Node<T>* current = rhs.head;

        while (current)
        {
            this->push_back
            (current->data);
            current = current->next;
        }
        return *this;
    }

    void insert(int index, T value)
    {
        if (index < 0)
            return;

        if (index == 0)
        {
            push_front(value);
            return;
        }

        push_back(value);
        return;
    }

    friend std::ostream& operator << (std::ostream& os, const List<T>& list)
    {
        Node<T>* current = nullptr;

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

    void clear()
    {
        Node<T>* deleteNode = nullptr;
        Node<T>* current = nullptr;

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
};

int main()
{
#if defined(debug) | defined(_debug)
    _crtsetdbgflag(_crtdbg_alloc_mem_df | _crtdbg_leak_check_df);
#endif

    List<int> a, b;
    a.push_front(1);
    a.push_back(2);
    a.push_back(3);
    a = a; // test self-assignment
    b = a; // test an assignment
    std::cout << b;

    List<double> c;
    c.insert(0, 10.1);
    c.insert(1, 20.1);
    std::cout << c;
    return 0;
}
*/