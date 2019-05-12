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

// Rotate a vector with this quaternion.
// The basic equation is qpq* (the * means inverse) but we use a simplified version of equation.
const Vector Quaternion::operator*(const Vector & V) const
{
	Quaternion p;
	p.w = 0;
	p.v = V;

	// Could do it it this way:
	/*
	const Quaternion& q = (*this);
	return (q * p * q.Inverted()).v;
	*/

	// But let's optimize it a bit instead.

	/*
	qpq* = p + 2w(v x V) + 2 * ( v x (v x V))
	*/

	Vector vcV = v.Cross(V);
	return V + vcV * (2 * w) + v.Cross(vcV) * 2;
}


const Quaternion Quaternion::operator^(float t) const
{
	// Convert the quaternion back into axis/angle
	float a;
	Vector n;

	ToAxisAngle(n, a);

	// Scale the angle by t
	float at = a * t / 2;	// ---------------------------------<< correction ?

	// Make a new quaternion out of the new value
	return Quaternion(n, at);
}


// Spherical linear interpolation of a quaternion
const Quaternion Quaternion::Slerp(const Quaternion& other, float t) const
{
	const Quaternion& q = *this;
	Quaternion r = other;
	
	// This is the quaternion equation!
	//return ((r * q.Inverted()) ^ t) * q;
	// |--------------------------------------------|
	// |return ((r * q.Inverted()) ^ t) * q;		|
	// |--------------------------------------------|

	// But this one is faster. For the derivation, see "3D Primer For Graphics
	// and Game Development" by Dunn and Parberry, section 10.4.13. I may go
	// over this one in a future video, but the two formulas come out the same.
	// (Hint: I'm more likely to do it if I get a request!)
	float flCosOmega = w * r.w + r.v.Dot(v);
	if (flCosOmega < 0)
	{
		// Avoid going the long way around.
		r.w = -r.w;
		r.v = -r.v;
		flCosOmega = -flCosOmega;
	}

	float k0, k1;
	if (flCosOmega > 0.9999f)
	{
		// Very close, use a linear interpolation.
		k0 = 1 - t;
		k1 = t;
	}
	else
	{
		// Trig identity, sin^2 + cos^2 = 1
		float flSinOmega = sqrt(1 - flCosOmega * flCosOmega);

		// Compute the angle omega
		float flOmega = atan2(flSinOmega, flCosOmega);
		float flOneOverSinOmega = 1 / flSinOmega;

		k0 = sin((1 - t)*flOmega) * flOneOverSinOmega;
		k1 = sin(t*flOmega) * flOneOverSinOmega;
	}

	// Interpolate
	Quaternion result;
	result.w = q.w * k0 + r.w * k1;
	result.v = q.v * k0 + r.v * k1;

	return result;
}


void Quaternion::ToAxisAngle(Vector& vecAxis, float& flAngle) const
{
	// Convert the quaternion back into axis/angle
	if (v.LengthSqr() < 0.0001f)
		vecAxis = Vector(1.0f, 0.0f, 0.0f);
	else
		vecAxis = v.Normalized();

	//TAssert(fabs(vecAxis.LengthSqr() - 1) < 0.000001f);

	// This is the opposite procedure as explained in Quaternion-Constructor
	flAngle = acos(w) * 2;

	// Convert to degrees
	flAngle *= 180 / (float(M_PI));
}


std::ostream & operator<<(std::ostream & stream, const Quaternion & q)
{
	stream << "Qtn: (" << q.w << ", " << q.v.x << ", " << q.v.y << ", " << q.v.z << ")";
	return stream;
}
