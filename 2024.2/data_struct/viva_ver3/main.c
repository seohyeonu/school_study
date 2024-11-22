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
    int new_line;
};
typedef struct row Pad;

// KMP 알고리즘: 실패 함수 생성
void computeLPSArray(const char* pattern, int M, int* lps) {
    int length = 0;
    int i = 1;
    lps[0] = 0;

    while (i < M) {
        if (pattern[i] == pattern[length]) {
            length++;
            lps[i] = length;
            i++;
        } else {
            if (length != 0) {
                length = lps[length - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// KMP 알고리즘: 문자열 검색
int KMPSearch(const char* pattern, const char* text, int* results, int max_results) {
    int M = strlen(pattern);
    int N = strlen(text);
    int lps[M];
    int j = 0; // 패턴의 인덱스
    int k = 0; // 결과 배열 인덱스

    computeLPSArray(pattern, M, lps);

    int i = 0; // 텍스트의 인덱스
    while (i < N) {
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }
        if (j == M) {
            if (k < max_results) {
                results[k++] = i - j; // 일치하는 시작 인덱스를 저장
            }
            j = lps[j - 1];
        } else if (i < N && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    return k; // 찾은 패턴의 개수 반환
}


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

int find_idx(Pad* cur_row, int row_position, int cols_position) {
    Pad* cur = cur_row;
    int new_line_count = 0;
    int idx = 0;

    for (int i = 0; i < cur->count_for_cols; i++) {
        // 현재 위치가 목표하는 행(row_position)에 도달하면
        if (new_line_count == row_position) {
            // 현재 위치에서 cols_position만큼 이동한 인덱스를 반환
            if (i + cols_position < cur->count_for_cols) {
                idx = i + cols_position;
            } else {
                // 열이 범위를 벗어나면 가능한 최대 인덱스로 설정
                idx = cur->count_for_cols - 1;
            }
            return idx;
        }

        // '\n'을 만나면 새로운 행 시작을 의미하므로 new_line_count 증가
        if (cur->arr[i] == '\n') {
            new_line_count++;
        }
    }

    // 만약 row_position에 해당하는 행이 존재하지 않는다면
    // 배열의 끝을 반환 (이후 처리가 가능하도록 -1을 반환할 수도 있음)
    return cur->count_for_cols;
}


Pad* get_new_char(Pad* cur_row, int row_position, int cols_position, char x, FILE *log_file){
    Pad* cur = cur_row;
    int idx = find_idx(cur_row, row_position, cols_position);
    log_message(log_file, "get_new_char idx 위치임", idx);

    // 배열이 다 찼을 경우 배열의 크기를 2배로 증가
    if (cur->count_for_cols == BUFFER_SIZE * (cur->buffer_up + 1)) {
        cur->buffer_up++; // 2배 확장을 기록
        cur->arr = (char *) realloc(cur->arr, sizeof(char) * BUFFER_SIZE * (1 << cur->buffer_up));

        // realloc 실패 체크
        if (!cur->arr) {
            fprintf(stderr, "메모리 할당 오류!\n");
            exit(1); // 프로그램 종료
        }
    }

    // `idx` 위치에 `x` 문자 삽입
    if (idx == cur->count_for_cols) {
        cur->arr[idx-1] = x;
        cur->count_for_cols++;
    } else if (idx < cur->count_for_cols) {
        // `idx` 위치에 문자 삽입을 위해 뒤로 밀기
        memmove(cur->arr + idx , cur->arr + idx-1, sizeof(char) * (cur->count_for_cols - idx+1));
        cur->arr[idx-1] = x;
        cur->count_for_cols++;
    }

    return cur;
}


Pad* load_file_new_char(Pad* cur_row, char x) {
    Pad* cur = cur_row;

    // 배열이 다 찼을 경우 배열의 크기를 2배로 증가
    if (cur->count_for_cols == BUFFER_SIZE * (cur->buffer_up)) {
        cur->buffer_up++;
        size_t new_size = BUFFER_SIZE * (cur->buffer_up);

        char* temp = (char *) realloc(cur->arr, new_size);
        if (!temp) {
            fprintf(stderr, "메모리 재할당 오류! 요청한 크기: %zu bytes\n", new_size);
            exit(1);
        }
        cur->arr = temp;
    }

    // `x` 문자 삽입
    cur->arr[cur->count_for_cols++] = x;
    if(x == '\n'){
        cur->new_line++;
    }
//    printf("%s\n", cur->arr);
    return cur;
}

void del_char(Pad* cur_row,  int row_position, int cols_position) {
    Pad *cur = cur_row;
    int idx = find_idx(cur, row_position, cols_position);

    memmove(cur->arr + idx-1, cur->arr + idx, sizeof(char) * (cur->count_for_cols - idx));
    cur->count_for_cols--;
}

int* get_row_array(Pad* head, int start_idx, int end_idx, int max_col) {
    int one_row_len = 0;
    int row_idx = 0;
    int* result = (int*)malloc(sizeof(int) * 200); // 충분한 크기 확보

    for (int i = start_idx; i < end_idx + max_col; i++) {
        one_row_len++;
        if (head->arr[i] == '\n' || one_row_len == max_col) {
            one_row_len--;
            result[row_idx++] = one_row_len;
            one_row_len = 0;
        }
    }
    return result;
}


void print_win(WINDOW* win, Pad* head, int start, int end) {
    int start_idx = find_idx(head, start,0);
    char str1[10000] = {'\0', };
    int idx = 0;
    for(int i=0; i<end-start; i++){
        int str_idx = 0;
        while(head->arr[start_idx + idx] != '\n' && head->arr[start_idx + idx] != '\0') {
            str1[str_idx++] = head->arr[start_idx + idx++];
            str1[str_idx] = '\0';

        }
        str1[str_idx] ='\n';
//        fprintf(stderr,"%d\n", i);
        mvwprintw(win, i, 0, "%s", str1);
        idx ++;
        str1[0] = '\0';
    }
}




void save_file(FILE *save_file, Pad* head){
    fprintf(save_file, "%s\n", head->arr);
}



Pad* load_file(Pad* head, const char* filename, int size_of_cols) {
    FILE *fp = fopen(filename, "r");

    if (fp != NULL) {
        char buffer[1024] = {0,};
        int count = 0;
        while (feof(fp) == 0) {
            count = fread(buffer, 1, sizeof(buffer), fp);
            if (count > 0) {
                for (int i = 0; i < strlen(buffer); i++) {
                    head = load_file_new_char(head, buffer[i]);
                }
                memset(buffer, 0, sizeof(buffer));
            }
        }

        fclose(fp);
    }
    return head;
}

int main(int argc, char* argv[]) {
    FILE *log_file = fopen("log.txt", "a");


// 변수 테이블
    Pad* head = (Pad*)malloc(sizeof(Pad)); //모든 row의 최상의 row
    head->arr = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    head->buffer_up = 1;
    head->count_for_cols = 0;
    head->new_line = 0;


    int row_location, cols_location; //현재 커서의 row, cols 위치를 확인하는 변수
    int is_changed = 0; //문서의 내용이 바뀌었는지 안 바뀌었는지 확인하는 변수
    int start=0, end=0; // win에 뿌릴 시작과 끝점
    int size_of_row, size_of_cols; // window 사이즈 받아오기



    // curses 색상 및 기타 설정
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    initscr();


    getmaxyx(stdscr, size_of_row, size_of_cols);
    if (argc >= 2) {
        head = load_file(head, argv[1], size_of_cols);
    }
    refresh();



    // 새 윈도우 작성
    WINDOW *main_win = newwin(size_of_row-2, size_of_cols, 0, 0);
    WINDOW *messenger_bar = newwin(1, size_of_cols, size_of_row-1,0);
    WINDOW *status_bar = newwin(1, size_of_cols, size_of_row-2,0);

    noecho();
    raw();
    keypad(main_win, TRUE);


    // 윈도우에 텍스트 출력
    mvwprintw(main_win, size_of_row/2, size_of_cols/2-22, "Visual Text editor -- version 0.0.1");

    // 반전 색상 켜기
    wbkgd(status_bar, COLOR_PAIR(1) | A_REVERSE);
    mvwprintw(status_bar, 0, 0, "[No Name] - 0 lines");
    mvwprintw(status_bar, 0, size_of_cols - 11, "no ft | 1/0"); // size_of_cols에 맞게 수정

    // 윈도우에 도움말 출력
    mvwprintw(messenger_bar, 0, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");

    // 윈도우의 내용을 실제 화면에 갱신
    wrefresh(main_win);
    wrefresh(messenger_bar);
    wrefresh(status_bar);

    row_location = 0;
    cols_location = 0;

    while (True)
    {

        wclear(main_win);

        print_win(main_win, head, start, start + size_of_row - 2);
        wmove(main_win, row_location, cols_location);
        wrefresh(main_win);
        int start_idx = find_idx(head,start,0);
        int end_idx = find_idx(head, start+size_of_row-2, 0);
        int c = wgetch(main_win);
        int* row_array = get_row_array(head, start_idx, end_idx, size_of_cols);



        if(c == 17){
            // ctrl+Q : 나가기 ctrl+Q를 두번 누르면 저장되지 않은 상태로 나가기
            // 2024.11.2 맥에서 동작하지 않은 윈도우는 확인 못함   ->  raw(); keypad(stdscr, TRUE); 이 두 친구들을 추가해줬어야 함.
            if(is_changed) {
                wclear(messenger_bar);
                mvwprintw(messenger_bar, 0, 0, "Press Ctrl+q without saving changes");
                wrefresh(messenger_bar);
                int temp_c = getch();
                if(temp_c == 17) {
                    break;
                }
                else {
                    wclear(messenger_bar);
                    mvwprintw(messenger_bar, 0, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");
                    wrefresh(messenger_bar);
                    continue;
                }
            }
            else {
                break;
            }

        }
        // save_file
        else if(c == 19){
            if(argc >=2){
                const char *file_name = argv[1];
                FILE *save_file_path = fopen(file_name, "w");
                save_file(save_file_path, head);
                wclear(messenger_bar);
                mvwprintw(messenger_bar, 0, 0, "File_is_saved");
                wrefresh(messenger_bar);
                is_changed = 0;
            } else{
                wclear(messenger_bar);
                char file_name[256]; // 파일 이름을 저장할 버퍼
                // 메시지 바에서 파일 이름 입력받기
                mvwprintw(messenger_bar, 0, 0, "Enter file name to save: ");
                wrefresh(messenger_bar);

                echo(); // 입력된 문자를 화면에 표시
                wgetnstr(messenger_bar, file_name, sizeof(file_name) - 1); // 파일 이름 입력 받기
                noecho(); // 입력된 문자 표시 중단
                wclear(messenger_bar);
                wrefresh(messenger_bar);

                FILE *save_file_path = fopen(file_name, "w");
                save_file(save_file_path, head);
                fclose(save_file_path);
                mvwprintw(messenger_bar, 0, 0, "File saved to: %s", file_name);
                wrefresh(messenger_bar);
                is_changed = 0;
            }
        }

        else if (c == 6) { // Ctrl+F
            wclear(messenger_bar);
            char search_pattern[256];
            mvwprintw(messenger_bar, 0, 0, "Enter search pattern: ");
            wrefresh(messenger_bar);

            echo();
            wgetnstr(messenger_bar, search_pattern, 255);
            noecho();
            wclear(messenger_bar);
            mvwprintw(messenger_bar, 0, 0, "Searching...");
            wrefresh(messenger_bar);

            int max_results = 100;
            int results[max_results];
            int found_count = KMPSearch(search_pattern, head->arr, results, max_results);

            if (found_count > 0) {
                mvwprintw(messenger_bar, 0, 0, "Found %d matches.", found_count);
                wrefresh(messenger_bar);

                // 검색 결과 강조 표시
                for (int i = 0; i < found_count; i++) {
                    int match_row = 0, match_col = 0;
                    int match_idx = results[i];

                    // match_idx에서 행과 열 계산
                    for (int j = 0; j < match_idx; j++) {
                        if (head->arr[j] == '\n') {
                            match_row++;
                            match_col = 0;
                        } else {
                            match_col++;
                        }
                    }

                    if (match_row >= start && match_row < start + size_of_row - 2) {
                        wattron(main_win, A_REVERSE);
                        mvwprintw(main_win, match_row - start, match_col, "%.*s", (int)strlen(search_pattern), search_pattern);
                        wattroff(main_win, A_REVERSE);
                    }
                }
                wrefresh(main_win);
            } else {
                mvwprintw(messenger_bar, 0, 0, "No matches found.");
                wrefresh(messenger_bar);
            }
        }


        else if(c == KEY_UP){
//            getsyx(row_location, cols_location);
            if(row_location>0) {
                if(cols_location > row_array[row_location - 1] ) {
                    cols_location = row_array[row_location - 1];

                }
                row_location--;
                wmove(main_win, row_location, cols_location);
                wrefresh(main_win);
            } else {
                start--;
                row_array = get_row_array(head, start_idx, end_idx, size_of_cols);
                if (cols_location > row_array[0]) {
                    cols_location = row_array[0];
                }
            }

        }

        else if(c == KEY_DOWN){
//            getsyx(row_location, cols_location);
            if(row_location<size_of_row-2) {

                if(cols_location > row_array[row_location + 1]) {
                    cols_location = row_array[row_location + 1];
                }
                row_location++;
                wmove(main_win, row_location, cols_location);
                wrefresh(main_win);
            } else {
                start++;
                row_array = get_row_array(head, start_idx, end_idx, size_of_cols);
                if (cols_location > row_array[size_of_row]) {
                    cols_location = row_array[size_of_row];
                }
            }

        }

        else if(c == KEY_RIGHT){
//            getsyx(row_location, cols_location);
            if(cols_location == row_array[row_location]){
                continue;
            }
            cols_location++;
            wmove(main_win, row_location, cols_location);
            log_message(log_file, "KEY_RIGHT", cols_location);
            wrefresh(main_win);
        }

        else if(c == KEY_LEFT){
            if(cols_location == 0){
                continue;
            }
            cols_location--;
            wmove(main_win, row_location, cols_location);
            log_message(log_file, "KEY_LEFT", cols_location);
            wrefresh(main_win);
        }

        else if(c == KEY_BACKSPACE || c == 127 || c == '\b'){
            is_changed = 1;
            //getsyx(row_location, cols_location);

            if(cols_location == 0){
                del_char(head, row_location, cols_location);
                cols_location = row_array[row_location-1];
                row_location--;
            } else{
                del_char(head, row_location, cols_location);
                cols_location--;
            }
        }

        else if(c == KEY_HOME){
            cols_location = 0;
        }

        else if(c == KEY_END){
            row_array = get_row_array(head, start_idx, end_idx, size_of_cols);
            cols_location = row_array[row_location];
        }

        else if(c == KEY_NPAGE){
            start = start + size_of_row;
        }

        else if(c == KEY_PPAGE){
            start = start - size_of_row;
        }

        else if ((c >= 'a' && c <= 'z') || (c <= 'Z' && c >= 'A') || (c >= '0' && c <= '9') || c == ' ' || c == '.' || c == ',' ||
                 c == '?' || c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&' || c == '*' || c == '('
                 || c == ')' || c == '-' || c == '_' || c == '+' || c == '=' || c == '<' || c == '>' || c == '/' || c == '|' || c == '\\'
                 || c == '[' || c == ']' || c == '{' || c == '}' || c == ':' || c == ';' || c == '"' || c == '\'' ) {
            // 문자 입력 일 때
            //getsyx(row_location, cols_location);
//            if (cols_location == size_of_cols - 1) { // 커서가 오른쪽 끝에 있는 경우
//                head = get_new_char(head, row_location, cols_location, '\n', log_file);
//                row_location++; // 줄을 한 줄 내림
//                cols_location = 0; // 커서를 맨 앞 열로 설정
//                wmove(main_win,row_location, cols_location);
//            } else {
            cols_location++;
            head = get_new_char(head, row_location + start, cols_location, c, log_file);
            wmove(main_win,row_location, cols_location); // 오른쪽으로 한 칸 이동
//            }
            is_changed = 1;
            log_row_cols_message(log_file,"row 와 cols 위치", row_location, cols_location);
        }

        else if (c == '\n') {
            // 엔터 키를 입력할 때
            //getsyx(row_location, cols_location);
            if(row_location == size_of_row-2){
                start++;
                head = get_new_char(head, row_location + start, cols_location, '\n', log_file);
            } else{
                head = get_new_char(head, row_location + start, cols_location, '\n', log_file);
                row_location++;
            }
            cols_location = 0; // 커서를 맨 앞 열로 설정
            wmove(main_win,row_location, cols_location); // 새로운 줄로 커서 이동
            is_changed = 1;
        }
        wmove(main_win, row_location, cols_location);
        wrefresh(main_win);
    }

    endwin();
    return 0;
}