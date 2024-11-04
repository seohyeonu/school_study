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
    struct row *next;
    struct row *pre;
    char* arr;
};
typedef struct row Row;

Row* get_new_row(Row* cur_row) {
    // 변수 디스크립션 : row_position -> main에서 받아온 새로운 행을 추가할 이전 행
    Row* pre = cur_row;

    // 마지막 행에 새로운 행을 추가하는 경우
    if (pre->next == NULL) {
        Row *temp = (Row *) malloc(sizeof(Row));
        pre->next = temp;
        temp->buffer_up = 0;
        temp->count_for_cols = 0;
        temp->next = NULL;
        temp->pre = pre;
        temp -> arr = (char*)malloc(sizeof(char) * buffer_size);
        for (int i = 0; i < buffer_size; i++) {
            temp->arr[i] = '\0';
        }
        return temp;
    }
    // 행 중간에 새로운 행을 추가하는 경우
    else {
        Row *temp = (Row *) malloc(sizeof(Row));
        temp->buffer_up = 0;
        temp->next = pre->next;
        temp->pre = pre;
        pre->next->pre = temp;
        pre->next = temp;
        temp->count_for_cols = 0;
        temp -> arr = (char*)malloc(sizeof(char) * buffer_size);
        for (int i = 0; i < buffer_size; i++) {
            temp->arr[i] = '\0';
        }
        return temp;
    }
}

Row* get_new_char(Row* cur_row,  char x){
    Row* cur = cur_row;

    // buffer가 다 채워진 경우
    if(cur->count_for_cols == buffer_size * cur->buffer_up+1){
        cur->arr = realloc(cur->arr, sizeof(char) * cur->buffer_up+2)
    }
}


int main(int argc, char* argv[]) {
    // 변수 테이블
    Row* head = (Row*)malloc(sizeof(Row)); //모든 row의 최상의 row
    Row* cur_row = NULL; //현재 커서가 위치한 row의 포인터
    head = get_new_row(cur_row);

    while (True)
    {
        printf("새로운 문자를 입력하세요");
        int c = getch();

        if(c == 17){
            continue;
        }
        else if(c == 19){
            // ctrl+S 새로운 파일을 저장하려고 하는 경우에는 파일 이름을 입력해야한다.
            continue;
        }

        else if(c == 6){
            //ctrl+F

            continue;
        }

        else if(c == KEY_UP){
           cur_row =  cur_row_update(cur_row, -1);
        }

        else if(c == KEY_DOWN){
            cur_row =  cur_row_update(cur_row, 1);
        }

        else if(c == KEY_RIGHT){
            continue;
        }

        else if(c == KEY_LEFT){
            continue;
        }

        else if(c == KEY_BACKSPACE){
            is_changed = 1;
            getsyx(row_location, cols_location);
            //cur_row = del_char(head, row_location, cols_location);

        }

        else if(c == KEY_ENTER){
            is_changed = 1;
            getsyx(row_location, cols_location);
            cur_row = get_new_row(cur_row);
        }

        else if(c == KEY_HOME){
            continue;
        }

        else if(c == KEY_END){
            continue;
        }

        else if(c == KEY_NPAGE){
            continue;
        }

        else if(c == KEY_PPAGE){
            continue;
        }

        else if((c >= 'a' && c <= 'z') || (c <= 'Z' && c >= 'A') || (c >= '0' && c <= '9') || c == ' ' || c == '.' || c == ',' ||
                c == '?' || c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&' || c == '*' || c == '('
                || c == ')' || c == '-' || c == '_' || c == '+' || c == '=' || c == '<' || c == '>' || c == '/' || c == '|' || c == '\\'
                || c == '[' || c == ']' || c == '{' || c == '}' || c == ':' || c == ';' || c == '"' || c == '\''){
            //문자 입력 일 때
            cur_row = get_new_char(cur_row, c);
        }
    }

    while (True){
        for(int i=0; i<buffer_size * head->buffer_up+1; i++){
            if(head->arr[i] == '\0') break;
            printf("%c", head->arr[i]);
        }
        if(head->next == NULL) break;
        head = head->next;
    }
    return 0;
}
