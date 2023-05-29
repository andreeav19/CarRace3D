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
float x = 0.0f, z = 5.0f;

const char* flowerColors[4] = { "pink", "blue", "red", "purple" };
const char* color;

float speed = 0.0;
float carRotationAngle = 0;

float roadPosition = 0.0f;

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

void drawIntermittentLines()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.00f, -roadPosition);

	glColor3f(1.0f, 1.0f, 1.0f); // White color for the lines
	glLineWidth(3.0f);

	glBegin(GL_LINES);
	for (int i = 0; i < 1500; i++) {
		// First intermittent line
		glVertex3f(-3.34, 0.0f, - i * 7);
		glVertex3f(-3.34, 0.0f, - i * 7 + 5);

		// Second intermittent line
		glVertex3f(3.34, 0.0f, - i * 7);
		glVertex3f(3.34, 0.0f, - i * 7 + 5);
	}
	glEnd();

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

	roadPosition -= speed;

	// Draw gray road in the middle
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, roadPosition);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	glVertex3f(-10.0f, 0.0f, -100.0f);
	glVertex3f(-10.0f, 0.0f, 100.0f);
	glVertex3f(10.0f, 0.0f, 100.0f);
	glVertex3f(10.0f, 0.0f, -100.0f);
	glEnd();
	


	drawIntermittentLines();

	// Draw green grass on the left side of the road
	glColor4f(0.0f, 0.5f, 0.0f, 0.8f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(-10.0f, 0.0f, 100.0f);
	glVertex3f(-10.0f, 0.0f, -100.0f);
	glEnd();

	// Draw green grass on the right side of the road
	glBegin(GL_QUADS);
	glVertex3f(10.0f, 0.0f, -100.0f);
	glVertex3f(10.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

	glPopMatrix(); // end road transition 

	// Draw Flowers
	float mi = 0.0; // multiply i
	for (int c = 0; c < 4; c++) {
		color = flowerColors[c];
		mi = mi + 2.0;
		for (int i = -5; i < 5; i++)
			for (int j = -5; j < 5; j++)
			{
				if (i * mi < -10 || i * mi > 10) {
					glPushMatrix();
					glTranslatef(i * mi, 0.05f, j * mi);
					glScalef(0.5, 0.5, 0.5);
					glRotatef(90.0, 1.0, 0.0, 0.0);
					drawFlower();
					glPopMatrix();
					glPopMatrix();
					glPopMatrix();
				}
			}
	}

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
	speed = 0;
}

void moveCarForwards() {
	speed = 0.25;
	x += lx * speed;
	z += lz * speed;
}

void moveCarBackwards() {
	speed = 0.25;
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