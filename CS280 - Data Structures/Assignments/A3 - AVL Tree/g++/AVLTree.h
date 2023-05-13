/******************************************************************************/
/*!
\file:		AVLTree.h
\author:    Goh Wei Zhe, weizhe.goh, 440000119
\par email: weizhe.goh\@digipen.edu
\date:		March 12, 2021
\brief		This file contains the declarations needed to implement the simple 
            API for AVL Trees with recursive algorithms.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

//---------------------------------------------------------------------------
#ifndef AVLTREE_H
#define AVLTREE_H
//---------------------------------------------------------------------------
#include <stack>
#include "BSTree.h"

/*!
  Definition for the AVL Tree
*/
template <typename T>
class AVLTree : public BSTree<T>
{
  public:
    AVLTree(ObjectAllocator *OA = 0, bool ShareOA = false);
    virtual ~AVLTree() = default; // DO NOT IMPLEMENT
    virtual void insert(const T& value) override;
    virtual void remove(const T& value) override;

      // Returns true if efficiency implemented
    static bool ImplementedBalanceFactor(void);

  private:
    // private stuff
    
    unsigned int node_count(typename BSTree<T>::BinTree& tree) const;
    
    void insert_start(typename BSTree<T>::BinTree& tree, const T& value);
    void insert_node(typename BSTree<T>::BinTree& node, const T& value,
                     std::stack<typename BSTree<T>::BinTree*>& nodes);

    void remove_start(typename BSTree<T>::BinTree& tree, const T& value);
    void remove_node(typename BSTree<T>::BinTree &tree, const T& value, 
    	             std::stack<typename BSTree<T>::BinTree*>& nodes);

    
    void RotateLeft(typename BSTree<T>::BinTree& tree);
    void RotateRight(typename BSTree<T>::BinTree& tree);

    void BalanceAVLTree(std::stack<typename BSTree<T>::BinTree*>& nodes);
};

#include "AVLTree.cpp"

#endif
//---------------------------------------------------------------------------
