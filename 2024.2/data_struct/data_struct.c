#include <stdio.h>
#include <stdlib.h>
#define max_len 500

struct row{
    int count_for_cols_len;
    struct row *next;
    struct row *pre;
    char arr[max_len];
};
typedef struct row Row;

Row* get_new_row(Row* pre) {
    // 마지막 행에 새로운 행을 추가하는 경우
    if (pre->next == NULL) {
        Row *temp = (Row *) malloc(sizeof(Row));
        pre->next = temp;
        temp->count_for_cols_len = 0;
        temp->next = NULL;
        temp->pre = pre;
        for (int i = 0; i < max_len; i++) {
            temp->arr[i] = '0';
        }
        return temp;
    }
    // 행 중간에 새로운 행을 추가하는 경우
    else {
        Row *temp = (Row *) malloc(sizeof(Row));
        temp->count_for_cols_len = 0;
        temp->next = pre->next;
        temp->pre = pre;
        pre->next->pre = temp;
        pre->next = temp;
        for (int i = 0; i < max_len; i++) {
            temp->arr[i] = '0';
        }
        return temp;
    }
}

Row* get_new_char(Row* cur, int cols, char x){
    if(cur->count_for_cols_len+1 == max_len){
        Row *temp = get_new_row(cur);
        temp->arr[0] = x;
        temp->count_for_cols_len++;
        return temp;
    } else{
        cur->arr[cols] = x;
        cur->count_for_cols_len++;
        return cur;
    }
}

void del_char(Row* cur, int cols){
    // 행에 문자가 하나 밖에 없는 경우
    if(cur->count_for_cols_len ==1){
        cur->pre->next = cur->next;
        cur->next->pre = cur->pre;
        free(cur);
    }
    // 행에 문자가 하나 초과인 경우
    else{
        cur->arr[cols] = '0';
        cur->count_for_cols_len--;
    }
}

void search(Row* head){
    pass;
}