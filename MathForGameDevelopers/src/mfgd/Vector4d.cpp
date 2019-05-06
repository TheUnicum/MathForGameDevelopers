#include "Vector4d.h"

Vector4d::Vector4d()
	: x(0), y(0), z(0), w(0) {}

Vector4d::Vector4d(float x, float y, float z, float w)
	: x(x), y(y), z(z), w(w) {}

Vector4d::Vector4d(const Point & p)
	: x(p.x), y(p.y), z(p.z), w(0) {}

Vector4d::Vector4d(const Vector & v3)
	: x(v3.x), y(v3.y), z(v3.z), w(0) {}

Vector4d::Vector4d(const Vector & v3, float w)
	: x(v3.x), y(v3.y), z(v3.z), w(w) {}

Vector4d Vector4d::Normalized() const
{
	Vector4d Normalized;
	Normalized = (*this) / Length();
	return Normalized;
}

