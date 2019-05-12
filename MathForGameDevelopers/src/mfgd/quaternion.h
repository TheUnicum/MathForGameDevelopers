#pragma once

#include "mfgd/mfgd_Classes.h"

#include <iostream>

#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif


class Quaternion
{
public:
	Quaternion() {};
	Quaternion(const Vector&n, float a);

public:
	const Quaternion Inverted() const;
	const Quaternion operator*(const Quaternion& q) const;
	const Vector operator*(const Vector& V) const;
	const Quaternion operator^(float t) const;
	const Quaternion Slerp(const Quaternion& other, float t) const;

	void ToAxisAngle(Vector& vecAxis, float& flAngle) const;

public:
	float w;
	Vector v; // x, y, z;
};

// utility overload std::cout
std::ostream& operator<<(std::ostream& stream, const Quaternion& q);
