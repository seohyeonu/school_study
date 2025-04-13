#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECORD_SIZE 23  // 학번(8) + 과목 5개×3(15) = 23바이트
#define MAX_STUDENTS 1000  // 최대 학생 수 가정

/* 과목 인덱스: 0=국어, 1=영어, 2=수학, 3=사회, 4=과학 */
enum { KOR, ENG, MAT, SOC, SCI, SUBJECT_COUNT };

typedef struct {
    char studentId[9];  // 학번 (8자리 + '\0')
    int scores[SUBJECT_COUNT];  // 5과목 점수
} Student;

/* 등급을 저장하기 위한 예시 구조체
   각 학생별, 과목별 등급(A/B/C/D/F) 보관 */
typedef struct {
    char grade[SUBJECT_COUNT][2];
} GradeBook;

/* 쉼표(',') 제거 함수 (예: "49," -> "49") */
static void removeComma(char *scoreField) {
    for (int i = 0; i < 3; i++) {
        if (scoreField[i] == ',') {
            scoreField[i] = '\0';
            break;
        }
    }
}

/* 점수 분포(0~9, 10~19, …, 90~100)에 대해 몇 명이 있는지 계산 */
void printScoreDistribution(const Student *students, int count, int subjectIndex) {
    // 11개의 구간 (0~9, 10~19, ..., 90~99, 그리고 100점)
    int distribution[11];
    memset(distribution, 0, sizeof(distribution));

    for (int i = 0; i < count; i++) {
        int sc = students[i].scores[subjectIndex];
        if (sc == 100) {
            // 100점은 마지막 구간으로 처리
            distribution[10]++;
        } else {
            int idx = sc / 10; // 0~9점 -> idx=0, 10~19->1, ...
            distribution[idx]++;
        }
    }

    printf("점수 분포:\n");
    for (int i = 0; i < 10; i++) {
        printf(" %2d~%2d: %d명\n", i*10, i*10+9, distribution[i]);
    }
    printf("   100 : %d명\n", distribution[10]);
}

/* 등급 부여:
   - 상위 30% A, 70% B, 90% C, 95% D, 나머지 F
   - 과목별로 별도 정렬(내림차순) 후 순위 계산 */
void assignGrades(const Student *students, int count, GradeBook *gradeBook) {
    // 각 과목마다 학생들을 점수 기준으로 내림차순 정렬해야 함.
    // 정렬을 위해 (점수, 원래 인덱스) 쌍을 담아두고 정렬.
    typedef struct {
        int score;
        int index; // 원본 students 배열에서의 위치
    } ScoreIndex;

    for (int subj = 0; subj < SUBJECT_COUNT; subj++) {
        // 1) ScoreIndex 배열 만들기
        ScoreIndex *arr = (ScoreIndex *)malloc(sizeof(ScoreIndex)*count);
        for (int i = 0; i < count; i++) {
            arr[i].score = students[i].scores[subj];
            arr[i].index = i;
        }
        // 2) 내림차순 정렬
        //    점수가 동일하면 index가 작은 순으로 둔다고 가정(별도의 조건 필요 시 추가)
        for (int i = 0; i < count - 1; i++) {
            for (int j = i+1; j < count; j++) {
                if (arr[i].score < arr[j].score) {
                    ScoreIndex temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }
            }
        }
        // 3) 등급 매기기 (상위 30% A, 70% B, 90% C, 95% D, 나머지 F)
        for (int rank = 0; rank < count; rank++) {
            double percentile = (double)rank / count; // 0.0 ~ (count-1)/count
            char grade;
            if (percentile < 0.3) grade = 'A';
            else if (percentile < 0.7) grade = 'B';
            else if (percentile < 0.9) grade = 'C';
            else if (percentile < 0.95) grade = 'D';
            else grade = 'F';

            // 실제 학생의 위치에 grade 기록
            int originalIdx = arr[rank].index;
            gradeBook[originalIdx].grade[subj][0] = grade;
            gradeBook[originalIdx].grade[subj][1] = '\0';
        }
        free(arr);
    }
}

int main(void){
    // 1) 원본 파일(Score.bat) 읽기 전용 오픈
    FILE *fpBat = fopen("Score.bat", "r");
    if (!fpBat) {
        perror("Score.bat 파일 열기 실패");
        exit(EXIT_FAILURE);
    }
    // 2) 임시파일(Score.bak) 생성
    FILE *fpBak = fopen("Score.bak", "w+");
    if (!fpBak) {
        perror("Score.bak 파일 생성 실패");
        fclose(fpBat);
        exit(EXIT_FAILURE);
    }
    // 3) 원본 -> 임시파일 복사
    char buf[BUFSIZ];
    while (fgets(buf, sizeof(buf), fpBat)) {
        fputs(buf, fpBak);
    }
    fclose(fpBat);

    // 4) 임시파일에서 학생 레코드를 읽어들임
    rewind(fpBak); // fpBak 파일 포인터를 처음으로
    Student students[MAX_STUDENTS];
    int studentCount = 0;

    while (1) {
        char record[RECORD_SIZE+1];
        if (fread(record, 1, RECORD_SIZE, fpBak) != RECORD_SIZE) {
            break; // 더 이상 읽을 레코드가 없으면 종료
        }
        record[RECORD_SIZE] = '\0';

        // 학번
        strncpy(students[studentCount].studentId, record, 8);
        students[studentCount].studentId[8] = '\0';

        // 각 과목 점수 파싱
        // record[8..10]=국어, [11..13]=영어, [14..16]=수학, [17..19]=사회, [20..22]=과학
        char tmp[4];
        // 국어
        strncpy(tmp, record+8, 3); tmp[3] = '\0';
        removeComma(tmp);
        students[studentCount].scores[KOR] = atoi(tmp);
        // 영어
        strncpy(tmp, record+11, 3); tmp[3] = '\0';
        removeComma(tmp);
        students[studentCount].scores[ENG] = atoi(tmp);
        // 수학
        strncpy(tmp, record+14, 3); tmp[3] = '\0';
        removeComma(tmp);
        students[studentCount].scores[MAT] = atoi(tmp);
        // 사회
        strncpy(tmp, record+17, 3); tmp[3] = '\0';
        removeComma(tmp);
        students[studentCount].scores[SOC] = atoi(tmp);
        // 과학
        strncpy(tmp, record+20, 3); tmp[3] = '\0';
        removeComma(tmp);
        students[studentCount].scores[SCI] = atoi(tmp);

        studentCount++;
        if (studentCount >= MAX_STUDENTS) break; // 배열 한도 초과 방지
    }
    fclose(fpBak);

    if (studentCount == 0) {
        printf("읽어온 학생 레코드가 없습니다.\n");
        return 0;
    }

    // --------------------------
    // (1) 전체 학생 평균 점수
    // --------------------------
    double totalSumAll = 0.0;
    int totalCountAll = studentCount * SUBJECT_COUNT;
    for (int i = 0; i < studentCount; i++){
        for (int subj = 0; subj < SUBJECT_COUNT; subj++){
            totalSumAll += students[i].scores[subj];
        }
    }
    double avgAll = totalSumAll / totalCountAll;
    printf("[전체 학생의 평균 점수] = %.2f\n", avgAll);

    // --------------------------
    // (2) 과목별 평균 점수
    // --------------------------
    const char *subjName[SUBJECT_COUNT] = {"국어", "영어", "수학", "사회", "과학"};
    for (int subj = 0; subj < SUBJECT_COUNT; subj++){
        double sum = 0.0;
        for (int i = 0; i < studentCount; i++){
            sum += students[i].scores[subj];
        }
        double avg = sum / studentCount;
        printf("[%s 평균] = %.2f\n", subjName[subj], avg);
    }

    // --------------------------
    // (3) 최고점, 최저점
    //     (여기서는 전체 과목 통합으로 예시.
    //      필요 시 과목별 최고/최저도 비슷하게 계산)
    // --------------------------
    int minScore = 101, maxScore = -1;
    for (int i = 0; i < studentCount; i++){
        for (int subj = 0; subj < SUBJECT_COUNT; subj++){
            int s = students[i].scores[subj];
            if (s > maxScore) maxScore = s;
            if (s < minScore) minScore = s;
        }
    }
    printf("[전체 과목 통합 최고점] = %d\n", maxScore);
    printf("[전체 과목 통합 최저점] = %d\n", minScore);

    // --------------------------
    // (4) 성적 분포 출력
    //     과목별 분포를 출력하는 예시
    // --------------------------
    for (int subj = 0; subj < SUBJECT_COUNT; subj++) {
        printf("\n[%s] 과목 점수 분포\n", subjName[subj]);
        printScoreDistribution(students, studentCount, subj);
    }

    // --------------------------
    // (5) 등급 부여
    //     각 과목별로 상위 30% A, 70% B, 90% C, 95% D, 나머지 F
    //     GradeBook 구조체를 이용해 등급을 저장한 뒤,
    //     결과를 학생별로 출력
    // --------------------------
    GradeBook *gradeBook = (GradeBook *)calloc(studentCount, sizeof(GradeBook));
    assignGrades(students, studentCount, gradeBook);

    printf("\n[각 학생, 각 과목별 등급]\n");
    for (int i = 0; i < studentCount; i++) {
        printf("학번: %s  =>  ", students[i].studentId);
        for (int subj = 0; subj < SUBJECT_COUNT; subj++) {
            printf("%s:%s ", subjName[subj], gradeBook[i].grade[subj]);
        }
        printf("\n");
    }
    free(gradeBook);

    // 프로그램 종료 시, Score.bak 등 임시파일은 삭제하거나 남기는 정책은
    // 과제 요구사항에 따라 정하면 됩니다.
    // 여기서는 임시파일을 그대로 놔둔다고 가정.
    return 0;
}
