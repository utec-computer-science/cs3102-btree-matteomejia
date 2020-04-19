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
    }*/

    BTree t(2); //El valor es el grado minimo
    t.insert(1);
    t.insert(2);
    t.insert(3);
    t.insert(4);
    t.insert(5);
    t.insert(6);
    t.insert(7);
    t.insert(8);
    t.insert(9);
    t.insert(10);

    t.remove(7);
    t.remove(8);
    t.remove(9);
    t.remove(4);
    t.traverse();

    return 0;
}
