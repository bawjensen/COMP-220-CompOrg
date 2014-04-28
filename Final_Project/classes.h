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
	string bitString;

	Bit();
	Bit(string);
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
	int sTime, eTime;

	Wire();
	Wire(const Coord3f&, const Coord3f&);
	Wire(const Coord3f&, const Coord3f&, int startTime, int endTime);

	void display();
	void attach(Bit);
	void animate(int);
};

class Component {
private:
public:
	string label;

	bool isOval;
	bool isAND;

	Coord3f position;
	Coord3f scale;

	Component() { isAND = false; };

	void display();
};

class ControlUnit : public Component {
private:
public:
	Coord3f input0;
	Coord3f output0, output1, output2;
};

class RegAccess : public Component {
private:
public:
	Coord3f input0, input1;
	Coord3f output0, output1;

};

class SignExtend : public Component {
private:
public:
	Coord3f input0;
	Coord3f output0;

};

class ALUControl : public Component {
private:
public:
	Coord3f input0;
	Coord3f output0;

};

class ALU : public Component {
private:
public:
	Coord3f input0, input1, input2;
	Coord3f output0;

};

class ShiftLeftTwo : public Component {
private:
public:
	Coord3f input0;
	Coord3f output0;

};

class ALUAdd : public Component {
private:
public:
	Coord3f input0, input1;
	Coord3f output0;

};

class Multiplexer1 : public Component {
private:
public:
	Coord3f input0;
	Coord3f output0, output1, output2;

};

class Multiplexer2 : public Component {
private:
public:
	Coord3f input0;
	Coord3f output0, output1, output2;

};

class ProgramControl : public Component {
private:
public:
	Coord3f input0;
	Coord3f output0, output1, output2;

};

class AndGate : public Component {
private:
public:
	Coord3f input0;
	Coord3f output0, output1, output2;

};

class Add4 : public Component {
private:
public:
	Coord3f input0;
	Coord3f output0, output1, output2;

};

class Input : public Component {
private:
public:
	Coord3f input0;
	Coord3f output0, output1, output2;

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