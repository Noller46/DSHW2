using namespace std;
#include <memory>
#include "AVLTree.h"
#include "HashMap.h"


class Set {
    struct Node {
        int id;
        shared_ptr<Hunter> hunterP;
        shared_ptr<Node> parentP;
        Node(int id, shared_ptr<Node> parent) : id(id), hunterP(nullptr), parentP(parent) {}
    };

    int id;
    shared_ptr<Node> head;
    int size;
    HashMap<Hunter> huntersHash;
    AVLTree<Squad> squadsTree;

public:
    Set(int elem_id, OBJ value) : head(Node(elem_id, value, nullptr)), size(1) {}
    ~Set() = default;
    Set& find(int elem_id) {
        Node* current = AVL_Find(elem_id); //sussss
        Node* original = current;
        while (current->parent != nullptr) {
            current = current->parent;
        }
        original->parent = current;
        return current;
    }
    Set Union(Set s1, Set s2){
        if (s1->size <= s2->size) {
            s1->head->parent = s2->head;
            //AVL_Delete(s1->head);
            return s2;
        }
        s2->head->parent = s1->head;
        return s1;
        //AVL_Delete(s2->head);
    }
};