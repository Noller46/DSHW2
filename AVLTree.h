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
    shared_ptr<Node> rotate_left(shared_ptr<Node>& a) // abs(bf)>=2
    {
      if (!a || !a->right_son)
        return a;
      shared_ptr<Node> b = a->right_son;
      shared_ptr<Node> parent = a->parent.lock();
      int below_a = a->below();
      a->right_son = b->left_son;
      if (a->right_son) {
        a->right_son->parent = a;
        a->below = a->below - b->below + a->right_son->below;
      }
      b->left_son = a;
      b->below = below_a;
      b->parent = a->parent;
      a->parent = b;
      if (parent)
      {
        if (parent->left_son == a)
          parent->left_son = b;
        else
          parent->right_son = b;
      }else{
        this->root = b;
      }
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
    int below_c = c->below();
    c->left_son = b->right_son;
    b->right_son = c;
    b->parent = c->parent;
    c->parent = b;
    if (c->left_son) {
      c->left_son->parent = c;
      c->below = c->below - b->below + c->left_son->below;
    }
    b->below = below_c;
    if (parent)
    {
      if (parent->left_son == c)
        parent->left_son = b;
      else
        parent->right_son = b;
    }else{
      this->root = b;
    }
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
  void insert_recursive(shared_ptr<Node>& curr, const T& val, weak_ptr<Node> parent) override
  {
    BinaryTree<T>::insert_recursive(curr, val, parent);
    if (curr)
    {
      this->updateHeight(curr);
      balance(curr);
    }
  }
  void remove_recursive(shared_ptr<Node>& curr, int key)override
  {
    BinaryTree<T>::remove_recursive(curr, key);
    if (curr)
    {
      this->updateHeight(curr);
      balance(curr);
    }
  }
public:
  AVLTree() = default;
  ~AVLTree() override = default;
  };

#endif //AVLTREE_H
