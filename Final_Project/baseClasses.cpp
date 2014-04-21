#include "baseClasses.h"

// -------------------------------------------------------------------------------------------

Color3f::Color3f() {

}

Color3f::Color3f(float r, float g, float b) {
	this->red = r;
	this->green = g;
	this->blue = b;
}

// -------------------------------------------------------------------------------------------

Coord2i::Coord2i() {

}

Coord2i::Coord2i(int nX, int nY) {
	this->x = nX;
	this->y = nY;
}

// -------------------------------------------------------------------------------------------

Coord3f::Coord3f() {
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}

Coord3f::Coord3f(float nX, float nY, float nZ) {
	this->x = nX;
	this->y = nY;
	this->z = nZ;
}

Coord3f Coord3f::operator+(const Coord3f& other) const {
	return Coord3f(this->x + other.x, this->y + other.y, this->z + other.z);
}

Coord3f Coord3f::operator/(float divisor) const {
	return Coord3f(this->x / divisor, this->y / divisor, this->z / divisor);
}

ostream& operator<<(ostream& co, const Coord3f& point) {
	co << "(" << point.x << ", " << point.y << ", " << point.z << ")";

	return co;
}

// -------------------------------------------------------------------------------------------

Vec3f::Vec3f() {
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->xzAngle = 0.0f;
}

Vec3f::Vec3f(float nX, float nY, float nZ) {
	this->x = nX;
	this->y = nY;
	this->z = nZ;
	this->xzAngle = 0.0f;
}

void Vec3f::set(float nX, float nY, float nZ) {
	this->x = nX;
	this->y = nY;
	this->z = nZ;
	this->xzAngle = 0.0f;
}

float Vec3f::magnitude() {
	return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
}

Vec3f Vec3f::rotate(float radians, const Vec3f& axis) {

}

Vec3f Vec3f::rotateX(float radians) {
	Vec3f tempVec = (*this);
	float rotationArray[4][4] = { 	{0,				0, 				0, 				0}, 
									{0, 			cos(radians), 	sin(radians), 	0},
									{0, 			-sin(radians), 	cos(radians), 	0},
									{0, 			0, 				0, 				1}
								};

	Matrix44f rMatrix(rotationArray);
	tempVec *= rMatrix;

	return tempVec;
}

Vec3f Vec3f::rotateY(float radians) {
	Vec3f tempVec = (*this);
	float rotationArray[4][4] = { 	{cos(radians),	0, 				-sin(radians), 	0}, 
									{0, 			1, 				0, 				0},
									{sin(radians), 	0, 				cos(radians), 	0},
									{0, 			0, 				0, 				1}
								};

	Matrix44f rMatrix(rotationArray);
	tempVec *= rMatrix;

	return tempVec;
}

Vec3f Vec3f::rotateZ(float radians) {
	Vec3f tempVec = (*this);
	float rotationArray[4][4] = { 	{cos(radians),	sin(radians), 	0, 				0}, 
									{-sin(radians), cos(radians), 	0, 				0},
									{0, 			0,				1, 				0},
									{0, 			0,				0, 				1}
								};

	Matrix44f rMatrix(rotationArray);
	tempVec *= rMatrix;

	return tempVec;
}

Vec3f Vec3f::cross(const Vec3f& other) const {
	return Vec3f(this->y * other.z - this->z * other.y,
				 this->z * other.x - this->x * other.z,
				 this->x * other.y - this->y * other.x);
}

Vec3f& Vec3f::operator*=(Matrix44f matrix) {
	float vec4[4] = { this->x, this->y, this->z, 1 };
	float tempVec[3] = { 0, 0, 0 };

	for (int i = 0; i < 4; i++) {
		float sum = 0;
		for (int j = 0; j < 4; j++) {
			 sum += (*this)[j] * matrix.matrix[i][j];
		}
		tempVec[i] = sum;
	}

	this->x = tempVec[0];
	this->y = tempVec[1];
	this->z = tempVec[2];

	return (*this);
}

Vec3f& Vec3f::operator*=(float multiplier) {
	this->x *= multiplier;
	this->y *= multiplier;
	this->z *= multiplier;

	return (*this);
}

Vec3f& Vec3f::operator+=(const Vec3f& other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;

	return (*this);
}

Vec3f& Vec3f::operator-=(const Vec3f& other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;

	return (*this);
}

Vec3f& Vec3f::operator=(const Vec3f& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;

	return (*this);
}

Vec3f Vec3f::operator-(const Vec3f& other) const {
	return Vec3f(this->x - other.x, this->y - other.y, this->z - other.z);
}

Vec3f Vec3f::operator+(const Vec3f& other) const {
	return Vec3f(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vec3f Vec3f::operator*(const float scalar) const {
	return Vec3f(this->x * scalar, this->y * scalar, this->z * scalar);
}

float Vec3f::operator[](int i) {
	if (i == 0) return this->x;
	if (i == 1) return this->y;
	if (i == 2) return this->z;
}

ostream& operator<<(ostream& co, Vec3f& vec) {
	co << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return co;
}

// -------------------------------------------------------------------------------------------

Quat4f::Quat4f() {

}

Quat4f::Quat4f(float w, float x, float y, float z) {
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

Quat4f::Quat4f(Vec3f vec, float w) {
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
	this->w = w;
}

Quat4f::Quat4f(float angle, Vec3f axis) {
	angle *= (M_PI / 180);

	this->w = cos(angle / 2);

	float sine = sin(angle / 2);
	this->x = axis.x * sine;
	this->y = axis.y * sine;
	this->z = axis.z * sine;
}

float Quat4f::magnitude() {
	return sqrt((this->w * this->w) + (this->x * this->x) + (this->y * this->y) + (this->z * this->z));
}

Quat4f Quat4f::conjugate() {
	return Quat4f(complex() * -1, this->w);
}

Quat4f Quat4f::inverse() {
	float mag = this->magnitude();
	return this->conjugate() / (mag * mag);
}

Vec3f Quat4f::complex() {
	return Vec3f(this->x, this->y, this->z);
}

Vec3f Quat4f::rotateVector(const Vec3f& v) {
	return (((*this) * Quat4f(v, 0)) * inverse()).complex();
}

Quat4f Quat4f::operator*(const Quat4f& other) const {
	float x = (this->y * other.z) - (this->z * other.y) + (this->x * other.w) + (this->w * other.x);
	float y = (this->z * other.x) - (this->x * other.z) + (this->y * other.w) + (this->w * other.y);
	float z = (this->x * other.y) - (this->y * other.x) + (this->z * other.w) + (this->w * other.z);
	float w = (this->w * other.w) - (this->x * other.x) - (this->y * other.y) - (this->z * other.z);

	return Quat4f(w, x, y, z);
}

Quat4f Quat4f::operator/(float scalar) const {
	return Quat4f(this->w / scalar, this->x / scalar, this->y / scalar, this->z / scalar);
}


// -------------------------------------------------------------------------------------------

Matrix44f::Matrix44f() { // Creates an identity matrix
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				this->matrix[i][j] = 1;
			}
			else {
				this->matrix[i][j] = 0;
			}
		}
	}
}

Matrix44f::Matrix44f(float array[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->matrix[i][j] = array[i][j];
		}
	}
}

Matrix44f& Matrix44f::operator*=(Matrix44f other) {
	Matrix44f tempMatrix;
	float sum;

	for (int firstRow = 0; firstRow < 4; firstRow++) {
		for (int secondColumn = 0; secondColumn < 4; secondColumn++) {
			sum = 0;
			for (int secondRow = 0; secondRow < 4; secondRow++) {
				sum += this->matrix[firstRow][secondRow] * other.matrix[secondRow][secondColumn];
			}
			tempMatrix.matrix[firstRow][secondColumn] = sum;
		}
	}
	(*this) = tempMatrix;

	return *this;
}

Matrix44f& Matrix44f::operator=(const Matrix44f& other) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->matrix[i][j] = other.matrix[i][j];
		}
	}
}

ostream& operator<<(ostream& co, Matrix44f& matrix) {
	co << "[";
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			co << matrix.matrix[i][j] << ", ";
		}
		co << endl;
	}
	co << "]";
	return co;
}