#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <algorithm>

#include "data.h"

#define UNUSED(x) (void)x;

namespace AI 
{

    // A simple graph node definition with serialization functions
    template<typename T>
    struct Node
    {
        // Member data

        T value;
        Node* parent;
        std::list<Node*> children;

        Node(T value = {}, Node* parent = nullptr,
            const std::list<Node*>& children = {})
            : value{ value }, parent{ parent }, children{ children }
        {

        }

        ~Node()
        {
            for (auto child : children)
                delete child;
        }

        // Serialization
        friend std::ostream& operator<<(std::ostream& os, Node const& rhs)
        {
            os << rhs.value; // Just for test, you can change it
            // ...

            return os;
        }

        // Deserialization
        friend std::istream& operator>>(std::istream& is, Node& rhs)
        {
            is >> rhs.value; // Just for test, you can change it
            // ...

            return is;
        }

        // Returns values from root to this node as an array
        std::vector<T> getPath() const
        {
            std::vector<T> r;
            // ...

            return r;
        }
    };


    // The actual node type for this assignment
    using TreeNode = Node<std::string>;


    // Abstract base class for domain specific functors that return adjacent nodes
    class GetAdjacents
    {

    public:

        virtual ~GetAdjacents()
        {
        }

        //       virtual std::vector<Node*> operator()(Key key) = 0;
        virtual std::vector<TreeNode*> operator()(TreeNode* pNode) = 0;
    };


    // Domain specific functor that returns adjacent nodes
    class GetTreeAdjacents : public GetAdjacents
    {
 
    public:

        GetTreeAdjacents()
            : GetAdjacents()
        {
        }

        std::vector<TreeNode*> operator()(TreeNode* pNode)
        {
           UNUSED(pNode)

            std::vector<AI::TreeNode*> list = {};

            // Push to the list all children of pNode excluding 
            // those with value not equal "x"

            return list;
        }
    };

    // Domain specific functor that returns shuffled adjacent nodes
    class GetTreeStochasticAdjacents : public GetTreeAdjacents
    {
    public:

        GetTreeStochasticAdjacents()
            : GetTreeAdjacents{}
        {
        }

        std::vector<TreeNode*> operator()(TreeNode* pNode)
        {
            UNUSED(pNode)

            std::vector<TreeNode*> adjacents;

            // Use the base class operator() and then shuffle the result

            return adjacents;
        }
    };

    // Wrappers that provide same interface for queue and stack

    struct Interface
    {
        virtual void clear() = 0;

        virtual void push(TreeNode* pNode) = 0;

        virtual TreeNode* pop() = 0;
    };

    struct Queue : Interface //...
    {
        void clear()
        {
            //...
        }

        void push(TreeNode* pNode)
        {
           UNUSED(pNode)
           //...
        }

        TreeNode* pop()
        {
            TreeNode* pNode = nullptr;
            //...
            return pNode;
        }
    };

    struct Stack : Interface //...
    {
        void clear()
        {
            //...
        }

        void push(TreeNode* pNode)
        {
            UNUSED(pNode)
            //...
        }

        TreeNode* pop()
        {
            TreeNode* pNode = nullptr;
            //...
            return pNode;
        }
    };

    // Recursive Flood Fill
    class Flood_Fill_Recursive
    {
        GetTreeAdjacents* pGetAdjacents;

    public:
        Flood_Fill_Recursive(GetTreeAdjacents* pGetAdjacents)
            : pGetAdjacents{ pGetAdjacents }
        {
        }

        void run(TreeNode* pNode, std::string value)
        {
            UNUSED(pNode)
            UNUSED(value)

            // Implement the flood fill
        }
    };

    // Iterative Flood Fill
    // Type T defines is it depth- or breadth-first
    template<typename T>
    class Flood_Fill_Iterative
    {
        GetTreeAdjacents* pGetAdjacents;
        T openlist;

    public:
        Flood_Fill_Iterative(GetTreeAdjacents* pGetAdjacents)
            : pGetAdjacents{ pGetAdjacents }, openlist{}
        {
        }

        void run(TreeNode* pNode, std::string value)
        {
            UNUSED(pNode)
            UNUSED(value)

            // Implement the flood fill
         }
    };

} // end namespace

#endif