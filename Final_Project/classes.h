#ifndef _CLASSES
#define _CLASSES

#ifdef __APPLE__
	#include <GLUT/glut.h>
#elif __linux
	#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <random>

#include "baseClasses.h"

using namespace std;

class Camera {
private:
public:
	static const int FORWARD = 0;
	static const int BACKWARD = 1;
	static const int LEFT = 2;
	static const int RIGHT = 3;
	static const int UP = 4;
	static const int DOWN = 5;

	const Vec3f UP_VECT = Vec3f(0.0, 1.0, 0.0);

	Vec3f pos, origPos; 		// Position and original position vectors
	Vec3f focus;				// Where the camera is focus-looking (aka the focus doesn't move, the camera rotates around)
	Vec3f viewDir, origViewDir; // Where the camera is looking, and the original value
	Vec3f strafeVec, upVec;			//
	Vec3f pOrigin;
	bool isFocusing, panActive; // Booleans for whether or not the camera is a certain mode
	bool moving[10]; 			// Boolean array for status of movement in up to 10 directions
	float horizAngle, vertAngle;
	float deltaHorizAngle, deltaVertAngle;
	float angularScrollSpeed;
	float rotationRadius;
	float depthOfView;
	float moveSpeed;

	Camera();

	void setPos(float, float, float);
	void setViewDir(float, float, float);
	void setFocus(float, float, float);
	void setRotationRadius(int);
	void setDepthOfView(float);

	void update();
	void move(int);
	void stopMove(int);

	void rotateTo(float, float);
	void rotate(float, float);
	void handleClick(int, int, int, int);
	void handleMovement(int, int);
};

class Wire;

class Bit {
private:
public:
	bool isMoving;
	Wire* hostWire;
	Coord3f position;
	Coord3f direction;
	int stage;

	Bit();
	Bit(Coord3f);

	void update();
	void display();
};

class Wire {
private:
public:
	vector<Coord3f> points;
	Bit bit;
	bool hasBit;

	Wire();
	Wire(Coord3f, Coord3f);

	void display();
	void attach(Bit);
};

class ControlUnit {
private:
public:
	ControlUnit();

	void display();
};

class RegAccess {
private:
public:
	RegAccess();

	void display();
};

class SignExtend {
private:
public:
	SignExtend();

	void display();
};

class ALUControl {
private:
public:
	ALUControl();

	void display();
};

class ALU {
private:
public:
	ALU();

	void display();
};

class ShiftLeftTwo {
private:
public:
	ShiftLeftTwo();

	void display();
};

class ALUAdd {
private:
public:
	ALUAdd();

	void display();
};

class Multiplexer1 {
private:
public:
	Multiplexer1();

	void display();
};

class Multiplexer2 {
private:
public:
	Multiplexer2();

	void display();
};

class ProgramControl {
private:
public:
	ProgramControl();

	void display();
};

class CPU {
private:
public:
	int numBits;
	vector<Wire> wires;

	CPU();

	void initialize();
	void display();
};

#endif