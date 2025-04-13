#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

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
    // 숫자 문자열 길이가 3보다 작으면 남은 자리를 ','로 채움
    for (int i = len; i < 3; i++) {
        head[i] = ',';
    }
    // 3자리 이후에 '\0' 추가
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
    // record[0..7]   = 학번
    // record[8..10]  = 국어
    // record[11..13] = 영어
    // record[14..16] = 수학
    // record[17..19] = 사회
    // record[20..22] = 과학

    // 과목명에 따라 인덱스 계산
    int offset = 0;  // record에서 시작 인덱스
    if (strcmp(subject, "국어") == 0) {
        offset = 8;
    } else if (strcmp(subject, "영어") == 0) {
        offset = 11;
    } else if (strcmp(subject, "수학") == 0) {
        offset = 14;
    } else if (strcmp(subject, "사회") == 0) {
        offset = 17;
    } else if (strcmp(subject, "과학") == 0) {
        offset = 20;
    } else {
        // 잘못된 과목명이라면 scoreBuf에 빈 문자열을 넣고 반환
        strcpy(scoreBuf, "");
        return;
    }

    // 추출
    strncpy(scoreBuf, record + offset, 3);
    scoreBuf[3] = '\0';
    removeComma(scoreBuf);
}

/* Insert 함수: 새 학번이든 중복 학번이든 입력받아
   파일 내에 없으면 레코드 추가, 있으면 해당 위치에 덮어쓰는 예제.
   (원하는 로직에 따라 Insert와 Modify를 분리해도 됨) */
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
        // 학번 길이 검사
        if (strlen(st_no) != 8) {
            // '0' 입력 시 Insert 종료
            if (strlen(st_no) == 1 && st_no[0] == '0') {
                printf("입력모드를 종료합니다.\n");
                return;
            }
            printf("잘못된 학번입니다. 다시 입력하세요.\n");
            continue;
        }
        // 학번이 숫자인지 검사
        int validStudent = 1;
        for (int i = 0; i < 8; i++) {
            if (st_no[i] < '0' || st_no[i] > '9') {
                printf("학번에 숫자외 다른 문자가 포함되었습니다. 다시 입력해 주세요.\n");
                validStudent = 0;
                break;
            }
        }
        if (!validStudent) continue;

        input_score:
        while (1) {
            int result;
            printf("성적을 입력하세요.\n");
            printf("성적은 국어, 영어, 수학, 사회, 과학 순으로 정수(0~100)를 입력하세요:\n");
            result = scanf("%d %d %d %d %d",
                           &score[0], &score[1], &score[2], &score[3], &score[4]);
            if (result != 5) {
                printf("잘못된 입력입니다. 정수만 입력하세요.\n");
                while(getchar() != '\n'); // 입력 버퍼 비우기
                continue;
            }
            // 각 과목 점수 범위 검사
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

        // 점수를 3자리 문자열로 변환
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

        // 레코드 생성 (23바이트)
        memset(student, 0, sizeof(student));
        strcat(student, st_no);
        strcat(student, ko_st);
        strcat(student, en_st);
        strcat(student, math_st);
        strcat(student, so_st);
        strcat(student, sc_st);

        // 중복 학번 검사 (이미 Insert에서 중복처리를 하려면)
        fseek(fpBak, 0, SEEK_SET);
        char recordBuf[24];
        int found = 0;
        long pos = 0;
        while (fread(recordBuf, 1, 23, fpBak) == 23) {
            recordBuf[23] = '\0';
            if (strncmp(recordBuf, st_no, 8) == 0) {
                found = 1;
                // 해당 레코드의 시작 위치
                pos = ftell(fpBak) - 23;
                break;
            }
        }
        if (found) {
            // 덮어쓰기
            fseek(fpBak, pos, SEEK_SET);
            fwrite(student, 1, 23, fpBak);
            printf("이미 존재하는 학번이므로 기존 레코드가 갱신되었습니다.\n");
        } else {
            // 파일 끝에 신규 추가
            fseek(fpBak, 0, SEEK_END);
            fwrite(student, 1, 23, fpBak);
            printf("새 레코드가 추가되었습니다.\n");
        }
    }
}

/* 수정 모드 */
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

        // 종료 조건
        if (strlen(st_no) == 1 && st_no[0] == '0') {
            printf("수정 모드를 종료합니다.\n");
            return;
        }

        // 학번 길이/숫자 여부 검사
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

        // 파일에서 해당 학번 검색
        fflush(fpBak);
        fseek(fpBak, 0, SEEK_SET);

        int found = 0;
        long pos = 0;
        // 23바이트씩 읽으며 학번 비교
        while (fread(recordBuf, 1, 23, fpBak) == 23) {
            recordBuf[23] = '\0';  // null-terminate
            if (strncmp(recordBuf, st_no, 8) == 0) {
                found = 1;
                // 해당 레코드 시작 위치
                pos = ftell(fpBak) - 23;
                break;
            }
        }

        if (!found) {
            printf("해당 학번 [%s] 은(는) 파일에 존재하지 않습니다.\n", st_no);
            continue;
        }

        // 해당 학번 레코드를 찾아냈으므로, 새 성적 입력
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

        // 레코드 새로 구성 (학번 + 5과목)
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

        // 최종 레코드 (23바이트)
        strcat(student, st_no);
        strcat(student, ko_st);
        strcat(student, en_st);
        strcat(student, math_st);
        strcat(student, so_st);
        strcat(student, sc_st);

        // 해당 레코드 위치로 이동 후 덮어쓰기
        fseek(fpBak, pos, SEEK_SET);
        fwrite(student, 1, 23, fpBak);

        printf("학번 [%s] 의 레코드가 수정되었습니다.\n", st_no);
        // 수정 모드는 계속해서 다른 학번 수정 가능
    }
}

/* Print 함수: 23바이트 레코드(학번8 + 과목5*3)를 읽어 화면에 출력 */
void Print(FILE *fpBak) {
    int choice = 0;
    char record[24];

    while (1) {
        // 소메뉴
        printf("\n[출력 모드]\n");
        printf("1. 전체 성적 출력\n");
        printf("2. 특정 학번 검색\n");
        printf("3. 특정 학번 + 과목 검색\n");
        printf("4. 메인 메뉴로 돌아가기\n");
        printf("원하는 메뉴를 선택하세요: ");
        scanf("%d", &choice);

        // 메뉴 선택 처리
        if (choice == 4) {
            // 메인 메뉴로 복귀
            printf("출력 모드를 종료하고 메인 메뉴로 돌아갑니다.\n");
            return;
        }

        // 항상 파일 포인터를 다시 처음으로
        fflush(fpBak);
        fseek(fpBak, 0, SEEK_SET);

        if (choice == 1) {
            // 1) 전체 성적 출력
            int recordCount = 0;
            while (fread(record, 1, 23, fpBak) == 23) {
                record[23] = '\0';
                // 학번 + 과목 점수들 파싱
                char studentId[9];
                char korean[4], english[4], math[4], social[4], science[4];
                strncpy(studentId, record, 8); studentId[8] = '\0';
                strncpy(korean,  record+8,  3); korean[3]  = '\0';
                strncpy(english, record+11, 3); english[3] = '\0';
                strncpy(math,    record+14, 3); math[3]    = '\0';
                strncpy(social,  record+17, 3); social[3]  = '\0';
                strncpy(science, record+20, 3); science[3] = '\0';

                // 점수 끝의 ',' 제거
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
            // 2) 특정 학번 검색
            char targetId[20];
            printf("검색할 학번(8자리) 입력: ");
            scanf("%s", targetId);

            if (strlen(targetId) != 8) {
                printf("학번은 8자리이어야 합니다.\n");
                continue;
            }

            int found = 0;
            while (fread(record, 1, 23, fpBak) == 23) {
                record[23] = '\0';
                if (strncmp(record, targetId, 8) == 0) {
                    // 학번이 일치하면 해당 레코드 출력
                    char studentId[9];
                    char korean[4], english[4], math[4], social[4], science[4];
                    strncpy(studentId, record, 8); studentId[8] = '\0';
                    strncpy(korean,  record+8,  3); korean[3]  = '\0';
                    strncpy(english, record+11, 3); english[3] = '\0';
                    strncpy(math,    record+14, 3); math[3]    = '\0';
                    strncpy(social,  record+17, 3); social[3]  = '\0';
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
            // 3) 학번 + 특정 과목 검색
            char targetId[20];
            char subjectName[20];
            printf("검색할 학번(8자리) 입력: ");
            scanf("%s", targetId);
            printf("검색할 과목명 (국어/영어/수학/사회/과학): ");
            scanf("%s", subjectName);

            if (strlen(targetId) != 8) {
                printf("학번은 8자리이어야 합니다.\n");
                continue;
            }

            // 파일 전체 레코드 탐색
            int found = 0;
            while (fread(record, 1, 23, fpBak) == 23) {
                record[23] = '\0';
                if (strncmp(record, targetId, 8) == 0) {
                    found = 1;
                    // 학번이 일치 -> 해당 레코드에서 과목 점수만 추출
                    char scoreBuf[4];
                    getSubjectScore(record, subjectName, scoreBuf);

                    if (strlen(scoreBuf) == 0) {
                        // 과목명이 잘못되었거나 없는 경우
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
            // 잘못된 메뉴 선택
            printf("잘못된 메뉴 번호입니다. 다시 선택하세요.\n");
        }
    }
}

int main(void) {
    int menu = 0;
    FILE *fpBat, *fpBak;
    char buf[BUFSIZ];

    /* Score.bat 열기 (없으면 생성) */
    fpBat = fopen("Score.bat", "r");
    if (fpBat == NULL) {
        fpBat = fopen("Score.bat", "w+");
        if (fpBat == NULL) {
            perror("Score.bat 파일 생성 및 열기 실패");
            exit(EXIT_FAILURE);
        }
    }

    /* Score.bat -> Score.bak 복제 (w+ : 읽기/쓰기 모두 가능) */
    fpBak = fopen("Score.bak", "w+");
    if (fpBak == NULL) {
        perror("Score.bak 파일 열기 실패");
        fclose(fpBat);
        exit(EXIT_FAILURE);
    }
    while (fgets(buf, BUFSIZ, fpBat) != NULL) {
        fputs(buf, fpBak);
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
                // 종료 전 bak 내용 -> bat 반영
                fclose(fpBat);
                fclose(fpBak);

                fpBak = fopen("Score.bak", "r");
                if (fpBak == NULL) {
                    perror("Score.bak 재열기 실패");
                    exit(EXIT_FAILURE);
                }
                fpBat = fopen("Score.bat", "w");
                if (fpBat == NULL) {
                    perror("Score.bat 쓰기모드 열기 실패");
                    fclose(fpBak);
                    exit(EXIT_FAILURE);
                }
                while (fgets(buf, BUFSIZ, fpBak) != NULL) {
                    fputs(buf, fpBat);
                }
                fclose(fpBak);
                fclose(fpBat);
                return 0;
            default:
                printf("잘못된 입력입니다. 다시 입력해주세요.\n");
        }
    }
}
