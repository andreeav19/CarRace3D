/* Deplasarea observatorului intr-o scena 3D
SURSA:  lighthouse3D:  http://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving-around-the-world/
Elemente de retinut:
- folosirea functiilor de desenare pentru a schita obiecte 3D
- schimbarea pozitiei observatorului se face in functia gluLookAt
- folosirea glutSpecialFunc si glutKeyboardFunc pentru interactiunea cu tastatura
*/

#include<gl/freeglut.h>
#include<math.h>
#include <string>
#include <iostream>
#include "SOIL.h"

using namespace std;
// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 0.0f;

bool carCollision = false;
bool coinCollision = false;
bool fuelCollision = false;

const char* flowerColors[4] = { "pink", "blue", "red", "purple" };
const char* color;

float speed = 0.4;
float carRotationAngle = 0;

float oppositeCarZ = -100;
float oppositeCarXArr[3] = { -6.5, 0, 6.5 };
float oppositeCarX = oppositeCarXArr[rand() % 3];

float coinPosX = oppositeCarXArr[rand() % 3];
float coinPosZ = -50;

float fuelPosXArr[3] = { -6.5, 0, 6.5 };
float fuelPosX = fuelPosXArr[rand() % 3];
float fuelPosZ = -150;

int points = 0;

float PI = 3.14;

// textures
GLuint grassTexture, roadTexture, skyTexture, cityTexture;

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
	glPushAttrib(GL_LIGHTING_BIT | GL_BLEND);

	GLfloat shadowAmbient[] = { 0.2f, 0.2f, 0.2f, 0.5f };
	GLfloat shadowDiffuse[] = { 0.2f, 0.2f, 0.2f, 0.5f };
	GLfloat shadowSpecular[] = { 0.0f, 0.0f, 0.0f, 0.5f };
	GLfloat shadowShininess = 0.0f;

	GLfloat mat_ambient[] = { 0.7f, 0.0f, 0.0f, 1.0f };
	GLfloat mat_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[] = { 50.0f };

	// Set car's material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// Draw the car
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glScalef(0.75, 0.9, 2);
	glutSolidCube(0.25f);
	glPopMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw shadow in front of the car
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, shadowAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, shadowDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, shadowSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shadowShininess);
	glTranslatef(0.1f, 0.01f, -0.5f);
	glScalef(1.5f, 0.0f, 3.0f);
	glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
	glutSolidCube(0.25f);
	glPopMatrix();

	glDisable(GL_BLEND);

	// Reset car's material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// Draw the car again
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glScalef(0.75, 0.9, 2);
	glutSolidCube(0.25f);
	glPopMatrix();

	glPopAttrib();
}

void setLight(void) {
	GLfloat ambientLumina[] = { 0.7f, 0.7f, 0.6f, 1.0f };
	GLfloat difuzLumina[] = { 1.0f, 1.0f, 0.9f, 1.0f };;
	GLfloat alb[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat directieSpot[] = { x - lx, 1.0f, z - lz, 1.0f };

	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);

	// Set light properties
	glLightfv(GL_LIGHT0, GL_POSITION, directieSpot);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLumina);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difuzLumina);
	glLightfv(GL_LIGHT0, GL_SPECULAR, alb);
}

void drawOpposingCar() {
	glColor3f(0.0f, 0.0f, 1.0f);

	// Body

	glPushMatrix();
	glScalef(1.5, 0.5, 3.0);
	glutSolidCube(1.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.4, 0.0);
	glScalef(1.0, 0.4, 2.0);
	glutSolidCube(1.0f);
	glPopMatrix();

	// Wheels

	glColor3f(0.22, 0.23, 0.23);
	glPushMatrix();
	glTranslatef(-0.8, -0.25, 0.8);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glutSolidCylinder(0.25, 0.1, 50, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8, -0.25, -0.8);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glutSolidCylinder(0.25, 0.1, 50, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7, -0.25, 0.8);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glutSolidCylinder(0.25, 0.1, 50, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7, -0.25, -0.8);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glutSolidCylinder(0.25, 0.1, 50, 20);
	glPopMatrix();

	// Headlights
	glColor3f(0.9, 0.82, 0.37);

	glPushMatrix();
	glTranslatef(0.6, 0.1, 1.5);
	glutSolidCube(0.2f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 0.1, 1.5);
	glutSolidCube(0.2f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6, 0.1, -1.5);
	glutSolidCube(0.2f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 0.1, -1.5);
	glutSolidCube(0.2f);
	glPopMatrix();

	// Glass

	glColor3f(0.6, 0.92, 0.94);

	glBegin(GL_POLYGON);
	glVertex3f(-0.4, 0.25, 1.01);
	glVertex3f(-0.4, 0.55, 1.01);
	glVertex3f(0.4, 0.55, 1.01);
	glVertex3f(0.4, 0.25, 1.01);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.4, 0.25, -1.01);
	glVertex3f(-0.4, 0.55, -1.01);
	glVertex3f(0.4, 0.55, -1.01);
	glVertex3f(0.4, 0.25, -1.01);
	glEnd();


	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(-0.9, 0.25, 0.51);
	glVertex3f(-0.9, 0.55, 0.51);
	glVertex3f(0.9, 0.55, 0.51);
	glVertex3f(0.9, 0.25, 0.51);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(-0.9, 0.25, -0.51);
	glVertex3f(-0.9, 0.55, -0.51);
	glVertex3f(0.9, 0.55, -0.51);
	glVertex3f(0.9, 0.25, -0.51);
	glEnd();
	glPopMatrix();
}

void drawCoin() {
	float radius = 0.5, height = 0.05, sides = 50;
	float angle;
	float x, y;
	float step = 2.0f * PI / sides;

	glDisable(GL_LIGHTING);

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
	glEnable(GL_LIGHTING);
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

void drawSkybox()
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glPushMatrix();
	glTranslatef(x, 0.0f, z);
	glScalef(70.0f, 70.0f, 70.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cityTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_QUADS);
	// Top face
	glTexCoord2f(0.0f, 5.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(5.0f, 5.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(5.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cityTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_QUADS);
	// Bottom face
	glTexCoord2f(0.0f, 5.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(5.0f, 5.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(5.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cityTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_QUADS);
	// Front face
	glTexCoord2f(0.0f, 5.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(5.0f, 5.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(5.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cityTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_QUADS);
	// Back face
	glTexCoord2f(0.0f, 5.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(5.0f, 5.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(5.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cityTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_QUADS);
	// Left face
	glTexCoord2f(0.0f, 5.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(5.0f, 5.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(5.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cityTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_QUADS);
	// Right face
	glTexCoord2f(0.0f, 5.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(5.0f, 5.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(5.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPopAttrib();
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

	setLight();
	drawSkybox();
	glColor3f(1.0f, 1.0f, 1.0f);

	// Draw gray road in the middle
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, roadTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, visibleRoadStartZ);
	glTexCoord2f(20.0f, 0.0f); glVertex3f(-10.0f, 0.0f, visibleRoadEndZ);
	glTexCoord2f(20.0f, 20.0f); glVertex3f(10.0f, 0.0f, visibleRoadEndZ);
	glTexCoord2f(0.0f, 20.0f); glVertex3f(10.0f, 0.0f, visibleRoadStartZ);
	glEnd();
	glDisable(GL_TEXTURE_2D);

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
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, grassTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, 0.0f, visibleRoadStartZ);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(-100.0f, 0.0f, visibleRoadEndZ);
	glTexCoord2f(10.0f, 10.0f); glVertex3f(-10.0f, 0.0f, visibleRoadEndZ);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(-10.0f, 0.0f, visibleRoadStartZ);
	glEnd();

	// Draw green grass on the right side of the road
	glBindTexture(GL_TEXTURE_2D, grassTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glColor4f(0.0f, 0.5f, 0.0f, 0.8f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 0.0f, visibleRoadStartZ);
	glTexCoord2f(10.0f, 0.0f); glVertex3f(10.0f, 0.0f, visibleRoadEndZ);
	glTexCoord2f(10.0f, 10.0f); glVertex3f(100.0f, 0.0f, visibleRoadEndZ);
	glTexCoord2f(0.0f, 10.0f); glVertex3f(100.0f, 0.0f, visibleRoadStartZ);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_LIGHTING);

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
	glEnable(GL_LIGHTING);

	// draw coin - if u want to see it i mean
	if (visibleRoadEndZ < coinPosZ) {
		coinPosZ = visibleRoadStartZ + 10;
		coinPosX = oppositeCarXArr[rand() % 3];
	}
	glPushMatrix();
	glTranslatef(coinPosX, 0.5, coinPosZ);
	drawCoin();
	glPopMatrix();

	// draw fuel
	if (visibleRoadEndZ < fuelPosZ) {
		fuelPosZ = visibleRoadStartZ;
		fuelPosX = fuelPosXArr[rand() % 3];
	}
	glPushMatrix();
	glTranslatef(fuelPosX, 0.75, fuelPosZ);
	drawFuelTank();
	glPopMatrix();

	//draw oposing car
	if (z <= 0) {
		oppositeCarZ += speed + 0.2;
	}
	else {
		oppositeCarZ -= speed + 0.2;
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
	glTranslatef(oppositeCarX, 0.5, oppositeCarZ);
	drawOpposingCar();
	glPopMatrix();

	if (abs(oppositeCarZ - z) < 2 && abs(oppositeCarX - x) < 2) {
		carCollision = true;
	}

	if (abs(coinPosZ - z) < 2 && abs(coinPosX - x) < 2) {
		coinCollision = true;
	}

	if (abs(fuelPosZ - z) < 2 && abs(fuelPosX - x) < 2) {
		fuelCollision = true;
	}

	if (coinCollision) {
		// handle coin collision
		coinCollision = false;
		points += 100;
	}

	if (fuelCollision) {
		// handle fuel collision
		fuelCollision = false;
	}

	if (carCollision) {
		// handle collision here
		exit(1);
	}

	// draw main car
	glPushMatrix();
	glTranslatef(x, 0.825f, z);
	glRotatef(0 - angle * 57.3, 0, 1, 0);
	drawMainCar();
	glPopMatrix();

	// Switch to 2D rendering mode
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1200, 0, 768);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Disable depth testing to render the UI element on top
	glDisable(GL_DEPTH_TEST);

	// Render the UI element here
	// Example: Render a simple point counter at the top left corner
	glRasterPos2i(580, 10); // Set position
	std::string pointsString = "Points: " + std::to_string(points);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, reinterpret_cast<const unsigned char*>(pointsString.c_str()));

	// Re-enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Restore the original projection and modelview matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glutPostRedisplay();
	glutSwapBuffers();
}

void loadTextures(void) {
	grassTexture = SOIL_load_OGL_texture
	(
		"grass03.png",
		SOIL_LOAD_RGBA,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MIPMAPS
	);
	roadTexture = SOIL_load_OGL_texture
	(
		"road.png",
		SOIL_LOAD_RGBA,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MIPMAPS
	);
	cityTexture = SOIL_load_OGL_texture
	(
		"city.jpg",
		SOIL_LOAD_RGBA,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MIPMAPS
	);
}
void enableFog(void) {
	glEnable(GL_FOG);
	GLfloat fogColor[] = { 0.8f, 0.8f, 0.8f, 0.05f };
	glFogfv(GL_FOG_COLOR, fogColor);
	// Set fog parameters
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.03f);
	glFogf(GL_FOG_START, 100.0f);
	glFogf(GL_FOG_END, 150.0f);
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

	loadTextures();
	enableFog();

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