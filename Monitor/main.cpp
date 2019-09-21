
#include <windows.h>
#include "TextureManager.h"
#include "Cube.h"
#include "Solver.h"



static Cube cube;

static GLint index[3];
static GLint direction[3];
static HANDLE handle;
static GLboolean solving;

DWORD WINAPI solveCube(LPVOID lpParameter)
{
	Solver::solve(cube);

	solving = 0;
	return 0;
}


void keyboardFunc(unsigned char k, int x, int y)
{
	if (solving == 1)
		return;

	switch (k)
	{	
	case 'w':
		index[0] = 1;
		index[1] = 1;
		index[2] = 1;
		direction[0] = -1;
		direction[1] = 0;
		direction[2] = 0;
		cube.startRotate(index, direction);
		break;
	case 's':
		index[0] = 1;
		index[1] = 1;
		index[2] = 1;
		direction[0] = 1;
		direction[1] = 0;
		direction[2] = 0;
		cube.startRotate(index, direction);
		break;
	case 'a':
		index[0] = 1;
		index[1] = 1;
		index[2] = 1;
		direction[0] = 0;
		direction[1] = -1;
		direction[2] = 0;
		cube.startRotate(index, direction);
		break;
	case 'd':
		index[0] = 1;
		index[1] = 1;
		index[2] = 1;
		direction[0] = 0;
		direction[1] = 1;
		direction[2] = 0;
		cube.startRotate(index, direction);
		break;
	case 'r':
		index[0] = 0;
		index[1] = 0;
		index[2] = 1;
		direction[0] = -1;
		direction[1] = 0;
		direction[2] = 0;
		cube.startRotate(index, direction);
		break;
	case 'e':
		index[0] = 0;
		index[1] = 0;
		index[2] = 1;
		direction[0] = 1;
		direction[1] = 0;
		direction[2] = 0;
		cube.startRotate(index, direction);
		break;
	case 'u':
		index[0] = 0;
		index[1] = 0;
		index[2] = 1;
		direction[0] = 0;
		direction[1] = -1;
		direction[2] = 0;
		cube.startRotate(index, direction);
		break;
	case 'y':
		index[0] = 0;
		index[1] = 0;
		index[2] = 1;
		direction[0] = 0;
		direction[1] = 1;
		direction[2] = 0;
		cube.startRotate(index, direction);
		break;
	case 'f':
		index[0] = 0;
		index[1] = 0;
		index[2] = 1;
		direction[0] = 0;
		direction[1] = 0;
		direction[2] = -1;
		cube.startRotate(index, direction);
		break;
	case 'g':
		index[0] = 0;
		index[1] = 0;
		index[2] = 1;
		direction[0] = 0;
		direction[1] = 0;
		direction[2] = 1;
		cube.startRotate(index, direction);
		break;
	case 'l':
		index[0] = 1;
		index[1] = 0;
		index[2] = 0;
		direction[0] = 1;
		direction[1] = 0;
		direction[2] = 0;
		cube.startRotate(index, direction);
		break;
	case 'k':
		index[0] = 1;
		index[1] = 0;
		index[2] = 0;
		direction[0] = -1;
		direction[1] = 0;
		direction[2] = 0;
		cube.startRotate(index, direction);
		break;
	case 'm':
		index[0] = 0;
		index[1] = 1;
		index[2] = 0;
		direction[0] = -1;
		direction[1] = 0;
		direction[2] = 0;
		cube.startRotate(index, direction);
		break;
	case 'n':
		index[0] = 0;
		index[1] = 1;
		index[2] = 0;
		direction[0] = 1;
		direction[1] = 0;
		direction[2] = 0;
		cube.startRotate(index, direction);
		break;
	case 'p':
		solving = 1;
		handle = CreateThread(NULL, 0, solveCube, NULL, 0, NULL);
		CloseHandle(handle);
		WaitForSingleObject(handle, INFINITE);
		break; 
	case '1':
		cube.clearAll();
		break;

	default:
		break;

	}
}

void keyboardUpFunc(unsigned char k, int x, int y)
{
	
}

void passiveMotionFunc(int x, int y)
{

}

void mouseFunc(int button, int state, int x, int y)
{

}

void idleFunc(void)
{
	glutPostRedisplay();
}

void reshapeFunc(int width, int height)
{
	if (height == 0)									
		height = 1;										

	glViewport(0, 0, width, height);					

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();									
														
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 210.0f);

	glMatrixMode(GL_MODELVIEW);
}

void displayFunc(void)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(8, 10, 0, 0, 0, -10, 0, 1, 0);
	cube.drawCube();
		
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	
	glutCreateWindow("CubeMonitor");;

	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutKeyboardFunc(keyboardFunc);
	glutKeyboardUpFunc(keyboardUpFunc);
	glutPassiveMotionFunc(passiveMotionFunc);
	glutMouseFunc(mouseFunc);
	glutIdleFunc(idleFunc);

	cube.initCube();
	Solver::init();

	glutMainLoop();
	return 0;

}

