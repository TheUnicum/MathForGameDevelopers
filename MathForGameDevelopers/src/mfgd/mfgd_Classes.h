#pragma once

#include <iostream>

class Vector
{
public:
	Vector();
	Vector(float x, float y, float z);
	Vector(const class Point& p);

	float Length() const;
	float LengthSqr() const;

	Vector operator+() const;
	Vector operator-() const;

	Vector operator+(const Vector& v) const;
	Vector operator-(const Vector& v) const;

	Vector operator*(float) const;
	Vector operator/(float) const;

	Vector Normalized() const;

	float Dot(const Vector& v) const;
	Vector Cross(const Vector& v) const;

public:
	//float x, y, z;
	union {
		struct
		{
			float x, y, z;
		};
		float v[3];
	};
};


inline const Vector operator*(float s, const Vector& v)
{
	return Vector(s * v.x, s * v.y, s * v.z);
}

inline const Vector operator/(float s, const Vector& v) 
{ 
	return Vector(s / v.x, s / v.y, s / v.z); 
}


class Point
{
public:
	Point();
	Point(float x, float y, float z);

	Point AddVector(Vector v);

	Point operator-() const;

	Point operator+(const Vector& v) const;
	Point operator-(const Vector& v) const;

	float x, y, z;
};

Vector operator-(Point a, Point b);
float DotProduct(const Vector& a, const Vector& b);


// Test MFGD
void Test_MFGD();


// MFGD 12 - Smooth Move(ment) (Linear Interpolation)
float Approch(float flGoal, float flCurrent, float dVelocity);

// MFGD 17 - Explosions! (The Remap Function)
float Remap(float x, float t1, float t2, float s1, float s2);

// MFGD 18 - Bullet Whizzes (Projections)
float BulletWhizzes(Point player, Point enemy, Vector direction);
