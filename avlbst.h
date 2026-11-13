#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

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
    return balance_;
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
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
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
    AVLNode<Key, Value>* rotateLeft(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rotateRight(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rebalance(AVLNode<Key, Value>* node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO

    if (this->root_ == nullptr) {
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);

      return;
    }

    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);

    AVLNode<Key, Value>* parent = nullptr;

    while (current != nullptr) {
      parent = current;

      if (new_item.first < current->getKey()){
        current = current->getLeft();

      }
      else if (current->getKey() < new_item.first) {
        current = current->getRight();
      }
      else {
        current->setValue(new_item.second);
        return;
      }
    }
    AVLNode<Key, Value>* added = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    
    if (new_item.first < parent->getKey()){
      parent->setLeft(added);
    }
    else {
      parent->setRight(added);
    }
    AVLNode<Key, Value>* child = added;

    while (parent != nullptr) {
      if (child == parent->getLeft()) {
        parent->updateBalance(-1);
      }
      else {
        parent->updateBalance(1);
      }
      if (parent->getBalance() == 0){
        return;
      }
      if (parent->getBalance() == -2 || parent->getBalance() == 2){
        rebalance(parent);
        return;
      }
      child = parent;
      parent = parent->getParent();
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* target = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

    if(target == nullptr) {
      return;
    }
    if (target->getLeft() != nullptr && target->getRight() != nullptr) {
      AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(target));

      nodeSwap(target, pred);
    }
    AVLNode<Key, Value>* parent = target->getParent();
    AVLNode<Key, Value>* child = target->getLeft();

    if (child == nullptr) { 
      child = target->getRight();
    }
    int diff = 0;

    if (parent != nullptr) {
      if (target == parent->getLeft()){
        diff = 1;
      }
      else {
        diff = -1;
      }
    }
    if (child != nullptr) {
      child->setParent(parent);
    }
    if (parent == nullptr) {
      this->root_ = child;

    }
    else if (target == parent->getLeft()) {
      parent->setLeft(child);
    }
    else{
      parent->setRight(child);
    }
    delete target;

    while (parent != nullptr) {
      parent->updateBalance(diff);

      AVLNode<Key, Value>* subtreeRoot = parent;

      if ((parent->getBalance() == -1) || (parent->getBalance() == 1)) {
        return;
      }
      if ((parent->getBalance() == -2) || (parent->getBalance() == 2)) {
        subtreeRoot = rebalance(parent);

        if (subtreeRoot->getBalance() != 0) {
          return;
        }
      }
      AVLNode<Key, Value>* nextParent = subtreeRoot->getParent();

      if (nextParent != nullptr) {
        if (subtreeRoot == nextParent->getLeft()){
          diff = 1;
        }
        else{
          diff = -1;
        }
      }
      parent = nextParent;
    }
}
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node){
  
  AVLNode<Key, Value>* promoted = node->getRight();
  AVLNode<Key, Value>* middle = promoted->getLeft();
  AVLNode<Key, Value>* parent = node->getParent();

  promoted->setParent(parent);

  if (parent == nullptr) {
    this->root_ = promoted;
  }
  else if (parent->getLeft() == node) {
    parent->setLeft(promoted);
  }
  else{
    parent->setRight(promoted);
  }
  node->setRight(middle);

  if (middle != nullptr) {
    middle->setParent(node);
  }

  promoted->setLeft(node);
  node->setParent(promoted);

  int nodeBalance = node->getBalance();
  int promotedBalance = promoted->getBalance();
  nodeBalance = nodeBalance -1 - std::max(promotedBalance, 0);

  promotedBalance = promotedBalance - 1 + std::min(nodeBalance, 0);

  node->setBalance(nodeBalance);
  promoted->setBalance(promotedBalance);

  return promoted;

}
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node){
  
  AVLNode<Key, Value>* promoted = node->getLeft();
  AVLNode<Key, Value>* middle = promoted->getRight();
  AVLNode<Key, Value>* parent = node->getParent();
  promoted->setParent(parent);

  if (parent == nullptr) {
    this->root_ = promoted;
  }
  else if (parent->getLeft() == node) {
    parent->setLeft(promoted);
  }
  else{
    parent->setRight(promoted);
  }
  node->setLeft(middle);

  if (middle != nullptr) {
    middle->setParent(node);
  }

  promoted->setRight(node);
  node->setParent(promoted);

  int nodeBalance = node->getBalance();
  int promotedBalance = promoted->getBalance();
  nodeBalance = nodeBalance + 1 - std::min(promotedBalance, 0);

  promotedBalance = promotedBalance + 1 + std::max(nodeBalance, 0);

  node->setBalance(nodeBalance);
  promoted->setBalance(promotedBalance);

  return promoted;
  
}
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rebalance(AVLNode<Key, Value>* node){

  if (node->getBalance() == -2) {
    if (node->getLeft()->getBalance() > 0) {
      rotateLeft(node->getLeft());
    }
    return rotateRight(node);
  }
  if (node->getBalance() == 2){
    if (node->getRight()->getBalance() < 0){
      rotateRight(node->getRight());

    }
    return rotateLeft(node);
  }
  return node;
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
