#include <GL/glut.h>
#include <math.h>
#include <stdio.h>  // printf 함수 선언
#include <time.h>   // 시간 측정을 위한 헤더

void drawCircleWithoutSymmetry(float cx, float cy, float r) {
    glBegin(GL_POINTS);
    for (float x = -r; x <= r; x += 0.01) { // x를 -r에서 r까지 이동
        float y_positive = sqrt(r * r - x * x); // 원 위의 y 좌표 (양수)
        float y_negative = -y_positive;        // 원 아래의 y 좌표 (음수)

        // 중심 좌표 (cx, cy)를 기준으로 점 그리기
        glVertex2f(cx + x, cy + y_positive);
        glVertex2f(cx + x, cy + y_negative);
    }
    glEnd();
}

void display() {
    struct timespec start, end; // 시간 측정을 위한 구조체
    clock_gettime(CLOCK_MONOTONIC, &start); // 시작 시간 측정

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0); // 빨간색
    drawCircleWithoutSymmetry(250.0, 250.0, 100.0); // 중심 (250, 250), 반지름 100

    clock_gettime(CLOCK_MONOTONIC, &end); // 종료 시간 측정

    // 시간 계산 (초 단위)
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Time to draw circle: %f seconds\n", elapsed_time);

    glFlush();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0); // 배경색: 검정
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 500.0); // 직교 투영
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Circle Drawing - Without Symmetry");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

