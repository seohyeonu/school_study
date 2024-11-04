/* TODO
 * 현재 각 행의 열의 배열을 초기화 할 때 '0'으로 초기화 하고 있는데 이걸 고쳐야 할 것 같음
 * 그리고 더블 링크드 리스트까지 필요 없을 것 같음, 어차피 파라미터로 head와 현재 커서 위치를 넘겨주기 때문에 리니어 search를 통해 해당 row 노드를 찾아야함
 *
 */

#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define buffer_size 1000
#define True 1
#define False 0


struct row{
    int buffer_up;
    int count_for_cols;
    char* arr;
};
typedef struct row Pad;


Pad* get_new_char(Pad* cur_row, char x, int idx){
    Pad* cur = cur_row;

    cur->arr[idx] = x;
    return cur;
}


int main(int argc, char* argv[]) {
    // 변수 테이블
    Pad *head = (Pad *) malloc(sizeof(Pad)); //모든 row의 최상의 row
    head->arr = (char *) malloc(sizeof(char) * buffer_size);
    for (int i = 0; i < buffer_size; i++) {
        head->arr[i] = '\0';
    }

    int idx=0;
    while (True){
        int c = getch();
        if(c == '9') break;
        get_new_char(head, c, idx);
        idx++;
    }

    int i=0;
    while (True){
        if(head->arr[i] == '\0') break;
        printf("%c", head->arr[i]);
        i++;
    }
}

