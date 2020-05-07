#ifndef TREE_HELPER_H
#define TREE_HELPER_H

#include <iostream>
#include <stack>
#include "BNode.h"
#include "BPlusNode.h"
#include "BSTNode.h"
using namespace std;

template<typename  Node, typename  ValueNode, int Flag>
struct TreeHelper{
  typedef Node  node_t;
  typedef typename node_t::value_t value_t;
  
  static void  insert (node_t** head, const value_t& val, unsigned int order){
    cout << "el tipo de dato no es compatible" << endl;
  }

  static void  print (node_t** head, std::ostream &out){
  }
};


template<typename  ValueNode>
struct TreeHelper<BSTNode<ValueNode>,ValueNode,BST_NODE_FLAGXX>{
  typedef BSTNode<ValueNode>  node_t;
  typedef typename node_t::value_t value_t;
  
  static void  insert (node_t** head, const value_t& val, unsigned int order){
    auto temp = new node_t(val);
    
    if ((*head)== nullptr) {
      (*head) = temp;     
      return;
    }
    
    if ((*head)->data > val){
     insert(&(*head)->children[0], val, order);
     return;
    }

    if ((*head)->data < val){
      insert(&(*head)->children[1], val, order);
      return;
    }
    cout << "Duplicates not allowed" << endl;
  }

  //BST Print
  static void  print (node_t** head, std::ostream &out){
    std::stack<node_t*> nodes;
    node_t *curr = *head;
    
    while(curr) {
      out << "Node: " << curr->data << endl;
      if (curr->children[1]) {
        nodes.push(curr->children[1]);
      }
      if (curr->children[0]) {
        curr = curr->children[0];
      } else {
        if (!nodes.empty()) {
          curr = nodes.top(); 
          nodes.pop();
        } else {
          curr = nullptr;
        }
      } 
    }
  }
};

template<typename ValueNode>
struct TreeHelper<BNode<ValueNode>,ValueNode,B_NODE_FLAGXX>{
  typedef BNode<ValueNode>  node_t;
  typedef typename node_t::value_t value_t;
  typedef typename node_t::container_t container_t;

  static int find (const container_t& container, const value_t& val) {
    int i = 0;
    for (auto value : container) {
      if (value < val) 
        i++; 
    }
    return i;
  }
  
  static void  insert (node_t** head, const value_t& val, unsigned int order){
    node_t *curr = *head;
    node_t *next = *head;
        
    if ((*head) == nullptr) {
      *head = new node_t;
      (*head)->insertToContainer(val);
    } else {
      while(next != nullptr) {
        curr = next;
        next = curr->children[find(curr->data, val)];
      }
      
      if (!curr->isContainerFull()) {
        curr->insertToContainer(val);
      } else {
        curr->insertToContainer(val);

        node_t *left = curr->leftChild();
        node_t *right = curr->rightChild();
      
        if (curr->parent == nullptr) {
          node_t *parent = new node_t;
          parent->insertToContainer(curr->data[1]);
          parent->children[0] = left;
          parent->children[1] = right;
          *head = &*parent;
          left->parent = head;
          right->parent = head;
        } else {
          node_t *parent = *(curr->parent);
          parent->children[parent->insertToContainer(curr->data[1]) + 1] = left;
          parent->children[parent->insertToContainer(curr->data[1])] = right;  
        }
      } 
    }
    std::cout << "Insertando para un nodo B" << std::endl;
  }

  static void  print (node_t** head, std::ostream &out){
    
  }
};

#endif