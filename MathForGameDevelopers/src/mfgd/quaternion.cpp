#include "quaternion.h"


// Building a quaternion from an axis-angle rotation.
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

	v = n * sin(a / 2);
}

const Quaternion Quaternion::Inverted() const
{
	Quaternion q;
	q.w = w;
	q.v = -v;
	return q;
}

// Multiplying two quaternions together combines the rotations.
//http://graphics.stanford.edu/courses/cs348a-17-winter/Papers/quaternion.pdf
// 2.1 Addition and Multiplication
const Quaternion Quaternion::operator*(const Quaternion & q) const
{
	/*
	q	= (p0 + p1i + p2j + p3k)(q0 + q1i + q2j + q3k)
		= p0q0 - (p1q1 + p2q2 + p3q3) + p0(q1i + q2j + q3k) + q0(p1i + p2j + p3k)
		+ (p2q3 - p3q2)i + (p3q1 - p1q3)j + (p1q2 - p2q1)k


		= p0q0 - (p Dot q) + p0q + q0p + (p x q)
		  |----w part----| + |---vector part---|
	*/
	Quaternion r;
	//r.w = w*q.w + 
	r.w = w * q.w + v.Dot(q.v);
	r.v = v * q.w + q.v*w + v.Cross(q.v);
	return r;
}

std::ostream & operator<<(std::ostream & stream, const Quaternion & q)
{
	stream << "Qtn: (" << q.w << ", " << q.v.x << ", " << q.v.y << ", " << q.v.z << ")";
	return stream;
}
