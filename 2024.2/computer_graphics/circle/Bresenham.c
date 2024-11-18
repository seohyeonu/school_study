#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
void plotCirclePoints(int cx, int cy, int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(cx + x, cy + y);
    glVertex2i(cx - x, cy + y);
    glVertex2i(cx + x, cy - y);
    glVertex2i(cx - x, cy - y);
    glVertex2i(cx + y, cy + x);
    glVertex2i(cx - y, cy + x);
    glVertex2i(cx + y, cy - x);
    glVertex2i(cx - y, cy - x);
    glEnd();
}

void drawCircle(int cx, int cy, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r; // 초기 오류값

    plotCirclePoints(cx, cy, x, y);

    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        plotCirclePoints(cx, cy, x, y);
    }
}

void display() {
    struct timespec start, end; // 시간 측정을 위한 구조체
    clock_gettime(CLOCK_MONOTONIC, &start); // 시작 시간 측정

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0); // 빨간색
    drawCircle(250, 250, 100); // 중심 (250, 250), 반지름 100

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
    glutCreateWindow("Circle Drawing - Bresenham Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

