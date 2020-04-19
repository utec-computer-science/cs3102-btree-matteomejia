//
// Created by matte on 16/11/2018.
//

#ifndef BTREE_NODE_H
#define BTREE_NODE_H

#include <iostream>

class Node {
public:
    int *keys;
    bool isLeaf;
    int min_degree;
    int nkeys;
    Node **childs;
public:
    Node(bool isLeaf, int min_degree) {
        this->min_degree = min_degree;
        this->isLeaf = isLeaf;

        this->keys = new int[2 * min_degree - 1];
        this->childs = new Node *[2 * min_degree];

        this->nkeys = 0;
    }

    ~Node() = default;

    void traverse() {
        int i = 0;

        std::cout<<"keys:";
        for(i = 0; i < nkeys; ++i) {
            std::cout << " " << keys[i];
        }

        if(!isLeaf) {
            for (i = 0; i < nkeys + 1; ++i) {
                std::cout << "\nHijo " << i << ":\n";
                childs[i]->traverse();
            }
        }
    }

    Node *search(int key) {
        int count = 0;
        while(count < nkeys && key > keys[count]) {
            count++;
        }

        if(keys[count] == key) {
            return this;
        }

        if(isLeaf) {
            return nullptr;
        }

        return childs[count]->search(key);
    }


    Node insert(int key){
        int count = 0;
        while(count < nkeys && key > keys[count]) {
            count++;
        }

        if(isLeaf && !is_full()) {
            int temp = 0;
            for(;count<2 * min_degree - 1;count++){
                temp = keys[count];
                keys[count] = key;
                key = temp;
            }
            nkeys++;
        }
        else if(!isLeaf) {
            childs[count]->insert(key);
        }
    }

    bool is_full(){
        return nkeys >= 2 * min_degree - 1;
    }

    void split_child(int pos_node_full){
        Node * node_izq, *node_der;
        node_izq = new Node (childs[pos_node_full]->isLeaf, min_degree);
        node_der = new Node (childs[pos_node_full]->isLeaf, min_degree);
        for(int i = 0; i < min_degree - 1; i++){
            node_izq->keys[i] = childs[pos_node_full]->keys[i];
            node_der->keys[i] = childs[pos_node_full]->keys[i+min_degree];
        }
        if(!childs[pos_node_full]->isLeaf){
            for(int i = 0; i < min_degree; i++){
                node_izq->childs[i] = childs[pos_node_full]->childs[i];
                node_der->childs[i] = childs[pos_node_full]->childs[i+min_degree];
            }
        }

        node_izq->nkeys = min_degree-1;
        node_der->nkeys = min_degree-1;

        if(nkeys>0) {
            for (int i = nkeys - 1; i >= pos_node_full; i--) {
                keys[i + 1] = keys[i];
                childs[i + 2] = childs[i+1];
            }
        }

        keys[pos_node_full] = childs[pos_node_full]->keys[min_degree - 1];
        delete childs[pos_node_full];
        childs[pos_node_full] = node_izq;
        childs[pos_node_full + 1] = node_der;
        nkeys++;
    }

    void remove(int key){

        Node * current = search(key);
        if(current) {
            if (current->isLeaf) {

                int count = 0;
                while (count < current->nkeys && key > current->keys[count]) {
                    count++;
                }

                for(int i = count; i < current->nkeys; i++){
                    if(i != current->nkeys-1)
                        current->keys[i] = current->keys[i+1];
                    else
                        current->keys[i] = 0;
                }

                current->nkeys--;
                review_bro(current,key);
                } else{
                    remove_internal_node(key,current);
                }
            }
        }


    void review_bro(Node * current, int key_d){

        if(current->nkeys<min_degree-1){
            Node * temp = this;
            int count = 0;
            while(!temp->isLeaf) {
                count = 0;
                while (count < temp->nkeys && key_d > temp->keys[count]) {
                    count++;
                }
                if(temp->childs[count] == current)
                    break;
                temp = temp->childs[count];
            }

            //verificamos si uno de los hermanos tiene mas del minimo de keys
            int itemp =0;
            if(count>0 && temp->childs[count-1]->nkeys > min_degree - 1){
                itemp = temp->keys[count-1];
                temp->keys[count-1] = temp->childs[count-1]->keys[temp->childs[count-1]->nkeys-1];
                temp->childs[count-1]->keys[temp->childs[count-1]->nkeys-1] = 0;
                temp->childs[count - 1]->nkeys--;
                for(int j = temp->childs[count]->nkeys-2;j>=0;j--){
                    temp->childs[count]->keys[j+1] = temp->childs[count]->keys[j];
                }
                temp->childs[count]->keys[0] = itemp;
                temp->childs[count]->nkeys++;
            }
            else if(count+1<=temp->nkeys && temp->childs[count+1]->nkeys > min_degree - 1){
                itemp = temp->keys[count];
                temp->keys[count] = temp->childs[count+1]->keys[0];
                for(int j = 0; j<temp->childs[count+1]->nkeys-1;j++){
                    temp->childs[count+1]->keys[j] = temp->childs[count+1]->keys[j+1];
                }
                temp->childs[count+1]->keys[temp->childs[count+1]->nkeys-1] = 0;
                temp->childs[count+1]->nkeys--;

                temp->childs[count]->keys[temp->childs[count]->nkeys] = itemp;
                temp->childs[count]->nkeys++;
            }
            else{
                merge(temp);
            }
        }
    }

    void merge(Node *current){
        int count = 0;
        for(; count <current->nkeys +1;count++){
            if(current->childs[count]->nkeys < min_degree-1)
                break;
        }
        int itemp=0;
        if(count>0 && current->childs[count-1]->nkeys == min_degree - 1){
            itemp = current->keys[count-1];
            current->childs[count-1]->keys[current->childs[count-1]->nkeys] = itemp;
            current->childs[count-1]->nkeys++;
            int ind = current->childs[count-1]->nkeys;
            for(int i = 0; i < current->childs[count]->nkeys; i++){
                current->childs[count-1]->keys[i+ind] =
                        current->childs[count]->keys[i];
                current->childs[count-1]->nkeys++;
            }

            for(int i = count - 1; i <current->nkeys-1;i++){
                current->keys[i] = current->keys[i+1];

            }
            current->keys[current->nkeys-1] = 0;
            for(int i = count; i < current->nkeys; i++){
                current->childs[i] = current->childs[i+1];
            }
            current->nkeys--;
            current->childs[nkeys+1] = nullptr;

        }
        else if(count+1<=current->nkeys){
            if(count == 0) {
                itemp = current->keys[0]; //60
                current->childs[0]->keys[current->childs[0]->nkeys] = itemp;
                current->childs[0]->nkeys++;
                int ind = current->childs[0]->nkeys;
                for(int i = 0; i < current->childs[1]->nkeys; i++){
                    current->childs[0]->keys[i+ind] =
                            current->childs[1]->keys[i];
                    current->childs[0]->nkeys++;
                }

                for(int i = 0; i <current->nkeys-1;i++){
                    current->keys[i] = current->keys[i+1];

                }

                current->keys[current->nkeys-1] = 0;
                for(int i = 1; i < current->nkeys; i++){
                    current->childs[i] = current->childs[i+1];
                }
                current->nkeys--;
                current->childs[nkeys+1] = nullptr;

            }
        }

        if(current->nkeys < min_degree - 1) {
            Node *temp = this;
            while (!temp->isLeaf) {
                count = 0;
                while (count < temp->nkeys && current->keys[0] > temp->keys[count]) {
                    count++;
                }
                if (temp->childs[count] == current)
                    break;
                temp = temp->childs[count];
            }
            merge(temp);
        }
    }

    void remove_internal_node(int key, Node * key_node){
        int count = 0;
        for(; count < key_node->nkeys;count++){
            if(key_node->keys[count] == key)
                break;
        }

        if(key_node->childs[count]->nkeys > min_degree - 1){
            key_node->keys[count] = key_node->childs[count]->keys[key_node->childs[count]->nkeys-1];
            key_node->childs[count]->keys[key_node->childs[count]->nkeys-1] = 0;
            key_node->childs[count]->nkeys--;
        }
        else if (key_node->childs[count+1]->nkeys > min_degree - 1){
            key_node->keys[count] = key_node->childs[count+1]->keys[0];
            for(int i = 0; i < key_node->childs[count+1]->nkeys-1; i++){
                key_node->childs[count+1]->keys[i] = key_node->childs[count+1]->keys[i+1];
            }
            key_node->childs[count]->keys[key_node->childs[count]->nkeys-1] = 0;
            key_node->childs[count]->nkeys--;
        }
        else {
            for(int i = 0; i < min_degree - 1; i++){
                key_node->childs[count]->keys[min_degree-1+i] = key_node->childs[count+1]->keys[i];
                key_node->childs[count]->nkeys++;
            }
            for (int j = count; j < key_node->nkeys - 1; j++) {
                key_node->keys[j] = key_node->keys[j+1];
                key_node->childs[j+1] = key_node->childs[j+2];
            }
            key_node->keys[key_node->nkeys - 1] = 0;
            key_node->nkeys--;
            key_node->childs[key_node->nkeys] = nullptr;
        }
    }

};

#endif //BTREE_NODE_H
