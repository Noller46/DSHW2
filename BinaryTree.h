//
// Created by ronca on 12/1/2025.
//
#include  <memory>
#include <iostream>
#include <stdexcept>

using namespace std;
#ifndef BINARYTREE_H
#define BINARYTREE_H

struct TreeKey {
    int value;
    int id;

    explicit TreeKey(int v = 0, int i = 0) : value(v), id(i) {}

    bool operator<(const TreeKey& other) const {
        if (value != other.value) return value < other.value;
        return id < other.id;
    }
    bool operator>(const TreeKey& other) const { return other < *this; }
    bool operator==(const TreeKey& other) const { return value == other.value && id == other.id; }
    bool operator!=(const TreeKey& other) const { return !(*this == other); }


    friend std::ostream& operator<<(std::ostream& os, const TreeKey& k) {
        return os << "{" << k.value << "," << k.id << "}";
    }
};

template <typename T>
class BinaryTree
{
protected:
        struct Node
        {
            weak_ptr<Node> parent;
            shared_ptr<Node> left_son;
            shared_ptr<Node> right_son;
            TreeKey key;
            T data;
            int height;
            int size;
            explicit Node(T val, TreeKey key): key(key), data(val), height(1), size(1){}
            ~Node() = default;
        };
    shared_ptr<Node> root;


    int getSize(const shared_ptr<Node>& node) const {
        return node ? node->size : 0;
    }

    void updateSize(shared_ptr<Node>& node) {
        if (node) {
            node->size = 1 + getSize(node->left_son) + getSize(node->right_son);
        }
    }
    int getHeight(const shared_ptr<Node>& node)const{
        return node? node->height:0;
    }

    void updateHeight(shared_ptr<Node>& node)
    {
        if (node)
        {
            int left_height = this->getHeight(node->left_son);
            int right_height = this->getHeight((node->right_son));
            node->height = 1 + (left_height > right_height ? left_height : right_height);
        }
    }
    void updateNodeData(shared_ptr<Node>& node) {
            updateHeight(node);
            updateSize(node);
    }
    shared_ptr<Node> select_recursive(shared_ptr<Node> curr, int k) const {
        if (!curr) throw std::runtime_error("Tree Corruption: Size mismatch detected in select()");
        int leftSize = getSize(curr->left_son);
        int cur_rank = leftSize + 1;
        if (cur_rank == k) return curr;
        if (cur_rank > k)  return select_recursive(curr->left_son, k);
        return select_recursive(curr->right_son, k - cur_rank);
    }
    /*
    template<typename U>
    static const U& unwrap(const U& v) { return v; }

    template<typename U>
    static const U& unwrap(const std::shared_ptr<U>& p) { return *p; }

    template<typename U>
    static const U& unwrap(const std::unique_ptr<U>& p) { return *p; }
*/


    virtual bool insert_recursive(shared_ptr<Node>& curr, const T& val, TreeKey key, weak_ptr<Node> parent){
        if (!curr) {
            curr = make_shared<Node>(val, key);
            curr->parent = parent;
            updateNodeData(curr);
            return true;
        }

        if (key < curr->key) {
            insert_recursive(curr->left_son, val, key, curr);
        }
        else if (curr->key < key) {
            insert_recursive(curr->right_son, val, key, curr);
        }
        else {
            throw invalid_argument("Already here!");
        }
        updateNodeData(curr);
        return true;
    }

    virtual bool remove_recursive(shared_ptr<Node>& curr, TreeKey key)
    {
        if (!curr)
            throw invalid_argument("Object not in tree!");
        TreeKey currKey = curr->key;
        bool res = true;
        if (key < currKey)
            res = remove_recursive(curr->left_son, key);
        else if (currKey < key)
            res = remove_recursive(curr->right_son, key);
        else
        {
            if (!curr->right_son)
            {
                auto child = curr->left_son;
                if (child) child->parent = curr->parent;
                curr = child;
            }
            else if (!curr->left_son)
            {
                auto child = curr->right_son;
                if (child) child->parent = curr->parent;
                curr = child;
            }
            else
            {
                auto successor = curr->right_son;
                while (successor->left_son)
                    successor = successor->left_son;

                curr->data = successor->data;
                curr->key = successor->key;
                res = remove_recursive(curr->right_son,successor->key);
            }
        }
       if (curr)
           updateNodeData(curr);
        return res;
    }
    shared_ptr<Node> find_recursive(const shared_ptr<Node>& curr, TreeKey key)const
    {
        if (!curr)
              throw invalid_argument("Object Not in tree");
        TreeKey currKey = curr->key;
        if (currKey > key)
            return find_recursive(curr->left_son,key);
        if (currKey < key)
            return find_recursive(curr->right_son,key);
        return curr;
    }
    public:
    BinaryTree():root(nullptr){}
    virtual ~BinaryTree() = default;
    bool isEmpty() const{return !root;}
    int getSize() const
    {
        return getSize(root);
    }

    int recomputeSize(const shared_ptr<Node>& node) const {
        if (!node) return 0;
        return 1 + recomputeSize(node->left_son) + recomputeSize(node->right_son);
    }


    virtual void insert(const T& val, TreeKey key)
    {
        insert_recursive(root, val, key, weak_ptr<Node>());
    }
    virtual void remove(TreeKey key)
    {
        remove_recursive(root,key);
    }
    T& find(TreeKey key)const
    {
        return find_recursive(root, key)->data;
    }

    T& select(int i) const {
        return select_recursive(root, i)->data;
    }

    void print_in_order_recursive(const shared_ptr<Node>& node) const {
        if (!node) return;
        print_in_order_recursive(node->left_son);
        // Note: Make sure T supports << operator, or print node->key
        cout << "[ID:" << node->key << " H:" << node->height << " S:" << node->size << " V:" << node->data << "] ";
        print_in_order_recursive(node->right_son);
    }

    void print() const {
        cout << "Tree Structure: ";
        print_in_order_recursive(root);
        cout << endl;
    }
};
#endif //BINARYTREE_H
