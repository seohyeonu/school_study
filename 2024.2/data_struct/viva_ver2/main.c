/* TODO
 * 현재 각 행의 열의 배열을 초기화 할 때 '0'으로 초기화 하고 있는데 이걸 고쳐야 할 것 같음
 * 그리고 더블 링크드 리스트까지 필요 없을 것 같음, 어차피 파라미터로 head와 현재 커서 위치를 넘겨주기 때문에 리니어 search를 통해 해당 row 노드를 찾아야함
 *
 */

#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define BUFFER_SIZE 1000
#define True 1
#define False 0


struct row{
    int buffer_up;
    int count_for_cols;
    char* arr;
};
typedef struct row Pad;





void log_message(FILE *log_file, const char* level, const int message) {
    time_t now;
    time(&now);
    fprintf(log_file, "[%s] %s: %d\n", ctime(&now), level, message);
}

void log_row_cols_message(FILE *log_file, const char* level, const int row, const int cols) {
    time_t now;
    time(&now);
    fprintf(log_file, "[%s] %s:  row : %d ,  cols : %d\n", ctime(&now), level, row, cols);
}




Pad* get_new_buffer(Pad* cur_row) {
    Pad* cur = cur_row;
    cur->arr = realloc(cur->arr, sizeof(char) * cur->buffer_up+1);
    return cur;
}

int find_idx(Pad* cur_row, int row_position, int cols_position){
    Pad* cur = cur_row;
    int new_line_count =0, idx =0;

//    if(row_position == 0){
//        return cols_position-1;
//    } else{
//        for(int i=0; i<cur->count_for_cols;i++){
//
//        }
//    }

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

Pad* get_new_char(Pad* cur_row, int row_position, int cols_position, char x, FILE *log_file){
    Pad* cur = cur_row;
    int idx = find_idx(cur_row, row_position, cols_position);
    log_message(log_file, "get_new_char idx 위치임", idx);

    if(cur->count_for_cols == BUFFER_SIZE * (cur->buffer_up+1)){
        cur->arr = (char *) realloc(cur->arr, sizeof(char) * BUFFER_SIZE * (cur->buffer_up+2));
        if(idx == cur->count_for_cols+1){
            cur->arr[idx] = x;
            cur->count_for_cols++;
        } else if(idx < cur->count_for_cols){
            memmove(cur->arr + idx + 1, cur->arr + idx, sizeof(char) * (cur->count_for_cols - idx + 1));
            cur->arr[idx] = x;
            cur->count_for_cols++;
        }

    } else if(cur->count_for_cols < BUFFER_SIZE * (cur->buffer_up+1)){
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

void save_file(FILE *log_file, Pad* head){
    fprintf(log_file, "%s\n", head->arr);
}

Pad* open_file()

int main(int argc, char* argv[]) {
    FILE *log_file = fopen("log.txt", "a");
    FILE *save_file_path = fopen("save.txt", "a");
    // 변수 테이블
    Pad* head = (Pad*)malloc(sizeof(Pad)); //모든 row의 최상의 row
    head->arr = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    head->count_for_cols++;
    int row_location, cols_location; //현재 커서의 row, cols 위치를 확인하는 변수
    int is_changed = 0; //문서의 내용이 바뀌었는지 안 바뀌었는지 확인하는 변수
    int start=0, end=0; // win에 뿌릴 시작과 끝점
    int size_of_row, size_of_cols; // window 사이즈 받아오기

    // curses 색상 및 기타 설정
    initscr();
    noecho();
    raw();
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    getmaxyx(stdscr, size_of_row, size_of_cols);
    refresh();

    // 새 윈도우 작성
    WINDOW *main_win = newwin(size_of_row-2, size_of_cols, 0, 0);
    WINDOW *messenger_bar = newwin(1, size_of_cols, size_of_row-1,0);
    WINDOW *status_bar = newwin(1, size_of_cols, size_of_row-2,0);

    // 윈도우에 텍스트 출력
    mvwprintw(main_win, size_of_row/2, size_of_cols/2-22, "Visual Text editor -- version 0.0.1");

    // 반전 색상 켜기
    wbkgd(status_bar, COLOR_PAIR(1) | A_REVERSE);
    mvwprintw(status_bar, 0, 0, "[No Name] - 0 lines");
    mvwprintw(status_bar, 0, size_of_cols - 11, "no ft | 1/0"); // size_of_cols에 맞게 수정

    // 스크롤 기능 활성화
    scrollok(main_win, TRUE);

    // 윈도우에 도움말 출력
    mvwprintw(messenger_bar, 0, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");

    // 윈도우의 내용을 실제 화면에 갱신
    wrefresh(main_win);
    wrefresh(messenger_bar);
    wrefresh(status_bar);

    while (True)
    {
        end = size_of_row;
        print_win(main_win, head, start, end);

        int c = getch();
        werase(main_win);
        if(c == 17){
            // ctrl+Q : 나가기 ctrl+Q를 두번 누르면 저장되지 않은 상태로 나가기
            // 2024.11.2 맥에서 동작하지 않은 윈도우는 확인 못함   ->  raw(); keypad(stdscr, TRUE); 이 두 친구들을 추가해줬어야 함.
            if(is_changed) {
                mvprintw(size_of_row -1, 0, "Press Ctrl+q without saving changes                                                            ");
                refresh();
                int temp_c = getch();
                if(temp_c == 17) {
                    break;
                }
                else {
                    mvprintw(size_of_row-1, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");
                    refresh();
                    continue;
                }
            }
            else {
                break;
            }

        }
        else if(c == 19){
            // ctrl+S 새로운 파일을 저장하려고 하는 경우에는 파일 이름을 입력해야한다.
            save_file(save_file_path, head);
            break;
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
            log_row_cols_message(log_file, "main", row_location, cols_location);
            log_message(log_file, "배열의 어디까지 채워졌니??", head->count_for_cols);
            head = get_new_char(head, row_location, cols_location, c, log_file);
        }
    }

    endwin();
    return 0;
}