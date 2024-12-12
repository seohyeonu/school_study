#ifndef CALC_H
#define CALC_H

#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define error_num (-4.9406564584124654e-324)

struct QueueNode_ {
    char data;
    struct QueueNode_* next;
};

typedef struct QueueNode_ QueueNode;

struct Queue_ {
    QueueNode* front;
    QueueNode* rear;
};

typedef struct Queue_ Queue;

struct Bst_Stack_ {
    Bst_Node* node;
    struct Bst_Stack_* next;
};

typedef struct Bst_Stack_ Bst_Stack;

struct StackNode_ {
    char data;
    struct StackNode_ * link;
};

struct Stack {
    struct StackNode_ * top;
};


struct IntStack {
    struct IntStackNode_ * top;
};

typedef struct IntStack IntStack;
typedef struct StackNode_ StackNode;
typedef struct Stack Stack;

Stack* push(Stack* stack, char x);
Stack* pop(Stack* stack);
char Top(Stack* stack);
int isEmpty_stack(Stack* stack);

int isEmpty_int_stack(IntStack* stack);

QueueNode* getNewNode(char x);
Queue* enqueue(Queue* queue, char x);
char dequeue(Queue* queue);
int isEmpty_queue(Queue* queue);

Queue* infix_to_postfix(Queue* queue, char* infix);
Bst_Node* make_expression_tree(Queue* queue);

Bst_Stack* node_push(Bst_Node* temp, Bst_Stack* stack);
Bst_Stack* node_pop(Bst_Stack* stack);
Bst_Node* node_top(Bst_Stack* stack);

double calc_Bst_tree(Bst_Node* root);

#endif
