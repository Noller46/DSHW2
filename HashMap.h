//
// Created by yoavf on 08/01/2026.
//
#include <memory>
#include <iostream>
#include <exception>

using namespace std;

#ifndef HASHMAP_H
#define HASHMAP_H

template <typename T>
class HashMap {
private:
    struct Node {
        shared_ptr<Node> next;
        int key;
        shared_ptr<T> data;

        Node() : key(-1), data(nullptr), next(nullptr) {}
        Node(const T& val, int key)
            : key(key), data(make_shared<T>(val)), next(nullptr) {}
        ~Node() = default;
    };

    int length;
    int count;

    // array of buckets
    shared_ptr<shared_ptr<Node>[]> objects;

    int hash(int key) {
        return key % length;
    }

    void dive(shared_ptr<Node> curr, HashMap<T>* hashmap) {
        while (curr != nullptr) {
            hashmap->put(*(curr->data), curr->key);
            curr = curr->next;
        }
    }

    void resize() {
        HashMap hold = HashMap(length * 2);
        for (int i = 0; i < length; i++) {
            dive(objects[i], &hold);
        }
        length *= 2;
        objects = hold.objects;
        count = hold.count;
    }

public:
    HashMap() : length(8), count(0) {
        objects = shared_ptr<shared_ptr<Node>[]>(
            new shared_ptr<Node>[length](),
            default_delete<shared_ptr<Node>[]>()
        );
    }

    explicit HashMap(int len) : length(len), count(0) {
        objects = shared_ptr<shared_ptr<Node>[]>(
            new shared_ptr<Node>[length](),
            default_delete<shared_ptr<Node>[]>()
        );
    }

    ~HashMap() = default;

    bool isEmpty() const {
        return count == 0;
    }

    bool traverse(shared_ptr<Node> curr, int key) {
        while (true) {
            if (curr->key == key) return true;
            if (curr->next == nullptr) return false;
            curr = curr->next;
        }
    }

    bool contains(int key) const {
        int location = hash(key);
        if (objects[location] == nullptr) return false;
        return traverse(objects[location], key);
    }

    void insert(shared_ptr<Node> curr, const T& data, int key) {
        while (curr->next != nullptr) {
            curr = curr->next;
        }
        curr->next = make_shared<Node>(data, key);
    }

    void put(const T& data, int key) {
        if (contains(key)) {
            throw invalid_argument("already exists");
        }

        int location = hash(key);

        if (objects[location] == nullptr) {
            objects[location] = make_shared<Node>(data, key);
        } else {
            insert(objects[location], data, key);
        }

        count += 1;

        if ((double)count / length >= 0.75) {
            resize();
        }
    }

    void extract(shared_ptr<Node> curr, int key) {
        while (true) {
            if (curr->next->key == key) {
                curr->next = curr->next->next;
                return;
            }
            curr = curr->next;
        }
    }

    void remove(int key) {
        if (!contains(key)) {
            throw invalid_argument("not present");
        }

        int location = hash(key);

        if (objects[location]->key == key) {
            objects[location] = objects[location]->next;
        } else {
            extract(objects[location], key);
        }

        count -= 1;
    }

    T locate(shared_ptr<Node> curr, int key) {
        while (true) {
            if (curr->key == key) {
                return *(curr->data);
            }
            curr = curr->next;
        }
    }

    T get(int key) {
        if (!contains(key)) {
            throw invalid_argument("not present");
        }
        int location = hash(key);
        return locate(objects[location], key);
    }

    int size() {
        return count;
    }
};

#endif // HASHMAP_H