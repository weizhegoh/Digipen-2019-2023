#include "Test5.h"
#include <iostream>

/*
namespace linkedList
{
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

        int count_Node()
        {
            Node* current = nullptr;
            int count = 0;

            current = head;

            while (current)
            {
                count++;
                current = current->next;
            }
            return count;
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

        void insertAfter(int index, int value)
        {
            if (index < 0)
            {
                return;
            }

            if (index >= count_Node())
            {
                return;
            }

            Node* newNode = new Node;
            Node* current = nullptr;
            Node* temp = nullptr;

            int count = 0;

            newNode->data = value;
            newNode->next = nullptr;

            current = head;

            while (current)
            {
                if (index == count)
                {
                    temp = current->next;
                    current->next = newNode;
                    newNode->next = temp;
                    break;
                }
                current = current->next;
                count++;
            }
        }

        void remove(int index)
        {
            int count = 0;

            if (index < 0)
            {
                return;
            }

            if (index >= count_Node())
            {
                return;
            }

            Node* deleteNode = nullptr;
            Node* current = nullptr;

            current = head;

            while (current->next)
            {
                if (count == (index - 1))
                {
                    deleteNode = current->next;

                    current->next = current->next->next;

                    delete deleteNode;
                    break;
                }
                current = current->next;
                count++;
            }
        }

        void print()
        {
            Node* current = nullptr;

            if (head == nullptr)
            {
                std::cout << std::endl;
                return;
            }

            current = head;

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

        /////////////////////////////////////////////
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
            Node* current = nullptr;

            if (list == nullptr)
            {
                std::cout << std::endl;
                return;
            }

            current = list;

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

        ~List()
        {
            clear();
        }
    };
}

//using namespace linkedList;

int main()
{
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    linkedList::List list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    // Do not insert when index is
    //   out of range
    list.insertAfter(-100, 20);
    list.insertAfter(100, 20);

    // Insert as a new 3rd element
    list.insertAfter(1, 20);

    // Remove 2nd element
    list.remove(1);

    list.print();

    /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////

    linkedList::List list2;
    linkedList::Node* pHead = nullptr;

    list.print(pHead);

    list.push_front(&pHead, 1);
    list.print(pHead);  //1
    list.push_front(&pHead, 2);
    list.print(pHead);  //2,1
    list.push_back(&pHead, 3);
    list.print(pHead);  //2,1,3
    list.push_back(&pHead, 4);
    list.print(pHead);  //2,1,3,4
    // Remove all nodes from the list
    list.clear(&pHead);
    list.print(pHead);

    ////////////////////////////////////////////////////////

    linkedList::Node* head = nullptr;
    head = new linkedList::Node;
    linkedList::Node* node1 = new linkedList::Node;
    linkedList::Node* node2 = new linkedList::Node;

    head->data = 1;
    node1->data = 2;
    node2->data = 3;

    head->next = node1;
    node1->next = node2;
    node2->next = NULL;

    list.print(head);

    delete node2;
    delete node1;
    delete head;

    return 0;
}
*/