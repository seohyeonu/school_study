#include <stdio.h>
#include "unistd.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

void int2String(char *head) {
    int len = strlen(head);
    // 만약 숫자 문자열의 길이가 3보다 작으면 나머지를 ','로 채움.
    for (int i = len; i < 3; i++) {
        head[i] = ',';
    }
    // 3자리 이후에 반드시 null 종료 문자 추가
    head[3] = '\0';
}

void Insert(){
    FILE *rfp = NULL;
    FILE *wfp = NULL;
    char buf[BUFSIZ], student[100]; memset(student, 0, sizeof(student));
    char st_no[200] = {"\0", };
    int score[5];
    char ko_st[4], en_st[4], math_st[4], so_st[4], sc_st[4];
    char temp;

    if((rfp = fopen("Score.bat", "a")) == NULL){
        perror("Open Fail!\n");
        exit(1);
    }
    if((wfp = fopen("Score.bak", "w")) == NULL){
        perror("Open Fail!\n");
        exit(1);
    }
    while (fgets(buf, BUFSIZ, rfp) != NULL){
        fputs(buf, wfp);
    }

    printf("입력 모드로 진입합니다.\n");

    while (1){
        printf("학번을 입력하세요, 학번은 숫자 8글자 입니다.\n");
        scanf("%s", st_no);
        // 학번 길이 예외처리
        if(strlen(st_no) != 8 ){
            if(strlen(st_no) == 1 && st_no[0] == '0'){
                printf("입력모드를 종료합니다.\n");
                return;
            }
            printf("잘못된 학번입니다. 다시 입력하세요.\n");
            continue;
        }
        // 입력 문자에 대한 예외처리
        for(int i=0; i<8; i++){
            if(st_no[i] >= '0' && st_no[i] <= '9') {
                continue;
            }
            else printf("학번에 숫자외 다른 문자가 포함되었습니다. 다시 입력해 주세요.\n");
        }

        //학번 중복 검사
        input:
        while (1){
            int is_okay=1;
            printf("성적을 입력하세요. 성적은 국어, 영어, 수학, 사회, 과학 순으로 입력하세요.\n");
            scanf("%d %d %d %d %d", &score[0], &score[1], &score[2], &score[3], &score[4]);
            for(int i=0; i<5; i++){
                if(score[i] < 0 || score[i] > 100){
                    printf("잘못된 점수 입니다. 점수의 범위는 0이상 100이하입니다.\n");
                    is_okay = 0;
                    break;
                }
            }
            if(is_okay == 1) break;
        };
        printf("입력하신 성적은 아래와 같습니다. 맞다면 y 다시 입력하시려면 n을 눌려주세요\n");
        printf("%d %d %d %d %d\n", score[0], score[1], score[2], score[3], score[4]);
        scanf("%c", &temp);
        scanf("%c", &temp);
        if(temp == 'y'){
            sprintf(ko_st, "%d", score[0]);
            sprintf(en_st, "%d", score[1]);
            sprintf(math_st, "%d", score[2]);
            sprintf(so_st, "%d", score[3]);
            sprintf(sc_st, "%d", score[4]);
            int2String(ko_st); int2String(en_st); int2String(math_st); int2String(so_st); int2String(sc_st);
        }
        else goto input;
        memset(student, 0, sizeof(student));
        strcat(student, st_no); strcat(student, ko_st); strcat(student, en_st); strcat(student, math_st); strcat(student, so_st); strcat(student, sc_st);
        fwrite(student, sizeof (char ), 23, rfp);
    }

}

void Modify(){
    printf("수정 모드로 진입합니다.\n");
}

void Print(){
    printf("출력 모드로 진입합니다.\n");


}

int main(void) {
    int menu=0;

    while (1){
        printf("입력 모드: 1, 수정 모드: 2, 출력 모드: 3, 종료: 4\n");
        scanf("%d", &menu);

        switch (menu) {
            case 1:
                Insert();
                break;
            case 2:
                Modify();
                break;
            case 3:
                Print();
                break;
            case 4:
                printf("종료합니다.\n");
                return 0;
            default:
                printf("잘못 된 입력입니다. 다시 입력해 주세요\n");
        }
    }
}
