#include "classes.h"

// -------------------------------------------------------------------------------------------

Camera::Camera() {
	this->pos = Vec3f(-100.0f, 25.0f, 0.0f); // Camera position
	this->origViewDir = Vec3f(1.0f, 0.0f, 0.0f); // View direction
	this->viewDir = this->origViewDir; // View direction
	this->strafeVec = this->viewDir.rotateY(-M_PI / 2);
	this->upVec = this->viewDir.rotateZ(-M_PI / 2);

	this->isFocusing = false; // Whether or not the camera is "viewing" or "focusing"
	this->focus = Vec3f(0.0f, 0.0f, 0.0f); // Where the camera is focusing

	this->angularScrollSpeed = 1 / (float)200; // Rotation speed of the camera
	this->panActive = false; // Whether or not the camera is panning (via the mouse)

	this->moveSpeed = 0.5;
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