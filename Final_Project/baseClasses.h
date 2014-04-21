#ifndef _BASECLASSES
#define _BASECLASSES

#ifdef __APPLE__
	#include <GLUT/glut.h>
#elif __linux
	#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES // Needed to get MATH_PI from cmath
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

class Matrix44f; // Forward definition to allow use in Vec3f

class Color3f {
private:
public:
	float red, green, blue;

	Color3f();
	Color3f(float, float, float);
};

class Coord2i {
private:
public:
	int x, y;

	Coord2i();
	Coord2i(int, int);
};

class Coord3f {
private:
public:
	float x, y, z;

	Coord3f();
	Coord3f(float, float, float);

	Coord3f operator+(const Coord3f&) const;
	Coord3f operator/(float) const;

	friend ostream& operator<<(ostream&, const Coord3f&);
};

class Vec3f : public Coord3f {
private:
public:
	float xzAngle;

	Vec3f();
	Vec3f(float nX, float nY, float nZ);

	void set(float, float, float);
	float magnitude();

	Vec3f rotate(float, const Vec3f&);
	Vec3f rotateX(float);
	Vec3f rotateY(float);
	Vec3f rotateZ(float);

	Vec3f cross(const Vec3f&) const;

	Vec3f& operator*=(Matrix44f);
	Vec3f& operator*=(float);
	Vec3f& operator+=(const Vec3f&);
	Vec3f& operator-=(const Vec3f&);
	Vec3f& operator=(const Vec3f&);
	Vec3f operator-(const Vec3f&) const;
	Vec3f operator+(const Vec3f&) const;
	Vec3f operator*(const float) const;
	float operator[](int);

	friend ostream& operator<<(ostream&, Vec3f&);
};
class Quat4f {
private:
public:
	float x, y, z, w;

	Quat4f();
	Quat4f(float, float, float, float);
	Quat4f(Vec3f, float);
	Quat4f(float, Vec3f);

	float magnitude();
	Quat4f conjugate();
	Quat4f inverse();
	Vec3f complex();
	Vec3f rotateVector(const Vec3f&);

	Quat4f operator*(const Quat4f&) const;
	Quat4f operator/(float) const;
};

class Matrix44f {
private:
public:
	float matrix[4][4];

	Matrix44f();
	Matrix44f(float array[4][4]);

	Matrix44f& operator*=(Matrix44f);
	Matrix44f& operator=(const Matrix44f&);

	friend ostream& operator<<(ostream&, Matrix44f&);
};

#endif