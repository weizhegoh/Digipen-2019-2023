#include "Final.h"
//
//#include <iostream>
//
//struct Node {
//    int data;
//    Node* next;
//};
//
//int main(void) {
//    // Do not change the following
//    //  definitions
//    Node n1;
//    n1.data = 1;
//    Node n2;
//    n2.data = 2;
//    Node n3;
//    n3.data = 3;
//    Node* pHead = nullptr;
//
//    // Connect pHead, n1, n2 and n3 
//    //  together to make a linked list
//
//    pHead->next = &n1;
//    n1.next = &n2;
//    n2.next = &n3;
//    
//
//while (pHead) {
//    std::cout << pHead->data << ',';
//    pHead = pHead->next;
//}
//return 0;
//}

#include <iostream>

template <typename T>
struct Node {
    T data;
    Node* next;
};

template <typename T>
class List {
    Node<T>* pHead = nullptr;
public:
    void push_front(T v)
    {
        if (!pHead)
        {
            pHead = new Node<T>();
            pHead->data = v;
            pHead->next = nullptr;
        }
        else
        {
            Node<T>* newPHead = new Node<T>();
            newPHead->data = v;
            newPHead->next = pHead;
            pHead = newPHead;
        }
    }
    Node<T>* getHead() {
        return pHead;
    }

    int getSize()
    {
        Node<T>* current = nullptr;
        int count = 0;

        current = pead;

        while (current)
        {
            count++;
            current = current->next;
        }
        return count;
    }
};

int main(void) {
    List<double> list;
    std::cout << list.getSize();
    list.push_front(3.1);
    std::cout << list.getSize();
    list.push_front(2.1);
    std::cout << list.getSize();
    list.push_front(1.1);
    std::cout << list.getSize();
    return 0;
}