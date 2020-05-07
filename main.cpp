#include <iostream>
#include <vector>
#include <stack>
#include <time.h>
#include <chrono> 
#include <stdio.h>
#include <unistd.h>

#include "Tree.h"


// CHICOS UNANSE AL ZOOM
// ZOOM: https://utec.zoom.us/j/815711285

int main() {
  typedef BSTNode<float> bst_node;
  typedef BNode<int> b_node;
  typedef Tree<bst_node> bst_tree;
  typedef Tree<b_node> b_tree;

  bst_tree bsttree(4); 
  bsttree.insert(30);
  bsttree.insert(10);
  bsttree.insert(60);

  cout << bsttree << endl;

  b_tree btree(4); 
  btree.insert(30);
  btree.insert(10);
  btree.insert(60);
  
  cout << btree << endl;
}

// https://repl.it/login?goto=/join/znxxkwam-carlosarias3
