#pragma once

#include "mfgd/mfgd_Classes.h"

#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif

// Rotation of a vector around an axis by theta degrees.
const Vector RotateVectorAroundAxisAngle(const Vector& n, float a, const Vector& v)
{
	// a will be our theta.
	// We must convert degree to radians.
	// 360 degree == 2pi radians
	a = a / 180 * (float)M_PI;

	return  v * cos(a) + v.Dot(n)*n*(1 - cos(a)) + n.Cross(v)*sin(a);
}
