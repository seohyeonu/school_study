#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
void drawCirclePolar(float cx, float cy, float r) {
    glBegin(GL_LINE_LOOP);
    for (float angle = 0; angle < 2 * M_PI; angle += 0.01) {
        float x = cx + r * cos(angle);
        float y = cy + r * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void display() {
    struct timespec start, end; // 시간 측정을 위한 구조체
    clock_gettime(CLOCK_MONOTONIC, &start); // 시작 시간 측정

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0); // 초록색
    drawCirclePolar(250.0, 250.0, 100.0); // 중심 (250, 250), 반지름 100

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
    glutCreateWindow("Circle Drawing - Polar Coordinates");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

