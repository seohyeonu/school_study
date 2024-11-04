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
//#define 1000 1000
#define True 1
#define False 0


struct row{
    int buffer_up;
    int count_for_cols;
    char* arr;
};
typedef struct row Pad;

Pad* get_new_buffer(Pad* cur_row) {
    Pad* cur = cur_row;
    cur->arr = realloc(cur->arr, sizeof(char) * cur->buffer_up+1);
    return cur;
}

int find_idx(Pad* cur_row, int row_position, int cols_position){
    Pad* cur = cur_row;
    int new_line_count =0, idx =0;

    for(int i=0; i<cur->count_for_cols; i++){
        if(cur->arr[i] == '\n'){
            new_line_count++;
        } else if(cur->arr[i+1] == '\0'){
            idx = i;
            break;
        }
        if(new_line_count == row_position){
            idx = i+cols_position;
        }
    }
    return idx;
}

Pad* get_new_char(Pad* cur_row, int row_position, int cols_position, char x){
    Pad* cur = cur_row;
    int idx = find_idx(cur_row, row_position, cols_position);

    if(cur->count_for_cols == 1000 * (cur->buffer_up+1)){
        cur->arr = (char *) realloc(cur->arr, sizeof(char) * 1000 * (cur->buffer_up+2));
        if(idx == cur->count_for_cols+1){
            cur->arr[idx] = x;
            cur->count_for_cols++;
        } else if(idx < cur->count_for_cols){
            memmove(cur->arr + idx + 1, cur->arr + idx, sizeof(char) * (cur->count_for_cols - idx + 1));
            cur->arr[idx] = x;
            cur->count_for_cols++;
        }

    } else if(cur->count_for_cols < 1000 * (cur->buffer_up+1)){
        if(idx == cur->count_for_cols){
            cur->arr[idx] = x;
            cur->count_for_cols++;
        } else if(idx < cur->count_for_cols){
            memmove(cur->arr + idx + 1, cur->arr + idx, sizeof(char) * (cur->count_for_cols - idx + 1));
            cur->arr[idx] = x;
            cur->count_for_cols++;
        }
    }
    return cur;
}


void del_char(Pad* cur_row,  int row_position, int cols_position) {
    Pad *cur = cur_row;
    int idx = find_idx(cur, row_position, cols_position);

    memmove(cur->arr + idx, cur->arr + idx+1, sizeof(char) * (cur->count_for_cols - idx+1));
    cur->count_for_cols--;
}


void print_win(WINDOW* win, Pad* head, int start, int end) {
    //int start_line_idx = find_idx(head, start, 0);

    mvwprintw(win, 0,0,"%s", head->arr);
    wrefresh(win);  // 반복문 밖에서 한 번만 호출
}



//Pad* cur_row_update(Pad* cur_row, int update_size){
//    if(update_size > 0){
//        for(int i=0; i<update_size; i++){
//            cur_row = cur_row->next;
//        }
//    } else{
//        for(int i=0; i<-update_size; i++){
//            cur_row = cur_row->pre;
//        }
//    }
//    return cur_row;
//}

int main(int argc, char* argv[]) {
    // 변수 테이블
    Pad* head = (Pad*)malloc(sizeof(Pad)); //모든 row의 최상의 row
    head->arr = (char*)malloc(sizeof(char)*1000);
    //for (int i =0; i<1000; i++){
    //    head->arr[i] = (char)i;
    //}
    //head->arr = "hello world";
    int row_location, cols_location; //현재 커서의 row, cols 위치를 확인하는 변수
    int is_changed = 0; //문서의 내용이 바뀌었는지 안 바뀌었는지 확인하는 변수
    int start=0, end=0; // win에 뿌릴 시작과 끝점
    int size_of_row, size_of_cols; // window 사이즈 받아오기

    initscr();
    noecho();
    raw();
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, size_of_row, size_of_cols);
    refresh();

    // 새 윈도우 작성
    WINDOW *main_win = newwin(size_of_row, size_of_cols, 0, 0);

    // 윈도우에 텍스트 출력
    mvwprintw(main_win, size_of_row/2, size_of_cols/2-22, "Visual Text editor -- version 0.0.1");

    // 반전 색상 켜기
    wattron(main_win, A_REVERSE);
    mvwprintw(main_win, size_of_row - 2, 0, "[No Name] - 0 lines");
    mvwprintw(main_win, size_of_row - 2, size_of_cols - 11, "no ft | 1/0"); // size_of_cols에 맞게 수정
    wattroff(main_win, A_REVERSE); // A_REVERSE 속성 끄기

    // 윈도우에 도움말 출력
    mvwprintw(main_win, size_of_row-1, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");

    // 윈도우의 내용을 실제 화면에 갱신
    wrefresh(main_win);

    while (True)
    {
        end = size_of_row;
        print_win(main_win, head, start, end);

        wrefresh(main_win);
        int c = getch();
        //int c = 'h';
        if(c == 17){
            // ctrl+Q : 나가기 ctrl+Q를 두번 누르면 저장되지 않은 상태로 나가기
            // 2024.11.2 맥에서 동작하지 않은 윈도우는 확인 못함
            if(is_changed) {
                mvprintw(size_of_row -10, 0, "Press Ctrl+q without saving changes                                                            ");
                refresh();
                int temp_c = getch();
                if(temp_c == 17) {
                    break;
                }
                else {
                    mvprintw(size_of_row -10 , 0, "HELP : Press 'CTRL+Q' to exit | Press 'CTRL+S' to save | Press 'CTRL+F' to find");
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
            getsyx(row_location, cols_location);
            move(row_location-1, cols_location);
        }

        else if(c == KEY_DOWN){
            getsyx(row_location, cols_location);
            move(row_location+1, cols_location);
        }

        else if(c == KEY_RIGHT){
            getsyx(row_location, cols_location);
            move(row_location, cols_location+1);
        }

        else if(c == KEY_LEFT){
            getsyx(row_location, cols_location);
            move(row_location, cols_location-1);
        }

        else if(c == KEY_BACKSPACE || c == 127 || c == '\b'){
            is_changed = 1;
            getsyx(row_location, cols_location);
            del_char(head, row_location, cols_location);

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
                || c == '[' || c == ']' || c == '{' || c == '}' || c == ':' || c == ';' || c == '"' || c == '\'' || c == '\n'){
            //문자 입력 일 때
            is_changed = 1;
            getsyx(row_location, cols_location);
            head = get_new_char(head, row_location, cols_location, c);
        }
    }

    endwin();
    return 0;
}