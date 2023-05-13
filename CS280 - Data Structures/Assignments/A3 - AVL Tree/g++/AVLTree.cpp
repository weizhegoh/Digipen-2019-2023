/******************************************************************************/
/*!
\file:		AVLTree.cpp
\author:    Goh Wei Zhe, weizhe.goh, 440000119
\par email: weizhe.goh\@digipen.edu
\date:		March 12, 2021
\brief		This file contains the definitions needed to implement the simple 
            API for AVL Trees with recursive algorithms.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "AVLTree.h"

/******************************************************************************/
/*!
\fn     template<typename T>
        AVLTree<T>::AVLTree(ObjectAllocator *OA, bool ShareOA):
        BSTree<T>(OA, ShareOA){}

\brief  Constructor of AVLTree

\param  OA - Object allocator for the BSTree nodes 

\param  ShareOA - boolean for sharing object allocator 
*/
/******************************************************************************/
template<typename T>
AVLTree<T>::AVLTree(ObjectAllocator *OA, bool ShareOA):BSTree<T>(OA, ShareOA){}

/******************************************************************************/
/*!
\fn     template<typename T>
        void AVLTree<T>::insert(const T& value)

\brief  Insert a node into the tree 

\param  value - The value of the node to be inserted
*/
/******************************************************************************/
template<typename T>
void AVLTree<T>::insert(const T& value)
{
    insert_start(BSTree<T>::get_root(), value);
    node_count(BSTree<T>::get_root());
}

/******************************************************************************/
/*!
\fn     template <typename T>
        void AVLTree<T>::insert_start(typename BSTree<T>::BinTree& tree,
        const T& value)

\brief  Function to start the recursion to insert a node into the tree 

\param  tree - The tree that the node to be inserted 

\param  value - The value of the node to be inserted
*/
/******************************************************************************/
template <typename T>
void AVLTree<T>::insert_start(typename BSTree<T>::BinTree& tree, const T& value)
{
    std::stack<typename BSTree<T>::BinTree*> stack_;
    
    insert_node(tree, value, stack_);
}

/******************************************************************************/
/*!
\fn     template <typename T>
        void AVLTree<T>::insert_node(typename BSTree<T>::BinTree & node,
        const T& value, std::stack<typename BSTree<T>::BinTree*> & nodes) 

\brief  Helper function to insert a node into the tree by recursion

\param  node - The tree or subtree for node to be inserted 

\param  value - The value of the node to be inserted

\param  nodes - The stack to push or pop the nodes 
*/
/******************************************************************************/
template <typename T>
void AVLTree<T>::insert_node(typename BSTree<T>::BinTree & node, const T& value,
std::stack<typename BSTree<T>::BinTree*> & nodes) 
{
    if(node == 0)
    {
        node = BSTree<T>::make_node(value);
        BalanceAVLTree(nodes);
    }
    else if (value < node->data)
    {
        nodes.push(&node);
        insert_node(node->left, value, nodes);
    }
    else if (value > node->data)
    {
        nodes.push(&node);
        insert_node(node->right, value, nodes);
    }
    else
        std::cout << "Error, duplicate item" << std::endl;
}

/******************************************************************************/
/*!
\fn     template <typename T>
        unsigned int AVLTree<T>::node_count(typename BSTree<T>::BinTree& tree) 
        const 

\brief  Count the number of nodes in a tree or subtree 

\param  tree - The tree to count the number of total nodes in that tree. 

\return Return the total node count of a tree 
*/
/******************************************************************************/
template <typename T>
unsigned int AVLTree<T>::node_count(typename BSTree<T>::BinTree& tree) const 
{
    if(tree == NULL)
        return 0;
        
    tree->count = 1 + node_count(tree->left) + node_count(tree->right);
    
    return tree->count;
}
/******************************************************************************/
/*!
\fn     template <typename T>
        void AVLTree<T>::RotateLeft(typename BSTree<T>::BinTree& tree)

\brief  Rotate the tree around the node to the left 

\param  tree - The node to rotate about
*/
/******************************************************************************/
template <typename T>
void AVLTree<T>::RotateLeft(typename BSTree<T>::BinTree& tree)
{
    typename BSTree<T>::BinTree temp = tree;
    tree = tree->right;
    temp->right = tree->left;
    tree->left = temp;
    
    tree->count = temp->count;
    
    //recount node count 
    unsigned leftCount = (temp->left) ? temp->left->count : 0;
    unsigned rightCount = (temp->right) ? temp->right->count : 0;
    
    temp->count = leftCount + rightCount + 1;
}

/******************************************************************************/
/*!
\fn     template <typename T>
        void AVLTree<T>::RotateRight(typename BSTree<T>::BinTree& tree)

\brief  Rotate the tree around the node to the right 

\param  tree - The node to rotate about
*/
/******************************************************************************/
template <typename T>
void AVLTree<T>::RotateRight(typename BSTree<T>::BinTree& tree)
{
   typename BSTree<T>::BinTree temp = tree;
   tree = tree->left;
   temp->left = tree->right;
   tree->right = temp;
   
   tree->count = temp->count;
   
   //recount node count 
   unsigned leftCount = (temp->left) ? temp->left->count : 0;
   unsigned rightCount = (temp->right) ? temp->right->count : 0;
   
   temp->count = leftCount + rightCount + 1;
}

/******************************************************************************/
/*!
\fn     template <typename T>
        void AVLTree<T>::BalanceAVLTree
        (std::stack<typename BSTree<T>::BinTree*>& nodes)

\brief  Balance the AVL tree

\param  nodes - The stack of nodes used to balance the tree
*/
/******************************************************************************/
template <typename T>
void AVLTree<T>::BalanceAVLTree(std::stack<typename BSTree<T>::BinTree*>& nodes)
{
    while(!nodes.empty())
    {
        typename BSTree<T>::BinTree* topnode = nodes.top();
        nodes.pop();
        
        typename BSTree<T>::BinTree& y = *topnode;
        
        int RH = BSTree<T>::tree_height(y->right);
        int LH = BSTree<T>::tree_height(y->left);
        
        if(abs(LH - RH) > 1)
        {
            if(RH > (LH + 1))
            {
                //promote twice
                if(BSTree<T>::tree_height(y->right->left) > 
                BSTree<T>::tree_height(y->right->right))
                {
                    RotateRight(y->right);
                    RotateLeft(y);
                }
                else 
                {
                    //promote once
                    RotateLeft(y);
                }
            }
            else if ((RH + 1) < LH)
            {
                //promote once 
                if(BSTree<T>::tree_height(y->left->left) > 
                BSTree<T>::tree_height(y->left->right))
                {
                    RotateRight(y);
                }
                else 
                {
                   //promote twice
                   RotateLeft(y->left);
                   RotateRight(y);
                }
            }
        }
    }
}
/******************************************************************************/
/*!
\fn     template <typename T>
        void AVLTree<T>::remove(const T& value)

\brief  Remove a node in the tree

\param  value - The value of the node to be removed
*/
/******************************************************************************/
template <typename T>
void AVLTree<T>::remove(const T& value)
{
    remove_start(BSTree<T>::get_root(), value);
    node_count(BSTree<T>::get_root());
}

/******************************************************************************/
/*!
\fn     template <typename T>
        void AVLTree<T>::remove_start(typename BSTree<T>::BinTree& tree,
        const T& value)

\brief  Function to begin the recusion to remove node from the tree 

\param  tree - The tree to remove the node from

\param  value - The value of the node to be removed
*/
/******************************************************************************/
template <typename T>
void AVLTree<T>::remove_start(typename BSTree<T>::BinTree& tree, const T& value)
{
    std::stack <typename BSTree<T>::BinTree*> stack_;
    remove_node(tree, value, stack_);
}

/******************************************************************************/
/*!
\fn     template<typename T>
        void AVLTree<T>::remove_node(typename BSTree<T>::BinTree &tree,
        const T& value, std::stack<typename BSTree<T>::BinTree*>& nodes)

\brief  Helper function to remove the node to delete by recursion

\param  tree - The tree to remove the node from

\param  value - The value of the node to be removed

\param  nodes - The stack to pop out the nodes from
*/
/******************************************************************************/
template<typename T>
void AVLTree<T>::remove_node(typename BSTree<T>::BinTree &tree, const T& value,
	std::stack<typename BSTree<T>::BinTree*>& nodes)
{
    if(tree == 0)
        return ;
    else if (value < tree->data)
    {
        nodes.push(&tree);
        remove_node(tree->left, value, nodes);
    }
    else if (value > tree->data)
    {
        nodes.push(&tree);
        remove_node(tree->right, value, nodes);
    }
    else 
    {
        if(tree->left == 0)
        {
            typename BSTree<T>::BinTree temp = tree;
            tree = tree->right;
            BSTree<T>::free_node(temp);
            BalanceAVLTree(nodes);
        }
        else if (tree->right == 0)
        {
            typename BSTree<T>::BinTree temp = tree;
            tree = tree->left;
            BSTree<T>::free_node(temp);
            BalanceAVLTree(nodes);
        }
        else 
        {
            //two child
            typename BSTree<T>::BinTree pred = 0;
            BSTree<T>::find_predecessor(tree, pred);
            tree->data = pred->data;
            nodes.push(&tree);
            remove_node(tree->left, tree->data, nodes);
        }
    }
}

/******************************************************************************/
/*!
\fn     template<typename T>
        bool AVLTree<T>::ImplementedBalanceFactor(void)

\brief  Function for efficient balancing

\return Returns true if implemented efficient balancing, else return false
*/
/******************************************************************************/
template<typename T>
bool AVLTree<T>::ImplementedBalanceFactor(void)
{
    return false;
}