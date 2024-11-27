enum MODE { NULL, LINE, RECTANGLE, TRIANGLE, POINTS, STAR, CIRCLE, DRAW, ERASE };
typedef enum { false1, true1 } boolean;

#include<math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>

void mouse(int, int, int, int);
void key(unsigned char, int, int);
void display(void);
void drawSquare(int, int);
void myeshape(GLsizei, GLsizei);

void myinit(void);
void drawCurrentColor(int, int, int, int);
void drawToolButton(int, int, int);
int getCurrentToolIndex(void);
void drawToolIcon(int, int, int, int);
void drawStar(int, int, int);
void drawColorPalette(int, int, int, int);

void screen_box(int, int, int);
void right_menu(int);
void middle_menu(int);
void color_menu(int);
void pixel_menu(int);
void fill_menu(int);
int pick(int, int);

void menubar();
void Functionbar();
void Button(int, int);
boolean Button_locate(int, int, int, int);
void StringText(char String[], int x, int y);
void Button_click(int);
void getRGB(int, int);
void eraser_thumnail();
boolean menubarFuction(int, int);
/* glbals */

GLsizei wh = 500, ww = 500; /* initial window size */
GLfloat size = 3.0;   /* half side length of square */
int draw_mode = 0; /* drawing mode */
int rx, ry; /*raster position*/
static int xp[2], yp[2];
static int count;

GLfloat r = 1.0, g = 1.0, b = 1.0; /* drawing color */
GLfloat bg_r = 1.0, bg_g = 1.0, bg_b = 1.0;

int fill = 0; /* fill flag */
int dragCheck = 0;

int menubarcheck = 0;
int menucheck = 0;
int buttoncheck = 0;
int drawsize;

struct vertexNode
{
	GLint vx;
	GLint vy;
	struct vertexNode* next;
};


struct object
{
	GLfloat color[4];
	struct vertexNode* vertex;
	int fill;
	int mode;
	struct object* next;
}*head, *tail, *lineObj, *draw, *redo;


void initObj(struct object* obj)
{
	obj->vertex = '\0';
	obj->fill = 0;
	obj->next = '\0';
	obj->mode = 0;
}


void initNode(struct vertexNode* node)
{
	node->next = '\0';
	node->vx = 0;
	node->vy = 0;
}

void addObj(struct object* new1, struct object** head)
{
	struct object* p = *head;
	if (p->next == NULL)
		p->next = new1;
	else
	{
		while (p->next != tail)
			p = p->next;
		p->next = new1;
	}
	new1->next = tail;
}



void addVertex(struct vertexNode* new1, struct vertexNode** vertexHead)
{
	struct vertexNode* p = *vertexHead;
	if (p->next == NULL)
		p->next = new1;
	else
	{
		while (p->next != NULL)
			p = p->next;
		p->next = new1;
	}
}

void drawList()
{
	int count = 0;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	struct object* p = head;
	if (p->next == NULL)
		return;
	while (p->next != tail)
	{
		count++;
		p = p->next;
		struct vertexNode* node = p->vertex;
		glColor3f(p->color[0], p->color[1], p->color[2]);
		if (p->fill == 1) glBegin(GL_POLYGON);
		else if (p->fill == 2) glBegin(GL_LINE_STRIP);
		else glBegin(GL_LINE_LOOP);
		if (p->mode != STAR || (p->mode == STAR && p->fill == 0))
		{
			while (node->next != NULL)
			{
				node = node->next;
				glVertex2i((node->vx*drawsize) / 500 + 80, wh - (node->vy*drawsize) / 500 - 20);
			}
			glEnd();
		}
		else
		{
			while (node->next != NULL)

			{
				int count = 0;
				while (count < 3)
				{
					node = node->next;
					glVertex2i((node->vx*drawsize) / 500 + 80, wh - (node->vy*drawsize) / 500 - 20);
					count++;
				}
				glEnd();
				glBegin(GL_POLYGON);
			}
			glEnd();
		}
	}
	glPopAttrib();
}

void removeVertex(struct vertexNode** head)
{
	struct vertexNode* vnode = *head;
	if (vnode->next != NULL)
	{
		while (vnode->next != NULL)
		{
			struct vertexNode* tmp = vnode->next;
			vnode->next = NULL;
			vnode = NULL;
			free(vnode);
			vnode = tmp;
		}
		free(vnode);
		vnode = NULL;
	}
}

void redoInit()
{
	struct object* p = redo;
	if (redo != NULL)
	{
		while (p != NULL)
		{
			struct object* tmp = p->next;
			p->next = NULL;
			removeVertex(&p->vertex);
			p = NULL;
			free(p);
			p = tmp;
		}
		free(redo);
		redo = NULL;
	}
}

struct object* pop()
{
	struct object* p = redo;
	struct object* prior = NULL;
	if (redo != NULL)
	{
		while (p->next != NULL)
		{
			prior = p;
			p = p->next;
		}
		if (prior != NULL)
			prior->next = NULL;
		else
			redo = NULL;
		return p;
	}
	return NULL;
}

void push(struct object* node)
{
	struct object* p = redo;
	node->next = NULL;
	if (redo == NULL)
	{
		redo = node;
		return;
	}
	else
	{
		while (p->next != NULL)
		{
			p = p->next;
		}
		p->next = node;
	}
}

void Redo(struct object** head)
{
	struct object *p = *head;
	if ((redo != NULL) && (head != NULL))
	{
		while (p->next != tail)
		{
			p = p->next;
		}
		p->next = pop();
		p->next->next = tail;
	}
	glutPostRedisplay();
}

void undo(struct object** head)
{
	struct object* p = *head;
	if ((p->next != NULL) && (p->next != tail))
	{
		while (p->next->next != tail)
			p = p->next;
		push(p->next);
		p->next = tail;
		glutPostRedisplay();
	}
}

struct object* makeObj(int fill)
{
	struct object* p = (struct object*)malloc(sizeof(struct object));
	initObj(p);

	p->vertex = (struct vertexNode*)malloc(sizeof(struct vertexNode));
	initNode(p->vertex);

	p->fill = fill;
	p->mode = draw_mode;
	if (draw_mode != 8)
	{
		p->color[0] = r;
		p->color[1] = g;
		p->color[2] = b;
		p->color[3] = 1.0;
	}
	else
	{
		p->color[0] = bg_r;
		p->color[1] = bg_g;
		p->color[2] = bg_b;
		p->color[3] = 1.0;
	}
	return p;
}


void makeVertex(int x, int y, struct object* p)
{
	struct vertexNode* v = (struct vertexNode*)malloc(sizeof(struct vertexNode));
	initNode(v);
	v->vx = 500 * (x - 80) / drawsize;
	v->vy = 500 * (y - 20) / drawsize;
	addVertex(v, &p->vertex); //ÃÊ±â¹öÀü
}

void CreateCircle1(int centerx, int centery, int rad, int fill)
{
	struct object* p = makeObj(fill);
	for (int angle = 0; angle < 360; angle += 1)
	{
		int px = rad * cos(angle*0.017);
		int py = rad * sin(angle*0.017);

		makeVertex(centerx + px, wh - centery + py, p);
	}
	addObj(p, &head);
}

void CreateStar(int x1, int y1, int x2, int y2, struct object* p)
{
	int tmp = 0;
	if (x2 < x1)
	{
		tmp = x2;
		x2 = x1;
		x1 = tmp;
	}
	if (y2 < y1)
	{
		tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	int xlength = x2 - x1;
	int ylength = y2 - y1;

	if (p->fill == 0)
	{
		makeVertex(0.5*xlength + x1, y1, p);					//1
		makeVertex(0.38*xlength + x1, 0.38*ylength + y1, p);	//2
		makeVertex(x1, 0.4*ylength + y1, p);					//3
		makeVertex(0.3*xlength + x1, 0.62*ylength + y1, p);		//4
		makeVertex(0.19*xlength + x1, y2, p);					//5
		makeVertex(0.5*xlength + x1, 0.77*ylength + y1, p);		//6
		makeVertex(0.81*xlength + x1, y2, p);					//7
		makeVertex(0.7*xlength + x1, 0.62*ylength + y1, p);		//8
		makeVertex(x2, 0.4*ylength + y1, p);					//9
		makeVertex(0.62*xlength + x1, 0.38*ylength + y1, p);	//10
	}
	else
	{
		makeVertex(0.5*xlength + x1, y1, p);					//1
		makeVertex(0.19*xlength + x1, y2, p);					//5
		makeVertex(0.7*xlength + x1, 0.62*ylength + y1, p);		//8
		makeVertex(0.5*xlength + x1, y1, p);					//1
		makeVertex(0.3*xlength + x1, 0.62*ylength + y1, p);		//4
		makeVertex(0.81*xlength + x1, y2, p);					//7
		makeVertex(x1, 0.4*ylength + y1, p);					//3
		makeVertex(0.5*xlength + x1, 0.77*ylength + y1, p);		//6
		makeVertex(x2, 0.4*ylength + y1, p);					//9
	}

}

void drawSquare(int x, int y)
{
	y = wh - y;
	struct object *p = makeObj(1);

	makeVertex(x + size, wh - y + size, p);
	makeVertex(x - size, wh - y + size, p);
	makeVertex(x - size, wh - y - size, p);
	makeVertex(x + size, wh - y - size, p);

	addObj(p, &head);
}

void clearObject()
{
	struct object* p = head;

	while (p->next != tail)
	{
		undo(&head);
	}
	glutPostRedisplay();
}


void myReshape(GLsizei w, GLsizei h)
{

	/* adjust clipping box */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* adjust viewport and  clear */

	glViewport(0, 0, w, h);
	glClearColor(0.8, 0.8, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	display();
	glutSwapBuffers();

	/* set global size for use by drawing routine */

	ww = w;
	wh = h;
}

void myinit(void)
{

	glViewport(0, 0, ww, wh);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)ww, 0.0, (GLdouble)wh, -1.0, 1.0);

	/* set clear color to black and clear window */

	glClearColor(0.8, 0.8, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

// 현재 선택된 색상을 표시하는 함수
void drawCurrentColor(int x, int y, int width, int height) {
    // 배경 (체커보드 패턴 - 투명도 표시용)
    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_POLYGON);
    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y - height);
    glVertex2i(x, y - height);
    glEnd();

    // 현재 색상
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2i(x+2, y-2);
    glVertex2i(x + width-2, y-2);
    glVertex2i(x + width-2, y - height+2);
    glVertex2i(x+2, y - height+2);
    glEnd();

    // 테두리
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y - height);
    glVertex2i(x, y - height);
    glEnd();
}

// 현재 선택된 도구의 인덱스를 반환하는 함수
int getCurrentToolIndex() {
    switch(draw_mode) {
        case LINE: 
            return 1;
        case TRIANGLE: 
            return 2;
        case POINTS: 
            return 3;
        case CIRCLE: 
            return 4;
        case STAR: 
            return 5;
        case DRAW: 
            return 6;
        case RECTANGLE: 
            return 7;
        case ERASE: 
            return 8;
        default: 
            return 0;
    }
}

// 도구 버튼을 그리는 함수
void drawToolButton(int x, int y, int size) {
    // 버튼 배경
    if (buttoncheck == getCurrentToolIndex()) {
        glColor3f(0.4, 0.4, 0.4); // 선택된 버튼은 더 어둡게
    } else {
        glColor3f(0.3, 0.3, 0.3);
    }
    
    glBegin(GL_POLYGON);
    glVertex2i(x, y);
    glVertex2i(x + size, y);
    glVertex2i(x + size, y - size);
    glVertex2i(x, y - size);
    glEnd();

    // 버튼 테두리
    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x, y);
    glVertex2i(x + size, y);
    glVertex2i(x + size, y - size);
    glVertex2i(x, y - size);
    glEnd();

    // 선택된 버튼 하이라이트
    if (buttoncheck == getCurrentToolIndex()) {
        glColor3f(0.5, 0.5, 0.8);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
        glVertex2i(x + 2, y - 2);
        glVertex2i(x + size - 2, y - 2);
        glVertex2i(x + size - 2, y - size + 2);
        glVertex2i(x + 2, y - size + 2);
        glEnd();
        glLineWidth(1.0);
    }
}



// 도구 아이콘을 그리는 함수
void drawToolIcon(int x, int y, int size, int toolType) {
    int center_x = x + size/2;
    int center_y = y - size/2;
    int margin = size/4;
    
    glColor3f(0.9, 0.9, 0.9);
    
    switch(toolType) {
        case 1: // LINE
            glBegin(GL_LINES);
            glVertex2i(x + margin, y - margin);
            glVertex2i(x + size - margin, y - size + margin);
            glEnd();
            break;
            
        case 2: // TRIANGLE
            glBegin(GL_LINE_LOOP);
            glVertex2i(center_x, y - margin);
            glVertex2i(x + size - margin, y - size + margin);
            glVertex2i(x + margin, y - size + margin);
            glEnd();
            break;
            
        case 3: // POINTS
            glPointSize(4.0);
            glBegin(GL_POINTS);
            glVertex2i(center_x, center_y);
            glEnd();
            glPointSize(1.0);
            break;
            
        case 4: // CIRCLE
            glBegin(GL_LINE_LOOP);
            for(int angle = 0; angle < 360; angle += 30) {
                float rad = angle * 0.017453;
                float px = center_x + (size/3) * cos(rad);
                float py = center_y + (size/3) * sin(rad);
                glVertex2f(px, py);
            }
            glEnd();
            break;
            
        case 5: // STAR
            drawStar(center_x, center_y, size/3);
            break;
            
        case 6: // DRAW
            glBegin(GL_LINE_STRIP);
            glVertex2i(x + margin, center_y);
            glVertex2i(center_x, y - size + margin);
            glVertex2i(x + size - margin, center_y);
            glEnd();
            break;
            
        case 7: // RECTANGLE
            glBegin(GL_LINE_LOOP);
            glVertex2i(x + margin, y - margin);
            glVertex2i(x + size - margin, y - margin);
            glVertex2i(x + size - margin, y - size + margin);
            glVertex2i(x + margin, y - size + margin);
            glEnd();
            break;
            
        case 8: // ERASER
            glColor3f(0.4, 0.3, 0.7);
            glBegin(GL_POLYGON);
            glVertex2i(x + margin, y - margin);
            glVertex2i(x + size - margin, y - margin);
            glVertex2i(x + size - margin, y - size + margin);
            glVertex2i(x + margin, y - size + margin);
            glEnd();
            
            glColor3f(0.9, 0.9, 0.9);
            glBegin(GL_LINE_LOOP);
            glVertex2i(x + margin, y - margin);
            glVertex2i(x + size - margin, y - margin);
            glVertex2i(x + size - margin, y - size + margin);
            glVertex2i(x + margin, y - size + margin);
            glEnd();
            break;
    }
}


void mouse(int btn, int state, int x, int y)
{
	int where = draw_mode;

	struct object* p;
	double rad;
	//¿ÞÂÊ ¹öÆ°À» ´©¸£¸é
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		if (menubarFuction(x, y))	where = pick(x, y);
		//COLOR¸Þ´º¿¡¼­ Á¤ÇÑ °ªÀÌ µé¾î°£´Ù.
		glColor3f(r, g, b);

		if (where != 0)
		{
			count = 0;
			draw_mode = where;
		}
		else
		{
			redoInit();
			switch (draw_mode)
			{
			case(DRAW):
				if (!fill) fill = 2;
				lineObj = makeObj(fill);
				break;
			case(LINE):
				if (count == 0)
				{
					count++;
					xp[0] = x;
					yp[0] = y;

					draw = makeObj(fill);
				}
			
			case(RECTANGLE):

				if (count == 0)
				{
					count++;
					xp[0] = x;
					yp[0] = y;

					draw = makeObj(fill);
				}
				break;
			case (TRIANGLE):
				switch (count)
				{
				case(0):
					count++;
					xp[0] = x;
					yp[0] = y;
					break;
				case(1):
					count++;
					xp[1] = x;
					yp[1] = y;

					draw = makeObj(fill);
					break;
				}

				break;
			case(POINTS):
			{
				drawSquare(x, y);
				count++;
			}
			break;
			case(STAR):
			{
				xp[0] = x;
				yp[0] = y;
				count++;

				draw = makeObj(fill);
			}
			case(CIRCLE):
			{
				if (count == 0)
				{
					count++;
					xp[0] = x;
					yp[0] = y;

					draw = makeObj(fill);
				}
				break;
			}
			case(ERASE):
				if (count == 0)
				{
					count++;
					xp[0] = x;
					yp[0] = y;

					draw = makeObj(1);
				}
			}
		}
	}
	else if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (buttoncheck != 0) buttoncheck = 0;

		if (count > 0)
		{
			switch (draw_mode)
			{
			case LINE:
				p = makeObj(fill);
				makeVertex(x, y, p);
				makeVertex(xp[0], yp[0], p);

				addObj(p, &head);
				count = 0;
				break;
			case RECTANGLE:
				p = makeObj(fill);

				makeVertex(x, y, p);
				makeVertex(x, yp[0], p);
				makeVertex(xp[0], yp[0], p);
				makeVertex(xp[0], y, p);

				addObj(p, &head);
				count = 0;
				break;
			case CIRCLE:
				rad = sqrt(pow(x - xp[0], 2) + pow(y - yp[0], 2));
				if (rad < 0) rad = -rad;
				CreateCircle1(xp[0], wh - yp[0], rad, fill);
				count = 0;
				break;
			case ERASE:
				p = makeObj(1);

				makeVertex(x, y, p);
				makeVertex(x, yp[0], p);
				makeVertex(xp[0], yp[0], p);
				makeVertex(xp[0], y, p);

				addObj(p, &head);
				count = 0;
				break;
			case TRIANGLE:
				if (count == 2)
				{
					p = makeObj(fill);

					makeVertex(xp[0], yp[0], p);
					makeVertex(xp[1], yp[1], p);
					makeVertex(x, y, p);

					addObj(p, &head);

					//draw_mode = 0;
					count = 0;
				}
				break;
			case STAR:
				p = makeObj(fill);
				CreateStar(xp[0], yp[0], x, y, p);

				addObj(p, &head);
				count = 0;
				break;
			default:
				return;
			}
		}
	}
	glutPostRedisplay();
	glPopAttrib();
}




int pick(int x, int y) {
    // 드로잉 영역 체크
    if (80 < x && x < 80 + drawsize && 20 < y && 20 + drawsize > y) {
        return 0;
    }

    int buttonSize = 40;
    int margin = 10;

    // 각 도구 버튼의 위치 체크
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 2; j++) {
            int buttonX = margin + j * (buttonSize + margin);
            int buttonY = 60 + i * (buttonSize + margin);
            
            if (x >= buttonX && x <= buttonX + buttonSize &&
                y >= buttonY && y <= buttonY + buttonSize) {
                buttoncheck = i * 2 + j + 1;
                
                // 버튼 위치에 따른 도구 반환
                switch(i * 2 + j + 1) {
                    case 1: return LINE;
                    case 2: return TRIANGLE;
                    case 3: return POINTS;
                    case 4: return CIRCLE;
                    case 5: return STAR;
                    case 6: return DRAW;
                    case 7: return RECTANGLE;
                    case 8: return ERASE;
                }
            }
        }
    }
    
    return draw_mode;
}


void drawInit()
{
	removeVertex(&draw->vertex);
	glClearColor(bg_r, bg_g, bg_b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawList();
	menubar();
	Functionbar();

	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_POLYGON);
	glVertex2i(80, wh - 20 - drawsize);
	glVertex2i(80, 0);
	glVertex2i(ww, 0);
	glVertex2i(ww, wh - 20 - drawsize);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2i(80 + drawsize, wh - 20);
	glVertex2i(ww, wh - 20);
	glVertex2i(ww, wh - 20 - drawsize);
	glVertex2i(80 + drawsize, wh - 20 - drawsize);
	glEnd();
}

void rever(struct object** head)
{
	bg_r = 1 - bg_r;
	bg_g = 1 - bg_g;
	bg_b = 1 - bg_b;

	struct object* p = *head;
	if (p->next != NULL)
	{
		while (p->next != tail)
		{
			p = p->next;
			p->color[0] = 1 - p->color[0];
			p->color[1] = 1 - p->color[1];
			p->color[2] = 1 - p->color[2];
		}
		glutPostRedisplay();
	}
}

void xInversion(struct object** head)
{
	struct object* p = *head;;
	if (p->next != NULL)
	{
		struct vertexNode* node;
		while (p->next != tail)
		{
			p = p->next;
			for (node = p->vertex; node != NULL; node = node->next)
			{
				node->vx = 500 - node->vx;
			}
		}
	}

}

void yInversion(struct object** head)
{
	struct object* p = *head;;
	if (p->next != NULL)
	{
		struct vertexNode* node;
		while (p->next != tail)
		{
			p = p->next;
			for (node = p->vertex; node != NULL; node = node->next)
			{
				node->vy = 500 - node->vy;
			}
		}
	}

}

void drawObj(struct object* drawnode)
{
	struct vertexNode* node = drawnode->vertex;
	glColor3f(drawnode->color[0], drawnode->color[1], drawnode->color[2]);
	if (drawnode->fill == 1) glBegin(GL_POLYGON);
	else if (drawnode->fill == 2) glBegin(GL_LINE_STRIP);
	else glBegin(GL_LINE_LOOP);
	if (draw_mode != STAR || (draw_mode == STAR && drawnode->fill == 0))
	{
		while (node->next != NULL)
		{
			node = node->next;
			glVertex2i((node->vx*drawsize) / 500 + 80, wh - (node->vy*drawsize) / 500 - 20);
		}
		glEnd();
		glutSwapBuffers();
	}
	else
	{
		while (node->next != NULL)
		{
			int count = 0;
			while (count < 3)
			{
				node = node->next;
				glVertex2i((node->vx*drawsize) / 500 + 80, wh - (node->vy*drawsize) / 500 - 20);
				count++;
			}
			glEnd();
			glBegin(GL_POLYGON);
		}
		glEnd();
		glutSwapBuffers();
	}
}

void motion(int x, int y) {
	if (80 < x && x < 80 + drawsize && 20 < y && y < 20 + drawsize)
	{
		if (draw_mode == DRAW && lineObj != NULL)
		{
			makeVertex(x, y, lineObj);
			dragCheck = 1;

			drawObj(lineObj);
		}
		else if (draw_mode == RECTANGLE && draw != NULL)
		{
			drawInit();

			makeVertex(x, y, draw);
			makeVertex(x, yp[0], draw);
			makeVertex(xp[0], yp[0], draw);
			makeVertex(xp[0], y, draw);

			drawObj(draw);
		}
		else if (draw_mode == CIRCLE && draw != NULL)
		{
			drawInit();

			double rad = sqrt(pow(x - xp[0], 2) + pow(y - yp[0], 2));
			if (rad < 0) rad = -rad;

			for (int angle = 0; angle < 360; angle += 1)
			{
				int px = rad * cos(angle*0.017);
				int py = rad * sin(angle*0.017);

				makeVertex(xp[0] + px, yp[0] + py, draw);
			}

			drawObj(draw);
		}
		else if (draw_mode == LINE && draw != NULL)
		{
			drawInit();

			makeVertex(x, y, draw);
			makeVertex(xp[0], yp[0], draw);

			drawObj(draw);
		}
		else if (draw_mode == ERASE && draw != NULL)
		{
			drawInit();

			makeVertex(x, y, draw);
			makeVertex(x, yp[0], draw);
			makeVertex(xp[0], yp[0], draw);
			makeVertex(xp[0], y, draw);

			drawObj(draw);
		}
		else if (draw_mode == TRIANGLE && draw != NULL)
		{
			drawInit();

			makeVertex(xp[0], yp[0], draw);
			makeVertex(xp[1], yp[1], draw);
			makeVertex(x, y, draw);

			drawObj(draw);
		}
		else if (draw_mode == STAR && draw != NULL)
		{
			drawInit();
			CreateStar(xp[0], yp[0], x, y, draw);
			drawObj(draw);
		}
		//glutPostRedisplay();
	}
}

void pointMove(int x, int y)
{
	if (draw_mode == DRAW && dragCheck)
	{
		addObj(lineObj, &head);
		dragCheck = 0;
		lineObj = '\0';
		if (fill == 2)
			fill = 0;
	}

	if (menubarcheck != 0)
	{
		menucheck = y / 20;
		glutPostRedisplay();
	}
	else menucheck = 0;
}

void screen_box(int x, int y, int s)
{
	glBegin(GL_POLYGON);
	glVertex2i(x, y);
	glVertex2i(x + s, y);
	glVertex2i(x + s, y + s);
	glVertex2i(x, y + s);

	glEnd();
}

void right_menu(int id)
{
	if (id == 1) exit(1);
	else if (id == 3) rever(&head);
	else clearObject();
}

void middle_menu(int id)
{

}

//IDÀÇ °ª¿¡ µû¶ó¼­ ÄÃ·¯ °ªÀ» º¯°æ
void color_menu(int id)
{
	if (id == 1) { r = 1.0; g = 0.0; b = 0.0; }
	else if (id == 2) { r = 0.0; g = 1.0; b = 0.0; }
	else if (id == 3) { r = 0.0; g = 0.0; b = 1.0; }
	else if (id == 4) { r = 0.0; g = 1.0; b = 1.0; }
	else if (id == 5) { r = 1.0; g = 0.0; b = 1.0; }
	else if (id == 6) { r = 1.0; g = 1.0; b = 0.0; }
	else if (id == 7) { r = 1.0; g = 1.0; b = 1.0; }
	else if (id == 8) { r = 0.0; g = 0.0; b = 0.0; }
}

//ÇÈ¼¿ °ª Á¶Á¤
void pixel_menu(int id)
{
	if (id == 1)
		size = 2 * size;
	else if (size > 1)
		size = size / 2;
}

void fill_menu(int id)
{
	if (id == 1) fill = 1;
	else fill = 0;
}

void key(unsigned char k, int xx, int yy)
{
	if (k == 26)
	{
		int mod = glutGetModifiers();
		if (mod & GLUT_ACTIVE_SHIFT)
			Redo(&head);
		else
			undo(&head);
	}

}

void display(void)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glClearColor(bg_r, bg_g, bg_b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	if (ww - 80  > wh - 20) drawsize = wh - 20;
	else drawsize = ww - 80;


	drawList();
	Functionbar();
	menubar(); //ÀÌ·¸°Ô ÇØ¾ß ±×¸² À§¿¡ ¸Þ´º¹Ù°¡ ±×·ÁÁø´Ù.

	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_POLYGON);
	glVertex2i(80, wh - 20 - drawsize);
	glVertex2i(80, 0);
	glVertex2i(ww, 0);
	glVertex2i(ww, wh - 20 - drawsize);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2i(80 + drawsize, wh - 20);
	glVertex2i(ww, wh - 20);
	glVertex2i(ww, wh - 20 - drawsize);
	glVertex2i(80 + drawsize, wh - 20 - drawsize);
	glEnd();

	glutSwapBuffers();
	glPopAttrib();
}

void menubar() {
    // 상단 메뉴바 배경 - 진한 회색으로 변경
    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_POLYGON);
    glVertex2i(0, wh);
    glVertex2i(0, wh - 40); // 높이 증가
    glVertex2i(ww, wh - 40);
    glVertex2i(ww, wh);
    glEnd();

    // 메뉴 항목들 사이의 구분선 - 옅은 회색
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINES);
    glVertex2i(80, wh - 8);
    glVertex2i(80, wh - 32);

    glVertex2i(160, wh - 8);
    glVertex2i(160, wh - 32);

    glVertex2i(280, wh - 8);
    glVertex2i(280, wh - 32);

    glVertex2i(400, wh - 8);
    glVertex2i(400, wh - 32);
    glEnd();

    // 메뉴 텍스트 - 흰색으로 변경
    glColor3f(1.0, 1.0, 1.0);
    StringText("File", 20, wh - 25);
    StringText("Fill", 100, wh - 25);
    StringText("Background", 190, wh - 25);
    StringText("Inversion", 310, wh - 25);
}


boolean menubarFuction(int x, int y)
{
	if (menubarcheck != 0) 
	{
		if (menubarcheck == 1) 
		{
			if (menucheck == 1) 
			{
				exit(0);
			}
			menubarcheck = 0;
		}
		else if (menubarcheck == 2) 
		{
			if (menucheck == 1) fill = 1;
			else if (menucheck == 2) fill = 0;
			menubarcheck = 0;
		}
		else if (menubarcheck == 3) 
		{
			if (menucheck == 1) { bg_r = 1.0; bg_g = 0.0; bg_b = 0.0; }
			else if (menucheck == 2) { bg_r = 0.0; bg_g = 1.0; bg_b = 0.0; }
			else if (menucheck == 3) { bg_r = 0.0; bg_g = 0.0; bg_b = 1.0; }
			else if (menucheck == 4) { bg_r = 0.0; bg_g = 1.0; bg_b = 1.0; }
			else if (menucheck == 5) { bg_r = 1.0; bg_g = 0.0; bg_b = 1.0; }
			else if (menucheck == 6) { bg_r = 1.0; bg_g = 1.0; bg_b = 0.0; }
			else if (menucheck == 7) { bg_r = 1.0; bg_g = 1.0; bg_b = 1.0; }
			else if (menucheck == 8) { bg_r = 0.0; bg_g = 0.0; bg_b = 0.0; }
			menubarcheck = 0;
		}
		else if (menubarcheck == 4) //Reverse¸¦ ¼±ÅÃ½Ã
		{
			if (menucheck == 1) rever(&head);
			else if (menucheck == 2) yInversion(&head);
			else if (menucheck == 3) xInversion(&head);
			menubarcheck = 0;
		}
		return false1;
	}
	else if (x > 5 && x < 75 && y > 185 && y < 265)
	{
		getRGB(x - 5, y - 185);
		return false1;
	}
	if (y <= 20)
	{
		if (x <= 45)	menubarcheck = 1;	
		else if (x <= 90) menubarcheck = 2; 
		else if (x <= 195) menubarcheck = 3; 
		else if (x <= 295) menubarcheck = 4;
		else	menubarcheck = 0;
	}
	else menubarcheck = 0;


	return true1;
}
// 수정된 Functionbar 함수
void Functionbar() {
    // 좌측 툴바 배경 - 밝은 회색으로 변경
    glColor3f(0.25, 0.25, 0.25);
    glBegin(GL_POLYGON);
    glVertex2i(0, wh - 40);
    glVertex2i(0, 0);
    glVertex2i(80, 0);
    glVertex2i(80, wh - 40);
    glEnd();

    // 툴 버튼들의 간격과 크기 조정
    int buttonSize = 40;
    int margin = 10;
    
    // 각 도구 버튼 그리기
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 2; j++) {
            drawToolButton(margin + j * (buttonSize + margin), 
                         wh - (60 + i * (buttonSize + margin)), 
                         buttonSize);
            
            // 각 버튼에 해당하는 아이콘 그리기
            drawToolIcon(margin + j * (buttonSize + margin), 
                        wh - (60 + i * (buttonSize + margin)),
                        buttonSize,
                        i * 2 + j + 1);
        }
    }

    // 현재 선택된 색상 표시
    drawCurrentColor(margin, wh - 380, 60, 40);

    // 컬러 팔레트 테두리
    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_LINE_LOOP);
    glVertex2i(5, wh - 185);
    glVertex2i(5, wh - 255);
    glVertex2i(75, wh - 255);
    glVertex2i(75, wh - 185);
    glEnd();
}


// 별 모양을 그리는 함수
void drawStar(int cx, int cy, int size) {
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 5; i++) {
        // 외곽 점
        float angle = i * 72 * 0.017453; // 72도(360/5)를 라디안으로 변환
        float px = cx + size * cos(angle);
        float py = cy + size * sin(angle);
        glVertex2f(px, py);
        
        // 내부 점 (36도 회전)
        angle += 36 * 0.017453;
        px = cx + (size/2) * cos(angle); // 내부 점은 크기를 절반으로
        py = cy + (size/2) * sin(angle);
        glVertex2f(px, py);
    }
    glEnd();
}

void Button(int x, int y) 
{
	glColor3f(1.0, 1.0, 1.0); 
	glBegin(GL_LINE_STRIP);
	glVertex2i(x + 1, y - 30);
	glVertex2i(x + 1, y - 1);
	glVertex2i(x + 30, y - 1);
	glEnd();

	glColor3f(0.6, 0.6, 0.6);

	glBegin(GL_POLYGON); 
	glVertex2i(x, y - 28);
	glVertex2i(x, y - 30);
	glVertex2i(x + 30, y - 30);
	glVertex2i(x + 30, y - 28);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2i(x + 28, y);
	glVertex2i(x + 30, y);
	glVertex2i(x + 30, y - 28);
	glVertex2i(x + 28, y - 28);
	glEnd();
}

// Button_locate 함수도 업데이트
boolean Button_locate(int x, int y, int button_x, int button_y) {
    // 버튼 크기를 40x40으로 변경
    if (x > button_x + 40) return false1;
    if (x < button_x) return false1;
    if (y < button_y) return false1;
    if (y > button_y + 40) return false1;
    return true1;
}

void Button_click(int x)
{
	buttoncheck = x;
	glutPostRedisplay();
}

void getRGB(int x, int y)
{
	float RGBx = (float)x / 70;
	float RGBy = (float)y / 70;
	if (RGBx <= RGBy)
	{
		if (RGBx < 1 - RGBy)
		{
			r = (1 - RGBx)*(1 - RGBy);
			g = (1 - RGBx)*RGBy;
			b = 0;
		}
		else
		{
			r = 0;
			g = (1 - RGBx)*RGBy;
			b = RGBx*RGBy;

		}
	}
	else
	{
		r = (1 - RGBx)*(1 - RGBy);
		g = 0;
		b = RGBx*RGBy;

	}
	printf("R : %f G : %f B : %f\n", r, g, b);
	glutPostRedisplay();
}

void StringText(char String[], int x, int y) {
	int shift = x;
	for (int i = 0; String[i] != NULL; i++) {
		glRasterPos2i(shift, y);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, String[i]);
		shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, String[i]);
	}
}

void eraser_thumnail()
{
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	for (int angle = 0; angle < 180; angle += 1)
	{
		int px = 10 * cos(angle*0.017);
		int py = 10 * sin(angle*0.017);

		glVertex2f(60 + px, wh - 157 + py);
	}
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	for (int angle = 0; angle < 178; angle += 1)
	{
		int px = 10 * cos(angle*0.017);
		int py = 10 * sin(angle*0.017);

		glVertex2f(60 + px, wh - 157 + py);
	} //Áö¿ì°³ ¹Ý¿ø
	glEnd();

	glColor3f(0.4, 0.3, 0.7);
	glBegin(GL_POLYGON);
	glVertex2i(51, wh - 157);
	glVertex2i(69, wh - 157);
	glVertex2i(69, wh - 170);
	glVertex2i(51, wh - 170);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(51, wh - 157);
	glVertex2i(69, wh - 157);
	glVertex2i(69, wh - 170);
	glVertex2i(51, wh - 170);
	glEnd();
}


int main(int argc, char** argv)
{
	int c_menu, p_menu, f_menu;
	head = (struct object*)malloc(sizeof(struct object));
	initObj(head);
	tail = (struct object*)malloc(sizeof(struct object));
	initObj(tail);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("square");
	glutDisplayFunc(display);

	
	c_menu = glutCreateMenu(color_menu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Cyan", 4);
	glutAddMenuEntry("Magenta", 5);
	glutAddMenuEntry("Yellow", 6);
	glutAddMenuEntry("White", 7);
	glutAddMenuEntry("Black", 8);

	
	p_menu = glutCreateMenu(pixel_menu);
	glutAddMenuEntry("increase pixel size", 1);
	glutAddMenuEntry("decrease pixel size", 2);

	
	f_menu = glutCreateMenu(fill_menu);
	glutAddMenuEntry("fill on", 1);
	glutAddMenuEntry("fill off", 2);
	glutCreateMenu(right_menu);
	glutAddMenuEntry("quit", 1);
	glutAddMenuEntry("clear", 2);
	glutAddMenuEntry("rever", 3);

	
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	
	glutCreateMenu(middle_menu);
	glutAddSubMenu("Colors", c_menu);
	glutAddSubMenu("Pixel Size", p_menu);
	glutAddSubMenu("Fill", f_menu);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);


	myinit();

	//ÀÌº¥Æ® ºÙ¿©ÁÜ
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(pointMove);

	glutMainLoop();
}
