#pragma once

#include "mfgd/mfgd_Classes.h"

#include <iostream>

#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif


class Quaternion
{
public:
	Quaternion(const Vector&n, float a);

public:
	float w, x, y, z;
};

// utility overload std::cout
std::ostream& operator<<(std::ostream& stream, const Quaternion& q);
