#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool getHeight(Node* root, int& height) {
    if (!root) return true;
    
    if (!root->left && !root->right) {
        height = 1;
        return true;
    }
    
    int leftHeight = 0, rightHeight = 0;
    bool leftValid = getHeight(root->left, leftHeight);
    bool rightValid = getHeight(root->right, rightHeight);
    
    if (root->left && root->right && leftHeight != rightHeight) {
        return false;
    }
    
    height = (root->left ? leftHeight : rightHeight) + 1;
    return leftValid && rightValid;
}

bool equalPaths(Node* root) {
    int height = 0;
    return getHeight(root, height);
}

