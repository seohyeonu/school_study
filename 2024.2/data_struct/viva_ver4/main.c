#include <pdcurses/curses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#define BUFFER_SIZE 1000
#define True 1
#define False 0

struct row {
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

int* KMPSearch(const char* pattern, const char* text, int* found_count) {
    int M = strlen(pattern);
    int N = strlen(text);
    int* lps = (int*)malloc(M * sizeof(int));
    int* results = NULL;
    *found_count = 0;

    computeLPSArray(pattern, M, lps);

    int i = 0;
    int j = 0;
    while (i < N) {
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }
        if (j == M) {
            results = (int*)realloc(results, (*found_count + 1) * sizeof(int));
            if (!results) {
                fprintf(stderr, "Memory reallocation error!\n");
                exit(1);
            }
            results[*found_count] = i - j;
            (*found_count)++;
            j = lps[j - 1];
        } else if (i < N && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    free(lps);
    return results;
}

void log_message(FILE *log_file, const char* level, const int message) {
    time_t now;
    time(&now);
    fprintf(log_file, "[%s] %s: %d\n", ctime(&now), level, message);
}

void log_row_cols_message(FILE *log_file, const char* level, const int row, const int cols) {
    time_t now;
    time(&now);
    fprintf(log_file, "[%s] %s: row : %d , cols : %d\n", ctime(&now), level, row, cols);
}

int find_idx(Pad* cur_row, int row_position, int cols_position) {
    if (!cur_row || !cur_row->arr || cur_row->count_for_cols == 0) {
        return 0;
    }

    Pad* cur = cur_row;
    int new_line_count = 0;
    int idx = 0;

    // 첫 번째 문자 입력 시의 특별 처리
    if (cur->count_for_cols == 0) {
        return cols_position;
    }

    for (int i = 0; i < cur->count_for_cols; i++) {
        if (new_line_count == row_position) {
            if (i + cols_position < cur->count_for_cols) {
                idx = i + cols_position;
            } else {
                idx = cur->count_for_cols - 1;
            }
            return idx;
        }

        if (cur->arr[i] == '\n') {
            new_line_count++;
        }
    }

    return cur->count_for_cols;
}

Pad* get_new_char(Pad* cur_row, int row_position, int cols_position, char x, FILE *log_file) {
    Pad* cur = cur_row;
    int idx = find_idx(cur_row, row_position, cols_position);

    log_message(log_file, "get_new_char idx position", idx);

    if (cur->count_for_cols == BUFFER_SIZE * (cur->buffer_up + 1)) {
        cur->buffer_up++;
        cur->arr = (char*)realloc(cur->arr, sizeof(char) * BUFFER_SIZE * (1 << cur->buffer_up));

        if (!cur->arr) {
            fprintf(stderr, "Memory allocation error!\n");
            exit(1);
        }
    }

    if (idx == cur->count_for_cols) {
        cur->arr[idx] = x;
        cur->count_for_cols++;
    } else if (idx < cur->count_for_cols) {
        memmove(cur->arr + idx + 1, cur->arr + idx, sizeof(char) * (cur->count_for_cols - idx));
        cur->arr[idx] = x;
        cur->count_for_cols++;
    }

    return cur;
}

Pad* load_file_new_char(Pad* cur_row, char x) {
    Pad* cur = cur_row;

    if (cur->count_for_cols == BUFFER_SIZE * (cur->buffer_up)) {
        cur->buffer_up++;
        size_t new_size = BUFFER_SIZE * (cur->buffer_up);

        char* temp = (char*)realloc(cur->arr, new_size);
        if (!temp) {
            fprintf(stderr, "Memory reallocation error! Requested size: %zu bytes\n", new_size);
            exit(1);
        }
        cur->arr = temp;
    }

    cur->arr[cur->count_for_cols++] = x;
    if (x == '\n') {
        cur->new_line++;
    }
    return cur;
}

void del_char(Pad* cur_row, int row_position, int cols_position) {
    Pad* cur = cur_row;
    int idx = find_idx(cur, row_position, cols_position);
    if (idx == 0) {
        return;
    }

    memmove(cur->arr + idx - 1, cur->arr + idx, sizeof(char) * (cur->count_for_cols - idx));
    cur->count_for_cols--;
    if (cur->arr[idx - 1] == '\n') {
        cur->new_line--;
    }
}

int* get_row_array(Pad* head, int start_idx, int end_idx, int max_col) {
    int max_rows = (end_idx - start_idx + max_col) / max_col + 1;
    int* result = (int*)calloc(max_rows + 1, sizeof(int));

    if (!result) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    int one_row_len = 0;
    int row_idx = 0;

    for (int i = start_idx; i < end_idx + max_col && i < head->count_for_cols; i++) {
        one_row_len++;
        if (head->arr[i] == '\n' || one_row_len == max_col) {
            one_row_len--;
            result[row_idx++] = one_row_len;
            one_row_len = 0;
        }
    }
    result[row_idx] = one_row_len;
    return result;
}

void print_win(WINDOW* win, Pad* head, int start, int end) {
    int start_idx = find_idx(head, start, 0);
    char str1[10000] = { '\0', };
    int idx = 0;
    for (int i = 0; i < end - start; i++) {
        int str_idx = 0;
        while (head->arr[start_idx + idx] != '\n' && head->arr[start_idx + idx] != '\0') {
            str1[str_idx++] = head->arr[start_idx + idx++];
            str1[str_idx] = '\0';
        }
        str1[str_idx] = '\n';
        mvwprintw(win, i, 0, "%s", str1);
        idx++;
        str1[0] = '\0';
    }
}

void save_file(FILE* save_file, Pad* head) {
    fprintf(save_file, "%s", head->arr);
}

Pad* load_file(Pad* head, const char* filename, int size_of_cols) {
    FILE* fp = fopen(filename, "r");

    if (fp != NULL) {
        char buffer[1024] = { 0, };
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
    FILE* log_file = fopen("log.txt", "a");

    Pad* head = (Pad*)malloc(sizeof(Pad));
    head->arr = (char*)malloc(sizeof(char) * BUFFER_SIZE);
    head->arr[0] = '\0';
    head->buffer_up = 0;
    head->count_for_cols = 0;
    head->new_line = 0;

    int row_location, cols_location;
    int is_changed = 0;
    int start = 0, end = 0;
    int size_of_row, size_of_cols;
    const char* file_name = NULL;

    // PDCurses 초기화
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    getmaxyx(stdscr, size_of_row, size_of_cols);
    if (argc >= 2) {
        char windows_path[256];
        strcpy(windows_path, argv[1]);
        for (int i = 0; windows_path[i]; i++) {
            if (windows_path[i] == '/')
                windows_path[i] = '\\';
        }
        head = load_file(head, windows_path, size_of_cols);
        file_name = argv[1];
    }
    refresh();

    WINDOW* main_win = newwin(size_of_row - 2, size_of_cols, 0, 0);
    WINDOW* messenger_bar = newwin(1, size_of_cols, size_of_row - 1, 0);
    WINDOW* status_bar = newwin(1, size_of_cols, size_of_row - 2, 0);

    noecho();
    cbreak();  // raw() 대신 사용
    keypad(main_win, TRUE);

    mvwprintw(main_win, size_of_row / 2, size_of_cols / 2 - 22, "Visual Text editor -- version 0.0.1");
    wbkgd(status_bar, COLOR_PAIR(1) | A_REVERSE);
    mvwprintw(messenger_bar, 0, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");

    wrefresh(main_win);
    wrefresh(messenger_bar);
    wrefresh(status_bar);

    row_location = 0;
    cols_location = 0;

    while (True) {
        wclear(main_win);
        wclear(status_bar);

        print_win(main_win, head, start, start + size_of_row - 2);
        wmove(main_win, row_location, cols_location);
        wrefresh(main_win);
        int start_idx = find_idx(head, start, 0);
        int end_idx = find_idx(head, start + size_of_row - 2, 0);

        if (file_name == NULL) {
            mvwprintw(status_bar, 0, 0, "[No Name] - %d lines", head->new_line + 1);
        } else {
            mvwprintw(status_bar, 0, 0, "[%s] - %d lines", file_name, head->new_line);
        }

        mvwprintw(status_bar, 0, size_of_cols - 20, "no ft | %d/%d",
                  find_idx(head, row_location + start, cols_location), head->count_for_cols);
        wrefresh(status_bar);
        wrefresh(main_win);

        int c = wgetch(main_win);
        int* row_array = get_row_array(head, start_idx, end_idx, size_of_cols);

        if (c == 17) {  // Ctrl+Q
            if (is_changed) {
                wclear(messenger_bar);
                mvwprintw(messenger_bar, 0, 0, "Press Ctrl+q without saving changes");
                wrefresh(messenger_bar);
                int temp_c = getch();
                if (temp_c == 17) {
                    break;
                } else {
                    wclear(messenger_bar);
                    mvwprintw(messenger_bar, 0, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");
                    wrefresh(messenger_bar);
                    continue;
                }
            } else {
                break;
            }
        } else if (c == 19) {  // Ctrl+S
            if (argc >= 2) {
                FILE* save_file_path = fopen(file_name, "w");
                save_file(save_file_path, head);
                fclose(save_file_path);
                wclear(messenger_bar);
                mvwprintw(messenger_bar, 0, 0, "File_is_saved");
                wrefresh(messenger_bar);
                Sleep(1000);  // Windows sleep function
                wclear(messenger_bar);
                mvwprintw(messenger_bar, 0, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");
                wrefresh(messenger_bar);
                is_changed = 0;
            } else {
                wclear(messenger_bar);
                char file_name_save[5000];
                file_name = file_name_save;
                mvwprintw(messenger_bar, 0, 0, "Enter file name to save: ");
                wrefresh(messenger_bar);

                echo();
                wgetnstr(messenger_bar, file_name_save, 100);
                noecho();
                wclear(messenger_bar);
                wrefresh(messenger_bar);

                FILE* save_file_path = fopen(file_name, "w");
                save_file(save_file_path, head);
                fclose(save_file_path);
                mvwprintw(messenger_bar, 0, 0, "File saved to: %s", file_name);
                wrefresh(messenger_bar);
                Sleep(1000);  // Windows sleep
                wclear(messenger_bar);
                mvwprintw(messenger_bar, 0, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");
                wrefresh(messenger_bar);
                is_changed = 0;
            }
        } else if (c == 6) {  // Ctrl+F
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

            int found_count = 0;
            int* results = KMPSearch(search_pattern, head->arr, &found_count);

            if (found_count > 0) {
                mvwprintw(messenger_bar, 0, 0, "Found %d matches. Use arrow keys to navigate.", found_count);
                wrefresh(messenger_bar);

                int current_match = 0;
                int in_search_mode = True;

                while (in_search_mode) {
                    wclear(main_win);
                    print_win(main_win, head, start, start + size_of_row - 2);

                    // 모든 검색 결과 하이라이팅
                    for (int i = 0; i < found_count; i++) {
                        int match_idx = results[i];
                        int rel_row = 0, rel_col = 0;

                        for (int j = 0; j < match_idx; j++) {
                            if (head->arr[j] == '\n') {
                                rel_row++;
                                rel_col = 0;
                            } else {
                                rel_col++;
                            }
                        }

                        if (rel_row >= start && rel_row < start + size_of_row - 2) {
                            wattron(main_win, A_REVERSE);
                            mvwprintw(main_win, rel_row - start, rel_col, "%.*s",
                                      (int)strlen(search_pattern), search_pattern);
                            wattroff(main_win, A_REVERSE);
                        }
                    }

                    int selected_idx = results[current_match];
                    int selected_row = 0, selected_col = 0;

                    for (int j = 0; j < selected_idx; j++) {
                        if (head->arr[j] == '\n') {
                            selected_row++;
                            selected_col = 0;
                        } else {
                            selected_col++;
                        }
                    }

                    if (selected_row < start) {
                        start = selected_row;
                    } else if (selected_row >= start + size_of_row - 2) {
                        start = selected_row - (size_of_row - 3);
                    }

                    wattron(main_win, A_BOLD | A_UNDERLINE);
                    mvwprintw(main_win, selected_row - start, selected_col, "%.*s",
                              (int)strlen(search_pattern), search_pattern);
                    wattroff(main_win, A_BOLD | A_UNDERLINE);
                    wrefresh(main_win);

                    int nav_key = wgetch(main_win);

                    if (nav_key == KEY_DOWN) {
                        current_match = (current_match + 1) % found_count;
                    } else if (nav_key == KEY_UP) {
                        current_match = (current_match - 1 + found_count) % found_count;
                    } else if (nav_key == 27) {  // ESC
                        in_search_mode = False;
                        wclear(messenger_bar);
                        mvwprintw(messenger_bar, 0, 0, "Search cancelled.");
                        wrefresh(messenger_bar);
                        Sleep(1000);
                        wclear(messenger_bar);
                        mvwprintw(messenger_bar, 0, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");
                        wrefresh(messenger_bar);
                        break;
                    } else if (nav_key == '\n') {
                        row_location = selected_row - start;
                        cols_location = selected_col;
                        in_search_mode = False;
                        wclear(messenger_bar);
                        mvwprintw(messenger_bar, 0, 0, "Search ended. You can now edit.");
                        wrefresh(messenger_bar);
                        Sleep(1000);
                        wclear(messenger_bar);
                        mvwprintw(messenger_bar, 0, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");
                        wrefresh(messenger_bar);
                        break;
                    }
                }
            } else {
                mvwprintw(messenger_bar, 0, 0, "No matches found.");
                wrefresh(messenger_bar);
                Sleep(1000);
                wclear(messenger_bar);
                mvwprintw(messenger_bar, 0, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");
                wrefresh(messenger_bar);
            }
            free(results);
        } else if (c == KEY_UP) {
            if (row_location > 0) {
                if (cols_location > row_array[row_location - 1]) {
                    cols_location = row_array[row_location - 1];
                }
                row_location--;
                wmove(main_win, row_location, cols_location);
                wrefresh(main_win);
            } else if (start > 0) {
                start--;
                row_array = get_row_array(head, start_idx, end_idx, size_of_cols);
                if (cols_location > row_array[0]) {
                    cols_location = row_array[0];
                }
            }
        } else if (c == KEY_DOWN) {
            if (row_location + start > head->new_line - 2) {
                continue;
            }
            if (row_location < size_of_row - 2) {
                if (cols_location > row_array[row_location + 1]) {
                    cols_location = row_array[row_location + 1];
                }
                row_location++;
                wmove(main_win, row_location, cols_location);
                wrefresh(main_win);
            } else {
                start++;
                row_array = get_row_array(head, start_idx, end_idx, size_of_cols);
                if (cols_location > row_array[size_of_row - 2]) {
                    cols_location = row_array[size_of_row - 2];
                }
            }
        } else if (c == KEY_RIGHT) {
            if (cols_location < row_array[row_location]) {
                cols_location++;
                wmove(main_win, row_location, cols_location);
                log_message(log_file, "KEY_RIGHT", cols_location);
                wrefresh(main_win);
            }
        } else if (c == KEY_LEFT) {
            if (cols_location > 0) {
                cols_location--;
                wmove(main_win, row_location, cols_location);
                log_message(log_file, "KEY_LEFT", cols_location);
                wrefresh(main_win);
            }
        } else if (c == KEY_BACKSPACE || c == 8 || c == 127) {
            is_changed = 1;
            if (cols_location == 0) {
                if (row_location + start == 0) {
                    continue;
                }
                del_char(head, row_location + start, cols_location);
                row_location--;
                cols_location = row_array[row_location];
            } else {
                del_char(head, row_location + start, cols_location);
                cols_location--;
            }
        } else if (c == KEY_HOME) {
            cols_location = 0;
        } else if (c == KEY_END) {
            cols_location = row_array[row_location];
        } else if (c == KEY_NPAGE) {  // Page Down
            if (start + size_of_row > head->new_line) {
                continue;
            }
            start = start + size_of_row;
        } else if (c == KEY_PPAGE) {  // Page Up
            if (start - size_of_row < 0) {
                continue;
            }
            start = start - size_of_row;
        } else if ((c >= 32 && c <= 126)) {  // Printable ASCII characters
            head = get_new_char(head, row_location + start, cols_location, c, log_file);
            cols_location++;
            wmove(main_win, row_location, cols_location);
            is_changed = 1;
            log_row_cols_message(log_file, "row and cols position", row_location, cols_location);
        } else if (c == '\n') {
            if (row_location == size_of_row - 2) {
                start++;
                head = get_new_char(head, row_location + start, cols_location, '\n', log_file);
            } else {
                head = get_new_char(head, row_location + start, cols_location, '\n', log_file);
                row_location++;
            }
            cols_location = 0;
            wmove(main_win, row_location, cols_location);
            is_changed = 1;
            head->new_line++;
        }

        free(row_array);  // 메모리 해제
        wmove(main_win, row_location, cols_location);
        wrefresh(main_win);
    }

    // 프로그램 종료 전 메모리 해제
    free(head->arr);
    free(head);
    if (log_file) {
        fclose(log_file);
    }

    endwin();
    return 0;
}







#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
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

int* KMPSearch(const char* pattern, const char* text, int* found_count) {
    int M = strlen(pattern); // 패턴의 길이
    int N = strlen(text);    // 텍스트의 길이
    int* lps = (int*)malloc(M * sizeof(int)); // LPS 배열 생성
    int* results = NULL;     // 결과 배열 초기화
    *found_count = 0;        // 찾은 결과 개수 초기화

    computeLPSArray(pattern, M, lps); // LPS 배열 계산

    int i = 0; // 텍스트의 현재 인덱스
    int j = 0; // 패턴의 현재 인덱스
    while (i < N) {
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }
        if (j == M) {
            // 패턴이 일치하는 시작 인덱스 저장
            results = (int*)realloc(results, (*found_count + 1) * sizeof(int));
            if (!results) {
                fprintf(stderr, "메모리 재할당 오류!\n");
                exit(1);
            }
            results[*found_count] = i - j; // 일치하는 시작 인덱스
            (*found_count)++;
            j = lps[j - 1]; // 패턴의 다음 비교 위치 설정
        } else if (i < N && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1]; // LPS를 사용해 패턴 인덱스 재조정
            } else {
                i++;
            }
        }
    }

    free(lps); // LPS 배열 해제
    return results; // 결과 배열 반환
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
        if (!cur_row || !cur_row->arr || cur_row->count_for_cols == 0) {
            return 0;  // 초기 상태일 때는 0 반환
        }

        Pad* cur = cur_row;
        int new_line_count = 0;
        int idx = 0;

        // 첫 번째 문자 입력 시의 특별 처리
        if (cur->count_for_cols == 0) {
            return cols_position;
        }
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
    int  idx = find_idx(cur_row, row_position, cols_position);

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

    if (idx == cur->count_for_cols) {
        cur->arr[idx] = x;
        cur->count_for_cols++;
    } else if (idx < cur->count_for_cols) {
        memmove(cur->arr + idx + 1, cur->arr + idx, sizeof(char) * (cur->count_for_cols - idx));
        cur->arr[idx] = x;
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
    if(idx == 0){
        return;
    }

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
    head->arr[0] ='\0';
    head->buffer_up = 1;
    head->count_for_cols = 0;
    head->new_line = 0;


    int row_location, cols_location; //현재 커서의 row, cols 위치를 확인하는 변수
    int is_changed = 0; //문서의 내용이 바뀌었는지 안 바뀌었는지 확인하는 변수
    int start=0, end=0; // win에 뿌릴 시작과 끝점
    int size_of_row, size_of_cols; // window 사이즈 받아오기
    const char *file_name = NULL;



    // curses 색상 및 기타 설정
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    initscr();


    getmaxyx(stdscr, size_of_row, size_of_cols);
    if (argc >= 2) {
        head = load_file(head, argv[1], size_of_cols);
        file_name = argv[1];
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
        wclear(status_bar);

        print_win(main_win, head, start, start + size_of_row - 2);
        wmove(main_win, row_location, cols_location);
        wrefresh(main_win);
        int start_idx = find_idx(head,start,0);
        int end_idx = find_idx(head, start+size_of_row-2, 0);

        if(file_name == NULL){
            mvwprintw(status_bar, 0, 0, "[No Name] - %d lines", head->new_line+1);
        } else{
            mvwprintw(status_bar, 0, 0, "[%s] - %d lines", file_name, head->new_line);
        }

        mvwprintw(status_bar, 0, size_of_cols - 20, "no ft | %d/%d", find_idx(head, row_location+start, cols_location), head->count_for_cols); // size_of_cols에 맞게 수정
        wrefresh(status_bar);
        wrefresh(main_win);
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
                FILE *save_file_path = fopen(file_name, "w");
                save_file(save_file_path, head);
                wclear(messenger_bar);
                mvwprintw(messenger_bar, 0, 0, "File_is_saved");
                wrefresh(messenger_bar);
                sleep(1);
                wclear(messenger_bar);
                mvwprintw(messenger_bar, 0, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");
                wrefresh(messenger_bar);
                is_changed = 0;
            } else{
                wclear(messenger_bar);
                char file_name_save[5000]; // 파일 이름을 저장할 버퍼
                file_name = file_name_save;
                // 메시지 바에서 파일 이름 입력받기
                mvwprintw(messenger_bar, 0, 0, "Enter file name to save: ");
                wrefresh(messenger_bar);

                echo(); // 입력된 문자를 화면에 표시 sizeof(file_name) - 1
                wgetnstr(messenger_bar, file_name_save, 100); // 파일 이름 입력 받기
                noecho(); // 입력된 문자 표시 중단
                wclear(messenger_bar);
                wrefresh(messenger_bar);

                FILE *save_file_path = fopen(file_name, "w");
                save_file(save_file_path, head);
                fclose(save_file_path);
                mvwprintw(messenger_bar, 0, 0, "File saved to: %s", file_name);
                wrefresh(messenger_bar);
                sleep(1);
                wclear(messenger_bar);
                mvwprintw(messenger_bar, 0, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");
                wrefresh(messenger_bar);
                is_changed = 0;
            }
        }

        else if (c == 6) { // Ctrl+F
            wclear(messenger_bar);
            char search_pattern[256];
            mvwprintw(messenger_bar, 0, 0, "Enter search string: ");
            wrefresh(messenger_bar);

            echo();
            wgetnstr(messenger_bar, search_pattern, 255);
            noecho();
            wclear(messenger_bar);
            mvwprintw(messenger_bar, 0, 0, "Searching...");
            wrefresh(messenger_bar);

            int found_count = 0;
            int* results = KMPSearch(search_pattern, head->arr, &found_count);

            if (found_count > 0) {
                mvwprintw(messenger_bar, 0, 0, "KEY_UP: previous match / KEY_DOWN: next match");
                wrefresh(messenger_bar);

                int current_match = 0;
                bool in_search_mode = True;

                while (in_search_mode) {
                    wclear(main_win);
                    print_win(main_win, head, start, start + size_of_row - 2);

                    // 모든 검색 결과를 하이라이팅
                    for (int i = 0; i < found_count; i++) {
                        int match_idx = results[i];
                        int rel_row = 0, rel_col = 0;

                        // 현재 인덱스에서 행과 열 계산
                        for (int j = 0; j < match_idx; j++) {
                            if (head->arr[j] == '\n') {
                                rel_row++;
                                rel_col = 0;
                            } else {
                                rel_col++;
                            }
                        }

                        // 화면 내에 있는 결과만 하이라이팅
                        if (rel_row >= start && rel_row < start + size_of_row - 2) {
                            wattron(main_win, A_REVERSE);
                            mvwprintw(main_win, rel_row - start, rel_col, "%.*s", (int)strlen(search_pattern), search_pattern);
                            wattroff(main_win, A_REVERSE);
                        }
                    }

                    // 현재 선택된 검색 결과를 강조 표시
                    int selected_idx = results[current_match];
                    int selected_row = 0, selected_col = 0;

                    // 선택된 인덱스의 행과 열 계산
                    for (int j = 0; j < selected_idx; j++) {
                        if (head->arr[j] == '\n') {
                            selected_row++;
                            selected_col = 0;
                        } else {
                            selected_col++;
                        }
                    }

                    // 선택된 검색 결과가 화면 밖이면 이동
                    if (selected_row < start) {
                        start = selected_row; // 위로 스크롤
                    } else if (selected_row >= start + size_of_row - 2) {
                        start = selected_row - (size_of_row - 3); // 아래로 스크롤
                    }

                    // 선택된 검색 결과 강조
                    wattron(main_win, A_BOLD | A_UNDERLINE);
                    mvwprintw(main_win, selected_row - start, selected_col, "%.*s", (int)strlen(search_pattern), search_pattern);
                    wattroff(main_win, A_BOLD | A_UNDERLINE);
                    wrefresh(main_win);

                    // 사용자 입력 대기
                    int nav_key = wgetch(main_win);

                    if (nav_key == KEY_DOWN) { // 다음 검색 결과
                        current_match = (current_match + 1) % found_count;
                    } else if (nav_key == KEY_UP) { // 이전 검색 결과
                        current_match = (current_match - 1 + found_count) % found_count;
                    } else if (nav_key == 27) { // Esc로 검색 취소
                        in_search_mode = False;
                        wclear(messenger_bar);
                        mvwprintw(messenger_bar, 0, 0, "Search cancelled.");
                        sleep(1);
                        wclear(messenger_bar);
                        mvwprintw(messenger_bar, 0, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");
                        wrefresh(messenger_bar);
                        break;
                    } else if (nav_key == '\n') { // Enter로 탐색 종료
                        row_location = selected_row - start;
                        cols_location = selected_col;
                        in_search_mode = False;
                        wclear(messenger_bar);
                        mvwprintw(messenger_bar, 0, 0, "Search ended. You can now edit.");
                        wrefresh(messenger_bar);
                        sleep(2);
                        wclear(messenger_bar);
                        mvwprintw(messenger_bar, 0, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");
                        wrefresh(messenger_bar);
                        break;
                    }
                }
            } else {
                mvwprintw(messenger_bar, 0, 0, "No matches found.");
                wrefresh(messenger_bar);
                sleep(1);
                wclear(messenger_bar);
                mvwprintw(messenger_bar, 0, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");
                wrefresh(messenger_bar);
            }
            free(results); // 메모리 해제
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
                if(start == 0){
                    continue;
                } else{
                    start--;
                    row_array = get_row_array(head, start_idx, end_idx, size_of_cols);
                    if (cols_location > row_array[0]) {
                        cols_location = row_array[0];
                    }
                }

            }

        }

        else if(c == KEY_DOWN){
//            getsyx(row_location, cols_location);
            if(row_location+start > head->new_line -2){
                continue;
            }
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
                if(row_location + start == 0){
                    continue;
                }
                del_char(head, row_location + start, cols_location);
                row_location--;
                cols_location = row_array[row_location];  // 현재 행의 길이로 수정
            } else{
                del_char(head, row_location + start, cols_location);
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
            if(start + size_of_row > head->new_line){
                continue;
            }
            start = start + size_of_row;
        }

        else if(c == KEY_PPAGE){
            if(start - size_of_row < 0){
                continue;
            }
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
            head = get_new_char(head, row_location + start, cols_location, c, log_file);
            cols_location++;
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
            head->new_line++;
        }
        wmove(main_win, row_location, cols_location);
        wrefresh(main_win);
    }

    endwin();
    return 0;
}