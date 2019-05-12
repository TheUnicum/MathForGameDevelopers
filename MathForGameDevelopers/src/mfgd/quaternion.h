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

public:
	float w;
	Vector v; // x, y, z;
};

// utility overload std::cout
std::ostream& operator<<(std::ostream& stream, const Quaternion& q);
