//
// Created by matte on 16/11/2018.
//

#ifndef BTREE_BTREE_H
#define BTREE_BTREE_H

#include "node.h"
#include <iostream>

class BTree {
public:
    Node *root;
    int min_degree;
public:
    explicit BTree(int min_degree) {
        this->root = nullptr;
        this->min_degree = min_degree;
    }

    void traverse() {
        if(root != nullptr) {
            root->traverse();
        }
    }

    Node *search(int key) {
        if(root == nullptr) {
            return nullptr;
        }else {
            root->search(key);
        }
    }

    void insert(int key) {
        Node *node = root;

        if(root == nullptr) {
            root = new Node(true, min_degree);
            root->keys[0] = key;
            root->nkeys = 1;
        }
        else{

            int count;
            Node * temp = root;
            bool complete = root->is_full();
            Node * padre_split = nullptr;
            while(!temp->isLeaf && !temp->childs[0]->isLeaf) {
                count = 0;
                while (count < temp->nkeys && key > temp->keys[count]) {
                    count++;
                }
                if(!temp->is_full() && temp->childs[count]->is_full()){
                    padre_split = temp;
                }
                if (!temp->isLeaf && temp->childs[count]->is_full()) {
                    complete = true;
                }
                else if(!temp->isLeaf && !temp->childs[count]->is_full()){
                    complete = false;
                }
                temp = temp->childs[count];
            }

            if(!temp->isLeaf) {
                count = 0;

                while (count < temp->nkeys && key > temp->keys[count]) {
                    count++;
                }
                if (!temp->is_full() && temp->childs[count]->is_full()) {
                    padre_split = temp;
                }
                if (!temp->isLeaf && temp->childs[count]->is_full()) {
                    complete = true;
                } else if (!temp->isLeaf && !temp->childs[count]->is_full()) {
                    complete = false;
                }
            }

            if(complete) {
                temp = padre_split;
                if(padre_split == nullptr){
                    Node * new_root = new Node (false, min_degree);
                    new_root->childs[0] = root;
                    temp = new_root;
                    root = new_root;
                }
                while (!temp->childs[0]->isLeaf) {
                    count = 0;
                    while (count < temp->nkeys && key > temp->keys[count]) {
                        count++;
                    }
                    temp->split_child(count);
                    if(key>temp->keys[count])
                        count++;
                    temp = temp->childs[count];
                }
                count = 0;
                while (count < temp->nkeys && key > temp->keys[count]) {
                    count++;
                }
                temp->split_child(count);
            }

            root->insert(key);
        }

    }

    void remove(int key){
        if(root){
            root->remove(key);
        }
    }

};

#endif //BTREE_BTREE_H


/*
 //
// Created by matte on 16/11/2018.
//

// childs++ program for B-Tree insertion

#include<iostream>
#include "BTree.h"

using namespace std;

int main() {/*
    BTree t(3); //El valor es el grado minimo
    t.insert(10);
    t.insert(20);
    t.insert(5);
    t.insert(6);
    t.insert(12);
    t.insert(30);
    t.insert(7);
    t.insert(17);

    cout << "Traversal of the constucted tree is ";
    t.traverse();

    int k = 6;
    if (t.search(k) != nullptr){
        cout << "\nPresent";
     }else {
        cout << "\nNot Present";
    }

    k = 15;
    if (t.search(k) != nullptr){
        cout << "\nPresent";
    }else {
        cout << "\nNot Present";
    }

BTree t(2); //El valor es el grado minimo
t.insert(10);
t.insert(20);
t.insert(30);
t.insert(40);
t.insert(50);
t.insert(60);
t.insert(70);
t.insert(80);
t.insert(90);
t.insert(100);

t.traverse();

return 0;
}*/
