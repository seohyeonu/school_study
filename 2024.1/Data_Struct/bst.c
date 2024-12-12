#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

Bst_Node* getNewDataNode(double data){
    Bst_Node* temp = (Bst_Node*)malloc(sizeof(Bst_Node));
    temp->data = data;
    temp->operater = '$';
    temp -> right = NULL;
    temp -> left = NULL;
    temp -> is_null = 1;
    return temp;
}
Bst_Node* getNewOpNode(char op){
    Bst_Node* temp = (Bst_Node*)malloc(sizeof(Bst_Node));
    temp->data = -1.0;
    temp->operater = op;
    temp -> right = NULL;
    temp -> left = NULL;
    temp -> is_null = 1;
    return temp;
}

Bst_Node* insertData(Bst_Node* root, double data){
    if(root == NULL){
        return getNewDataNode(data);
    }
    else{
        if(root->left == NULL){
            root->left = insertData(root->left, data);
        }
        else if(root->right == NULL){
            root->right = insertData(root->right, data);
        }
        return root;
    }
}


Bst_Node* insertOp(Bst_Node* root, char op){
    if(root == NULL){
        return getNewOpNode(op);
    }
    else{
        if(root -> left == NULL){
            root->left = insertOp(root->left, op);
        }
        else if(root -> right == NULL){
            root->right = insertOp(root->right, op);
        }
        return root;
    }
}


Bst_Node* make_tree(Bst_Node* left, Bst_Node* root, Bst_Node* right){
    root->left = left;
    root->right = right;
    return root;
}