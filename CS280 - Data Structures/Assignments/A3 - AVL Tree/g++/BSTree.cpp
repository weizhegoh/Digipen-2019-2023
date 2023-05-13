/******************************************************************************/
/*!
\file:		BSTree.cpp
\author:    Goh Wei Zhe, weizhe.goh, 440000119
\par email: weizhe.goh\@digipen.edu
\date:		March 12, 2021
\brief		This file contains the definitions needed to implement the simple 
            API for Binary Search Tree with recursive algorithms.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "BSTree.h"

/******************************************************************************/
/*!
\fn     template<typename T> 
        BSTree<T>::BSTree(ObjectAllocator *OA, bool ShareOA)

\brief  Constructor of BSTree

\param  OA - Object allocator for the BSTree nodes 

\param  ShareOA - boolean for sharing object allocator 
*/
/******************************************************************************/
template<typename T> 
BSTree<T>::BSTree(ObjectAllocator *OA, bool ShareOA): oa{OA}, share{ShareOA},
root_{0}
{   
    if(OA)
        Custom_OA = false;
    else 
    {
        OAConfig config(true);
        oa = new ObjectAllocator(sizeof(BinTreeNode), config);
        Custom_OA = true;
    }
}

/******************************************************************************/
/*!
\fn     template<typename T> 
        BSTree<T>::BSTree(const BSTree& rhs)

\brief  Copy constructor of BSTree

\param  rhs - The BSTree to be copied from
*/
/******************************************************************************/
template<typename T> 
BSTree<T>::BSTree(const BSTree& rhs)
{
    if(rhs.share)
    {
        oa = rhs.oa;
        Custom_OA = false;
        share = true;
    }
    else
    {
        OAConfig config(true);
        oa = new ObjectAllocator(sizeof(BinTreeNode), config);
        Custom_OA = true;
        share = false;
    }
    
    copy_tree(root_, rhs.root_);
    
    root_->count = rhs.root()->count;
}

/******************************************************************************/
/*!
\fn     template<typename T>
        BSTree<T>::~BSTree()

\brief  Destructor of BSTree
*/
/******************************************************************************/
template<typename T>
BSTree<T>::~BSTree()
{
    clear();
    
    //false
    if(!share)
    delete oa;
}

/******************************************************************************/
/*!
\fn     template<typename T> 
        BSTree<T>& BSTree<T>::operator=(const BSTree& rhs)  

\brief  Assignment operator of BSTree

\param  rhs - The BSTree to be copied from 

\return Returns the assigned tree itself
*/
/******************************************************************************/
template<typename T> 
BSTree<T>& BSTree<T>::operator=(const BSTree& rhs)
{
    
    if(this == &rhs)
        return *this;
        
     if(rhs.share)
    {
        oa = rhs.oa;
        Custom_OA = false;
        share = true;
    }
    else
    {
        OAConfig config(true);
        delete oa;
        oa = new ObjectAllocator(sizeof(BinTreeNode), config);
        Custom_OA = true;
        share = false;
    }
    
    if(rhs.root())
    {
        clear();
        copy_tree(root_, rhs.root_);
    }
        
    return *this;
}

/******************************************************************************/
/*!
\fn     template <typename T>
        typename BSTree<T>::BinTree BSTree<T>::copy_tree(BinTree& destination,
        const BinTree& source)

\brief  Helper function to construct copy tree by recursion

\param  destination - copied BSTree

\param  source - BSTree to be copied from
*/
/******************************************************************************/
template <typename T>
typename BSTree<T>::BinTree BSTree<T>::copy_tree(BinTree& destination,
const BinTree& source)
{

    if(!source)
        return nullptr;
 
    destination = make_node(source->data);
    
    destination->balance_factor = source->balance_factor;
    
    destination->count = source->count; 
    
    destination->left = copy_tree(destination->left, source->left);
    destination->right = copy_tree(destination->right, source->right);
    
    return destination;
}

/******************************************************************************/
/*!
\fn     template<typename T>
        const typename BSTree<T>::BinTreeNode* BSTree<T>::operator[](int index) 
        const   

\brief  subscript operator to search in the tree

\param  index - the index to match

\return Returns the matching node 
*/
/******************************************************************************/
template<typename T>
const typename BSTree<T>::BinTreeNode* BSTree<T>::operator[](int index) const 
{
   return sub_node(root_, index);
}

/******************************************************************************/
/*!
\fn     template <typename T>
        const typename BSTree<T>::BinTreeNode*BSTree<T>::sub_node(BinTree tree, 
        int compares) const

\brief  Helper function to find the correct node by recursion

\param  tree - The tree to find the match 

\param  compares - the value to match

\return Returns the matching node 
*/
/******************************************************************************/
template <typename T>
const typename BSTree<T>::BinTreeNode*BSTree<T>::sub_node(BinTree tree, 
int compares) const
{
    if(!tree)
        return NULL;
        
    unsigned temp = (tree->left) ? tree->left->count : 0;
    
    if(temp > static_cast<unsigned>(compares))
        return sub_node(tree->left, compares);
    else if (temp < static_cast<unsigned>(compares))
        return sub_node(tree->right, compares - temp - 1);
    else 
        return tree;
}

/******************************************************************************/
/*!
\fn     template <typename T> 
        void BSTree<T>::insert(const T& value)

\brief  Insert a node into the tree

\param  value - the value of node to be inserted
*/
/******************************************************************************/
template <typename T> 
void BSTree<T>::insert(const T& value)
{
    insert_node(root_, value);
}

/******************************************************************************/
/*!
\fn     template<typename T>
        void BSTree<T>::insert_node(BinTree& tree, const T& value)

\brief  Helper function to insert a node into tree by recusion

\param  tree - The tree for node to be inserted

\param  value - the value of node to be inserted
*/
/******************************************************************************/
template<typename T>
void BSTree<T>::insert_node(BinTree& tree, const T& value)
{
    try
    {
        if(!tree)
        {
            tree = make_node(value);
        }
        else if (value < tree->data) 
        {
            ++tree->count;
            insert_node(tree->left, value);
        }
        else if (value > tree->data)
        {
            ++tree->count;
            insert_node(tree->right, value);
        }
        else 
        {
            std::cout << "Error, duplicated item" << std::endl;
        }
    }
    catch(const OAException& e)
    {
        throw(BSTException(BSTException::E_NO_MEMORY, e.what()));
    }
}

/******************************************************************************/
/*!
\fn     template <typename T>
        void BSTree<T>::remove(const T& value)

\brief  Remove node based on input value

\param  value - The value of node to be removed
*/
/******************************************************************************/
template <typename T>
void BSTree<T>::remove(const T& value)
{
    delete_node(root_, value);
}

/******************************************************************************/
/*!
\fn     template<typename T> 
        void BSTree<T>::clear()

\brief  Clear the tree's nodes
*/
/******************************************************************************/
template<typename T> 
void BSTree<T>::clear()
{
    free_tree(root_);
}

/******************************************************************************/
/*!
\fn     template<typename T>
        void BSTree<T>::free_tree(BinTree& tree)

\brief  Helper function to clear tree nodes by recursion

\param  tree - The tree to be freed
*/
/******************************************************************************/
template<typename T>
void BSTree<T>::free_tree(BinTree& tree)
{
    if(!tree)
        return;
    
    free_tree(tree->left);
    free_tree(tree->right);
    delete_node(tree, tree->data);
}
/******************************************************************************/
/*!
\fn     template<typename T>
        void BSTree<T>::delete_node(BinTree& tree, const T& value)

\brief  Helper function to delete node

\param  tree - the tree to remove node from 

\param  value - The value of node to be removed
*/
/******************************************************************************/
template<typename T>
void BSTree<T>::delete_node(BinTree& tree, const T& value)
{
    if(tree == NULL)
        return;
    else if (value < tree->data)
    {
        --tree->count;
        delete_node(tree->left, value);
    }
    else if (value > tree->data)
    {
        --tree->count;
        delete_node(tree->right, value);
    }
    else 
    {
        --tree->count;
        
        if(tree->left == 0)
        {
            BinTree temp = tree;
            tree = tree->right;
            free_node(temp);
        }
        else if (tree->right == 0)
        {
            BinTree temp = tree;
            tree = tree->left;
            free_node(temp);
        }
        else 
        {
            BinTree pred = 0;
            find_predecessor(tree, pred);
            tree->data = pred->data;
            delete_node(tree->left, tree->data);
        }
    }
}

/******************************************************************************/
/*!
\fn     template<typename T>
        bool BSTree<T>::find(const T& value, unsigned& compares) const 

\brief  Find the node in the tree with the matching value

\param  value - the value of node to be found

\param  compares - the number of function calls used to find the matching node 

\return Return true if node with matching value exist, else return false 
*/
/******************************************************************************/
template<typename T>
bool BSTree<T>::find(const T& value, unsigned& compares) const 
{
    return find_node(root_, value, compares);
}

/******************************************************************************/
/*!
\fn     template<typename T>
        bool BSTree<T>::find_node(BinTree tree, const T& value,
        unsigned& compares) const

\brief  Helper function to find a node in the tree with a matching value by 
        recursion

\param  tree - the tree to be searched

\param  value - the value of node to be found

\param  compares - the number of function calls used to find the matching node 

\return Return true if node with matching value exist, else return false 
*/
/******************************************************************************/
template<typename T>
bool BSTree<T>::find_node(BinTree tree, const T& value,unsigned& compares) const 
{
    ++compares;
    
    if(tree == 0)
        return false;
    else if(value == tree->data) 
        return true;
    else if(value < tree->data)
        return find_node(tree->left, value, compares);
    else 
        return find_node(tree->right, value, compares);
}

/******************************************************************************/
/*!
\fn     template<typename T>
        bool BSTree<T>::empty() const

\brief  Check if the tree is empty 

\return Return true if tree is empty, else return false
*/
/******************************************************************************/
template<typename T>
bool BSTree<T>::empty() const
{
    return (root_ == 0);
}

/******************************************************************************/
/*!
\fn     template<typename T>
        unsigned int BSTree<T>::size() const

\brief  Counts the number of nodes in the tree

\return Returns the number of nodes in the tree 
*/
/******************************************************************************/
template<typename T>
unsigned int BSTree<T>::size() const 
{
    return (root_) ? root_->count : 0;
}

/******************************************************************************/
/*!
\fn     template<typename T>
        int BSTree<T>::height() const 

\brief  Counts the height of a tree

\return Returns the height of a tree 
*/
/******************************************************************************/
template<typename T>
int BSTree<T>::height() const 
{
    return tree_height(root_);
}

/******************************************************************************/
/*!
\fn     template<typename T>
        typename BSTree<T>::BinTree BSTree<T>::root() const 

\brief  Get the root of the tree

\return Returns the root of the tree 
*/
/******************************************************************************/
template<typename T>
typename BSTree<T>::BinTree BSTree<T>::root() const 
{
    return root_;
}

/******************************************************************************/
/*!
\fn     template <typename T>
        typename BSTree<T>::BinTree& BSTree<T>::get_root()

\brief  Get the root of the tree

\return Returns the root of the tree 
*/
/******************************************************************************/
template <typename T>
typename BSTree<T>::BinTree& BSTree<T>::get_root()
{
    return root_;
}

/******************************************************************************/
/*!
\fn     template <typename T>
        typename BSTree<T>::BinTree BSTree<T>::make_node(const T& value) const 

\brief  Make a new node for the tree 

\param  value - the value of the new node

\return Returns the new node
*/
/******************************************************************************/
template <typename T>
typename BSTree<T>::BinTree BSTree<T>::make_node(const T& value) const 
{
    try
    {
        BinTree memory = reinterpret_cast<BinTreeNode*>(oa->Allocate());
        BinTree new_node = new (memory) BinTreeNode(value);
        return new_node;
    }
    catch(const OAException &e)
    {
        throw(BSTException(BSTException::E_NO_MEMORY, e.what()));
    }
}

/******************************************************************************/
/*!
\fn     template <typename T>
        void BSTree<T>::free_node(BinTree node)

\brief  Free a node from the tree

\param  node - The node to be freed
*/
/******************************************************************************/
template <typename T>
void BSTree<T>::free_node(BinTree node)
{
    node->~BinTreeNode();
    oa->Free(node);
}

/******************************************************************************/
/*!
\fn     template <typename T>
        int BSTree<T>::tree_height(BinTree tree) const

\brief  Helper function to find the height of a tree by recursion

\param  tree - The tree to be counted 

\return Returns the height of the tree 
*/
/******************************************************************************/
template <typename T>
int BSTree<T>::tree_height(BinTree tree) const
{
    if(tree == 0)
        return -1;
        
    int L = tree_height(tree->left);
    int R = tree_height(tree->right);
    
    if(L > R)
        return L + 1;
    else 
        return R + 1;
}

/******************************************************************************/
/*!
\fn     template <typename T>
        void BSTree<T>::find_predecessor(BinTree tree, BinTree& predecessor)
        const 

\brief  Finds the parent of a node

\param  tree - the node to be searched 

\param  predecessor - the node to fill as predecessor
*/
/******************************************************************************/
template <typename T>
void BSTree<T>::find_predecessor(BinTree tree, BinTree& predecessor) const 
{
    predecessor = tree->left;
    
    while(predecessor->right != 0)
        predecessor = predecessor->right;
}