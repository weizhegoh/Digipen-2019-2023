/******************************************************************************/
/*!
\file:		BSTree.h
\author:    Goh Wei Zhe, weizhe.goh, 440000119
\par email: weizhe.goh\@digipen.edu
\date:		March 12, 2021
\brief		This file contains the declarations needed to implement the simple 
            API for Binary Search Tree with recursive algorithms.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

//---------------------------------------------------------------------------
#ifndef BSTREE_H
#define BSTREE_H
//---------------------------------------------------------------------------
#include <string>    // std::string
#include <stdexcept> // std::exception

#include "ObjectAllocator.h"

/*!
  The exception class for the AVL/BST classes
*/
class BSTException : public std::exception
{
  public:
    /*!
      Non-default constructor

      \param ErrCode
        The kind of exception (only one currently)

      \param Message
        The human-readable reason for the exception.
    */
    BSTException(int ErrCode, const std::string& Message) :
      error_code_(ErrCode), message_(Message) {
    };

    /*!
      Retrieve the exception code.

      \return
        E_NO_MEMORY
    */
    virtual int code() const {
      return error_code_;
    }

    /*!
      Retrieve the message string

      \return
        The human-readable message.
    */
    virtual const char *what() const throw() {
      return message_.c_str();
    }

    //! Destructor
    virtual ~BSTException() {}

    //! The kinds of exceptions (only one currently)
    enum BST_EXCEPTION{E_NO_MEMORY};

  private:
    int error_code_;      //!< The code of the exception
    std::string message_; //!< Readable message text
};

/*!
  The definition of the BST
*/
template <typename T>
class BSTree
{
  public:
    //! The node structure
    struct BinTreeNode
    {
      BinTreeNode *left;  //!< The left child
      BinTreeNode *right; //!< The right child
      T data;             //!< The data
      int balance_factor; //!< optional for efficient balancing
      unsigned count;     //!< nodes in this subtree for efficient indexing
      //! Default constructor
      BinTreeNode() : left(0), right(0), data(0), balance_factor(0), count(1){};

      //! Conversion constructor
      BinTreeNode(const T& value) : 
      left(0), right(0), data(value), balance_factor(0), count(1) {};
    };

    //! shorthand
    typedef BinTreeNode* BinTree;

    BSTree(ObjectAllocator *OA = 0, bool ShareOA = false);
    BSTree(const BSTree& rhs);
    virtual ~BSTree();
    BSTree& operator=(const BSTree& rhs);
    const BinTreeNode* operator[](int index) const;//for r-values (Extra Credit)
    virtual void insert(const T& value);
    virtual void remove(const T& value);
    void clear();
    bool find(const T& value, unsigned &compares) const;
    bool empty() const;
    unsigned int size() const;
    int height() const;
    BinTree root() const;

  protected:
    BinTree& get_root();
    BinTree make_node(const T& value) const;
    void free_node(BinTree node);
    int tree_height(BinTree tree) const;
    void find_predecessor(BinTree tree, BinTree &predecessor) const;

  private:
    // private stuff...
    
    ObjectAllocator* oa;
    
    bool Custom_OA;
    bool share;
    BinTree root_;
    
    BinTree copy_tree(BinTree& destination, const BinTree& source);
    void free_tree(BinTree& root);
    void delete_node(BinTree& tree, const T& value);
    const BinTreeNode* sub_node(BinTree tree, int compares) const;
    void insert_node(BinTree& tree, const T& value);
    bool find_node (BinTree tree, const T& value, unsigned& compares) const;
};

#include "BSTree.cpp"

#endif
//---------------------------------------------------------------------------
