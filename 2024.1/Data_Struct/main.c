#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "bst.h"
#include "calc.h"



int main() {
    char* infix = (char*)malloc(sizeof(char)*20000);
    printf(">>>");
    fgets(infix, 20000, stdin);
    infix[strlen(infix)-1] = '\0';
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    while(infix[0] != '\0') {
        queue = infix_to_postfix(queue, infix);
        Bst_Node* tree;
        if(queue->front != NULL) {
            tree = make_expression_tree(queue);
            if(tree == NULL){
                continue;
            }
            double result = calc_Bst_tree(tree);
            if(result != error_num){
                printf("result: %.2f\n", result);
            }

        }
        queue->front = NULL;
        queue->rear = NULL;
        printf(">>>");
        fgets(infix, 20000, stdin);
        infix[strlen(infix)-1] = '\0';
    }
    free(queue);
    free(infix);
    return 0;
}