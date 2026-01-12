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
    int size = 9;
    int count = 0;
    int num1 = 2;
    T* storge = new int[size];
    int rem(int key) {
        return 1 + key % num1;
    }
    int hash(int key, int k) {
        return  (key + k*rem(key)) % size;
    }
public:
    HashMap():root(nullptr){}
    virtual ~HashMap() {
        delete[] storge;
    }
    bool isEmpty() const{return count == 0;}
    void insert(const T& val)
    {

    }
    void remove(int key)
    {

    }
    T find(int key)const
    {
        return storge[0];
    }
    void print() const {}

};




#endif //HASHMAP_H
