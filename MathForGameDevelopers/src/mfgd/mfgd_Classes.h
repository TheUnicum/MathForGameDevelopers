#pragma once

#include <iostream>

class Vector
{
public:
	Vector();
	Vector(float x, float y, float z);

	float Length() const;
	float LengthSqr() const;

	Vector operator+(const Vector& v) const;
	Vector operator-(const Vector& v) const;

	Vector operator*(float) const;
	Vector operator/(float) const;

	Vector Normalized() const;

	Vector Cross(const Vector& v) const;

public:
	float x, y, z;
};


class Point
{
public:
	Point();
	Point(float x, float y, float z);

	Point AddVector(Vector v);

	float x, y, z;
};

Vector operator-(Point a, Point b);
float DotProduct(const Vector& a, const Vector& b);


// Test MFGD
void Test_MFGD();


// MFGD 12 - Smooth Move(ment) (Linear Interpolation)
float Approch(float flGoal, float flCurrent, float dVelocity);

