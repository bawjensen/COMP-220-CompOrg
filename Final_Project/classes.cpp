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

	this->moveSpeed = 5.0;
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
	this->bitString = "";
}

Bit::Bit(string str) {
	this->isMoving = false;
	this->position = Coord3f(0, 0, 0);
	this->direction = Coord3f(0, 0, 0);
	this->stage = 0;
	this->hostWire = NULL;
	this->bitString = str;
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
	if (this->bitString == "") {
		glutSolidSphere(10.0, 20, 20);
	}
	else {
		float characterSize = 104.76;
		float scale = 0.25;
		glColor3f(0, 1, 0);
		glTranslatef(0, 12, this->bitString.length() * -characterSize / 2 * scale);
		glRotatef(-90, 0, 1, 0);
		glRotatef(-90, 1, 0, 0);
		glScalef(scale, scale, scale);
		for (int i = 0; i < this->bitString.length(); i++) {
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, this->bitString[i]);
		}
	}
	glPopMatrix();
}

// -------------------------------------------------------------------------------------------

Wire::Wire() {
	this->hasBit = false;
}

Wire::Wire(const Coord3f& start, const Coord3f& end) {
	this->points.push_back(start);
	this->points.push_back(end);
	this->hasBit = false;
}

Wire::Wire(const Coord3f& start, const Coord3f& end, int startTime, int endTime) {
	this->points.push_back(start);
	this->points.push_back(end);
	this->hasBit = false;
	this->sTime = startTime;
	this->eTime = endTime;
}

void Wire::display() {
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 1, 0);

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
	bit.position = this->points[0];
	this->hasBit = true;
}

void Wire::animate(int t) {
	if (this->hasBit) {
		if (t > this->sTime and t < this->eTime) {
			Coord3f path = this->points[1] - this->points[0];

			int animationElapsed = t - this->sTime;

			float timeScalar = (float)animationElapsed / (this->eTime - this->sTime);

			cout << "Percentage complete: " << timeScalar << endl;

			this->bit.position = path * timeScalar + this->points[0];
		}
		else if (t >= this->eTime) {
			this->hasBit = false;
		}
	}
}

// -------------------------------------------------------------------------------------------

void Component::display() {
	float characterSize = 104.76;

	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(0, 12, label.length() * -characterSize / 2 * .25);
	glRotatef(-90, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(0.25, 0.25, 0.25);
	for (int i = 0; i < label.length(); i++) {
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, label[i]);
	}
	glPopMatrix();

	glColor3f(1, 1, 1);
	glScalef(scale.x, scale.y, scale.z);

	if (isAND) {
		glutSolidCube(1.0);

		glPopMatrix();

		float xScale = 50;
		float yScale = 10;
		float zScale = 50;
		float xTrans = position.x;
		float yTrans = -5;
		float zTrans = 700;

		glPushMatrix();
		glTranslatef(xTrans, yTrans, zTrans);
		glScalef(xScale, yScale, zScale);
		glRotatef(-90, 1, 0, 0);
		gluDisk(gluNewQuadric(), 0, 1, 100, 1);
		gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 20);
		glPopMatrix();
	}
	else if (isOval) {
		glRotatef(-90, 1, 0, 0);
		gluDisk(gluNewQuadric(), 0, 1, 100, 1);
		gluCylinder(gluNewQuadric(), 1, 1, 1, 100, 1);
	}
	else {
		glutSolidCube(1.0);
	}

	glPopMatrix();
}

// -------------------------------------------------------------------------------------------

CPU::CPU() {
}

void CPU::initialize() {
}

void CPU::display() {
}

// -------------------------------------------------------------------------------------------
