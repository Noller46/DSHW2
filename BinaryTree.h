//
// Created by ronca on 12/1/2025.
//
#include  <memory>
#include <iostream>
#include <exception>

using namespace std;
#ifndef BINARYTREE_H
#define BINARYTREE_H
template <typename T>
class BinaryTree
{
protected:
        struct Node
        {
            weak_ptr<Node> parent;
            shared_ptr<Node> left_son;
            shared_ptr<Node> right_son;
            T data;
            int height;
            int below;
            explicit Node(T val): data(val), height(1), below(0){}
            ~Node() = default;
        };
    shared_ptr<Node> root;

    // unwrap helpers
    template<typename U>
    static const U& unwrap(const U& v) { return v; }

    template<typename U>
    static const U& unwrap(const std::shared_ptr<U>& p) { return *p; }

    template<typename U>
    static const U& unwrap(const std::unique_ptr<U>& p) { return *p; }

    int keyOf(const T& value) const
    {
        return unwrap(value).getId();
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
    virtual void insert_recursive(shared_ptr<Node>& curr, const T& val, weak_ptr<Node> parent){
        if (!curr)
        {
            curr = make_shared<Node>(val);
            curr->parent = parent;
            return;
        }
        int key = keyOf(val);
        int currKey = keyOf(curr->data);

        if (key < currKey)
            insert_recursive(curr->left_son, val, curr);
        else if (currKey < key)
            insert_recursive(curr->right_son, val, curr);
        else
            throw invalid_argument("Already here!");

        curr->below += 1;
    }
    virtual void remove_recursive(shared_ptr<Node>& curr, int key)
    {
        if (!curr)
            throw invalid_argument("Object not in tree!");
        int currKey = keyOf(curr->data);
        if (key < currKey)
            remove_recursive(curr->left_son, key);
        else if (currKey < key)
            remove_recursive(curr->right_son, key);
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
                remove_recursive(curr->right_son,keyOf(successor->data));
            }
        }
        curr->below -= 1;
    }
    shared_ptr<Node> find_recursive(const shared_ptr<Node>& curr, int key)const
    {
        if (!curr)
              throw invalid_argument("Object Not in tree");
        int currKey = keyOf(curr->data);
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
    virtual void insert(const T& val)
    {
        insert_recursive(root, val, weak_ptr<Node>());
    }
    virtual void remove(int key)
    {
        remove_recursive(root,key);
    }
    shared_ptr<Node> find(int key)const
    {
        return find_recursive(root, key);
    }


    void print_in_order_recursive(const shared_ptr<Node>& node) const {
        if (!node) return;
        print_in_order_recursive(node->left_son);
        cout << *node->data << "(H:" << node->height << ") ";
        print_in_order_recursive(node->right_son);
    }
    void print() const {
        cout << "Tree (In-Order, Sorted by ID): \n";
        print_in_order_recursive(root);
        cout << endl;
    }
};
#endif //BINARYTREE_H
