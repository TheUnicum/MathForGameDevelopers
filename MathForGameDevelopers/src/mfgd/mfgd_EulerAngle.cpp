#include "mfgd_EulerAngle.h"

EAngle::EAngle()
	: p(0), y(0), r(0) {}


EAngle::EAngle(float pitch, float yaw, float roll)
	: p(pitch), y(yaw), r(roll) {}

Vector EAngle::ToVector() const
{
	Vector result;
	result.x = cos(y)*cos(p);
	result.y = sin(p);
	result.z = sin(y)*cos(p);
	return result;
}

void EAngle::Normalize()
{
	if (p > 89)
		p = 89;
	if (p < -89)
		p = -89;

	while (y < -180)
		y += 360;
	while (y > 180)
		y -= 360;
}


