#ifndef BNODE_H
#define BNODE_H

#include <iostream>
#include <vector>
#include "Macros.h"
using namespace std;

template<typename T>
class BNode {
public: 
  typedef T value_t ;
  typedef vector<value_t> container_t;
  typedef vector<BNode*> pcontainer_t;

  unsigned int order;
  container_t data;
  pcontainer_t children;
  size_t sizeOfContainer;
  BNode **parent;

  BNode( unsigned int order = 4):order(order){
    //children = new BNode*[B_NODE_ORDER];
    children.resize(order, nullptr);
    data.resize(order, 0);
    sizeOfContainer = 0;
    parent = nullptr;
  }
  
  int insertToContainer(value_t value){
    int index = sizeOfContainer - 1;

    while(index >= 0 && data[index] > value){
      data[++index] = data[index];
      index--;
    }

    data[++index] = value;
    sizeOfContainer++;

    return index;
  }

  bool isContainerFull(){
    return sizeOfContainer == order;
  }

  BNode *leftChild() {
    BNode *node = new BNode();
    node->insertToContainer(this->data[0]);
    if (this->children[0] != nullptr) {
      node->children[0] = this->children[0];
    }
    if (this->children[1] != nullptr) {
      node->children[1] = this->children[1];
    }
    return node;
  }

  BNode *rightChild() {
    BNode *node = new BNode();
    node->insertToContainer(this->data[2]);
    node->insertToContainer(this->data[3]);
    if (this->children[2] != nullptr) {
      node->children[0] = this->children[2];
    }
    if (this->children[3] != nullptr) {
      node->children[1] = this->children[3];
    }
    return node;
  }

  BNode* getNextChild(value_t value){
    for(int i = 0; i < sizeOfContainer; i++){
      if (value <= data[i]) {
        return children[i];
      }
    }
    return children[sizeOfContainer];
  }

  ~BNode(void){}
};


#endif