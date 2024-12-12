#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>
struct Bst_node{
    double data;
    char operater;
    struct Bst_Node *right;
    struct Bst_Node *left;
    int is_null;
};
typedef struct Bst_node Bst_Node;

Bst_Node* getNewDataNode(double data);

Bst_Node* getNewOpNode(char op);

Bst_Node* insertData(Bst_Node* root, double data);

Bst_Node* insertOp(Bst_Node* root, char op);

Bst_Node* make_tree(Bst_Node* left, Bst_Node* root, Bst_Node* right);

#endif
