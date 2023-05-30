/* Deplasarea observatorului intr-o scena 3D
SURSA:  lighthouse3D:  http://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving-around-the-world/
Elemente de retinut:
- folosirea functiilor de desenare pentru a schita obiecte 3D
- schimbarea pozitiei observatorului se face in functia gluLookAt
- folosirea glutSpecialFunc si glutKeyboardFunc pentru interactiunea cu tastatura
*/

#include<gl/freeglut.h>
#include<math.h>
#include <iostream>
using namespace std;
// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 0.0f;

const char* flowerColors[4] = { "pink", "blue", "red", "purple" };
const char* color;

float speed = 0.4;
float carRotationAngle = 0;

float oppositeCarZ = 0;
float oppositeCarXArr[3] = { -6.5, 0, 6.5 };
float oppositeCarX = oppositeCarXArr[rand() % 3];

float PI = 3.14;

void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void drawFlower()
{
	float radius = 0.5f;
	int numPetals = 5;
	float angleIncrement = 360.0f / numPetals;

	if (strcmp(color, "blue") == 0)
		glColor3f(0.04, 0.54, 0.99);
	else if (strcmp(color, "pink") == 0)
		glColor3f(0.99, 0.58, 0.94);
	else if (strcmp(color, "red") == 0)
		glColor3f(1, 0.11, 0.33);
	else if (strcmp(color, "purple") == 0)
		glColor3f(0.71, 0.18, 0.79);

	// Draw the petals
	for (int i = 0; i < numPetals; i++)
	{
		glPushMatrix();

		// Rotate each petal around the center of the flower
		glRotatef(i * angleIncrement, 0.0f, 0.0f, 1.0f);

		// Translate to the position of the petal
		glTranslatef(radius * 0.2, 0.0f, 0.0f);

		// Draw the petal as a sphere
		glutSolidSphere(0.1f, 10, 10);

		glPopMatrix();
	}

	// Draw the center of the flower as a yellow sphere
	glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for the center
	glutSolidSphere(radius * 0.2f, 20, 20);
}

void drawMainCar() {
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glScalef(0.75, 0.9, 2);
	glutSolidCube(0.25f);
	glPopMatrix();
}

void drawOpposingCar() {
	glColor3f(0.0f, 0.0f, 1.0f);
	glutSolidCube(1.5f);
}

void drawCoin() {
	float radius = 0.5, height = 0.05, sides = 50;
	float angle;
	float x, y;
	float step = 2.0f * PI / sides;

	glColor3f(1, 0.84, 0.33);

	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++) {
		angle = i * step;
		x = 0.15 * cosf(angle);
		y = 0.20 * sinf(angle);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();

	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++) {
		angle = i * step;
		x = 0.15 * cosf(angle);
		y = 0.20 * sinf(angle);
		glVertex3f(x, y, height);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= sides; i++) {
		angle = i * step;
		x = 0.15 * cosf(angle);
		y = 0.20 * sinf(angle);
		glVertex3f(x, y, 0.0f);
		glVertex3f(x, y, height);
	}
	glEnd();

	glColor3f(0.92, 0.71, 0.17);

	// Define control points for the two ovals
	// First oval
	GLdouble controlPoints1[8][3] = {
		{0.0, -0.2, height + 0.001},
		{0.2, -0.2, height + 0.001},
		{0.2, 0.2, height + 0.001},
		{0.0, 0.2, height + 0.001},
		{0.0, 0.2, height + 0.001},
		{-0.2, 0.2, height + 0.001},
		{-0.2, -0.2, height + 0.001},
		{0.0, -0.2, height + 0.001}
	};

	// Second oval
	GLdouble controlPoints2[8][3] = {
		{0.0, -0.2, -0.001},
		{0.2, -0.2, -0.001},
		{0.2, 0.2, -0.001},
		{0.0, 0.2, -0.001},
		{0.0, 0.2, -0.001},
		{-0.2, 0.2, -0.001},
		{-0.2, -0.2, -0.001},
		{0.0, -0.2, -0.001}
	};

	// Draw model on the front of the coin
	// Draw first oval on the front
	glPushMatrix();
	glTranslatef(0.0, 0.025, 0.0);

	glMap1d(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 8, &controlPoints1[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glMapGrid1d(100, 0.0, 1.0);
	glEvalMesh1(GL_LINE, 0, 100);

	glPopMatrix();

	// draw crown
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, height + 0.001);
	glVertex3f(-0.08, 0.02, height + 0.001);
	glVertex3f(-0.04, -0.02, height + 0.001);
	glVertex3f(0.0, 0.06, height + 0.001);
	glVertex3f(0.04, -0.02, height + 0.001);
	glVertex3f(0.08, 0.02, height + 0.001);
	glVertex3f(0.08, -0.06, height + 0.001);
	glVertex3f(-0.08, -0.06, height + 0.001);
	glVertex3f(-0.08, 0.02, height + 0.001);

	glEnd();

	// Draw model on the back of the coin
	// Draw second oval on the back
	glPushMatrix();
	glTranslatef(0.0, 0.025, 0.0);

	glMap1d(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 8, &controlPoints2[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glMapGrid1d(100, 0.0, 1.0);
	glEvalMesh1(GL_LINE, 0, 100);

	glPopMatrix();

	// draw crown
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, -0.001);
	glVertex3f(-0.08, 0.02, -0.001);
	glVertex3f(-0.04, -0.02, -0.001);
	glVertex3f(0.0, 0.06, -0.001);
	glVertex3f(0.04, -0.02, -0.001);
	glVertex3f(0.08, 0.02, -0.001);
	glVertex3f(0.08, -0.06, -0.001);
	glVertex3f(-0.08, -0.06, -0.001);
	glVertex3f(-0.08, 0.02, -0.001);

	glEnd();
}

void drawFuelTank() {
	float height = 0.8f;
	float sides = 50;
	float angle;
	float x, y;
	float step = 2.0f * PI / sides;

	glColor3f(1, 0.0, 0.0);

	glPushMatrix();
	glRotatef(90, 1, 0, 0);

	// Draw body
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++) {
		angle = i * step;
		x = 0.5 * cosf(angle);
		y = 0.3 * sinf(angle);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();

	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++) {
		angle = i * step;
		x = 0.5 * cosf(angle);
		y = 0.3 * sinf(angle);
		glVertex3f(x, y, height);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= sides; i++) {
		angle = i * step;
		x = 0.5 * cosf(angle);
		y = 0.3 * sinf(angle);
		glVertex3f(x, y, 0.0f);
		glVertex3f(x, y, height);
	}
	glEnd();


	// Draw handle
	glPushMatrix();
	glTranslatef(0.6, 0, 0.4);
	glScalef(0.05, 0.05, 0.4);
	glutSolidCube(1.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.55, 0, 0.2);
	glScalef(0.15, 0.05, 0.15);
	glutSolidCube(1.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.55, 0.0, 0.6);
	glScalef(0.15, 0.05, 0.15);
	glutSolidCube(1.0f);
	glPopMatrix();

	// Draw cap
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(-0.3, 0.0, -0.1);
	glutSolidCylinder(0.05, 0.1, 50, 20);
	glPopMatrix();

	void* font = GLUT_STROKE_ROMAN;

	// Set the size of the letter
	float letterSize = 0.004;

	// Set the position of the letter

	glPushMatrix();
	glTranslatef(-0.1, 0.301, 0.6);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glScalef(letterSize, letterSize, 1.0);
	glutStrokeCharacter(font, 'F');
	glPopMatrix();

	glPopMatrix();
}


void renderScene(void)
{
	glClearColor(0.51, 0.78, 0.78, 1.0);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Set the camera
	gluLookAt(x, 1.0f, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);

	float visibleRoadStartZ = z - 100.0f;
	float visibleRoadEndZ = z + 100.0f;

	// Draw gray road in the middle
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	glVertex3f(-10.0f, 0.0f, visibleRoadStartZ);
	glVertex3f(-10.0f, 0.0f, visibleRoadEndZ);
	glVertex3f(10.0f, 0.0f, visibleRoadEndZ);
	glVertex3f(10.0f, 0.0f, visibleRoadStartZ);
	glEnd();

	// Draw intermittent lines on the gray road
	glColor3f(1.0f, 1.0f, 1.0f); // White color for the lines
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	for (int i = -1400; i < 1400; i++) {
		// First intermittent line
		glVertex3f(-3.34, 0.01f, -100.0f + i * 7);
		glVertex3f(-3.34, 0.01f, -100.0f + i * 7 + 5);

		// Second intermittent line
		glVertex3f(3.34, 0.01f, -100.0f + i * 7);
		glVertex3f(3.34, 0.01f, -100.0f + i * 7 + 5);
	}
	glEnd();

	// Draw green grass on the left side of the road
	glColor4f(0.0f, 0.5f, 0.0f, 0.8f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, visibleRoadStartZ);
	glVertex3f(-100.0f, 0.0f, visibleRoadEndZ);
	glVertex3f(-10.0f, 0.0f, visibleRoadEndZ);
	glVertex3f(-10.0f, 0.0f, visibleRoadStartZ);
	glEnd();

	// Draw green grass on the right side of the road
	glBegin(GL_QUADS);
	glVertex3f(10.0f, 0.0f, visibleRoadStartZ);
	glVertex3f(10.0f, 0.0f, visibleRoadEndZ);
	glVertex3f(100.0f, 0.0f, visibleRoadEndZ);
	glVertex3f(100.0f, 0.0f, visibleRoadStartZ);
	glEnd();

	// Draw Flowers
	for (int i = -62; i < 62; i++) {
		color = flowerColors[(abs(i) % 3 + 3) % 3];
		glPushMatrix();
		glTranslatef(13, 0.05f, i * 25);
		glScalef(0.5, 0.5, 0.5);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		drawFlower();
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}
	for (int i = -62; i < 62; i++) {
		color = flowerColors[(abs(i) % 3 + 3) % 3];
		glPushMatrix();
		glTranslatef(-13, 0.05f, i * 25);
		glScalef(0.5, 0.5, 0.5);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		drawFlower();
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}

	// draw coin - if u want to see it i mean
	glPushMatrix();
	glTranslatef(0, 1.0, -7);
	drawCoin();
	// drawFuelTank();

	//draw oposing car
	if (z <= 0) {
		oppositeCarZ += speed + 0.25;
	}
	else {
		oppositeCarZ -= speed + 0.25;
	}
	glPopMatrix();
	if (oppositeCarZ > visibleRoadEndZ) {
		oppositeCarZ = visibleRoadStartZ;
		oppositeCarX = oppositeCarXArr[rand() % 3];
	}
	else if (oppositeCarZ < visibleRoadStartZ) {
		oppositeCarZ = visibleRoadEndZ;
		oppositeCarX = oppositeCarXArr[rand() % 3];
	}
	glPushMatrix();
	glTranslatef(oppositeCarX, 0.1, oppositeCarZ);
	drawOpposingCar();
	glPopMatrix();

	// draw main car
	glPushMatrix();
	glTranslatef(x, 0.825f, z);
	glRotatef(0 - angle * 57.3, 0, 1, 0);
	drawMainCar();
	glPopMatrix();

	glutPostRedisplay();
	glutSwapBuffers();
}

bool keys[256]; // array to store the state of each key

void keyboard(unsigned char key, int xx, int yy)
{
	keys[key] = true;
}

void keyboardUp(unsigned char key, int x, int y)
{
	keys[key] = false; // set the corresponding element to false when a key is released
	if (key == 'w' || key == 's') {
		speed = 0;
	}
}

void moveCarForwards() {
	if (speed < 0.4) {
		speed += 0.05;
	}
	x += lx * speed;
	z += lz * speed;
}

void moveCarBackwards() {
	if (speed < 0.4) {
		speed += 0.05;
	}
	x -= lx * speed;
	z -= lz * speed;
}

void turnRight() {
	angle += 0.02f;
	lx = sin(angle);
	lz = -cos(angle);
}

void turnLeft() {
	angle -= 0.02f;
	lx = sin(angle);
	lz = -cos(angle);
}


void handleKeys()
{
	if (keys['w']) {
		moveCarForwards();
	}
	if (keys['s']) {
		moveCarBackwards();
	}
	if (keys['d']) {
		turnRight();
	}
	if (keys['a']) {
		turnLeft();
	}
	if (keys['e'] && speed < 0.8) {
		speed += 0.05;
	}
}

int main(int argc, char** argv)
{
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 10);
	glutInitWindowSize(1200, 768);
	glutCreateWindow("Very cool 3D car game (nr.1 on steam, overwhelmingly positive reviews)");

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);

	glutIdleFunc(handleKeys);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}