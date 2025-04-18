#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZ_CUSTOM 1024
#define RECORD_SIZE 23

/* 쉼표 제거: "49," -> "49" */
static void removeComma(char *scoreField) {
    for (int i = 0; i < 3; i++) {
        if (scoreField[i] == ',') {
            scoreField[i] = '\0';
            break;
        }
    }
}

/* 정수를 문자열로 변환 후, 길이가 3보다 작으면 부족한 자리를 ','로 채우고 마지막에 '\0' */
void int2String(char *head) {
    int len = strlen(head);
    for (int i = len; i < 3; i++) {
        head[i] = ',';
    }
    head[3] = '\0';
}

/* 성적이 0~100 범위 안에 있는지 검사 */
int scoreInputCheck(int score) {
    if (score < 0 || score > 100) {
        printf("잘못된 입력입니다. (0~100 사이의 정수이어야 합니다.)\n");
        return -1;
    }
    return 0;
}

/* record(23바이트)에서 주어진 과목("국어", "영어", "수학", "사회", "과학")의 점수를 scoreBuf에 저장 */
void getSubjectScore(const char *record, const char *subject, char *scoreBuf)
{
    int offset = 0;
    if (strcmp(subject, "국어") == 0)       offset = 8;
    else if (strcmp(subject, "영어") == 0)  offset = 11;
    else if (strcmp(subject, "수학") == 0)  offset = 14;
    else if (strcmp(subject, "사회") == 0)  offset = 17;
    else if (strcmp(subject, "과학") == 0)  offset = 20;
    else {
        strcpy(scoreBuf, "");
        return;
    }
    strncpy(scoreBuf, record + offset, 3);
    scoreBuf[3] = '\0';
    removeComma(scoreBuf);
}

/* Insert 함수
   - 사용자로부터 학번과 5과목 성적을 입력받아 23바이트 레코드를 만듭니다.
   - Score.bak(바이너리 파일)에서 동일 학번을 검색하여 있으면 해당 위치에 덮어쓰고,
     없으면 파일 끝에 새 레코드를 추가합니다.
*/
void Insert(FILE *fpBak) {
    char student[100];
    char st_no[100];
    int score[5];
    char ko_st[4], en_st[4], math_st[4], so_st[4], sc_st[4];
    char temp;

    printf("입력 모드로 진입합니다.\n");

    while (1) {
        printf("학번을 입력하세요 (8자리, 종료하려면 '0' 입력):\n");
        scanf("%s", st_no);
        if (strlen(st_no) != 8) {
            if (strlen(st_no) == 1 && st_no[0] == '0') {
                printf("입력모드를 종료합니다.\n");
                return;
            }
            printf("잘못된 학번입니다. 다시 입력하세요.\n");
            continue;
        }
        int validStudent = 1;
        for (int i = 0; i < 8; i++) {
            if (st_no[i] < '0' || st_no[i] > '9') {
                printf("학번에 숫자 외 다른 문자가 포함되었습니다. 다시 입력해 주세요.\n");
                validStudent = 0;
                break;
            }
        }
        if (!validStudent)
            continue;

        input_score:
        while (1) {
            int result;
            printf("성적을 입력하세요.\n");
            printf("국어, 영어, 수학, 사회, 과학 순으로 정수(0~100)를 입력하세요:\n");
            result = scanf("%d %d %d %d %d",
                           &score[0], &score[1], &score[2], &score[3], &score[4]);
            if (result != 5) {
                printf("잘못된 입력입니다. 정수만 입력하세요.\n");
                while(getchar() != '\n');
                continue;
            }
            int validScore = 1;
            for (int i = 0; i < 5; i++) {
                if (scoreInputCheck(score[i]) == -1) {
                    validScore = 0;
                    break;
                }
            }
            if (!validScore) {
                printf("입력한 성적 중 범위를 벗어난 값이 있습니다. 다시 입력해주세요.\n");
                continue;
            }
            break;
        }

        printf("입력하신 성적은 아래와 같습니다.\n");
        printf("%d %d %d %d %d\n", score[0], score[1], score[2], score[3], score[4]);
        printf("맞다면 y, 다시 입력하시려면 n: ");
        scanf(" %c", &temp);

        if (temp != 'y' && temp != 'Y') {
            printf("다시 성적을 입력합니다.\n");
            goto input_score;
        }

        /* 점수를 3자리 문자열로 변환 */
        sprintf(ko_st, "%d", score[0]);
        sprintf(en_st, "%d", score[1]);
        sprintf(math_st, "%d", score[2]);
        sprintf(so_st, "%d", score[3]);
        sprintf(sc_st, "%d", score[4]);
        int2String(ko_st);
        int2String(en_st);
        int2String(math_st);
        int2String(so_st);
        int2String(sc_st);

        /* 학번과 과목 문자열을 결합하여 23바이트 레코드 생성 */
        memset(student, 0, sizeof(student));
        strcat(student, st_no);
        strcat(student, ko_st);
        strcat(student, en_st);
        strcat(student, math_st);
        strcat(student, so_st);
        strcat(student, sc_st);

        /* 동일 학번이 이미 있는지 검사 */
        fseek(fpBak, 0, SEEK_SET);
        char recordBuf[24];
        int found = 0;
        long pos = 0;
        while (fread(recordBuf, 1, RECORD_SIZE, fpBak) == RECORD_SIZE) {
            recordBuf[RECORD_SIZE] = '\0';
            if (strncmp(recordBuf, st_no, 8) == 0) {
                found = 1;
                pos = ftell(fpBak) - RECORD_SIZE;
                break;
            }
        }
        if (found) {
            fseek(fpBak, pos, SEEK_SET);
            fwrite(student, 1, RECORD_SIZE, fpBak);
            printf("이미 존재하는 학번이므로 기존 레코드가 갱신되었습니다.\n");
        } else {
            fseek(fpBak, 0, SEEK_END);
            fwrite(student, 1, RECORD_SIZE, fpBak);
            printf("새 레코드가 추가되었습니다.\n");
        }
    }
}

/* Modify 함수
   - 사용자로부터 수정할 학번을 입력받고, 파일에서 해당 레코드를 검색하여
     새 성적 입력 후 같은 형식의 23바이트 레코드를 덮어씁니다.
*/
void Modify(FILE *fpBak)
{
    char st_no[100];
    char recordBuf[24];
    int score[5];
    char temp;

    printf("수정 모드로 진입합니다.\n");

    while (1) {
        printf("\n수정할 학번을 입력하세요 (8자리, 종료하려면 '0' 입력): ");
        scanf("%s", st_no);

        if (strlen(st_no) == 1 && st_no[0] == '0') {
            printf("수정 모드를 종료합니다.\n");
            return;
        }
        if (strlen(st_no) != 8) {
            printf("잘못된 학번입니다. 다시 입력하세요.\n");
            continue;
        }
        int validStudent = 1;
        for (int i = 0; i < 8; i++) {
            if (st_no[i] < '0' || st_no[i] > '9') {
                printf("학번에 숫자 외 다른 문자가 포함되었습니다. 다시 입력해 주세요.\n");
                validStudent = 0;
                break;
            }
        }
        if (!validStudent) continue;

        fflush(fpBak);
        fseek(fpBak, 0, SEEK_SET);

        int found = 0;
        long pos = 0;
        while (fread(recordBuf, 1, RECORD_SIZE, fpBak) == RECORD_SIZE) {
            recordBuf[RECORD_SIZE] = '\0';
            if (strncmp(recordBuf, st_no, 8) == 0) {
                found = 1;
                pos = ftell(fpBak) - RECORD_SIZE;
                break;
            }
        }
        if (!found) {
            printf("해당 학번 [%s] 은(는) 파일에 존재하지 않습니다.\n", st_no);
            continue;
        }

        input_score:
        while (1) {
            printf("수정할 성적을 입력하세요.\n");
            printf("국어, 영어, 수학, 사회, 과학 순으로 정수(0~100)를 입력: ");
            int result = scanf("%d %d %d %d %d",
                               &score[0], &score[1], &score[2], &score[3], &score[4]);
            if (result != 5) {
                printf("잘못된 입력입니다. 정수만 입력하세요.\n");
                while(getchar() != '\n');
                continue;
            }
            int validScore = 1;
            for (int i = 0; i < 5; i++) {
                if (scoreInputCheck(score[i]) == -1) {
                    validScore = 0;
                    break;
                }
            }
            if (!validScore) {
                printf("입력한 성적 중 범위를 벗어난 값이 있습니다. 다시 입력해주세요.\n");
                continue;
            }
            break;
        }

        printf("수정할 성적은 다음과 같습니다.\n");
        printf("%d %d %d %d %d\n", score[0], score[1], score[2], score[3], score[4]);
        printf("맞다면 y, 다시 입력하시려면 n: ");
        scanf(" %c", &temp);

        if (temp != 'y' && temp != 'Y') {
            printf("다시 성적 입력으로 돌아갑니다.\n");
            goto input_score;
        }

        char student[100];
        memset(student, 0, sizeof(student));

        char ko_st[4], en_st[4], math_st[4], so_st[4], sc_st[4];
        sprintf(ko_st, "%d", score[0]);
        sprintf(en_st, "%d", score[1]);
        sprintf(math_st, "%d", score[2]);
        sprintf(so_st, "%d", score[3]);
        sprintf(sc_st, "%d", score[4]);
        int2String(ko_st);
        int2String(en_st);
        int2String(math_st);
        int2String(so_st);
        int2String(sc_st);

        strcat(student, st_no);
        strcat(student, ko_st);
        strcat(student, en_st);
        strcat(student, math_st);
        strcat(student, so_st);
        strcat(student, sc_st);

        fseek(fpBak, pos, SEEK_SET);
        fwrite(student, 1, RECORD_SIZE, fpBak);

        printf("학번 [%s] 의 레코드가 수정되었습니다.\n", st_no);
    }
}

/* Print 함수
   - 바이너리 파일 Score.bak에서 23바이트 단위로 레코드를 읽어 화면에 출력합니다.
   - 소메뉴를 통해 전체 출력, 특정 학번 검색, 학번+과목 검색 기능을 제공합니다.
*/
void Print(FILE *fpBak) {
    int choice = 0;
    char record[RECORD_SIZE + 1];

    while (1) {
        printf("\n[출력 모드]\n");
        printf("1. 전체 성적 출력\n");
        printf("2. 특정 학번 검색\n");
        printf("3. 특정 학번 + 과목 검색\n");
        printf("4. 메인 메뉴로 돌아가기\n");
        printf("원하는 메뉴를 선택하세요: ");
        scanf("%d", &choice);

        if (choice == 4) {
            printf("출력 모드를 종료하고 메인 메뉴로 돌아갑니다.\n");
            return;
        }

        fflush(fpBak);
        fseek(fpBak, 0, SEEK_SET);

        if (choice == 1) {
            int recordCount = 0;
            while (fread(record, 1, RECORD_SIZE, fpBak) == RECORD_SIZE) {
                record[RECORD_SIZE] = '\0';
                char studentId[9];
                char korean[4], english[4], math[4], social[4], science[4];
                strncpy(studentId, record, 8); studentId[8] = '\0';
                strncpy(korean,  record + 8, 3); korean[3] = '\0';
                strncpy(english, record + 11, 3); english[3] = '\0';
                strncpy(math,    record + 14, 3); math[3] = '\0';
                strncpy(social,  record + 17, 3); social[3] = '\0';
                strncpy(science, record + 20, 3); science[3] = '\0';

                removeComma(korean);
                removeComma(english);
                removeComma(math);
                removeComma(social);
                removeComma(science);

                printf("학번: \"%s\"  국어:\"%s\" 영어:\"%s\" 수학:\"%s\" 사회:\"%s\" 과학:\"%s\"\n",
                       studentId, korean, english, math, social, science);
                recordCount++;
            }
            if (recordCount == 0) {
                printf("파일에 저장된 데이터가 없습니다.\n");
            }

        } else if (choice == 2) {
            char targetId[20];
            printf("검색할 학번(8자리) 입력: ");
            scanf("%s", targetId);
            if (strlen(targetId) != 8) {
                printf("학번은 8자리이어야 합니다.\n");
                continue;
            }
            int found = 0;
            while (fread(record, 1, RECORD_SIZE, fpBak) == RECORD_SIZE) {
                record[RECORD_SIZE] = '\0';
                if (strncmp(record, targetId, 8) == 0) {
                    char studentId[9];
                    char korean[4], english[4], math[4], social[4], science[4];
                    strncpy(studentId, record, 8); studentId[8] = '\0';
                    strncpy(korean,  record+8, 3); korean[3] = '\0';
                    strncpy(english, record+11, 3); english[3] = '\0';
                    strncpy(math,    record+14, 3); math[3] = '\0';
                    strncpy(social,  record+17, 3); social[3] = '\0';
                    strncpy(science, record+20, 3); science[3] = '\0';

                    removeComma(korean);
                    removeComma(english);
                    removeComma(math);
                    removeComma(social);
                    removeComma(science);

                    printf("\n[검색 결과]\n");
                    printf("학번: \"%s\"  국어:\"%s\" 영어:\"%s\" 수학:\"%s\" 사회:\"%s\" 과학:\"%s\"\n\n",
                           studentId, korean, english, math, social, science);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("해당 학번 [%s]에 대한 데이터가 없습니다.\n", targetId);
            }

        } else if (choice == 3) {
            char targetId[20], subjectName[20];
            printf("검색할 학번(8자리) 입력: ");
            scanf("%s", targetId);
            printf("검색할 과목명 (국어/영어/수학/사회/과학): ");
            scanf("%s", subjectName);
            if (strlen(targetId) != 8) {
                printf("학번은 8자리이어야 합니다.\n");
                continue;
            }
            int found = 0;
            while (fread(record, 1, RECORD_SIZE, fpBak) == RECORD_SIZE) {
                record[RECORD_SIZE] = '\0';
                if (strncmp(record, targetId, 8) == 0) {
                    found = 1;
                    char scoreBuf[4];
                    getSubjectScore(record, subjectName, scoreBuf);
                    if (strlen(scoreBuf) == 0) {
                        printf("올바르지 않은 과목명입니다: [%s]\n", subjectName);
                    } else {
                        printf("\n[검색 결과]\n");
                        printf("학번: \"%s\"  %s: \"%s\"\n\n", targetId, subjectName, scoreBuf);
                    }
                    break;
                }
            }
            if (!found) {
                printf("해당 학번 [%s]에 대한 데이터가 없습니다.\n", targetId);
            }

        } else {
            printf("잘못된 메뉴 번호입니다. 다시 선택하세요.\n");
        }
    }
}

int main(void) {
    int menu = 0;
    char buf[BUFSIZ_CUSTOM];

    /* Score.bat 바이너리 파일 열기 (없으면 생성) */
    FILE *fpBat = fopen("Score.bat", "rb");
    if (fpBat == NULL) {
        fpBat = fopen("Score.bat", "wb+"); // 생성 후 읽기/쓰기 가능
        if (fpBat == NULL) {
            perror("Score.bat 파일 생성 및 열기 실패");
            exit(EXIT_FAILURE);
        }
    }

    /* Score.bat -> Score.bak 복사 (바이너리 모드, wb+ : 읽기/쓰기 모두 가능) */
    FILE *fpBak = fopen("Score.bak", "wb+");
    if (fpBak == NULL) {
        perror("Score.bak 파일 열기 실패");
        fclose(fpBat);
        exit(EXIT_FAILURE);
    }
    size_t n;
    while ((n = fread(buf, 1, BUFSIZ_CUSTOM, fpBat)) > 0) {
        if (fwrite(buf, 1, n, fpBak) != n) {
            perror("Score.bat -> Score.bak 복사 중 오류");
            fclose(fpBat);
            fclose(fpBak);
            exit(EXIT_FAILURE);
        }
    }

    /* 메뉴 반복 */
    while (1) {
        printf("\n----------------------------------\n");
        printf("1. 입력 모드\n");
        printf("2. 수정 모드\n");
        printf("3. 출력 모드\n");
        printf("4. 종료\n");
        printf("----------------------------------\n");
        printf("메뉴를 선택하세요: ");
        scanf("%d", &menu);

        switch (menu) {
            case 1:
                Insert(fpBak);
                break;
            case 2:
                Modify(fpBak);
                break;
            case 3:
                Print(fpBak);
                break;
            case 4:
                printf("종료합니다.\n");
                fclose(fpBat);
                fclose(fpBak);

                /* 종료 전, Score.bak의 내용을 Score.bat에 덮어쓰기 (바이너리 모드) */
                fpBak = fopen("Score.bak", "rb");
                if (fpBak == NULL) {
                    perror("Score.bak 재열기 실패");
                    exit(EXIT_FAILURE);
                }
                fpBat = fopen("Score.bat", "wb");
                if (fpBat == NULL) {
                    perror("Score.bat 쓰기모드 열기 실패");
                    fclose(fpBak);
                    exit(EXIT_FAILURE);
                }
                while ((n = fread(buf, 1, BUFSIZ_CUSTOM, fpBak)) > 0) {
                    if (fwrite(buf, 1, n, fpBat) != n) {
                        perror("Score.bak -> Score.bat 복사 중 오류");
                        fclose(fpBat);
                        fclose(fpBak);
                        exit(EXIT_FAILURE);
                    }
                }
                fclose(fpBak);
                fclose(fpBat);
                return 0;
            default:
                printf("잘못된 입력입니다. 다시 입력해주세요.\n");
        }
    }
}
