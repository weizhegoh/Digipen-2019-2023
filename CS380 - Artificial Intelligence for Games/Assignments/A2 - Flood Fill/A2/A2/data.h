#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <vector>

namespace AI 
{

    // Key defines position of a cell in a grid:    
    struct Key 
    {
        int j; // defines 0-based index of a row
        int i; // defines 0-based index of a column
         
        Key(int j = 0, int i = 0) 
            : j{ j }, i{ i }
        {
        }

        bool operator==(const Key& rhs) const
        {
            return j==rhs.j && i==rhs.i;
        }

        friend std::ostream& operator<<(std::ostream& os, const Key& rhs)
        {
            os << rhs.j << "," << rhs.i;
            return os;
        }
    };

    // Node is used to keep track of visited nodes or nodes to be visited 
    // during search
    struct Node
    {
        Key key;     // position of the node in a grid
        int* pValue; // address of the node's value in memory

        Node(Key key = {}, int* pValue = nullptr)
            : key(key), pValue{ pValue }
        {
        }

        friend std::ostream& operator<<(std::ostream& os, const Node& rhs)
        {
            os << rhs.key;
            return os;
        }
    };

    // Abstract base class for domain specific functors that return adjacent nodes
    class GetAdjacents
    {

    public:

        virtual ~GetAdjacents()
        {
        }

        virtual std::vector<Node*> operator()(Key key) = 0;
    };

}

#endif