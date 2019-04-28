#include "mfgd_EulerAngle.h"

EAngle::EAngle()
	: p(0), y(0), r(0) {}


EAngle::EAngle(float pitch, float yaw, float roll)
	: p(pitch), y(yaw), r(roll) {}

Vector EAngle::ToVector() const
{
	return Vector();
}

void EAngle::Normalize()
{
}


