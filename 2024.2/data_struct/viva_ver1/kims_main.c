#ifdef _WIN32
    #include <stdio.h>
    #include <stdlib.h>
    #include "doublelinkedlist.h"
    #include <string.h>
    #include <unistd.h>
    #include "inputcontrol.h"
    #include "curses.h"
    #include "queue.h"
    #define WHAT_OS 0


struct Yindex_ {
    int y_index;
    int y_subindex;
    int y_submax;
    int y;
};

typedef struct Yindex_ Yindex;

struct XYindex_ {
    int x_index;
    int line;
    int subline;
};

typedef struct XYindex_ XYindex;

Yindex* increase_y(Yindex* Yi, char* index_list) {
    Yindex* temp = Yi;
    if(temp->y_subindex < temp->y_submax) {
        temp->y_subindex++;
    } else if (temp->y_subindex == temp->y_submax) {
        temp->y_index++;
        temp->y_subindex = 0;
        temp->y_submax = index_list[temp->y_index] - 1;
    }
    int result = 1;
    for(int p=0;p < temp->y_index;p++) {
        result += index_list[p];
    }
    temp->y = result + temp->y_subindex - 1;
    return temp;
}
    
Yindex* decrease_y(Yindex* Yi, char* index_list) {
    Yindex* temp = Yi;
    if(temp->y_subindex==0) {
        temp->y_index--;
        temp->y_submax = index_list[temp->y_index] - 1;
        temp->y_subindex = temp->y_submax;
    } else if (temp->y_subindex > 0) {
        temp->y_subindex--;
    }
    int result = 1;
    for(int p=0;p < temp->y_index;p++) {
        result += index_list[p];
    }
    temp->y = result + temp->y_subindex - 1;
    return temp;
}
    
Yindex* decision_y_in_end(Yindex* Yi, char* index_list, int is_bot) {
    Yindex* temp = Yi;
    if(is_bot == 1) {
        temp->y_index = strlen(index_list) - 1;
        temp->y_submax = index_list[temp->y_index];
        temp->y_subindex = temp->y_submax;
    } else {
        temp->y_index = 0;
        temp->y_submax = index_list[0];
        temp->y_subindex = 0;
    }
    int result = 1;
    for(int p=0;p < temp->y_index;p++) {
        result += index_list[p];
    }
    temp->y = result + temp->y_subindex - 1;
    return temp;
}
    
Yindex* update_y_when_enter(Yindex* Yi, char* index_list) {
    FILE* logf = fopen("logging.txt", "a+");
    Yindex* temp = Yi;
    temp->y_index++;
    temp->y_submax = index_list[temp->y_index] - 1;
    temp->y_subindex = 0;
    int result = 1;
    for(int p=0;p < temp->y_index - 1;p++) {
        result += index_list[p];
        fprintf(logf, "index_list : %d\n", index_list[p]);
        fprintf(logf, "prev : %d, %d, %d\n", temp->y_index, temp->y_submax, temp->y_subindex);
    }
    fprintf(logf, "end\n");
    temp->y = result - 1;
    fclose(logf);
    return temp;
}

XYindex* return_xy_for_node_index(Node* head, int n, int max_col) {
    Node* tmp = head;
    XYindex* result = (XYindex*)malloc(sizeof(XYindex*));
    int k = 0;
    while(nline_index(head, k) <= n) {
        k++;
    }
    if(k != 0) {
        k--;
    }
    int line_start = nline_index(head, k);
    result->subline = (n - line_start) / max_col;
    result->x_index = (n - line_start) % max_col;
    result->line = k;
    return result;
}

IntArray* find_string_ddl(Node* head, char* find_str, int str_length) {
    Node* tmp = head;
    IntArray* result_tmp;
    int str_index = 0;
    if(str_length == 1) {
        result_tmp = return_char_indecies(tmp, find_str[0]);
        return result_tmp;
    }
    
    result_tmp = (IntArray*)malloc(sizeof(IntArray));
    result_tmp->array = (int*)malloc(sizeof(int)*500);

    int index = 0;
    int new_index = 0;
    while(tmp != NULL) {
        if(tmp->data == find_str[0]) {
            int tmp_index = index;
            tmp = tmp->next;
            index++;
            int check = 1;
            for(int i = 1; i < strlen(find_str);i++) {
                if(tmp->data == find_str[i]) {
                    check++;
                    tmp = tmp->next;
                    index++;
                }
                else {
                    break;
                }
                if(check == strlen(find_str)) {
                    result_tmp->array[new_index] = tmp_index;
                    new_index++;
                    result_tmp->length = new_index;
                    tmp_index = 0;
                }
            }

            

        //for(int k = 1; k < result_tmp->length;k++) {
        //    if(printn(head, result_tmp->array[k] + 1) == find_str[i]) {
        //        result_tmp->array[new_index] = result_tmp->array[k];
        //        new_index++;
        //        result_tmp->length = k;
        //    }
            
        //for(int k1 = new_index; k1 < result_tmp->length; k1++) {
        //    result_tmp->array[k1] = -1;
        //}
                
        } else {
            tmp = tmp->next;
            index++;
        }
    }
    return result_tmp;
}

int check_next_char(Node* node, char find_char) {
    Node* tmp = node;
    if(tmp->next->data == find_char) {
        return 1;
    }
    else {
        return -1;
    }
}
int return_file_size(FILE* file) {
    FILE* temp = file;
    fseek(temp, 0, SEEK_END);
    return ftell(temp);
}
int countDigits(int num) {
    int count = 0;

    // 음수일 경우 양수로 변환
    if (num < 0) {
        num = -num;
    }

    // 0일 경우 자리수는 1
    if (num == 0) {
        return 1;
    }

    // 자리수 세기
    while (num > 0) {
        num = num / 10;
        count++;
    }

    return count;
}
int main(int argc, char* argv[]) {
    int cur_x = 0;
    int cur_y = 0;
    
    Queue* input_queue = (Queue*)malloc(sizeof(Queue));

    int max_row = 0;
    int max_col = 0;




    Node* head = NULL;

    char* filename = (char*)malloc(sizeof(char)*50); 

    FILE* logf2 = fopen("logging.txt", "a+");
    int is_file_open = 0;
    int total_lines = 0;
    int for_test_int = 0;
    int f_size = 0;
    // file read and input content in linked list
    if( argc >= 2 ){ 
        
        FILE *fp = fopen(argv[1], "r");
        if(fp == NULL) {
            printf("file open error\n");
            return 0;
        
        }
        head = insertAtHead(head, '\n');
        if(fp != NULL){
            char buffer[1024] = {0, };
            int count = 0;
            while(feof(fp) == 0){
                count = fread(buffer, 1, sizeof(buffer), fp);
                if(count > 0){
                    for(int i = 0; i < strlen(buffer); i++) {
                        head = insertAtHead(head, buffer[i]);
                        if(buffer[i] == '\n') {
                            total_lines++;
                        }
                    }
                    memset(buffer, 0, sizeof(buffer));
                }
            }
        }
        fclose(fp);


        for (int i = 0; i < strlen(argv[1]); i++) {
            filename[i] = argv[1][i];
            filename[i+1] = '\0';
        }
        is_file_open = 1;
    } else{
        for(int klkl = 0; klkl < 2; klkl++) {
            head = insertAtHead(head, '\n');
        }
        filename[0] = '[';
        filename[1] = 'N';
        filename[2] = 'o';
        filename[3] = ' ';
        filename[4] = 'N';
        filename[5] = 'a';
        filename[6] = 'm';
        filename[7] = 'e';
        filename[8] = ']';
        filename[9] = '\0';

    }
    fprintf(logf2, "uploaded\n");
    head = reverseList(head);
    printf("Reading file complete!\n");
    // printList(head);



    WINDOW *win;

    initscr();
    noecho();
    raw();
    keypad(stdscr, TRUE);


    max_row = LINES - 2;
    max_col = COLS;

    int start = 1;
    int end = max_row;

    printf("max_row: %d\n", max_row);

    win = newwin(max_row, max_col, 0, 0);
    wmove(win, cur_x, 1);
    wrefresh(win);

    WINDOW* status_bar;
    WINDOW* message_bar;

    status_bar = newwin(1, COLS, LINES-2, 0);
    message_bar = newwin(1, COLS, max_row + 1, 0);
    // mvwprintw(status_bar, 0, 0, "test.txt - 10lines");
    // mvwprintw(status_bar, 0,COLS-10 , "ft ");

    start_color();
    init_pair(1, 0, 7);
    wbkgd(status_bar, COLOR_PAIR(1));
    wrefresh(status_bar);
    // win_bar = newwin(2, 2000, 0, 0);

    int cur_linked_list_index = 0;

    char line_index_array[max_row];
    // 화면에 읽어온 리스트들 출력
    //output_s(win, head, 1, 10);

    mvprintw(max_row + 1, 0, "ind");
    refresh();
    // int c = getch();


    mvwprintw(message_bar,0, 0, "HELP : Press 'CTRL+Q' to exit | Press 'CTRL+S' to save | Press 'CTRL+F' to find");
    wrefresh(message_bar);

    Yindex* Yi = (Yindex*)malloc(sizeof(Yindex));
    Yi->y_index = 0;

    int is_this_first = 1;
    int is_changed = 0;

    if(!is_file_open) {
        total_lines++;
    }
    while(1) {
        

        wclear(win);
        char* line_array;
        line_array = output(win, head, start, end, max_col);
        int temp_k = 0; 

        int cur_line_index = nline_index(head, start + Yi->y_index - 1);
        cur_linked_list_index = cur_line_index + (cur_x + 1) + Yi->y_subindex*max_col;
                

        int cur_line = start + cur_y - 1;
        
        wmove(win, cur_y, cur_x);
        wrefresh(win);
        wmove(win, cur_y, cur_x);


        wclear(status_bar);
        mvwprintw(status_bar, 0,0 , "%s", filename);
        mvwprintw(status_bar, 0, strlen(filename), " - %d lines", total_lines - 1);
        mvwprintw(status_bar, 0, max_col - countDigits(start + cur_y) - countDigits(total_lines) - 2, "%d/%d", start + Yi->y_index - 1, total_lines - 1);
        mvwprintw(status_bar, 0, max_col - countDigits(start + cur_y) - countDigits(total_lines) - 2 - countDigits(cur_linked_list_index) - 6, "%d ft | ", cur_linked_list_index);

        wmove(win, cur_y, cur_x);
        wrefresh(status_bar);
        wrefresh(win);

        int c;
        if(is_this_first == 1) {
            c = KEY_DOWN;
            is_this_first = 0;
        } else {
            c = wgetch(win);
        }

        fprintf(logf2, "getch : %c\n", c);
        fclose(logf2);
        if(c == 17) {
            if(is_changed) {
                mvprintw(max_row + 1, 0, "Press Ctrl+q without saving changes                                                            ");
                refresh();
                int temp_c = wgetch(win);
                if(temp_c == 17) {
                    break;
                } 
                else {
                    mvprintw(max_row + 1, 0, "HELP : Press 'CTRL+Q' to exit | Press 'CTRL+S' to save | Press 'CTRL+F' to find");
                    continue;
                }
            } 
            else {
                break;   
            }
        }
        else if(c == KEY_UP) {
            if(cur_y > 1) {
                //cur_y--;
                Yi = decrease_y(Yi, line_array);
                cur_y = Yi->y;
                cur_x = 0;
                wmove(win, cur_y, cur_x);
                wrefresh(win);
            } else if (cur_y <= 1 && start > 1){
                start--;
                end--;
                
                //cur_y = 1;
                //Yi = decision_y_in_end(Yi, line_array, 0);
                cur_y = 0;
                cur_x = 0;
                wmove(win, cur_y, cur_x);
                wrefresh(win);
            }
        }
        else if(c == KEY_DOWN) {
            if(cur_y < max_row - 1) {
                //cur_y++;
                Yi = increase_y(Yi, line_array);
                cur_y = Yi->y;
                cur_x = 0;
                wmove(win, cur_y, cur_x);
                wrefresh(win);
            }
            else {
                start++;
                end++;

                Yi = decision_y_in_end(Yi, line_array, 1);
                cur_y = max_row - 1;
                cur_x = 0;
                wmove(win, cur_y, cur_x);
                wrefresh(win);
            }
        }
        else if(c == KEY_LEFT) {
            if(cur_x > 0) {
                cur_x--;
                wmove(win, cur_y, cur_x);
                wrefresh(win);
            }
        }
        else if(c == KEY_RIGHT) {
            if(cur_x < max_col) {
                cur_x++;
                wmove(win, cur_y, cur_x);
                wrefresh(win);
            }
        }
        else if((c >= 'a' && c <= 'z') || (c <= 'Z' && c >= 'A') || (c >= '0' && c <= '9') || c == ' ' || c == '.' || c == ',' ||
         c == '?' || c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&' || c == '*' || c == '(' 
         || c == ')' || c == '-' || c == '_' || c == '+' || c == '=' || c == '<' || c == '>' || c == '/' || c == '|' || c == '\\' 
         || c == '[' || c == ']' || c == '{' || c == '}' || c == ':' || c == ';' || c == '"' || c == '\'') {
            input_queue = enqueue(input_queue, c);
            head = insertAtn(head, cur_linked_list_index, c);
            cur_x++;
            wmove(win, cur_y, cur_x);
            wrefresh(win);
            is_changed = 1;
        }

        else if(c == KEY_BACKSPACE || c == 127) {
            if(total_lines <= 0) {
                continue;
            }
            head = deleteNode(head, cur_linked_list_index);
            int line_length = nline_length(head, cur_line);
            if((cur_x == 0 && cur_y >= 1) && start >= 1) {
                if(cur_y == 1) {
                    start--;
                    end--;
                    cur_x = nline_length(head, start + cur_y - 1);
                } else {
                    Yi = decrease_y(Yi, line_array);
                    cur_y = Yi->y;
                    cur_x = nline_length(head, start + cur_y - 1);
                }
                total_lines-=1;
                
            }
            cur_x--;
            wmove(win, cur_y, cur_x);
            wrefresh(win);
            is_changed = 1;
        }
        else if(c == '\n' || c == KEY_ENTER) {
            head = insertAtn(head, cur_linked_list_index, '\n');
            cur_x = 0;
            line_array = check_line(head, start, end, max_col);
            Yi = update_y_when_enter(Yi, line_array);
            cur_y = Yi->y + 1;
            total_lines++; 
            wmove(win, cur_y, cur_x);
            wrefresh(win);
            is_changed = 1;
        }
 
        else if(c == KEY_PPAGE) {
            if(start > max_row) {
                start -= max_row;
                end -= max_row;
            }
        }

        else if(c == KEY_NPAGE) {
            start += max_row;
            end += max_row;
        }


        // 끄기
        else if (c == 17) {
            if(is_changed == 1) {
                mvprintw(max_row + 1, 0, "Press Ctrl+q without saving changes");
                refresh();
                int temp_c = getch();
                if(temp_c == 17) {
                    break;
                } 
                else {
                    mvprintw(max_row + 1, 0, "HELP : Press 'CTRL+Q' to exit | Press 'CTRL+S' to save | Press 'CTRL+F' to find");
                    continue;
                }
            } 
            else {
                break;   
            }
            
        }
        // 저장
        else if (c == 19) {
            wmove(message_bar, 0, 0);
            wclear(message_bar); 
            //clrtoeol();
            int is_stop = 0;
            mvwprintw(message_bar, 0, 0, "Save as : %s", filename);
            wmove(message_bar,0, 11);
            wrefresh(message_bar);
            int c_s = 'a';
            int name_index = 0;
            int save_start = 1;
            int just_removed = 0;
            if(!is_file_open) {    
                filename[0] = '\0';
            }
            while(!is_file_open) {
                if(c_s == '\n') {
                    break;
                }
                wclear(message_bar);
                wrefresh(message_bar);
                mvwprintw(message_bar, 0, 0, "Save as : %s", filename);
                wrefresh(message_bar);
                c_s = getch();
                mvwprintw(message_bar, 0, 0, "Save as : %s", filename);
                wrefresh(message_bar);
                if(c_s == 17) {
                    is_stop = 1;
                    break;
                } else if (c_s == 127 || c_s == KEY_BACKSPACE) {
                    if(name_index > 0) {
                        filename[name_index] = '\0';
                        name_index--;
                    } else if (name_index == 0) {
                        filename[name_index] = '\0';

                        //clrtoeol();
                        mvwprintw(message_bar, 0, 0, "Save as : %s", filename);
                        wrefresh(message_bar);
                    }
                    save_start = 0;
                } else {
                    filename[name_index] = c_s;
                    filename[name_index + 1] = '\0';
                    name_index++;
                    //wmove(message_bar, 0, name_index + 11);
                    mvwprintw(message_bar, 0, 0, "Save as : %s", filename);
                    wrefresh(message_bar);
                    save_start = 0;
                }
            }
            if(is_stop) {
                mvwprintw(message_bar, 0, 0, "HELP : Press 'CTRL+Q' to exit | Press 'CTRL+S' to save | Press 'CTRL+F' to find");
                continue;
            }
            if(!is_file_open) {
                filename[strlen(filename) - 1] = '\0';
            }
            FILE* fp_s = fopen(filename, "w");
            if(fp_s == NULL) {
                printf("File open error\n");
                break;
            }
            
            head = deleteNode(head, 1);
            Node* temp_s = head;
            while(temp_s != NULL) {
                fprintf(fp_s, "%c", temp_s->data);
                temp_s = temp_s->next;
            }
            fclose(fp_s);
            mvwprintw(message_bar, 0, 0, "HELP : Press 'CTRL+Q' to exit | Press 'CTRL+S' to save | Press 'CTRL+F' to find");
            wrefresh(message_bar);
            wmove(win, cur_y, cur_x);
            wrefresh(win);
            is_file_open = 1;
            head = insertAtn(head, 0, '\n');
            is_changed = 0;
        }
        // 탐색 모드
        else if(c==6) {
            int c_f= 'a';
            char* find_str = (char*)malloc(sizeof(char)*50);
            int find_str_index = 0;
            XYindex* XYi;
            IntArray* searched_indecies;
            int* searching_indecies_array;
            int searching_index = 0;
            int cursor_searching_index = 0;
            int start_line = 0;
            char* subindex_array;
            int prev_line_count = 0;
            char search_target[500];
            int search_target_index = 0;
            int change_count = 0;
            int temp_cur_x = 0;
            int temp_cur_y = 0;

            int temp_node_index = 0;
            wmove(message_bar, 0, 13);
            wclear(message_bar);
            mvwprintw(message_bar, 0, 0, "Search for : ");
            wrefresh(message_bar);
            while(c_f != 27) {
                c_f = getch();
                if(c_f == 27) {
                    wclear(message_bar);
                    mvwprintw(message_bar, 0, 0, "HELP : Press 'CTRL+Q' to exit | Press 'CTRL+S' to save | Press 'CTRL+F' to find");
                    wrefresh(message_bar);
                    break;
                }
                else if(c_f == KEY_BACKSPACE || c_f == 127) {
                    if(find_str_index > 0) {
                        find_str[--find_str_index] = '\0';
                        wclear(message_bar);
                        mvwprintw(message_bar, 0, 0, "Search for : ");
                        mvwprintw(message_bar, 0, 13, "%s", find_str);
                        wrefresh(message_bar);
                        wmove(message_bar, 0, 13 + find_str_index);
                        wrefresh(message_bar);
                        continue;
                    } 
                    else {
                        continue;
                    }
                }

                
                else if (c_f == '\n') {
                 //엔터를 누르면 탐색 관련 입력을 그만 받음
                    wclear(win);
                    int c_fs = 0;
                    searched_indecies = find_string_ddl(head, find_str, strlen(find_str));
                    XYi = return_xy_for_node_index(head, searched_indecies->array[0], max_col);
                    start_line = XYi->line;
                    searching_indecies_array = output_f(win, head, XYi->line, max_row, max_col, searched_indecies, strlen(find_str), 0);
                    subindex_array = check_line(head, XYi->line, XYi->line + max_row, max_col);
                    wmove(win, XYi->subline + 1, XYi->x_index - 1);
                    wrefresh(win);
                    while(1) {
                        FILE* logf = fopen("logging.txt", "a+");
                        c_fs = getch();
                        if(c_fs == 27) {
                            fclose(logf);
                            break;
                        }
                        else if(c_fs == KEY_DOWN) {
                            if(cursor_searching_index == searched_indecies->length) {
                                continue;
                            } else if(cursor_searching_index <= searching_indecies_array[1]) {
                                XYi = return_xy_for_node_index(head, searched_indecies->array[cursor_searching_index], max_col);
                                for(int qkqk = 0; qkqk < XYi->line - start_line; qkqk++) {
                                    prev_line_count += subindex_array[qkqk];
                                    fprintf(logf, "subindex_array : %d\n", subindex_array[qkqk]);
                                }
                                fprintf(logf, "prev_line_count : %d\n", prev_line_count);
                                fprintf(logf, "XYi->subline : %d\n", XYi->subline);
                                temp_node_index = searched_indecies->array[cursor_searching_index];
                                temp_cur_y = prev_line_count + XYi->subline + 1;
                                temp_cur_x = XYi->x_index - 1; 
                                wrefresh(status_bar);
                                wmove(win, temp_cur_y, temp_cur_x);
                                wrefresh(win); 
                                cursor_searching_index++;
                                prev_line_count = 0;
                            } else {

                                searching_index = searching_indecies_array[1];
                                searching_index++;
                                XYi = return_xy_for_node_index(head, searched_indecies->array[searching_index], max_col);
                                start_line = XYi->line;
                                searching_indecies_array = output_f(win, head, XYi->line, max_row, max_col, searched_indecies, strlen(find_str), searching_index);
                                subindex_array = check_line(head, XYi->line, XYi->line + max_row, max_col);
                                temp_node_index = searched_indecies->array[cursor_searching_index];

                                temp_cur_y = XYi->subline + 1;
                                temp_cur_x = XYi->x_index - 1;
                                wmove(win, temp_cur_y, temp_cur_x);
                                wrefresh(win);
                                cursor_searching_index++;
                            }
                        }         
                        else if(c_fs == KEY_UP) {
                            if(cursor_searching_index == searched_indecies->length) {
                                continue;
                            } else if(cursor_searching_index <= searching_indecies_array[1]) {
                                XYi = return_xy_for_node_index(head, searched_indecies->array[cursor_searching_index], max_col);
                                for(int qkqk = 0; qkqk < XYi->line - start_line; qkqk++) {
                                    prev_line_count += subindex_array[qkqk];
                                }
                                wrefresh(status_bar);
                                temp_cur_y = prev_line_count + XYi->subline + 1;
                                temp_cur_x = XYi->x_index - 1; 
                                temp_node_index = searched_indecies->array[cursor_searching_index];
                                wmove(win, temp_cur_y, temp_cur_x);
                                wrefresh(win); 
                                cursor_searching_index--;
                                prev_line_count = 0;
                            } else {

                                searching_index = searching_indecies_array[0];
                                searching_index--;
                                XYi = return_xy_for_node_index(head, searched_indecies->array[searching_index], max_col);
                                start_line = XYi->line;
                                searching_indecies_array = output_f(win, head, XYi->line, max_row, max_col, searched_indecies, strlen(find_str), searching_index);
                                subindex_array = check_line(head, XYi->line, XYi->line + max_row, max_col);
                                temp_node_index = searched_indecies->array[cursor_searching_index];
                                wmove(win, XYi->subline + 1, XYi->x_index - 1);
                                wrefresh(win);
                                cursor_searching_index--;
                            }
                        
                        }
                        else if((c_fs >= 'a' && c_fs <= 'z') || (c_fs <= 'Z' && c_fs >= 'A') || (c_fs >= '0' && c_fs <= '9') || c_fs == ' ' || c_fs == '.' || c_fs == ',' ||
                        c_fs == '?' || c_fs == '!' || c_fs == '@' || c_fs == '#' || c_fs == '$' || c_fs == '%' || c_fs == '^' || c_fs == '&' || c_fs == '*' || c_fs == '(' 
                        || c_fs == ')' || c_fs == '-' || c_fs == '_' || c_fs == '+' || c_fs == '=' || c_fs == '<' || c_fs == '>' || c_fs == '/' || c_fs == '|' || c_fs == '\\' 
                        || c_fs == '[' || c_fs == ']' || c_fs == '{' || c_fs == '}' || c_fs == ':' || c_fs == ';' || c_fs == '"' || c_fs == '\'') {
                            change_count++;
                            head = insertAtn(head, temp_node_index, c_fs);
                            temp_node_index = searched_indecies->array[cursor_searching_index];
                            temp_cur_x++;
                            wmove(win, temp_cur_y, temp_cur_x);
                            
                            //searched_indecies = find_string_ddl(head, find_str, strlen(find_str));
                            
                            wrefresh(win);
                            for(int tempi1 = cursor_searching_index; tempi1 < searched_indecies->length; tempi1++) {
                                searched_indecies->array[tempi1]++;
                            }
                            searching_indecies_array = output_f(win, head, XYi->line, max_row, max_col, searched_indecies, strlen(find_str), searching_index);
                            subindex_array = check_line(head, XYi->line, XYi->line + max_row, max_col);
                        }
                        else if(c_fs == KEY_BACKSPACE || c_fs == 127) {
                            change_count--;
                            head = deleteNode(head, temp_node_index + (temp_cur_x - XYi->x_index + 1));
                            temp_cur_x--;
                            wmove(win, temp_cur_y, temp_cur_x);

                            //searched_indecies = find_string_ddl(head, find_str, strlen(find_str));
                            
                            wrefresh(win);
                            for(int tempi1 = cursor_searching_index; tempi1 < searched_indecies->length; tempi1++) {
                                searched_indecies->array[tempi1]--;
                            }
                            searching_indecies_array = output_f(win, head, XYi->line, max_row, max_col, searched_indecies, strlen(find_str), searching_index);
                            subindex_array = check_line(head, XYi->line, XYi->line + max_row, max_col);
                        }
                        else if(c_fs == KEY_LEFT) {
                            temp_cur_x--;
                            temp_node_index--;
                            wmove(win, temp_cur_y, temp_cur_x);
                            wrefresh(win);
                        }
                        else if(c_fs == KEY_RIGHT) {
                            temp_cur_x++;
                            temp_node_index++;
                            wmove(win, temp_cur_y, temp_cur_x);
                            wrefresh(win);
                        }
                        fclose(logf);         
                    }

                }
            

               	find_str[find_str_index] = c_f;
                find_str[find_str_index + 1] = '\0';
                find_str_index++;
                wclear(message_bar);
                mvwprintw(message_bar, 0, 0, "Search for : ");
                mvwprintw(message_bar, 0, 13, "%s", find_str);
                wrefresh(message_bar);
                wmove(message_bar, 0, 13 + find_str_index);
                wrefresh(message_bar);
                    
                


                wrefresh(status_bar);
            }
            free(XYi);
            free(find_str);
        }


        

    }
    endwin();
    free(Yi);
    free(filename);
    free(win);
    free(head);
    return 0;
}

    


#else
    #include <stdio.h>
    #include <stdlib.h>
    #include "doublelinkedlist.h"
    #include <string.h>
    #include <unistd.h>
    #include "inputcontrol.h"
    #include <ncurses.h>
    #include <termios.h>
    #include "queue.h"
    #include <sys/ioctl.h>
    #define WHAT_OS 1

struct Yindex_ {
    int y_index;
    int y_subindex;
    int y_submax;
    int y;
};

typedef struct Yindex_ Yindex;

struct XYindex_ {
    int x_index;
    int line;
    int subline;
};

typedef struct XYindex_ XYindex;

Yindex* increase_y(Yindex* Yi, char* index_list) {
    Yindex* temp = Yi;
    if(temp->y_subindex < temp->y_submax) {
        temp->y_subindex++;
    } else if (temp->y_subindex == temp->y_submax) {
        temp->y_index++;
        temp->y_subindex = 0;
        temp->y_submax = index_list[temp->y_index] - 1;
    }
    int result = 1;
    for(int p=0;p < temp->y_index;p++) {
        result += index_list[p];
    }
    temp->y = result + temp->y_subindex - 1;
    return temp;
}
    
Yindex* decrease_y(Yindex* Yi, char* index_list) {
    Yindex* temp = Yi;
    if(temp->y_subindex==0) {
        temp->y_index--;
        temp->y_submax = index_list[temp->y_index] - 1;
        temp->y_subindex = temp->y_submax;
    } else if (temp->y_subindex > 0) {
        temp->y_subindex--;
    }
    int result = 1;
    for(int p=0;p < temp->y_index;p++) {
        result += index_list[p];
    }
    temp->y = result + temp->y_subindex - 1;
    return temp;
}
    
Yindex* decision_y_in_end(Yindex* Yi, char* index_list, int is_bot) {
    Yindex* temp = Yi;
    if(is_bot == 1) {
        temp->y_index = strlen(index_list) - 1;
        temp->y_submax = index_list[temp->y_index];
        temp->y_subindex = temp->y_submax;
    } else {
        temp->y_index = 0;
        temp->y_submax = index_list[0];
        temp->y_subindex = 0;
    }
    int result = 1;
    for(int p=0;p < temp->y_index;p++) {
        result += index_list[p];
    }
    temp->y = result + temp->y_subindex - 1;
    return temp;
}
    
Yindex* update_y_when_enter(Yindex* Yi, char* index_list) {
    FILE* logf = fopen("logging.txt", "a+");
    Yindex* temp = Yi;
    temp->y_index++;
    temp->y_submax = index_list[temp->y_index] - 1;
    temp->y_subindex = 0;
    int result = 1;
    for(int p=0;p < temp->y_index - 1;p++) {
        result += index_list[p];
        fprintf(logf, "index_list : %d\n", index_list[p]);
        fprintf(logf, "prev : %d, %d, %d\n", temp->y_index, temp->y_submax, temp->y_subindex);
    }
    fprintf(logf, "end\n");
    temp->y = result - 1;
    fclose(logf);
    return temp;
}

XYindex* return_xy_for_node_index(Node* head, int n, int max_col) {
    Node* tmp = head;
    XYindex* result = (XYindex*)malloc(sizeof(XYindex*));
    int k = 0;
    while(nline_index(head, k) <= n) {
        k++;
    }
    if(k != 0) {
        k--;
    }
    int line_start = nline_index(head, k);
    result->subline = (n - line_start) / max_col;
    result->x_index = (n - line_start) % max_col;
    result->line = k;
    return result;
}

IntArray* find_string_ddl(Node* head, char* find_str, int str_length) {
    Node* tmp = head;
    IntArray* result_tmp;
    int str_index = 0;
    if(str_length == 1) {
        result_tmp = return_char_indecies(tmp, find_str[0]);
        return result_tmp;
    }
    
    result_tmp = (IntArray*)malloc(sizeof(IntArray));
    result_tmp->array = (int*)malloc(sizeof(int)*500);

    int index = 0;
    int new_index = 0;
    while(tmp != NULL) {
        if(tmp->data == find_str[0]) {
            int tmp_index = index;
            tmp = tmp->next;
            index++;
            int check = 1;
            for(int i = 1; i < strlen(find_str);i++) {
                if(tmp->data == find_str[i]) {
                    check++;
                    tmp = tmp->next;
                    index++;
                }
                else {
                    break;
                }
                if(check == strlen(find_str)) {
                    result_tmp->array[new_index] = tmp_index;
                    new_index++;
                    result_tmp->length = new_index;
                    tmp_index = 0;
                }
            }

            


                
        } else {
            tmp = tmp->next;
            index++;
        }
    }
    return result_tmp;
}

int check_next_char(Node* node, char find_char) {
    Node* tmp = node;
    if(tmp->next->data == find_char) {
        return 1;
    }
    else {
        return -1;
    }
}
int return_file_size(FILE* file) {
    FILE* temp = file;
    fseek(temp, 0, SEEK_END);
    return ftell(temp);
}
int countDigits(int num) {
    int count = 0;

    // 음수일 경우 양수로 변환
    if (num < 0) {
        num = -num;
    }

    // 0일 경우 자리수는 1
    if (num == 0) {
        return 1;
    }

    // 자리수 세기
    while (num > 0) {
        num = num / 10;
        count++;
    }

    return count;
}
int main(int argc, char* argv[]) {
    int cur_x = 0;
    int cur_y = 0;
    
    Queue* input_queue = (Queue*)malloc(sizeof(Queue));

    int max_row = 0;
    int max_col = 0;
    struct winsize w_size;


    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w_size);

    //max_row = LINES - 2;
    //max_col = COLS;
    max_row = w_size.ws_row  - 2;
    max_col = w_size.ws_col;


    Node* head = NULL;

    char* filename = (char*)malloc(sizeof(char)*50); 

    FILE* logf2 = fopen("logging.txt", "a+");
    int is_file_open = 0;
    int total_lines = 0;
    int for_test_int = 0;
    int f_size = 0;
    // file read and input content in linked list
    if( argc >= 2 ){ 
        
        FILE *fp = fopen(argv[1], "r");
        if(fp == NULL) {
            printf("file open error\n");
            return 0;
        
        }
        head = insertAtHead(head, '\n');
        if(fp != NULL){
            char buffer[1024] = {0, };
            int count = 0;
            while(feof(fp) == 0){
                count = fread(buffer, 1, sizeof(buffer), fp);
                if(count > 0){
                    for(int i = 0; i < strlen(buffer); i++) {
                        head = insertAtHead(head, buffer[i]);
                        if(buffer[i] == '\n') {
                            total_lines++;
                        }
                    }
                    memset(buffer, 0, sizeof(buffer));
                }
            }
        }
        fclose(fp);

        for (int i = 0; i < strlen(argv[1]); i++) {
            filename[i] = argv[1][i];
            filename[i+1] = '\0';
        }
        is_file_open = 1;
    } else{
        for(int klkl = 0; klkl < 2; klkl++) {
            head = insertAtHead(head, '\n');
        }
        filename[0] = '[';
        filename[1] = 'N';
        filename[2] = 'o';
        filename[3] = ' ';
        filename[4] = 'N';
        filename[5] = 'a';
        filename[6] = 'm';
        filename[7] = 'e';
        filename[8] = ']';
        filename[9] = '\0';

    }
    fprintf(logf2, "uploaded\n");
    head = reverseList(head);
    printf("Reading file complete!\n");
    // printList(head);



    WINDOW *win;

    initscr();
    noecho();
    raw();
    keypad(stdscr, TRUE);



    int start = 1;
    int end = max_row;

    printf("max_row: %d\n", max_row);

    win = newwin(max_row, max_col, 0, 0);
    wmove(win, cur_x, 1);
    wrefresh(win);

    WINDOW* status_bar;
    WINDOW* message_bar;

    status_bar = newwin(1, COLS, LINES-2, 0);
    message_bar = newwin(1, COLS, max_row + 1, 0);
    // mvwprintw(status_bar, 0, 0, "test.txt - 10lines");
    // mvwprintw(status_bar, 0,COLS-10 , "ft ");

    start_color();
    init_pair(1, 0, 7);
    wbkgd(status_bar, COLOR_PAIR(1));
    wrefresh(status_bar);
    // win_bar = newwin(2, 2000, 0, 0);

    int cur_linked_list_index = 0;

    char line_index_array[max_row];
    // 화면에 읽어온 리스트들 출력
    //output_s(win, head, 1, 10);

    mvprintw(max_row + 1, 0, "ind");
    refresh();
    // int c = getch();


    mvwprintw(message_bar,0, 0, "HELP : Press 'CTRL+Q' to exit | Press 'CTRL+S' to save | Press 'CTRL+F' to find");
    wrefresh(message_bar);


    Yindex* Yi = (Yindex*)malloc(sizeof(Yindex));
    Yi->y_index = 0;

    int is_this_first = 1;
    int is_changed = 0;

    if(!is_file_open) {
        total_lines++;
    }
    while(1) {
        


        wclear(win);
        char* line_array;
        fprintf(logf2, "before output\n");
        line_array = output(win, head, start, end, max_col);
        fprintf(logf2, "after output\n");
        int temp_k = 0; 

        int cur_line_index = nline_index(head, start + Yi->y_index - 1);
        cur_linked_list_index = cur_line_index + (cur_x + 1) + Yi->y_subindex*max_col;
                

        int cur_line = start + cur_y - 1;
        
        wmove(win, cur_y, cur_x);
        wrefresh(win);
        wmove(win, cur_y, cur_x);

        wclear(status_bar);
        mvwprintw(status_bar, 0,0 , "%s", filename);
        mvwprintw(status_bar, 0, strlen(filename), " - %d lines", total_lines - 1);
        mvwprintw(status_bar, 0, max_col - countDigits(start + cur_y) - countDigits(total_lines) - 2, "%d/%d", start + Yi->y_index - 1, total_lines - 1);
        mvwprintw(status_bar, 0, max_col - countDigits(start + cur_y) - countDigits(total_lines) - 2 - countDigits(cur_linked_list_index) - 6, "%d ft | ", cur_linked_list_index);

        wmove(win, cur_y, cur_x);
        wrefresh(status_bar);
        wrefresh(win);

        int c;
        if(is_this_first == 1) {
            c = KEY_DOWN;
            is_this_first = 0;
        } else {
            c = getch();
        }
        if(c == 17) {
            if(is_changed) {
                mvprintw(max_row + 1, 0, "Press Ctrl+q without saving changes                                                            ");
                refresh();
                int temp_c = getch();
                if(temp_c == 17) {
                    break;
                } 
                else {
                    mvprintw(max_row + 1, 0, "HELP : Press 'CTRL+Q' to exit | Press 'CTRL+S' to save | Press 'CTRL+F' to find");
                    continue;
                }
            } 
            else {
                break;   
            }
        }
        else if(c == KEY_UP) {
            if(cur_y > 1) {
                //cur_y--;
                Yi = decrease_y(Yi, line_array);
                cur_y = Yi->y;
                cur_x = 0;
                wmove(win, cur_y, cur_x);
                wrefresh(win);
            } else if (cur_y <= 1 && start > 1){
                start--;
                end--;
                
                //cur_y = 1;
                //Yi = decision_y_in_end(Yi, line_array, 0);
                cur_y = 0;
                cur_x = 0;
                wmove(win, cur_y, cur_x);
                wrefresh(win);
            }
        }
        else if(c == KEY_DOWN) {
            if(cur_y < max_row - 1) {
                //cur_y++;
                Yi = increase_y(Yi, line_array);
                cur_y = Yi->y;
                cur_x = 0;
                wmove(win, cur_y, cur_x);
                wrefresh(win);
            }
            else {
                start++;
                end++;
                Yi = decision_y_in_end(Yi, line_array, 1);
                cur_y = max_row - 1;
                cur_x = 0;
                wmove(win, cur_y, cur_x);
                wrefresh(win);
            }
        }
        else if(c == KEY_LEFT) {
            if(cur_x > 0) {
                cur_x--;
                wmove(win, cur_y, cur_x);
                wrefresh(win);
            }
        }
        else if(c == KEY_RIGHT) {
            if(cur_x < max_col) {
                cur_x++;
                wmove(win, cur_y, cur_x);
                wrefresh(win);
            }
        }
        else if((c >= 'a' && c <= 'z') || (c <= 'Z' && c >= 'A') || (c >= '0' && c <= '9') || c == ' ' || c == '.' || c == ',' ||
         c == '?' || c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&' || c == '*' || c == '(' 
         || c == ')' || c == '-' || c == '_' || c == '+' || c == '=' || c == '<' || c == '>' || c == '/' || c == '|' || c == '\\' 
         || c == '[' || c == ']' || c == '{' || c == '}' || c == ':' || c == ';' || c == '"' || c == '\'') {
            input_queue = enqueue(input_queue, c);
            head = insertAtn(head, cur_linked_list_index, c);
            cur_x++;
            wmove(win, cur_y, cur_x);
            wrefresh(win);
            is_changed = 1;
        }

        else if(c == KEY_BACKSPACE || c == 127) {
            if(total_lines <= 0) {
                continue;
            }
            head = deleteNode(head, cur_linked_list_index);
            int line_length = nline_length(head, cur_line);
            if((cur_x == 0 && cur_y >= 1) && start >= 1) {
                if(cur_y == 1) {
                    start--;
                    end--;
                    cur_x = nline_length(head, start + cur_y - 1);
                } else {
                    Yi = decrease_y(Yi, line_array);
                    cur_y = Yi->y;
                    cur_x = nline_length(head, start + cur_y - 1);
                }
                total_lines-=1;
                
            }
            cur_x--;
            wmove(win, cur_y, cur_x);
            wrefresh(win);
            is_changed = 1;
        }
        else if(c == '\n' || c == KEY_ENTER) {
            head = insertAtn(head, cur_linked_list_index, '\n');
            cur_x = 0;
            line_array = check_line(head, start, end, max_col);
            Yi = update_y_when_enter(Yi, line_array);
            cur_y = Yi->y + 1;
            total_lines++; 
            wmove(win, cur_y, cur_x);
            wrefresh(win);
            is_changed = 1;
        }
 
        else if(c == KEY_PPAGE) {
            if(start > max_row) {
                start -= max_row;
                end -= max_row;
            }
        }

        else if(c == KEY_NPAGE) {
            start += max_row;
            end += max_row;
        }


        // 끄기
        else if (c == 17) {
            if(is_changed == 1) {
                mvprintw(max_row + 1, 0, "Press Ctrl+q without saving changes");
                refresh();
                int temp_c = getch();
                if(temp_c == 17) {
                    break;
                } 
                else {
                    mvprintw(max_row + 1, 0, "HELP : Press 'CTRL+Q' to exit | Press 'CTRL+S' to save | Press 'CTRL+F' to find");
                    continue;
                }
            } 
            else {
                break;   
            }
            
        }
        // 저장
        else if (c == 19) {
            wmove(message_bar, 0, 0);
            wclear(message_bar); 
            //clrtoeol();
            int is_stop = 0;
            mvwprintw(message_bar, 0, 0, "Save as : %s", filename);
            wmove(message_bar,0, 11);
            wrefresh(message_bar);
            int c_s = 'a';
            int name_index = 0;
            int save_start = 1;
            int just_removed = 0;
            if(!is_file_open) {    
                filename[0] = '\0';
            }
            while(!is_file_open) {
                if(c_s == '\n') {
                    break;
                }
                wclear(message_bar);
                wrefresh(message_bar);
                mvwprintw(message_bar, 0, 0, "Save as : %s", filename);
                wrefresh(message_bar);
                c_s = getch();
                mvwprintw(message_bar, 0, 0, "Save as : %s", filename);
                wrefresh(message_bar);
                if(c_s == 17) {
                    is_stop = 1;
                    break;
                } else if (c_s == 127 || c_s == KEY_BACKSPACE) {
                    if(name_index > 0) {
                        filename[name_index] = '\0';
                        name_index--;
                    } else if (name_index == 0) {
                        filename[name_index] = '\0';

                        //clrtoeol();
                        mvwprintw(message_bar, 0, 0, "Save as : %s", filename);
                        wrefresh(message_bar);
                    }
                    save_start = 0;
                } else {
                    filename[name_index] = c_s;
                    filename[name_index + 1] = '\0';
                    name_index++;
                    //wmove(message_bar, 0, name_index + 11);
                    mvwprintw(message_bar, 0, 0, "Save as : %s", filename);
                    wrefresh(message_bar);
                    save_start = 0;
                }
            }
            if(is_stop) {
                mvwprintw(message_bar, 0, 0, "HELP : Press 'CTRL+Q' to exit | Press 'CTRL+S' to save | Press 'CTRL+F' to find");
                continue;
            }
            if(!is_file_open) {
                filename[strlen(filename) - 1] = '\0';
            }
            FILE* fp_s = fopen(filename, "w");
            if(fp_s == NULL) {
                printf("File open error\n");
                break;
            }
            
            head = deleteNode(head, 1);
            Node* temp_s = head;
            while(temp_s != NULL) {
                fprintf(fp_s, "%c", temp_s->data);
                temp_s = temp_s->next;
            }
            fclose(fp_s);
            mvwprintw(message_bar, 0, 0, "HELP : Press 'CTRL+Q' to exit | Press 'CTRL+S' to save | Press 'CTRL+F' to find");
            wrefresh(message_bar);
            wmove(win, cur_y, cur_x);
            wrefresh(win);
            is_file_open = 1;
            head = insertAtn(head, 0, '\n');
            is_changed = 0;
        }
        // 탐색 모드
        else if(c==6) {
            int c_f= 'a';
            char* find_str = (char*)malloc(sizeof(char)*50);
            int find_str_index = 0;
            XYindex* XYi;
            IntArray* searched_indecies;
            int* searching_indecies_array;
            int searching_index = 0;
            int cursor_searching_index = 0;
            int start_line = 0;
            char* subindex_array;
            int prev_line_count = 0;
            char search_target[500];
            int search_target_index = 0;
            int change_count = 0;
            int temp_cur_x = 0;
            int temp_cur_y = 0;

            int temp_node_index = 0;
            wmove(message_bar, 0, 13);
            wclear(message_bar);
            mvwprintw(message_bar, 0, 0, "Search for : ");
            wrefresh(message_bar);
            while(c_f != 27) {
                c_f = getch();
                if(c_f == 27) {
                    wclear(message_bar);
                    mvwprintw(message_bar, 0, 0, "HELP : Press 'CTRL+Q' to exit | Press 'CTRL+S' to save | Press 'CTRL+F' to find");
                    wrefresh(message_bar);
                    break;
                }
                else if(c_f == KEY_BACKSPACE || c_f == 127) {
                    if(find_str_index > 0) {
                        find_str[--find_str_index] = '\0';
                        wclear(message_bar);
                        mvwprintw(message_bar, 0, 0, "Search for : ");
                        mvwprintw(message_bar, 0, 13, "%s", find_str);
                        wrefresh(message_bar);
                        wmove(message_bar, 0, 13 + find_str_index);
                        wrefresh(message_bar);
                        continue;
                    } 
                    else {
                        continue;
                    }
                }

                
                else if (c_f == '\n') {
                 //엔터를 누르면 탐색 관련 입력을 그만 받음
                    wclear(win);
                    int c_fs = 0;
                    searched_indecies = find_string_ddl(head, find_str, strlen(find_str));
                    XYi = return_xy_for_node_index(head, searched_indecies->array[0], max_col);
                    start_line = XYi->line;
                    searching_indecies_array = output_f(win, head, XYi->line, max_row, max_col, searched_indecies, strlen(find_str), 0);
                    subindex_array = check_line(head, XYi->line, XYi->line + max_row, max_col);
                    wmove(win, XYi->subline + 1, XYi->x_index - 1);
                    wrefresh(win);
                    while(1) {
                        FILE* logf = fopen("logging.txt", "a+");
                        c_fs = getch();
                        if(c_fs == 27) {
                            fclose(logf);
                            break;
                        }
                        else if(c_fs == KEY_DOWN) {
                            if(cursor_searching_index == searched_indecies->length) {
                                continue;
                            } else if(cursor_searching_index <= searching_indecies_array[1]) {
                                XYi = return_xy_for_node_index(head, searched_indecies->array[cursor_searching_index], max_col);
                                for(int qkqk = 0; qkqk < XYi->line - start_line; qkqk++) {
                                    prev_line_count += subindex_array[qkqk];
                                    fprintf(logf, "subindex_array : %d\n", subindex_array[qkqk]);
                                }
                                fprintf(logf, "prev_line_count : %d\n", prev_line_count);
                                fprintf(logf, "XYi->subline : %d\n", XYi->subline);
                                temp_node_index = searched_indecies->array[cursor_searching_index];
                                temp_cur_y = prev_line_count + XYi->subline + 1;
                                temp_cur_x = XYi->x_index - 1; 
                                wrefresh(status_bar);
                                wmove(win, temp_cur_y, temp_cur_x);
                                wrefresh(win); 
                                cursor_searching_index++;
                                prev_line_count = 0;
                            } else {

                                searching_index = searching_indecies_array[1];
                                searching_index++;
                                XYi = return_xy_for_node_index(head, searched_indecies->array[searching_index], max_col);
                                start_line = XYi->line;
                                searching_indecies_array = output_f(win, head, XYi->line, max_row, max_col, searched_indecies, strlen(find_str), searching_index);
                                subindex_array = check_line(head, XYi->line, XYi->line + max_row, max_col);
                                temp_node_index = searched_indecies->array[cursor_searching_index];

                                temp_cur_y = XYi->subline + 1;
                                temp_cur_x = XYi->x_index - 1;
                                wmove(win, temp_cur_y, temp_cur_x);
                                wrefresh(win);
                                cursor_searching_index++;
                            }
                        }         
                        else if(c_fs == KEY_UP) {
                            if(cursor_searching_index == searched_indecies->length) {
                                continue;
                            } else if(cursor_searching_index <= searching_indecies_array[1]) {
                                XYi = return_xy_for_node_index(head, searched_indecies->array[cursor_searching_index], max_col);
                                for(int qkqk = 0; qkqk < XYi->line - start_line; qkqk++) {
                                    prev_line_count += subindex_array[qkqk];
                                }
                                wrefresh(status_bar);
                                temp_cur_y = prev_line_count + XYi->subline + 1;
                                temp_cur_x = XYi->x_index - 1; 
                                temp_node_index = searched_indecies->array[cursor_searching_index];
                                wmove(win, temp_cur_y, temp_cur_x);
                                wrefresh(win); 
                                cursor_searching_index--;
                                prev_line_count = 0;
                            } else {

                                searching_index = searching_indecies_array[0];
                                searching_index--;
                                XYi = return_xy_for_node_index(head, searched_indecies->array[searching_index], max_col);
                                start_line = XYi->line;
                                searching_indecies_array = output_f(win, head, XYi->line, max_row, max_col, searched_indecies, strlen(find_str), searching_index);
                                subindex_array = check_line(head, XYi->line, XYi->line + max_row, max_col);
                                temp_node_index = searched_indecies->array[cursor_searching_index];
                                wmove(win, XYi->subline + 1, XYi->x_index - 1);
                                wrefresh(win);
                                cursor_searching_index--;
                            }
                        
                        }
                        else if((c_fs >= 'a' && c_fs <= 'z') || (c_fs <= 'Z' && c_fs >= 'A') || (c_fs >= '0' && c_fs <= '9') || c_fs == ' ' || c_fs == '.' || c_fs == ',' ||
                        c_fs == '?' || c_fs == '!' || c_fs == '@' || c_fs == '#' || c_fs == '$' || c_fs == '%' || c_fs == '^' || c_fs == '&' || c_fs == '*' || c_fs == '(' 
                        || c_fs == ')' || c_fs == '-' || c_fs == '_' || c_fs == '+' || c_fs == '=' || c_fs == '<' || c_fs == '>' || c_fs == '/' || c_fs == '|' || c_fs == '\\' 
                        || c_fs == '[' || c_fs == ']' || c_fs == '{' || c_fs == '}' || c_fs == ':' || c_fs == ';' || c_fs == '"' || c_fs == '\'') {
                            change_count++;
                            head = insertAtn(head, temp_node_index, c_fs);
                            temp_node_index = searched_indecies->array[cursor_searching_index];
                            temp_cur_x++;
                            wmove(win, temp_cur_y, temp_cur_x);
                            
                            //searched_indecies = find_string_ddl(head, find_str, strlen(find_str));
                            
                            wrefresh(win);
                            for(int tempi1 = cursor_searching_index; tempi1 < searched_indecies->length; tempi1++) {
                                searched_indecies->array[tempi1]++;
                            }
                            searching_indecies_array = output_f(win, head, XYi->line, max_row, max_col, searched_indecies, strlen(find_str), searching_index);
                            subindex_array = check_line(head, XYi->line, XYi->line + max_row, max_col);
                        }
                        else if(c_fs == KEY_BACKSPACE || c_fs == 127) {
                            change_count--;
                            head = deleteNode(head, temp_node_index + (temp_cur_x - XYi->x_index + 1));
                            temp_cur_x--;
                            wmove(win, temp_cur_y, temp_cur_x);

                            //searched_indecies = find_string_ddl(head, find_str, strlen(find_str));
                            
                            wrefresh(win);
                            for(int tempi1 = cursor_searching_index; tempi1 < searched_indecies->length; tempi1++) {
                                searched_indecies->array[tempi1]--;
                            }
                            searching_indecies_array = output_f(win, head, XYi->line, max_row, max_col, searched_indecies, strlen(find_str), searching_index);
                            subindex_array = check_line(head, XYi->line, XYi->line + max_row, max_col);
                        }
                        else if(c_fs == KEY_LEFT) {
                            temp_cur_x--;
                            temp_node_index--;
                            wmove(win, temp_cur_y, temp_cur_x);
                            wrefresh(win);
                        }
                        else if(c_fs == KEY_RIGHT) {
                            temp_cur_x++;
                            temp_node_index++;
                            wmove(win, temp_cur_y, temp_cur_x);
                            wrefresh(win);
                        }
                        fclose(logf);         
                    }

                }
            
  
               	find_str[find_str_index] = c_f;
                find_str[find_str_index + 1] = '\0';
                find_str_index++;
                wclear(message_bar);
                mvwprintw(message_bar, 0, 0, "Search for : ");
                mvwprintw(message_bar, 0, 13, "%s", find_str);
                wrefresh(message_bar);
                wmove(message_bar, 0, 13 + find_str_index);
                wrefresh(message_bar);
                    

            
                wrefresh(status_bar);
            }
            free(XYi);
            free(find_str);
        }


    }
    endwin();
    free(Yi);
    free(filename);
    free(win);
    free(head);
    return 0;
}



#endif

