#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

bool pathLength(Node* node, int length, int& leafLength){ // need to keep leaf length so it can compare to others

  if (node == nullptr){
      return true;
    }
  if ((node->right == nullptr) &&(node->left == nullptr)){
    if (leafLength == -1){ // only first iteration
      leafLength = length;
    }
  return (length == leafLength);
  }
  return pathLength(node->left, length + 1, leafLength) && pathLength(node->right, length +1, leafLength);
}

bool equalPaths(Node * root)
{
    // Add your code below
    int leafLength = -1;
    return pathLength(root, 0, leafLength);

}

