#include "mfgd_EulerAngle.h"
#include "mfgd/quaternion.h"

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


void EAngle::ToAxisAngle(Vector & vecAxis, float & flAngle) const
{
	// Use Quaternions to do our dirty work.
	Quaternion qRoll(Vector(1, 0, 0), r);
	Quaternion qPitch(Vector(0, 0, 1), p);
	Quaternion qYaw(Vector(0, 1, 0), y);

	Quaternion qEuler = qYaw * qPitch * qRoll;

	qEuler.ToAxisAngle(vecAxis, flAngle);
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
