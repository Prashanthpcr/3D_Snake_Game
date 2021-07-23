#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <GL/glut.h>
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
GLint   pres_x = 5;
GLint   pres_z = 10;
GLint   prev_X[2] = {};
GLint   prev_Z[2] = {};
GLbyte  dir = 0;
GLint   fx = 0;
GLint   fz = 0;
GLint   wid = 800;
GLint   hei = 550;
GLint   iwid = 0;
GLint   ihei = 0;
GLint   fwid = 150;
GLint   fhei = 150;
GLint   level = 1;
GLint   score = 0;
GLint   length = 0;
GLbyte  dead = true;
GLbyte  E_Light = true;
GLint   body_Coor[2][100] = { {} };
static GLfloat rotate_x = 45.0F;
static GLfloat rotate_y = 0.0F;
static GLfloat rotate_z = 0.0F;
static GLfloat rotate_h = 90.0F;
static GLfloat zoom = -300.0F;
DWORD   g_dwLastFPS = 0;
int		g_nFPS = 0, g_nFrames = 0;

void Special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT:
		if (dir != LEFT)
			dir = RIGHT;
		break;
	case GLUT_KEY_LEFT:
		if (dir != RIGHT)
			dir = LEFT;
		break;
	case GLUT_KEY_UP:
		if (dir != UP)
			dir = DOWN;
		break;
	case GLUT_KEY_DOWN:
		if (dir != DOWN)
			dir = UP;
		break;
	}
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1, 800.0);
}

void ManipulateViewAngle() {
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	glRotatef(rotate_z, 0.0, 0.0, 1.0);
}

void Reset() {
	pres_x = 5;
	pres_z = 10;
	dir = 0;
	level = 1;
	score = 0;
	length = 0;
	dead = false;
	rotate_x = 45.0F;
	rotate_y = 0.0F;
	rotate_z = 0.0F;
	rotate_h = 90.0F;
	zoom = -300.0F;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'x': rotate_x += 2;
		glutPostRedisplay();
		break;
	case 'X': rotate_x -= 2;
		glutPostRedisplay();
		break;
	case 'y': rotate_y += 2;
		glutPostRedisplay();
		break;
	case 'Y': rotate_y -= 2;
		glutPostRedisplay();
		break;
	case 'z': rotate_z += 2;
		glutPostRedisplay();
		break;
	case 'Z': rotate_z -= 2;
		glutPostRedisplay();
		break;
	case 'a': length++;
		glutPostRedisplay();
		break;
	case 'A': length--;
		glutPostRedisplay();
		break;
	case '+': zoom++;
		glutPostRedisplay();
		break;
	case '-': zoom--;
		glutPostRedisplay();
		break;
	case 'n': Reset();
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void Write(char const* string) {
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
}

void WelcomeScreen() {
	char tmp_str[40];

	glColor3f(0.0, 0.0, 0);
	glRasterPos2f(20, 20);
	Write("3D Snake Game.");

	glColor3f(0, 0, 1);
	glRasterPos2f(0, 0);
	Write("To Start Playing,press 'n'.");
}

void DrawSnake() {
	int  i;
	glPushMatrix();
	ManipulateViewAngle();
	glPushMatrix();
	glColor3f(0.2, 0.2, 0.2);
	glTranslatef(75.0, -16.0, 75.0);
	glScalef(155, 5.0, 155);
	glutSolidCube(1);
	glPopMatrix();
	glColor3f(1.0, 0.86, 0);
	glTranslatef(pres_x, -10.0, pres_z);
	glScalef(0.5, 0.5, 0.5);
	glutSolidSphere(10, 20, 20);
	glRotatef(rotate_h, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(0, 0.0, 6.0);
	glScalef(0.8, 1.0, 1.0);
	glutSolidCone(10, 10, 20, 20);
	glColor3f(0, 0, 0);
	glTranslatef(-4.0, 10.0, 0.0);
	glScalef(0.3, 0.3, 0.3);
	glutSolidSphere(10, 20, 20);
	glTranslatef(26.0, 0.0, 0.0);
	glutSolidSphere(10, 20, 20);
	glPopMatrix();
	for (i = 0; i < length; i++) {
		glPushMatrix();
		ManipulateViewAngle();
		glTranslatef(body_Coor[0][i], -10.0, body_Coor[1][i]);
		glColor3f(0.91, 0.06, 0.17);
		glScalef(0.5, 0.5, 0.5);
		glutSolidSphere(7, 20, 20);
		glPopMatrix();
	}
}

void DrawFood()
{
	glPushMatrix();
	ManipulateViewAngle();
	glTranslatef(fx, -10.0, fz);
	glColor3f(0.4, 0.7, 0.4);
	glScalef(0.5, 0.5, 0.5);
	glutSolidSphere(7, 20, 20);
	glPopMatrix();
}

int RandomNumber(int high, int low)
{
	return (rand() % (high - low)) + low;
}

void newFood() {
	time_t seconds;
	time(&seconds);
	srand((unsigned int)seconds);
	fx = RandomNumber(fwid - iwid, iwid + 10);
	fz = RandomNumber(fhei - ihei, ihei + 10);
}

void GameStatus() {
	char tmp_str[40];
	glColor3f(0.18, 0.28, 0.29);
	glRasterPos2f(160, 70);
	sprintf_s(tmp_str, "Level: %d Score: %d", level, score);
	Write(tmp_str);
	glRasterPos2f(10, 20);
	Write("Press the arrow keys,to start.");
}

bool collision() {
	for (int i = 0; i < length; i++) {
		if ((body_Coor[0][i] == pres_x && body_Coor[1][i] == pres_z) ||
			((body_Coor[0][i] >= pres_x) && (body_Coor[0][i] <= pres_x + 5) && (body_Coor[1][i] >= pres_z) && (body_Coor[1][i] <= pres_z + 5)) ||
			((body_Coor[0][i] <= pres_x) && (body_Coor[0][i] >= pres_x - 5) && (body_Coor[1][i] <= pres_z) && (body_Coor[1][i] >= pres_z - 5)) ||
			((body_Coor[0][i] <= pres_x) && (body_Coor[0][i] >= pres_x - 5) && (body_Coor[1][i] >= pres_z) && (body_Coor[1][i] <= pres_z + 5)) ||
			((body_Coor[0][i] >= pres_x) && (body_Coor[0][i] <= pres_x + 5) && (body_Coor[1][i] <= pres_z) && (body_Coor[1][i] >= pres_z - 5)))
			return true;
	}
	return false;
}

void Run(int value) {
	prev_X[1] = pres_x;
	prev_Z[1] = pres_z;
	switch (dir) {
	case RIGHT:
		rotate_h = 90;
		pres_x += 6;
		if (pres_x > fwid - 2) pres_x = iwid - 1;
		break;
	case LEFT:
		rotate_h = -90;
		pres_x -= 6;
		if (pres_x < 0) pres_x = fwid - 2;
		break;
	case UP:
		rotate_h = 0;
		pres_z += 6;
		if (pres_z > fhei - 2) pres_z = ihei - 1;
		break;
	case DOWN:
		rotate_h = 180;
		pres_z -= 6;
		if (pres_z < 2) pres_z = fhei - 2;
		break;
	}

	if (collision()) dead = true;

	if ((pres_x == fx && pres_z == fz) ||
		((pres_x >= fx) && (pres_x <= fx + 4) && (pres_z >= fz) && (pres_z <= fz + 4)) ||
		((pres_x <= fx) && (pres_x >= fx - 4) && (pres_z <= fz) && (pres_z >= fz - 4)) ||
		((pres_x <= fx) && (pres_x >= fx - 4) && (pres_z >= fz) && (pres_z <= fz + 4)) ||
		((pres_x >= fx) && (pres_x <= fx + 4) && (pres_z <= fz) && (pres_z >= fz - 4))) {
		score++;
		if (score < 100) length++;
		if (score % 5 == 0 && level < 15) level++;
		newFood();
	}

	for (int i = 0; i < length; i++) {
		prev_X[0] = prev_X[1];
		prev_Z[0] = prev_Z[1];
		prev_X[1] = body_Coor[0][i];
		prev_Z[1] = body_Coor[1][i];
		body_Coor[0][i] = prev_X[0];
		body_Coor[1][i] = prev_Z[0];
	}

	glutTimerFunc(130 - level * 4, Run, 0);
}

void initLight()
{
	GLfloat ambientColor[] = { 0.3f, 0.4f, 0.8f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	GLfloat lightColor0[] = { 0.95f, 0.5f, 0.5f, 1.0f };
	GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f };
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
}

void myInit(void)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3, 0.80, 0.9, 0.0f); 
	if (E_Light) {
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING); 
		glEnable(GL_LIGHT0); 
		glEnable(GL_LIGHT1); 
		glEnable(GL_NORMALIZE); 
	}
}

void getFPS() {
	char tmp_str[40];

	if (GetTickCount64() - g_dwLastFPS >= 1000)
	{
		g_dwLastFPS = GetTickCount64();
		g_nFPS = g_nFrames;
		g_nFrames = 0;
	}
	g_nFrames++;

	glRasterPos2f(-110, 60);
	sprintf_s(tmp_str, "FPS: %d", g_nFPS);
	Write(tmp_str);

	glRasterPos2f(-120, 70);
	sprintf_s(tmp_str, "Pos X: %d Pos Z: %d", pres_x, pres_z);
	Write(tmp_str);
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(132, 13, 3, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (E_Light) initLight();
	glTranslatef(-60.0, 40.0, zoom);
	if (!dead) {
		GameStatus();
		DrawFood();
		DrawSnake();
	}
	else {
		WelcomeScreen();
	}
	getFPS();
	glutPostRedisplay();
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(wid, hei);
	glutInitWindowPosition(80, 80);
	glutCreateWindow("3D Snake Game");
	glutSpecialFunc(Special);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(Display);
	glutReshapeFunc(resize);
	newFood();
	Run(0);
	myInit();
	glutMainLoop();
	return 0;
}