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

class Component {
private:
public:
	string label;

	bool isOval;
	bool isAND;

	float xTrans;
	float yTrans;
	float zTrans;

	float xScale;
	float yScale;
	float zScale;

	Component() { isAND = false; };
	Component(float v1, float v2, float v3, float v4, float v5, float v6) : xTrans(v1), yTrans(v1), zTrans(v1), xScale(v1), yScale(v1), zScale(v1) {};

	void display();
};

class ControlUnit : public Component {};

class RegAccess : public Component {};

class SignExtend : public Component {};

class ALUControl : public Component {};

class ALU : public Component {};

class ShiftLeftTwo : public Component {};

class ALUAdd : public Component {};

class Multiplexer1 : public Component {};

class Multiplexer2 : public Component {};

class ProgramControl : public Component {};

class AndGate : public Component {};

class Add4 : public Component {};

class Input : public Component {};

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