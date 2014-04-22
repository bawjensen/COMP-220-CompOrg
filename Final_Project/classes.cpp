#include "classes.h"

// -------------------------------------------------------------------------------------------

Camera::Camera() {
	this->pos = Vec3f(-1000.0f, 1000.0f, 0.0f); // Camera position
	this->origViewDir = Vec3f(1.0f, 0.0f, 0.0f); // View direction
	this->viewDir = this->origViewDir; // View direction
	this->strafeVec = this->viewDir.rotateY(-M_PI / 2);
	this->upVec = this->viewDir.rotateZ(-M_PI / 2);

	this->isFocusing = false; // Whether or not the camera is "viewing" or "focusing"
	this->focus = Vec3f(0.0f, 0.0f, 0.0f); // Where the camera is focusing

	this->angularScrollSpeed = 1 / (float)200; // Rotation speed of the camera
	this->panActive = false; // Whether or not the camera is panning (via the mouse)

	this->moveSpeed = 2.0;
}

void Camera::setPos(float pX, float pY, float pZ) {
	this->origPos = Vec3f(pX, pY, pZ);
	this->pos = this->origPos;
	this->rotationRadius = max(pX, pZ);
	this->depthOfView = 2 * this->rotationRadius;
}

void Camera::setViewDir(float vX, float vY, float vZ) {
	this->viewDir = Vec3f(vX, vY, vZ);
}

void Camera::setFocus(float fX, float fY, float fZ) {
	this->focus = Vec3f(fX, fY, fZ);
}

void Camera::setRotationRadius(int r) {
	float radiusRatio = (float)r / this->rotationRadius;

	Vec3f distFromFocus = this->origPos - this->focus;

	distFromFocus *= radiusRatio;
	this->origPos = distFromFocus + this->focus;
	this->rotate(0, 0); // Call a dummy rotate to update the camera's position

	this->rotationRadius = r;
	this->depthOfView = 2 * this->rotationRadius;
}

void Camera::setDepthOfView(float dist) {
	this->depthOfView = dist;
}

void Camera::update() {
	if (this->moving[Camera::FORWARD]) {
		this->pos += this->viewDir * this->moveSpeed;
	}
	if (this->moving[Camera::BACKWARD]) {
		this->pos -= this->viewDir * this->moveSpeed;
	}
	if (this->moving[Camera::LEFT]) {
		this->pos += this->strafeVec * this->moveSpeed;
	}
	if (this->moving[Camera::RIGHT]) {
		this->pos -= this->strafeVec * this->moveSpeed;
	}
	if (this->moving[Camera::UP]) {
		this->pos += Camera::UP_VECT * this->moveSpeed;
	}
	if (this->moving[Camera::DOWN]) {
		this->pos -= Camera::UP_VECT * this->moveSpeed;
	}
}

void Camera::stopMove(int direction) {
	this->moving[direction] = false;
}

void Camera::move(int direction) {
	this->moving[direction] = true;
}

void Camera::rotateTo(float hAngle, float vAngle) {
	this->horizAngle = hAngle;
	this->vertAngle = vAngle;

	this->pos = this->origPos.rotateY(this->horizAngle);
}

void Camera::rotate(float hAngle, float vAngle) {
	this->horizAngle += hAngle;
	this->vertAngle += vAngle;

	float angleLimit = (M_PI / 2) - .01;

	if (this->vertAngle > angleLimit) this->vertAngle = angleLimit;
	else if (this->vertAngle < -angleLimit) this->vertAngle = -angleLimit;

	this->viewDir = this->origViewDir.rotateZ(-this->vertAngle).rotateY(this->horizAngle);
	this->strafeVec = this->strafeVec.rotateY(hAngle);
	this->upVec = this->upVec.rotateZ(vAngle);
}

void Camera::handleClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) { // only start motion if the left button is pressed
		if (state == GLUT_UP) { // when the button is released
			this->panActive = false;
		}
		else  { // state = GLUT_DOWN
			this->pOrigin = Vec3f(x, y, 0); // Ignore z for simplicity
			this->panActive = true;
		}
	}
}

void Camera::handleMovement(int x, int y) {
	if (panActive) { // this will only be true when the left button is down

		// update deltaHorizAngle
		this->deltaHorizAngle = -(this->pOrigin.x - x) * this->angularScrollSpeed;
		this->pOrigin.x = x;

		// update deltaVertAngle
		this->deltaVertAngle = (this->pOrigin.y - y) * angularScrollSpeed;
		this->pOrigin.y = y;

		this->rotate(deltaHorizAngle, deltaVertAngle);
	}
}

// -------------------------------------------------------------------------------------------

Bit::Bit() {
	this->isMoving = false;
	this->position = Coord3f(0, 0, 0);
	this->direction = Coord3f(0, 0, 0);
	this->stage = 0;
	this->hostWire = NULL;
}

Bit::Bit(Coord3f startPosition) {
	this->isMoving = false;
	this->position = startPosition;
	this->direction = Coord3f(0, 0, 0);
	this->stage = 0;
	this->hostWire = NULL;
}

void Bit::update() {
	if (this->isMoving)
		this->position += this->direction;
}

void Bit::display() {
	glPushMatrix();
	glTranslatef(this->position.x, this->position.y, this->position.z);
	glutSolidSphere(4.0, 20, 20);
	glPopMatrix();
}

// -------------------------------------------------------------------------------------------

Wire::Wire() {
	this->hasBit = false;
}

Wire::Wire(Coord3f start, Coord3f end) {
	this->points.push_back(start);
	this->points.push_back(end);
}

void Wire::display() {
	glBegin(GL_LINE_STRIP);

	for (vector<Coord3f>::iterator it = this->points.begin(); it != this->points.end(); ++it) {
		glVertex3f(it->x, it->y, it->z);
	}

	glEnd();

	if (this->hasBit) {
		this->bit.update();
		this->bit.display();
	}
}

void Wire::attach(Bit newBit) {
	this->bit = newBit;
	this->bit.hostWire = this;
	this->hasBit = true;
}

// -------------------------------------------------------------------------------------------

ControlUnit::ControlUnit() {

}

void ControlUnit::display() {
	float xTrans = 500;
	float yTrans = 0;
	float zTrans = -300;

	float xScale = 200;
	float yScale = 10;
	float zScale = 100;

	glPushMatrix();

	glColor3f(1, 1, 1);
	glTranslatef(xTrans, yTrans, zTrans);
	glScalef(xScale, yScale, zScale);
	glRotatef(-90, 1, 0, 0);
	gluDisk(gluNewQuadric(), 0, 1, 100, 1);

	glPopMatrix();
}

// -------------------------------------------------------------------------------------------


RegAccess::RegAccess() {

}

void RegAccess::display() {
	float xTrans = -200;
	float yTrans = 0;
	float zTrans = -200;

	float xScale = 400;
	float yScale = 10;
	float zScale = 200;

	glPushMatrix();

	glColor3f(1, 1, 1);
	glTranslatef(xTrans, yTrans, zTrans);
	glScalef(xScale, yScale, zScale);
	glutSolidCube(1.0);

	glPopMatrix();
}

// -------------------------------------------------------------------------------------------


SignExtend::SignExtend() {

}

void SignExtend::display() {
	float xTrans = -700;
	float yTrans = 0;
	float zTrans = 0;

	float xScale = 100;
	float yScale = 10;
	float zScale = 50;

	glPushMatrix();

	glColor3f(1, 1, 1);
	glTranslatef(xTrans, yTrans, zTrans);
	glScalef(xScale, yScale, zScale);
	glRotatef(-90, 1, 0, 0);
	gluDisk(gluNewQuadric(), 0, 1, 100, 1);

	glPopMatrix();
}

// -------------------------------------------------------------------------------------------


ALUControl::ALUControl() {

}

void ALUControl::display() {
	float xTrans = 250;
	float yTrans = 0;
	float zTrans = 50;

	float xScale = 100;
	float yScale = 10;
	float zScale = 50;

	glPushMatrix();

	glColor3f(1, 1, 1);
	glTranslatef(xTrans, yTrans, zTrans);
	glScalef(xScale, yScale, zScale);
	glRotatef(-90, 1, 0, 0);
	gluDisk(gluNewQuadric(), 0, 1, 100, 1);

	glPopMatrix();
}

// -------------------------------------------------------------------------------------------


ALU::ALU() {

}

void ALU::display() {
	float xTrans = 100;
	float yTrans = 0;
	float zTrans = 400;

	float xScale = 400;
	float yScale = 10;
	float zScale = 200;

	glPushMatrix();

	glColor3f(1, 1, 1);
	glTranslatef(xTrans, yTrans, zTrans);
	glScalef(xScale, yScale, zScale);
	glutSolidCube(1.0);

	glPopMatrix();
}

// -------------------------------------------------------------------------------------------


ShiftLeftTwo::ShiftLeftTwo() {

}

void ShiftLeftTwo::display() {
	float xTrans = -600;
	float yTrans = 0;
	float zTrans = 275;

	float xScale = 100;
	float yScale = 10;
	float zScale = 50;

	glPushMatrix();

	glColor3f(1, 1, 1);
	glTranslatef(xTrans, yTrans, zTrans);
	glScalef(xScale, yScale, zScale);
	glRotatef(-90, 1, 0, 0);
	gluDisk(gluNewQuadric(), 0, 1, 100, 1);

	glPopMatrix();
}

// -------------------------------------------------------------------------------------------


ALUAdd::ALUAdd() {

}

void ALUAdd::display() {
	float xTrans = -650;
	float yTrans = 0;
	float zTrans = 600;

	float xScale = 400;
	float yScale = 10;
	float zScale = 200;

	glPushMatrix();

	// float zTrans = 500;

	// float xScale = 400;
	// float yScale = 10;
	// float zScale = 300;

	glColor3f(1, 1, 1);
	glTranslatef(xTrans, yTrans, zTrans);
	glScalef(xScale, yScale, zScale);
	glutSolidCube(1.0);

	glPopMatrix();
}

// -------------------------------------------------------------------------------------------


Multiplexor::Multiplexor() {

}

void Multiplexor::display() {

}

// -------------------------------------------------------------------------------------------


ProgramControl::ProgramControl() {

}

void ProgramControl::display() {

	float xTrans = -100;
	float yTrans = 0;
	float zTrans = -850;

	float xScale = 200;
	float yScale = 10;
	float zScale = 100;

	glPushMatrix();

	glColor3f(1, 1, 1);
	glTranslatef(xTrans, yTrans, zTrans);
	glScalef(xScale, yScale, zScale);
	glutSolidCube(1.0);
	glPopMatrix();
}

// -------------------------------------------------------------------------------------------

CPU::CPU() {

}

void CPU::initialize() {
	int spacing = 10;

	float top = (float)(this->numBits - 1) / 2 * spacing;
	float bottom = -top;
	float left = -100.0;
	float right = 100.0;

	for (float i = bottom; i <= top; i += spacing) {
		this->wires.push_back(Wire(Coord3f(i, 5, left), Coord3f(i, 5, right)));
	}

	this->wires[0].attach(Bit(this->wires[0].points[0]));
}

void CPU::display() {
	// glDisable(GL_LIGHTING);
	glColor3f(1.0, 0.0, 0.0);

	// for (vector<Wire>::iterator it = this->wires.begin(); it != this->wires.end(); ++it) {
	// 	it->display();
	// }

	// glEnable(GL_LIGHTING);
}

// -------------------------------------------------------------------------------------------
