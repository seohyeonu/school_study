#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZ_CUSTOM 1024
#define RECORD_SIZE 23

static void removeComma(char *scoreField) {
    for (int i = 0; i < 3; i++) {
        if (scoreField[i] == ',') {
            scoreField[i] = '\0';
            break;
        }
    }
}

void int2String(char *head) {
    int len = strlen(head);
    for (int i = len; i < 3; i++) {
        head[i] = ',';
    }
    head[3] = '\0';
}

int scoreInputCheck(int score) {
    if (score < 0 || score > 100) {
        printf("잘못된 입력입니다. (0~100 사이의 정수이어야 합니다.)\n");
        return -1;
    }
    return 0;
}

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

void Insert(FILE *fpBak) {
    char student[100], st_no[100];
    int score[5];
    char ko_st[4], en_st[4], math_st[4], so_st[4], sc_st[4];
    char temp;

    printf("입력 모드로 진입합니다.\n");

    while (1) {
        // 1) 학번 입력
        printf("학번을 입력하세요 (8자리, 종료하려면 '0' 입력): ");
        scanf("%s", st_no);
        if (strlen(st_no) == 1 && st_no[0] == '0') {
            printf("입력모드를 종료합니다.\n");
            return;
        }
        if (strlen(st_no) != 8) {
            printf("잘못된 학번입니다. 다시 입력하세요.\n");
            continue;
        }
        int valid = 1;
        for (int i = 0; i < 8; i++) {
            if (st_no[i] < '0' || st_no[i] > '9') {
                printf("학번에 숫자가 아닌 문자가 포함되었습니다.\n");
                valid = 0; break;
            }
        }
        if (!valid) continue;

        // 2) 성적 입력
        while (1) {
            printf("성적을 입력하세요 (국어 영어 수학 사회 과학, 0~100): ");
            if (scanf("%d %d %d %d %d",
                      &score[0], &score[1], &score[2],
                      &score[3], &score[4]) != 5) {
                printf("잘못된 입력입니다. 정수 5개를 입력하세요.\n");
                while(getchar()!='\n');
                continue;
            }
            int ok = 1;
            for (int i = 0; i < 5; i++)
                if (scoreInputCheck(score[i]) < 0) ok = 0;
            if (!ok) continue;
            break;
        }

        // 3) 확인
        printf("입력하신 점수: %d %d %d %d %d\n",
               score[0], score[1], score[2], score[3], score[4]);
        printf("계속 입력하려면 Y, 재입력하려면 N을 입력: ");
        scanf(" %c", &temp);
        if (temp!='Y' && temp!='y') {
            printf("다시 처음부터 입력합니다.\n");
            continue;
        }

        // 4) 레코드 문자열 생성
        sprintf(ko_st, "%d", score[0]); int2String(ko_st);
        sprintf(en_st, "%d", score[1]); int2String(en_st);
        sprintf(math_st, "%d", score[2]); int2String(math_st);
        sprintf(so_st, "%d", score[3]); int2String(so_st);
        sprintf(sc_st, "%d", score[4]); int2String(sc_st);

        memset(student, 0, sizeof(student));
        strcat(student, st_no);
        strcat(student, ko_st);
        strcat(student, en_st);
        strcat(student, math_st);
        strcat(student, so_st);
        strcat(student, sc_st);

        // 5) 중복 학번 검사
        fseek(fpBak, 0, SEEK_SET);
        char recordBuf[RECORD_SIZE+1];
        int found = 0;
        long pos = 0;
        while (fread(recordBuf, 1, RECORD_SIZE, fpBak) == RECORD_SIZE) {
            if (strncmp(recordBuf, st_no, 8) == 0) {
                found = 1;
                pos = ftell(fpBak) - RECORD_SIZE;
                break;
            }
        }

        if (found) {
            // 6) 사용자 선택: 취소 vs 덮어쓰기
            printf("이미 학번[%s]이(가) 존재합니다. 취소하려면 C, 덮어쓰기하려면 O를 입력: ", st_no);
            scanf(" %c", &temp);
            if (temp=='C' || temp=='c') {
                printf("입력을 취소하고 다음 항목으로 넘어갑니다.\n");
                continue;  // while(1) 의 다음 loop
            }
            // temp=='O'인 경우만 덮어쓰기
            fseek(fpBak, pos, SEEK_SET);
            fwrite(student, 1, RECORD_SIZE, fpBak);
            printf("기존 레코드를 덮어썼습니다.\n");
        } else {
            // 7) 신규 레코드 추가
            fseek(fpBak, 0, SEEK_END);
            fwrite(student, 1, RECORD_SIZE, fpBak);
            printf("새 레코드를 추가했습니다.\n");
        }
    }
}

void Modify(FILE *fpBak)
{
    char st_no[100];
    char recordBuf[RECORD_SIZE+1];
    printf("수정/삭제 모드로 진입합니다.\n");

    while (1) {
        printf("\n수정/삭제할 학번을 입력하세요 (8자리, 종료하려면 '0' 입력): ");
        scanf("%s", st_no);

        if (strlen(st_no) == 1 && st_no[0] == '0') {
            printf("수정/삭제 모드를 종료합니다.\n");
            return;
        }
        if (strlen(st_no) != 8) {
            printf("학번은 8자리여야 합니다. 다시 입력하세요.\n");
            continue;
        }

        // 파일 전체 레코드를 메모리로 로드
        fseek(fpBak, 0, SEEK_END);
        long fileSize = ftell(fpBak);
        int totalRecs = fileSize / RECORD_SIZE;
        if (totalRecs == 0) {
            printf("저장된 레코드가 없습니다.\n");
            return;
        }
        rewind(fpBak);

        char (*allRecs)[RECORD_SIZE] = malloc(totalRecs * RECORD_SIZE);
        fread(allRecs, RECORD_SIZE, totalRecs, fpBak);

        // 대상 레코드 찾기
        int idx = -1;
        for (int i = 0; i < totalRecs; i++) {
            if (strncmp(allRecs[i], st_no, 8) == 0) {
                idx = i;
                break;
            }
        }
        if (idx < 0) {
            printf("해당 학번 [%s]이(가) 없습니다.\n", st_no);
            free(allRecs);
            continue;
        }

        // 메뉴
        printf("\n1) 전체 성적 재입력\n");
        printf("2) 특정 과목만 수정\n");
        printf("3) 레코드 삭제\n");
        printf("4) 취소 (메뉴로 돌아가기)\n");
        printf("선택: ");
        int choice;
        scanf("%d", &choice);

        if (choice == 4) {
            free(allRecs);
            continue;
        }
        if (choice == 3) {
            // 삭제: 배열에서 idx 항목 빼고 다시 파일 덮어쓰기
            fpBak = freopen(NULL, "wb+", fpBak);
            for (int i = 0; i < totalRecs; i++) {
                if (i == idx) continue;
                fwrite(allRecs[i], 1, RECORD_SIZE, fpBak);
            }
            printf("학번 [%s] 레코드를 삭제했습니다.\n", st_no);
            free(allRecs);
            return;
        }

        // 수정: 전체 or 부분
        int score[5];
        // 파싱된 기존 점수
        char tmp[4];
        for (int subj = 0; subj < 5; subj++) {
            strncpy(tmp, allRecs[idx] + 8 + subj*3, 3);
            tmp[3] = '\0'; removeComma(tmp);
            score[subj] = atoi(tmp);
        }

        if (choice == 1) {
            // 전체 재입력
            while (1) {
                printf("새 성적 입력 (국어 영어 수학 사회 과학, 0~100):\n");
                if (scanf("%d %d %d %d %d",
                          &score[0], &score[1], &score[2],
                          &score[3], &score[4]) != 5) {
                    printf("잘못된 입력, 정수 5개를 입력하세요.\n");
                    while(getchar()!='\n');
                    continue;
                }
                int ok = 1;
                for (int i = 0; i < 5; i++)
                    if (scoreInputCheck(score[i])<0) ok = 0;
                if (!ok) continue;
                break;
            }
        } else if (choice == 2) {
            // 특정 과목만
            static const char *names[5] = {"국어","영어","수학","사회","과학"};
            printf("수정할 과목 선택:\n");
            for (int i = 0; i < 5; i++)
                printf("%d) %s\n", i+1, names[i]);
            int sub;
            scanf("%d", &sub);
            if (sub < 1 || sub > 5) {
                printf("잘못된 과목 번호입니다.\n");
                free(allRecs);
                continue;
            }
            while (1) {
                printf("%s 점수 새로 입력 (0~100): ", names[sub-1]);
                int v; scanf("%d",&v);
                if (scoreInputCheck(v)==0) {
                    score[sub-1] = v;
                    break;
                }
            }
        } else {
            printf("잘못된 선택입니다.\n");
            free(allRecs);
            continue;
        }

        // 새 레코드 생성
        char newRec[RECORD_SIZE];
        char tmpStr[4];
        memcpy(newRec, st_no, 8);
        for (int i = 0; i < 5; i++) {
            sprintf(tmpStr, "%d", score[i]);
            int2String(tmpStr);
            memcpy(newRec+8+i*3, tmpStr, 3);
        }

        // 메모리 배열에 덮어쓰기, 파일 전체 덮어쓰기
        memcpy(allRecs[idx], newRec, RECORD_SIZE);
        fpBak = freopen(NULL, "wb+", fpBak);
        for (int i = 0; i < totalRecs; i++) {
            fwrite(allRecs[i], 1, RECORD_SIZE, fpBak);
        }
        printf("학번 [%s] 레코드를 수정했습니다.\n", st_no);
        free(allRecs);
        return;
    }
}

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

    /* Score.bat -> Score.bak 복사 */
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

                /* 종료 전, Score.bak의 내용을 Score.bat에 덮어쓰기*/
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
