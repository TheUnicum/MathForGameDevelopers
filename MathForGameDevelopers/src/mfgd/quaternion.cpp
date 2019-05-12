#include "quaternion.h"


Quaternion::Quaternion(const Vector& n, float a)
{
	/*
	( cos(a/2)	, n*sin(a/2) )
	( w			, x,  y,  z  )
	*/

	// a will be our theta.
	// We must convert degrees to radians.
	// 360 degrees == 2pi radians
	a = a / 180 * (float)M_PI;

	w = cos(a / 2);

	x = n.x*sin(a / 2);
	y = n.y*sin(a / 2);
	z = n.z*sin(a / 2);
}

std::ostream & operator<<(std::ostream & stream, const Quaternion & q)
{
	stream << "Qtn: (" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
	return stream;
}
