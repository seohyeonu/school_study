#include <stdio.h>
#include "unistd.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

void Insert(){
    FILE *rfp = NULL;
    FILE *wfp = NULL;
    char buf[BUFSIZ];
    char st_no[200] = {"\0", };
    char ko[3], en[3], math[3], so[3], sc[3];

    if((rfp = fopen("Score.bat", "wrb")) == NULL){
        perror("Open Fail!\n");
        exit(1);
    }
    if((wfp = fopen("Score.bak", "wrb")) == NULL){
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
            if(st_no[1] == 0){
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
