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
int gridWidth = 2000;
int gridBoundary = gridWidth / 2;

Camera cam;

ControlUnit controlUnit;
RegAccess regAccess;
SignExtend signExtend;
ALUControl aluControl;
ALU alu;
ShiftLeftTwo shiftLeftTwo;
ALUAdd aluAdd;
ProgramControl programControl;
Multiplexer1 mux1;
Multiplexer2 mux2;
Add4 add4;
AndGate andGate;
Input input;

vector< vector<Wire> > wireStages;

bool animating = false;
int animationTime = 0;
clock_t animationStart;
float animationSpeed = 0.00025;

int timeStage0[2] = { 0, 1000 };
int timeStage1[2] = { 1000, 2000 };
int timeStage2[2] = { 2000, 3000 };
int timeStage3[2] = { 3000, 4000 };


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

	controlUnit.label = "Control Unit";
	controlUnit.isOval = true;
	controlUnit.position.x = 500;
	controlUnit.position.y = 0;
	controlUnit.position.z = -300;
	controlUnit.scale.x = 200;
	controlUnit.scale.y = 10;
	controlUnit.scale.z = 100;
	controlUnit.initialize();

	regAccess.label = "Registers";
	regAccess.isOval = false;
	regAccess.position.x = -200;
	regAccess.position.y = 0;
	regAccess.position.z = -200;
	regAccess.scale.x = 400;
	regAccess.scale.y = 10;
	regAccess.scale.z = 200;
	regAccess.initialize();

	signExtend.label = "Sign Extend";
	signExtend.isOval = true;
	signExtend.position.x = -700;
	signExtend.position.y = 0;
	signExtend.position.z = 0;
	signExtend.scale.x = 100;
	signExtend.scale.y = 10;
	signExtend.scale.z = 50;
	signExtend.initialize();

	aluControl.label = "ALU Control";
	aluControl.isOval = true;
	aluControl.position.x = 250;
	aluControl.position.y = 0;
	aluControl.position.z = 50;
	aluControl.scale.x = 100;
	aluControl.scale.y = 10;
	aluControl.scale.z = 50;
	aluControl.initialize();

	alu.label = "ALU";
	alu.isOval = false;
	alu.position.x = -100;
	alu.position.y = 0;
	alu.position.z = 400;
	alu.scale.x = 400;
	alu.scale.y = 10;
	alu.scale.z = 200;
	alu.initialize();

	shiftLeftTwo.label = "Shift-Left 2";
	shiftLeftTwo.isOval = true;
	shiftLeftTwo.position.x = -550;
	shiftLeftTwo.position.y = 0;
	shiftLeftTwo.position.z = 275;
	shiftLeftTwo.scale.x = 100;
	shiftLeftTwo.scale.y = 10;
	shiftLeftTwo.scale.z = 50;
	shiftLeftTwo.initialize();

	aluAdd.label = "Add";
	aluAdd.isOval = false;
	aluAdd.position.x = -550;
	aluAdd.position.y = 0;
	aluAdd.position.z = 600;
	aluAdd.scale.x = 200;
	aluAdd.scale.y = 10;
	aluAdd.scale.z = 100;
	aluAdd.initialize();

	mux1.label = "Mux #1";
	mux1.isOval = true;
	mux1.position.x = -300;
	mux1.position.y = 0;
	mux1.position.z = 150;
	mux1.scale.x = 150;
	mux1.scale.y = 10;
	mux1.scale.z = 30;
	mux1.initialize();

	mux2.label = "Mux #2";
	mux2.isOval = true;
	mux2.position.x = -600;
	mux2.position.y = 0;
	mux2.position.z = 850;
	mux2.scale.x = 150;
	mux2.scale.y = 10;
	mux2.scale.z = 30;
	mux2.initialize();

	programControl.label = "PC";
	programControl.isOval = false;
	programControl.position.x = -400;
	programControl.position.y = 0;
	programControl.position.z = -850;
	programControl.scale.x = 200;
	programControl.scale.y = 10;
	programControl.scale.z = 100;
	programControl.initialize();

	andGate.label = "AND";
	andGate.isAND = true; // Note: Not oval, it's an AND gate
	andGate.position.x = 200;
	andGate.position.y = 0;
	andGate.position.z = 670;
	andGate.scale.x = 100;
	andGate.scale.y = 10;
	andGate.scale.z = 50;
	andGate.initialize();

	add4.label = "Increment";
	add4.isOval = false;
	add4.position.x = -900;
	add4.position.y = 0;
	add4.position.z = 300;
	add4.scale.x = 250;
	add4.scale.y = 10;
	add4.scale.z = 150;
	add4.initialize();

	input.label = "Input";
	input.isOval = false;
	input.position.x = -100;
	input.position.y = 0;
	input.position.z = -800;
	input.scale.x = 250;
	input.scale.y = 10;
	input.scale.z = 150;
	input.initialize();

	vector<Wire> stage0;
	stage0.push_back(Wire(input.output0, controlUnit.input0, timeStage0[0], timeStage0[1]));
	stage0.push_back(Wire(input.output1, regAccess.input0, timeStage0[0], timeStage0[1]));
	stage0.push_back(Wire(input.output2, regAccess.input1, timeStage0[0], timeStage0[1]));
	stage0.push_back(Wire(input.output3, signExtend.input0, timeStage0[0], timeStage0[1]));

	vector<Wire> stage1;
	stage1.push_back(Wire(controlUnit.output1, aluControl.input0, timeStage1[0], timeStage1[1]));
	stage1.push_back(Wire(controlUnit.output2, mux1.input2, timeStage1[0], timeStage1[1]));
	stage1.push_back(Wire(regAccess.output1, mux1.input0, timeStage1[0], timeStage1[1]));
	//stage1.push_back(Wire(signExtend.output1, mux1.input1, timeStage1[0], timeStage1[1]));

	vector<Wire> stage2;
	stage2.push_back(Wire(regAccess.output0, alu.input0, timeStage1[0], timeStage2[1]));
	stage2.push_back(Wire(mux1.output0, alu.input1, timeStage2[0], timeStage2[1]));
	stage2.push_back(Wire(aluControl.output0, alu.input2, timeStage2[0], timeStage2[1]));
	stage2.push_back(Wire(signExtend.output0, shiftLeftTwo.input0, timeStage2[0], timeStage2[1]));

	vector<Wire> stage3;
	stage3.push_back(Wire(controlUnit.output0, andGate.input0, timeStage1[0], timeStage3[1]));
	stage3.push_back(Wire(alu.output0, andGate.input1, timeStage3[0], timeStage3[1]));
	stage3.push_back(Wire(shiftLeftTwo.output0, aluAdd.input0, timeStage3[0], timeStage3[1]));
	stage3.push_back(Wire(add4.output0, aluAdd.input1, timeStage3[0], timeStage3[1]));



	wireStages.push_back(stage0);
	wireStages.push_back(stage1);
	wireStages.push_back(stage2);
	wireStages.push_back(stage3);

	wireStages[0][0].attach(Bit("000100"));				// input to CU
	wireStages[0][1].attach(Bit("01000"));  			// input to regAccess
	wireStages[0][2].attach(Bit("01001"));  			// input to regAcces
	wireStages[0][3].attach(Bit("0000000010010110"));	// input to SignExtend

	wireStages[1][0].attach(Bit("01"));					// CU to ALU Control
	wireStages[1][1].attach(Bit("0"));					// CU to Mux1
	wireStages[1][2].attach(Bit("01001"));				// RegAccess to Mux1

	wireStages[2][0].attach(Bit("01000")); 								// RegAccess to ALU
	wireStages[2][1].attach(Bit("01001")); 								// Mux1 to ALU
	wireStages[2][2].attach(Bit("0110")); 								// ALU Control to ALU
	wireStages[2][3].attach(Bit("00000000000000000000000010010110")); 	// SignExtend to Shift Left 2

	wireStages[3][0].attach(Bit("1"));									// CU to AND
	wireStages[3][1].attach(Bit("1"));									// ALU to AND
	wireStages[3][2].attach(Bit("00000000000000000000001001011000"));	// Shift Left 2 to aluAdd

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
	// drawReferenceGrid();
	// drawAxes();

	controlUnit.display();
	regAccess.display();
	signExtend.display();
	aluControl.display();
	alu.display();
	shiftLeftTwo.display();
	aluAdd.display();
	programControl.display();
	mux1.display();
	mux2.display();
	add4.display();
	andGate.display();
	input.display();

	for (vector< vector<Wire> >::iterator superIt = wireStages.begin(); superIt != wireStages.end(); ++superIt) {
		for (vector<Wire>::iterator it = superIt->begin(); it != superIt->end(); ++it) {
			it->display();
		}
	}

	glEnable(GL_LIGHTING);

	// Swap the buffers - flushing the current buffer
	glutSwapBuffers();
}

void idle() {
	clock_t endWait;

	if (animating) {
		animationTime = (clock() - animationStart) * animationSpeed;

		// cout << "Current animation time: " << animationTime << endl;

		for (vector< vector<Wire> >::iterator superIt = wireStages.begin(); superIt != wireStages.end(); ++superIt) {
			for (vector<Wire>::iterator it = superIt->begin(); it != superIt->end(); ++it) {
				it->animate(animationTime);
			}
		}
	}

	endWait = clock() + CLOCKS_PER_SEC / 60;

	/* please wait...*/
	while (clock() < endWait);

	glutPostRedisplay();
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


		case 'p': 	animating = !animating;
					if (animating) animationStart = clock() - (animationTime / animationSpeed);
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
	glutCreateWindow("CPU");

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