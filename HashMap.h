//
// Created by yoavf on 08/01/2026.
//
#include  <memory>
#include <iostream>
#include <exception>

using namespace std;
#ifndef HASHMAP_H
#define HASHMAP_H

template <typename T>
class HashMap{
private:
    struct Node
    {
        shared_ptr<Node> next;
        int key;
        T data;
        Node(): key(-1), data(new T) {}
        Node(T val, int key): data(val), key(key), next(nullptr){}
        ~Node() = default;
    };

    int length;
    int count;
    Node<T>* objects;

    int hash(int key) {
        return  key % size;
    }

    void dive(Node<T> curr, HashMap<T>* hashmap) {
        hashmap->put(curr.key, curr.data);
        if (curr->next != nullptr) {
            dive(curr->next, hashmap);
        }
    }

    void resize() {
        HashMap hold = HashMap(length*2);
        for(int i = 0; i < length; i++) {
            dive(objects[i], hold);
        }
        length*=2;
        delete objects;
        objects = hold->objects;
        count = hold->count;
    }



public:
    HashMap(): length(8), count(0) {
        objects = new Node<T>*[length]();
    }

    explicit HashMap(int len): length(len), count(0) {
        objects = new Node<T>*[length]();
    }

    virtual ~HashMap() {
        delete[] objects;
    }

    bool isEmpty() const{return count == 0;}

    bool traverse(Node<T> curr, int key) {
        if(curr.key == key){return true;}
        if(curr->next == nullptr){return false;}
        return traverse(curr->next, key);
    }

    bool contains (int key) const {
        int location = hash(key);
        if(objects[location] == nullptr){return false;}
        return traverse(objects[location], key);
    }

    void insert(Node<T> curr, T data, int key) {
        if (curr.next == nullptr){curr.next = new Node<T>(data, key); return;}
        insert(curr->next, data, key);
    }

    void put(const T& data, int key)
    {
        if(contains(key)){throw invalid_argument("already exists");}
        int location = hash(key);
        insert(objects[location], data, key);
        count += 1;
        if (((double) count) / length >= 0.75) {
            resize();
        }
    }

    void extract(Node<T> curr, int key) {
        if (curr->next.key == key) {
                curr->next = curr->next->next;
        }
        extract(curr->next, key);
    }

    void remove(int key)
    {
        if(!contains(key)){throw invalid_argument("not present");}
        int location = hash(key);
        if (objects[location].key == key) {
            next = objects[location].next;
        }
        extract(objects[location], key);
        count -= 1;
    }

    T locate(Node<T> curr, int key) {
        if (curr.key == key){return curr.data;}
        locate(curr->next, key);
    }

    T get(int key) {
        if(!contains(key)){throw invalid_argument("not present");}
        int location = hash(key);
        return extract(objects[location], key);
    }

    int size(){return count;}


};




#endif //HASHMAP_H
