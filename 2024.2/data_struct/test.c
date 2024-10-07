#include <curses.h>
#include <stdio.h>
#define size_of_row 23
#define size_of_cols 100

int main() {
    initscr();

    // 새 윈도우 작성
    WINDOW *main_win = newwin(size_of_row, size_of_cols, 0, 0);

    // 윈도우에 텍스트 출력
    mvwprintw(main_win, 10, 50, "Visual Text editor -- version 0.0.1");

    // 윈도우에 "~" 출력
    for(int i = 0; i < size_of_row; i++) {
        mvwprintw(main_win, i, 0, "~");
    }

    // 반전 색상 켜기
    wattron(main_win, A_REVERSE);
    mvwprintw(main_win, size_of_row - 1, 0, "[No Name] - 0 lines");
    mvwprintw(main_win, size_of_row - 1, size_of_cols - 14, "no ft | 1/0"); // size_of_cols에 맞게 수정
    wattroff(main_win, A_REVERSE); // A_REVERSE 속성 끄기

    // 윈도우에 도움말 출력
    mvwprintw(main_win, size_of_row, 0, "HELP: Ctrl - S = save | Ctrl-Q = quit | Ctrl-F = find");

    // 윈도우의 내용을 실제 화면에 갱신
    wrefresh(main_win);

    getch();
    endwin();
    return 0;
}

