//
// Created by yoavf on 08/01/2026.
//
#include "AVLTree.h"
using namespace std;
#ifndef UNIONFIND_H
#define UNIONFIND_H
template <typename G, typename A>
class UnionFind {
private:
    template <typename T>
    struct Node
    {
        shared_ptr<Node> next;
        T data;
        explicit Node(T val): data(val){}
        ~Node() = default;
    };
    AVLTree<shared_ptr<Node<G>>> GroupStorge;
    AVLTree<shared_ptr<Node<A>>> ArgumentStorge;
public:
    UnionFind() = default;
    int addGroupd() {
        GroupStorge.insert(make_shared<Node<G>>());
    }
    int addArgument() {

    }
    int find(int kye) const {
        A start = ArgumentStorge.find();

        return kye;
    }
    int unite(int group1, int group2) {
        return group1;
    }


};



#endif //UNIONFIND_H
