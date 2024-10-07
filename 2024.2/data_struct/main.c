/* TODO
 * 현재 각 행의 열의 배열을 초기화 할 때 '0'으로 초기화 하고 있는데 이걸 고쳐야 할 것 같음
 * 그리고 더블 링크드 리스트까지 필요 없을 것 같음, 어차피 파라미터로 head와 현재 커서 위치를 넘겨주기 때문에 리니어 search를 통해 해당 row 노드를 찾아야함
 *
 */


#include <curses.h>
#include <stdio.h>
#define size_of_row 23
#define size_of_cols 100
#define True 1
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

Row* get_new_row(Row* cur_row) {
    // 변수 디스크립션 : row_position -> main에서 받아온 새로운 행을 추가할 이전 행
    Row* pre = cur_row;

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

Row* get_new_char(Row* cur_row, int cols, char x){
    Row* cur = cur_row;
    
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

void del_char(Row* head, int row_position, int cols_position) {
    Row *cur = head;
    // 오류 발생할 가능성 매우 높을 것 같음
    // mian에서 현재 커서 위치를 받아와서 포인터를 옮긴 후 새로운 행을 추가하는 방식임
    for (int i = 0; i < row_position; i++) {
        if (cur->next != NULL)
            cur = cur->next;
    }
    // 행에 문자가 하나 밖에 없는 경우
    if (cur->count_for_cols_len == 1) {
        cur->pre->next = cur->next;
        cur->next->pre = cur->pre;
        free(cur);
    }
        // 행에 문자가 하나 초과인 경우
    else {
        cur->arr[cols_position] = '0';
        cur->count_for_cols_len--;
    }
}

void search(Row* head, char* find){
   int a = 0;
}

void print_win(WINDOW* win, Row* head, int start, int end){
        for(int i=0; i<start; i++){
            if(head->next != NULL)
                head = head->next;
        }

        for(int i=0; i<end-start; i++){
            if(head ->next != NULL){
                mvwprintw(win, i, 0, "%s", head->arr);
                head= head->next;
            }else {
                mvwprintw(win, i, 0, "~");
            }
        }
}

Row* cur_row_update(Row* cur_row, int update_size){
    if(update_size > 0){
        for(int i=0; i<update_size; i++){
            cur_row = cur_row->next;
        }
    } else{
        for(int i=0; i<-update_size; i++){
            cur_row = cur_row->pre;
        }
    }
    return cur_row;
}

int main(int argc, char* argv[]) {
    // 변수 테이블
    Row* head = (Row*)malloc(sizeof(Row)); //모든 row의 최상의 row
    Row* cur_row = NULL; //현재 커서가 위치한 row의 포인터
    int row_location, cols_location; //현재 커서의 row, cols 위치를 확인하는 변수
    int is_changed = 0; //문서의 내용이 바뀌었는지 안 바뀌었는지 확인하는 변수
    int start=0, end=0; // win에 뿌릴 시작과 끝점


    initscr();

    // 새 윈도우 작성
    WINDOW *main_win = newwin(size_of_row, size_of_cols, 0, 0);

    // 윈도우에 텍스트 출력
    mvwprintw(main_win, 10, 35, "Visual Text editor -- version 0.0.1");

    // 반전 색상 켜기
    wattron(main_win, A_REVERSE);
    mvwprintw(main_win, size_of_row - 2, 0, "[No Name] - 0 lines");
    mvwprintw(main_win, size_of_row - 2, size_of_cols - 14, "no ft | 1/0"); // size_of_cols에 맞게 수정
    wattroff(main_win, A_REVERSE); // A_REVERSE 속성 끄기

    // 윈도우에 도움말 출력
    mvwprintw(main_win, size_of_row-1, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");

    // 윈도우의 내용을 실제 화면에 갱신
    wrefresh(main_win);


    while (True)
    {
        end = size_of_row;
        int c = getch();
        print_win(main_win, head, start, end);

        if(c == 17){
            // ctrl+Q : 나가기 ctrl+Q를 두번 누르면 저장되지 않은 상태로 나가기
            if(is_changed) {
                mvprintw(size_of_row + 1, 0, "Press Ctrl+q without saving changes                                                            ");
                refresh();
                int temp_c = getch();
                if(temp_c == 17) {
                    break;
                }
                else {
                    mvprintw(size_of_row + 1, 0, "HELP : Press 'CTRL+Q' to exit | Press 'CTRL+S' to save | Press 'CTRL+F' to find");
                    continue;
                }
            }
            else {
                break;
            }
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
            is_changed = 1;
            addch(c);
            getsyx(row_location, cols_location);
            cur_row = get_new_char(cur_row, cols_location, c);
        }
    }

    endwin();
    return 0;
}
