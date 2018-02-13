#include<iostream>
#include<stdlib.h>
#include"subdivisionSurface.h"
#include<stdlib.h>
#include<gl\glut.h>

using namespace std;

subdivisionSurface woot;


float fTranslate;
float fRotate = 0.0f;
float teaRotate = 0.0f;
float fScale = 1.0f;	// set inital scale value to 1.0f
float tdx = 0.0f, tdy = 0.0f, tdz = 0.0f;
bool bPersp = false;
bool bAnim = false;
bool bWire = true;
bool teaAnim = false;
int wHeight = 0;
int wWidth = 0;

//todo
//hint: some additional parameters may needed here when you operate the teapot

void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

void Draw_Scene()
{
	glBegin(GL_QUADS);
	int limit = 77777;
	int i = 0;
	//cout << "##############################" << endl;
	glPushMatrix();
	glRotatef(45, 0, 1, 0);
	for (auto &it : woot.surfaceQuads) {
		i++;
		//cout << it.id << " " << it.pointPosList.size() << endl;

		for (auto &it1 : it.pointPosList) {
			glVertex3f(it1.x, it1.y, it1.z);
			//cout << it1.x << " " << it1.y << " " << it1.z << endl;
		}
		//if (i > limit) {
		//	break;
		//}
	}
	glEnd();
	glPopMatrix();
	//cout << "##############################" << endl;
}

void updateView(int width, int height)
{
	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;

	if (bPersp) {
		gluPerspective(45.0f, whRatio, 0.1f, 100.0f);							//todo when 'p' operation, hint: use FUNCTION gluPerspective
	}
	else
		glOrtho(-3, 3, -3, 3, -100, 100);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}

float eye[] = { 0, 0, 8 };
float center[] = { 0, 0, 0 };
//todo; hint: you may need another ARRAY when you operate the teapot

void key(unsigned char k, int x, int y)
{
	switch (k)
	{
		case 27:
		case 'q': {exit(0); break; }
		case 'p': {bPersp = !bPersp;  break; }
		case ' ': {bAnim = !bAnim; break; }
		case 'o': {bWire = !bWire; break; }

		case 'a': {
			eye[0] -= 0.2f;
			center[0] -= 0.2f;
			break;
		}
		case 'd': {
			eye[0] += 0.2f;
			center[0] += 0.2f;
			break;
		}
		case 'w': {
			eye[1] -= 0.2f;
			center[1] -= 0.2f;
			break;
		}
		case 's': {
			eye[1] += 0.2f;
			center[1] += 0.2f;
			break;
		}
		case 'z': {
			eye[2] -= 0.2f;
			center[2] -= 0.2f;
			break;
		}
		case 'c': {
			eye[2] += 0.2f;
			center[2] += 0.2f;
			break;
		}
		case 'm': {
			woot.nextRound();
			woot.preDraw();
			//woot.showQuads();
			break;
		}
	}
	updateView(wHeight, wWidth);
}
void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);				// ������0��0��0�����ӵ����� (0,5,50)��Y������

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = { 5, 5, 5, 1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glEnable(GL_LIGHT0);

	//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Scene();						// Draw Scene

	if (bAnim) fRotate += 0.5f;
	if (teaAnim) teaRotate += 0.5f;
	//todo; hint: when you want to rotate the teapot, you may like to add another line here =)
	glutSwapBuffers();
}

int main(int argc, char *argv[]) {
	woot.initSurface();
	//woot.showQuads();
	woot.preDraw();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);
	int windowHandle = glutCreateWindow("Big Proj");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}