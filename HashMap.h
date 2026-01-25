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
            : next(nullptr), key(key), data(make_shared<T>(val)) {}
        ~Node() = default;
    };

    int length;
    int count;

    // array of buckets
    shared_ptr<shared_ptr<Node>[]> objects;

    int hash(int key) const {
        if (key <= 0) {throw invalid_argument("key bellow 0");}
        return key % length;
    }

    void resize() {
        int newLength = length * 2;

        shared_ptr<shared_ptr<Node>[]> newObjects( new shared_ptr<Node>[newLength](),
            default_delete<shared_ptr<Node>[]>() );

        for (int i = 0; i < length; i++) {
            shared_ptr<Node> curr = objects[i];
            while (curr != nullptr) {
                shared_ptr<Node> nextNode = curr->next;

                int newIndex = curr->key % newLength;
                if (newIndex < 0) newIndex += newLength;

                curr->next = newObjects[newIndex];
                newObjects[newIndex] = curr;

                curr = nextNode;
            }
        }

        objects = newObjects;
        length = newLength;

    }

    bool traverse(shared_ptr<Node> curr, int key) const {
        while (true) {
            if (curr->key == key) return true;
            if (curr->next == nullptr) return false;
            curr = curr->next;
        }
    }

    int insert(shared_ptr<Node> curr, const T& data, int key) {
        int depth = 0;
        while (curr->next != nullptr) {
            curr = curr->next;
            depth += 1;
        }
        curr->next = make_shared<Node>(data, key);
        return depth;
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

    T locate(shared_ptr<Node> curr, int key) {
        while (true) {
            if (curr->key == key) {
                return *(curr->data);
            }
            curr = curr->next;
        }
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

    bool contains(int key) const {
        int location = hash(key);
        if (objects[location] == nullptr) return false;
        return traverse(objects[location], key);
    }

    // can be optimized if code is slow!!!
    void put(const T& data, int key) {
        if (contains(key)) {
            throw invalid_argument("already exists");
        }

        int location = hash(key);
        int depth = 0;

        if (objects[location] == nullptr) {
            objects[location] = make_shared<Node>(data, key);
        } else {
            depth = insert(objects[location], data, key);
        }

        count += 1;

        if ((double)count / length >= 0.75 || depth > 100) {
            resize();
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

    void print() const {
        cout << "lenght=" << length << " count=" << count << endl;
        for (int i = 0; i < length; i++) {
            shared_ptr<Node> curr = objects[i];
            while (curr) {
                cout << "Index=" << hash(curr->key) << " key=" << curr->key << " value=" << *(curr->data) << endl;
                curr = curr->next;
            }
        }
    }

};

#endif // HASHMAP_H