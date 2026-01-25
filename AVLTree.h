//
// Created by ronca on 12/5/2025.
//
#include "BinaryTree.h"
#include  <memory>
using namespace std;
#ifndef AVLTREE_H
#define AVLTREE_H
template<typename T>
class AVLTree : public BinaryTree<T> {
  private:
    using Node = typename BinaryTree<T>::Node;

  int getBF(const shared_ptr<Node>& node)const{
    if(!node) return 0;
    return this->getHeight(node->left_son)-this->getHeight(node->right_son);
  }
  shared_ptr<Node> rotate_left(shared_ptr<Node>& a)
  {
    if (!a || !a->right_son)
      return a;

    shared_ptr<Node> b = a->right_son;
    shared_ptr<Node> parent = a->parent.lock();

    a->right_son = b->left_son;
    if (a->right_son)
      a->right_son->parent = a;

    b->left_son = a;
    a->parent = b;

    b->parent = parent;
    if (parent)
    {
      if (parent->left_son == a)
        parent->left_son = b;
      else
        parent->right_son = b;
    }
    else
    {
      this->root = b;
    }

    a->below = 0;
    if (a->left_son)
      a->below += a->left_son->below + 1;
    if (a->right_son)
      a->below += a->right_son->below + 1;

    b->below = 0;
    if (b->left_son)
      b->below += b->left_son->below + 1;
    if (b->right_son)
      b->below += b->right_son->below + 1;

    this->updateHeight(a);
    this->updateHeight(b);

    return b;
  }
  shared_ptr<Node> rotate_right(shared_ptr<Node>& c)
  {
    if (!c || !c->left_son)
      return c;

    shared_ptr<Node> b = c->left_son;
    shared_ptr<Node> parent = c->parent.lock();

    c->left_son = b->right_son;
    if (c->left_son)
      c->left_son->parent = c;

    b->right_son = c;
    c->parent = b;

    b->parent = parent;
    if (parent)
    {
      if (parent->left_son == c)
        parent->left_son = b;
      else
        parent->right_son = b;
    }
    else
    {
      this->root = b;
    }

    c->below = 0;
    if (c->left_son)
      c->below += c->left_son->below + 1;
    if (c->right_son)
      c->below += c->right_son->below + 1;

    b->below = 0;
    if (b->left_son)
      b->below += b->left_son->below + 1;
    if (b->right_son)
      b->below += b->right_son->below + 1;

    this->updateHeight(c);
    this->updateHeight(b);

    return b;
  }
  void balance(shared_ptr<Node>& node)
  {
    if (!node)
      return;
    int balance = this->getBF(node);
    if (balance >= 2)
    {
      if (this->getBF(node->left_son) < 0)
      {
        node->left_son = rotate_left(node->left_son);
        if (node->left_son)
          node->left_son->parent = node;
      }
      node = rotate_right(node);
    }
    else if (balance <= -2)
    {
      if (getBF(node->right_son) > 0)
      {
        node->right_son = rotate_right(node->right_son);
        if (node->right_son)
          node->right_son->parent = node;
      }
      node = rotate_left(node);
    }
  }
  bool insert_recursive(shared_ptr<Node>& curr, const T& val, int key, weak_ptr<Node> parent) override
  {
    bool ret = BinaryTree<T>::insert_recursive(curr, val, key, parent);
    if (curr)
    {
      this->updateHeight(curr);
      balance(curr);
    }
    return ret;
  }
  bool remove_recursive(shared_ptr<Node>& curr, int key)override
  {
    bool ret = BinaryTree<T>::remove_recursive(curr, key);
    if (curr)
    {
      this->updateHeight(curr);
      balance(curr);
    }
    return ret;
  }
public:
  AVLTree() = default;
  ~AVLTree() override = default;
  };

#endif //AVLTREE_H
