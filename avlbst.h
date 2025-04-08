#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

using namespace std;

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return (this == nullptr) ? -1 : balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return (this == nullptr) ? nullptr : static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here

    Node<Key, Value>* createNode(const Key& key, const Value& value, 
        Node<Key, Value>* parent) override;

    // Same as Base Class insert, just with AVLNode
    void insertBase (const std::pair<const Key, Value> &new_item);


    void rotateWithLeftChild(AVLNode<Key, Value>* grandparent);
    void rotateWithRightChild(AVLNode<Key, Value>* grandparent);
    void doubleWithLeftChild(AVLNode<Key, Value>* grandparent);
    void doubleWithRightChild(AVLNode<Key, Value>* grandparent);


    void removeNoChild(AVLNode<Key, Value>* nodeToRemove);
    void removeOneChild(AVLNode<Key, Value>* nodeToRemove);



};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    //Uses derived createNode function
    BinarySearchTree<Key, Value>::insert(new_item);


    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(
        this->internalFind(new_item.first));

    //Balance + Rotations
    while (node != nullptr) {
        if (node->getLeft()->getBalance() - node->getRight()->getBalance() > 1) {
            if (node->getLeft()->getLeft()->getBalance() >= node->getLeft()->getRight()->getBalance()) {
                rotateWithLeftChild(node);
            }
            else {
                doubleWithLeftChild(node);
                
            }
        }
        else if (node->getRight()->getBalance() - node->getLeft()->getBalance() > 1) {
            if (node->getRight()->getRight()->getBalance() >= node->getRight()->getLeft()->getBalance()) {
                rotateWithRightChild(node);
            }
            else {
                doubleWithRightChild(node);
            }
        }

        node->setBalance(max(node->getLeft()->getBalance(), node->getRight()->getBalance()) + 1);
        node = node->getParent();
    }


}



template<class Key, class Value>
Node<Key, Value>* AVLTree<Key, Value>::createNode(const Key& key, const Value& value, Node<Key, Value>* parent)
{
    return new AVLNode<Key, Value>(key, value, 
        static_cast<AVLNode<Key, Value>*>(parent)); // Creates AVLNode
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateWithLeftChild(AVLNode<Key, Value>* n_3) {

    AVLNode<Key, Value>* n_2 = n_3->getLeft();
    AVLNode<Key, Value>* n_1 = n_2->getLeft();

    nodeSwap(n_2, n_3);
    n_2->setLeft(n_1);
    n_2->setRight(n_3);
    if (n_1 != nullptr) {n_1->setParent(n_2);}
    n_3->setParent(n_2);
    n_3->setLeft(nullptr);

    n_3->setBalance(max(n_3->getLeft()->getBalance(),
        n_3->getRight()->getBalance()) +1);
    n_2->setBalance(max(n_2->getLeft()->getBalance(),
        n_3->getBalance()) + 1);




}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateWithRightChild(AVLNode<Key, Value>* n_3) {

    AVLNode<Key, Value>* n_2 = n_3->getRight();
    AVLNode<Key, Value>* n_1 = n_2->getRight();

    nodeSwap(n_2, n_3);
    n_2->setRight(n_1);
    n_2->setLeft(n_3);
    if (n_1 != nullptr) {n_1->setParent(n_2);}
    n_3->setParent(n_2);
    n_3->setRight(nullptr);

    n_3->setBalance(max(n_3->getLeft()->getBalance(),
        n_3->getRight()->getBalance()) +1);
    n_2->setBalance(max(n_2->getRight()->getBalance(),
        n_3->getBalance()) + 1);
}

template<class Key, class Value>
void AVLTree<Key, Value>::doubleWithLeftChild(AVLNode<Key, Value>* n_3) {
    rotateWithRightChild(n_3->getLeft());
    rotateWithLeftChild(n_3);
}
template<class Key, class Value>
void AVLTree<Key, Value>::doubleWithRightChild(AVLNode<Key, Value>* n_3) {
    rotateWithLeftChild(n_3->getRight());
    rotateWithRightChild(n_3);
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    //reimplementing remove
    AVLNode<Key, Value>* nodeToRemove = static_cast<AVLNode<Key, Value>*>(
        this->internalFind(key));

    if (nodeToRemove == nullptr) {
        return;
    }


    // Case 1: Node has no children
    if (nodeToRemove->getLeft() == nullptr && nodeToRemove->getRight() == nullptr) {
        removeNoChild(nodeToRemove);
    }
    // Case 2: Node has one child
    else if (nodeToRemove->getLeft() == nullptr || nodeToRemove->getRight() == nullptr) {
        removeOneChild(nodeToRemove);
    }
    // Case 3: Node has two children, swap with predecessor
    else {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(nodeToRemove));
        nodeSwap(nodeToRemove, pred);
        
        //Node has no children
        if (nodeToRemove->getLeft() == nullptr && nodeToRemove->getRight() == nullptr) {
            removeNoChild(nodeToRemove);
        }

        //Node has one child
        else if (nodeToRemove->getLeft() == nullptr || nodeToRemove->getRight() == nullptr) {
            removeOneChild(nodeToRemove);
        }
    }
    

}

template<class Key, class Value>
void AVLTree<Key, Value>::removeNoChild(AVLNode<Key, Value>* nodeToRemove) 
{
    if (nodeToRemove == this->root_){
        this->root_ = nullptr;
    } 
    else {
        // Remove Left Child
        if (nodeToRemove->getParent()->getLeft() == nodeToRemove) {
            nodeToRemove->getParent()->setLeft(nullptr);
        } 
        // Or Removes Right Child
        else {
            nodeToRemove->getParent()->setRight(nullptr);
        }
    }
    delete nodeToRemove;
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeOneChild(AVLNode<Key, Value>* nodeToRemove) 
{
    AVLNode<Key, Value>* child = (nodeToRemove->getLeft() != nullptr) ? 
                                    nodeToRemove->getLeft() : nodeToRemove->getRight();
    if (nodeToRemove == this->root_) {
        this->root_ = child;
        child->setParent(nullptr);
    } 
    else {
        // Remove Left Child
        if (nodeToRemove->getParent()->getLeft() == nodeToRemove) {
            nodeToRemove->getParent()->setLeft(child);
        } 
        // Or Removes Right
        else {
            nodeToRemove->getParent()->setRight(child);
        }
        child->setParent(nodeToRemove->getParent());
    }
    delete nodeToRemove;

    while (child != nullptr) {
        if (child->getLeft()->getBalance() - child->getRight()->getBalance() > 1) {
            if (child->getLeft()->getLeft()->getBalance() >= child->getLeft()->getRight()->getBalance()) {
                rotateWithLeftChild(child);
            }
            else {
                doubleWithLeftChild(child);
                
            }
        }
        else if (child->getRight()->getBalance() - child->getLeft()->getBalance() > 1) {
            if (child->getRight()->getRight()->getBalance() >= child->getRight()->getLeft()->getBalance()) {
                rotateWithRightChild(child);
            }
            else {
                doubleWithRightChild(child);
            }
        }

        child->setBalance(max(child->getLeft()->getBalance(), child->getRight()->getBalance()) + 1);
        child = child->getParent();
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
