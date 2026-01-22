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
    int length;
    int height;
    int count = 0;
    int num1 = 2;

    int* index;
    int* link;
    int* storge;
    shared_ptr<T>* keys;

// int arr[10] = {-1}

    int rem(int key) {
        return 1 + key % num1;
    }
    int hashk(int key, int k) {
        return  (key + k*rem(key)) % size;
    }
    void resize() {
        length*=3;
        num1*=2;

        int* old_index = index;
        int* old_link = link;
        int* old_storge = storge;
        shared_ptr<T>* old_keys = keys;
        int old_height = height;

        index = new int[length];
        link = new int[length];
        storge = new int[length];
        keys = new shared_ptr<T>[length];
        height = 0;

        for (int i = 0; i < length/3; i++) {
            if (old_index[old_link[i]] == i && old_link[i] < old_height) {
                insert(old_storge[i], old_keys[i]);
            }
        }

        delete old_index;
        delete old_link;
        delete old_storge;
        delete old_keys;
    }



public:
    HashMap(): length(9), height(0), count(0), num1(2) {
        index = new int[length];
        link = new int[length];
        keys = new int[length];
        storge = new shared_ptr<T>[length];
    };
    virtual ~HashMap() {
        delete[] storge;
    }
    bool isEmpty() const{return count == 0;}

    int find (int key) const {
        int k = 0;
        int i;
        while(true) {
            i = hashk(key, k);
            if (index[link[i]] != i || keys[i] == key || link[i] >= height) {
                return i;
            }
            k += 1;
        }
    }

    int find_e (int key) const {
        int k = 0;
        int i;
        while(true) {
            i = hashk(key, k);
            if (index[link[i]] != i || keys[i] == -1 || keys[i] == key || link[i] >= height) {
                return i;
            }
            k += 1;
        }
    }

    void put(const T& val, int key)
    {
        int idx = find(key);
        if (index[link[idx]] == idx && link[idx] < height && keys[idx] == key) {
            throw invalid_argument("Already here!");
        }
        idx = find_e(key);
        if (index[link[idx]] != idx) {
            link[idx] = height;
            index[link[idx]] = idx;
            height += 1;
        }
        keys[idx] = key;
        storge[idx] = val;
        count += 1;
    }
    void remove(int key)
    {
        int idx = find(key);
        if (index[link[idx]] != idx || link[idx] >= height || keys[idx] != key) {
            throw invalid_argument("Not found");
            return;
        }
        keys[idx] = -1;
        storge[idx] = nullptr;
        count -= 1;
    }

    T get(int key) {
        int idx = find(key);
        if (index[link[idx]] != idx || link[idx] >= height || keys[idx] != key) {
            throw invalid_argument("Not found");
        }
        return *storge[idx];
    }

    bool contains(int key) {
        int idx = find(key);
        return index[link[idx]] == idx && link[idx] < height && keys[idx] == key;
    }

    int size(){return count;}

    void print() const {}

};




#endif //HASHMAP_H
