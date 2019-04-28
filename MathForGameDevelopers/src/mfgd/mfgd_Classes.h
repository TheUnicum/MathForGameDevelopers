#pragma once

#include <iostream>

class Vector
{
public:
	Vector() {}
	Vector(float x, float y)
		: x(x), y(y) {}

	float Length() const;
	float LengthSqr() const;

	Vector operator+(const Vector& v) const;
	Vector operator-(const Vector& v) const;

	Vector operator*(float) const;
	Vector operator/(float) const;

	Vector Normalized() const;

public:
	float x, y;
};


class Point
{
public:
	Point AddVector(Vector v);

	float x, y;
};

Vector operator-(Point a, Point b);
float DotProduct(const Vector& a, const Vector& b);


// Test MFGD
void Test_MFGD();


// MFGD 12 - Smooth Move(ment) (Linear Interpolation)
float Approch(float flGoal, float flCurrent, float dVelocity);

