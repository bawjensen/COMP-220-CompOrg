#ifdef __APPLE__
	#include <GLUT/glut.h>
#elif __linux
	#include <GL/glut.h>
#endif

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include "classes.h"
#include "baseClasses.h"

using namespace std;

// -------------------------------------------------------------------------------------------

string DEMFileName = "";

int initialWindowWidth = 800;
int initialWindowHeight = 800;

int currentWindowWidth = initialWindowWidth;
int currentWindowHeight = initialWindowHeight;

float verticalFieldOfView = 60.0f;
float horizFieldOfView = ((float)initialWindowWidth / (float)initialWindowHeight) * verticalFieldOfView;

// grid dimensions for the reference grid (used in debugging)
int gridWidth = 1000;
int gridBoundary = gridWidth / 2;

// radius of rotation for the camera
int rotationRadius = 1250;

// Camera mCam;
Camera cam;
CPU cpu;
ControlUnit controlUnit;
RegAccess regAccess;
SignExtend signExtend;
ALUControl aluControl;
ALU alu;
ShiftLeftTwo shiftLeftTwo;
ALUAdd aluAdd;
Multiplexor multiplexor;
ProgramControl programControl;

// -------------------------------------------------------------------------------------------

void quit() {
	exit(1);
}

// -------------------------------------------------------------------------------------------

void userInputAndInstructions() {
}

void updateUserAttributes(bool afterGeneration) {
}

void menuCallback(int choice) {
}

void initMenu() {
	glutCreateMenu(menuCallback);

	glutAddMenuEntry("Choices:", 0);
	glutAddMenuEntry("", 0);
	glutAddMenuEntry("Option", 1);
	glutAddMenuEntry("", 0);
	glutAddMenuEntry("Quit", -1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init(int numArgs, char** argArray) {
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);

	// Turn on OpenGL lighting.
	glEnable(GL_LIGHTING);

	// Light property vectors.
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

	// Light0 properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);

	glEnable(GL_LIGHT0); // Enable particular light source.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint

	cam.setDepthOfView(10000);

	cpu.numBits = 32;
	cpu.initialize();
}

void resize(int w, int h) {
	if (h == 0) h = 1; // Prevent division by 0

	currentWindowWidth = w;
	currentWindowHeight = h;

	float ratio =  (float)w / h;

	horizFieldOfView = ratio * verticalFieldOfView;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(verticalFieldOfView, ratio, 0.1f, cam.depthOfView);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

// -------------------------------------------------------------------------------------------

void drawAxes() {
	// Draw axes
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(-1000, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, -1000, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, -1000);

	glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(1000, 0, 0);
	glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1000, 0);
	glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1000);
	glEnd();
}

void drawReferenceGrid() {
	// Draw axes
	// glColor3f(1.0, 1.0, 1.0);
	// glLineWidth(10);
	// glBegin(GL_LINES);
	// 	glVertex3f(-gridBoundary * 1.1, 0, 	0);
	// 	glVertex3f(gridBoundary * 1.1, 	0, 	0);
	// 	glVertex3f(0, 	0, 	-gridBoundary * 1.1);
	// 	glVertex3f(0, 	0, 	gridBoundary * 1.1);
	// glEnd();

	// Draw grid on "ground"
	glColor3f(0.9, 0.9, 0.9);
	glLineWidth(1);
	glBegin(GL_LINES);
	int gridIterator = gridBoundary / 10;
	for (int i = -gridBoundary; i <= gridBoundary; i += gridIterator) {
		if (i == 0) continue;
		glVertex3f(i, 0, -gridBoundary);
		glVertex3f(i, 0, gridBoundary);
		glVertex3f(-gridBoundary, 0, i);
		glVertex3f(gridBoundary, 0, i);
	}
	glEnd();
}

void display() {
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Light position vectors.
	float lightPos[] = { 200.0, 1000.0, 0.0, 1.0 };

	float amb = 1.0;
	// float diff = 0.5;
	float spec = 0.0;

	// Material property vectors.
	float matAmb[] = { amb, amb, amb, 1.0 };
	// float matDif[] = { 0.0, diff, 0.0, 1.0 };
	float matSpec[] = { spec, spec, spec, 1.0 };
	float matShine[] = { 50.0 };
	float matEmission[] = { 0.0, 0.0, 0.0, 1.0 };

	// Light quadratic attenuation factor.
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);

	cam.update();

	// Set the camera
	gluLookAt(	cam.pos.x,					cam.pos.y,	 				cam.pos.z,
				cam.pos.x+cam.viewDir.x, 	cam.pos.y+cam.viewDir.y, 	cam.pos.z+cam.viewDir.z,
				// cam.focus.x, 				cam.focus.y, 				cam.focus.z,
				0.0f, 						1.0f,  						0.0f);

	// Draw light source spheres after disabling lighting.
	// glDisable(GL_LIGHTING);

	// Light0 and its sphere positioned.
	// glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	// glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
	// glColor3f(1.0, 1.0, 1.0);
	// glScalef(3.0, 3.0, 3.0);
	// glutWireSphere(0.05, 8, 8);
	// glPopMatrix();

	// glEnable(GL_LIGHTING);

	// Material properties of sphere.
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	// glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif); // Created inside triangles
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
	glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);


	glDisable(GL_LIGHTING);
	drawReferenceGrid();
	drawAxes();

	cpu.display();
	controlUnit.display();
	regAccess.display();
	signExtend.display();
	aluControl.display();
	alu.display();
	shiftLeftTwo.display();
	aluAdd.display();
	multiplexor.display();
	programControl.display();
	glEnable(GL_LIGHTING);

	// Swap the buffers - flushing the current buffer
	glutSwapBuffers();
}

void idle() {
	display();
}

// -------------------------------------------------------------------------------------------

void mousePassiveMoveCallback(int x, int y) {
}

void mouseMoveCallback(int x, int y) {
	cam.handleMovement(x, y);
}

void mouseCallback(int button, int state, int x, int y) {
	cam.handleClick(button, state, x, y);
}

void keyUpCallback(unsigned char key, int x, int y) {
	switch(key) {
		case 27:	quit();
					break;
		case ' ':	cam.stopMove(Camera::UP);
					break;
		case 'x':	cam.stopMove(Camera::DOWN);
					break;
		case 's':
		case '2':	cam.stopMove(Camera::BACKWARD);
					break;
		case 'w':
		case '8':	cam.stopMove(Camera::FORWARD);
					break;
		case 'a':
		case '4':	cam.stopMove(Camera::LEFT);
					break;
		case 'd':
		case '6':	cam.stopMove(Camera::RIGHT);
					break;
	}
}

void keyDownCallback(unsigned char key, int x, int y) {
	switch(key) {
		case 27:	quit();
					break;
		case ' ':	cam.move(Camera::UP);
					break;
		case 'x':	cam.move(Camera::DOWN);
					break;
		case 's':
		case '2':	cam.move(Camera::BACKWARD);
					break;
		case 'w':
		case '8':	cam.move(Camera::FORWARD);
					break;
		case 'a':
		case '4':	cam.move(Camera::LEFT);
					break;
		case 'd':
		case '6':	cam.move(Camera::RIGHT);
					break;



		case 'q': 	mouseCallback(3, GLUT_DOWN, x, y);
					break;
		case 'Q': 	mouseCallback(4, GLUT_DOWN, x, y);
					break;
	}
}

void specialDownCallback(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP: keyDownCallback('8', x, y);
					break;
		case GLUT_KEY_DOWN: keyDownCallback('2', x, y);
					break;
		case GLUT_KEY_LEFT: keyDownCallback('4', x, y);
					break;
		case GLUT_KEY_RIGHT: keyDownCallback('6', x, y);
					break;
	}
}

void specialUpCallback(int key, int x, int y) { 	
	switch (key) {
		case GLUT_KEY_UP: keyUpCallback('8', x, y);
					break;
		case GLUT_KEY_DOWN: keyUpCallback('2', x, y);
					break;
		case GLUT_KEY_LEFT: keyUpCallback('4', x, y);
					break;
		case GLUT_KEY_RIGHT: keyUpCallback('6', x, y);
					break;
	}
}

// -------------------------------------------------------------------------------------------

int main(int argc, char** argv) {
	userInputAndInstructions();

	// Init GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// Create GLUT window
	glutInitWindowSize(initialWindowWidth, initialWindowHeight); // Scale factor is scaling the map to fit screen 
	glutInitWindowPosition(0, 0); 
	glutCreateWindow("Lit Landscape");

	// Custom init
	init(argc, argv);

	// Register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMouseFunc(mouseCallback); // Mouse up/down
	glutMotionFunc(mouseMoveCallback); // Mouse movement
	glutPassiveMotionFunc(mousePassiveMoveCallback); // Mouse movement

	// glutIgnoreKeyRepeat(1); // Ignore the repetition of chars that occur when holding a key
	glutKeyboardFunc(keyDownCallback); // Key down
	glutKeyboardUpFunc(keyUpCallback); // Key up
	glutSpecialFunc(specialDownCallback); // Special key down
	glutSpecialUpFunc(specialUpCallback); // Special key up

	// Begin processing.
	glutMainLoop();

	return 0;
}