#include <iostream>
#include <ctime>
#include <windows.h>  
#include <gl\GL.h>
#include <gl\GLU.h> 
#include <glut.h> 
using namespace std;

int width = 800, height = 800;

const int N = 50;
float xCell = 2.0 / N, yCell = 2.0 / N;
int SleepTime = 200;

bool Evolv = 0;//Режим развития
bool Draw = 0;// 0 - стираем, 1 рисуем.

struct Cell {
	bool color;
	bool was_changed;
	int amount;
};

Cell Map[N][N];



void MenuKeyboard(unsigned char button, int x, int y) {
	switch (button)
	{
		//ESCAPE
	case 27: {
		break;
	}
		   //ENTER
	case 13: {
		break;
	};

		   //BACKSPACE
	case 8: {
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++) {
				Map[i][j].color = 0;
				Map[i][j].amount = 0;
			}
		break;
	}
	}
}

void reloadAmount() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			Map[i][j].amount = 0;

			if (Map[i - 1][j - 1].color	&& i - 1 > 0 && j - 1 > 0)	Map[i][j].amount++;
			if (Map[i - 1][j].color		&& i - 1 > 0)				Map[i][j].amount++;
			if (Map[i - 1][j + 1].color	&& i - 1 > 0 && j + 1 < N)	Map[i][j].amount++;
			if (Map[i][j - 1].color		&& j - 1 > 0)				Map[i][j].amount++;
			if (Map[i][j + 1].color		&& j + 1 < N)				Map[i][j].amount++;
			if (Map[i + 1][j - 1].color && i + 1 < N && j - 1 > 0)	Map[i][j].amount++;
			if (Map[i + 1][j].color		&& i + 1 < N)				Map[i][j].amount++;
			if (Map[i + 1][j + 1].color	&& i + 1 < N && j + 1 < N)	Map[i][j].amount++;
		}
	}
}

void reloadMap() {
	reloadAmount();
	//B3-S23
	//for (int i = 0; i < N; i++) {
	//	for (int j = 0; j < N; j++) {
	//		if (Map[i][j].color == 1) {
	//			if (Map[i][j].amount == 3 || Map[i][j].amount == 2) Map[i][j].color = 1;
	//			else Map[i][j].color = 0;
	//		}
	//		else if (Map[i][j].color == 0) {
	//			if (Map[i][j].amount == 3) Map[i][j].color = 1;
	//			else Map[i][j].color = 0;
	//		}
	//	}
	//}

	//B5678-S45678
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (Map[i][j].color == 1) {
				if (Map[i][j].amount >= 4) Map[i][j].color = 1;
				else Map[i][j].color = 0;
			}
			else if (Map[i][j].color == 0) {
				if (Map[i][j].amount >=5) Map[i][j].color = 1;
				else Map[i][j].color = 0;
			}
		}
	}
}

void DrawQuad(int i, int j) {
	if (Evolv == 1) {
		glColor3f(0, 0, 0);
	}
	else if (Evolv == 0) {
		glColor3f(0, 0, 0.5);
	}

	glLoadIdentity();

	glScalef(xCell, yCell, 0);  
	glTranslatef((-N / 2.0) + i, (N / 2.0) - j, 0);


	glBegin(GL_QUADS);
		glVertex2f(0,0);
		glVertex2f(0,-1);
		glVertex2f(1,-1);
		glVertex2f(1,0);
	glEnd();
}

void InverseColor(int i, int j) {
	if (!Draw == Map[i][j].color)
		Map[i][j].color = Draw;
	Map[i][j].was_changed = 1;
}


void ClickMouse(int button, int state, int x, int y)
{
	if (Evolv == 0) //Отслеживается только если не режим развития
	{
		if (button == GLUT_LEFT_BUTTON && (state == GLUT_DOWN))
		{
			int i = y / (width / N), j = x / (height / N);
			if (Map[i][j].color == 1) Draw = 0;
			else Draw = 1;
			InverseColor(i, j);

			cout << x << ' ' << y << endl;

		}
		if (button == GLUT_LEFT_BUTTON && (state == GLUT_UP))
		{
			for (int i = 0; i < N; i++)
				for (int j = 0; j < N; j++) //рисование плиток
					Map[j][i].was_changed = 0;
		}
	}
	if (button == GLUT_RIGHT_BUTTON && (state == GLUT_DOWN))
	{
		if (Evolv == 1) Evolv = 0;
		else if (Evolv == 0) Evolv = 1;
	}
		
	glutPostRedisplay();
}

void MoveMouse(int x, int y)
{
	int i = y / (width / N), j = x / (height / N);
	if (Map[i][j].was_changed == 0) {
		InverseColor(i, j);
	}
	cout << x << ' ' << y << endl;
	glutPostRedisplay();
}

void display() {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (Evolv == 1) {
		reloadMap();
	}

	glColor3f(0, 0, 0);
	glLineWidth(1);
	for (int i = 0; i < N + 1; i++) {//horizontal lines
		glLoadIdentity();
		glBegin(GL_LINES);
		glVertex2f(-1, 1 - (yCell * i));
		glVertex2f(1, 1 - (yCell * i));
		glEnd();
		glLoadIdentity();
		glBegin(GL_LINES);
		glVertex2f(1 - (yCell * i), -1);
		glVertex2f(1 - (yCell * i), 1);
		glEnd();
	}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			{
				if (Map[j][i].color == 1)
					DrawQuad(i, j);
			}
		}


	if(Evolv)
		Sleep(SleepTime);
	glFlush();
}

void timer(int = 0)
{
	display();
	glutTimerFunc(50, timer, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(460, 40);
	glutCreateWindow("Game of life");
	glutDisplayFunc(display);
	glutMouseFunc(ClickMouse);
	glutMotionFunc(MoveMouse);
	glutKeyboardFunc(MenuKeyboard);
	glutTimerFunc(50, timer, 0);
	glutMainLoop();
	return 0;
}