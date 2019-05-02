#include "mfgd/AABB.h"

AABB AABB::operator+(const Point & p) const
{
	AABB result = (*this);
	result.vecMin = p + vecMin;
	result.vecMax = p + vecMax;
	return result;
}

bool ClipLine(int d, const AABB & aabbBox, const Vector & v0, const Vector & v1, float & f_low, float & f_high)
{
	// MFGD 16
	/*
	fy = (Py - v0.y) / (v1.y - v0.y)
	*/

	float f_dim_low, f_dim_high;

	f_dim_low = (aabbBox.vecMin.v[d] - v0.v[d]) / (v1.v[d] - v0.v[d]);
	f_dim_high = (aabbBox.vecMax.v[d] - v0.v[d]) / (v1.v[d] - v0.v[d]);

	if (f_dim_high < f_dim_low)
		std::swap(f_dim_high, f_dim_low);

	if (f_dim_high < f_low)
		return false;

	if (f_dim_low > f_high)
		return false;

	f_low = std::max(f_dim_low, f_low);
	f_high = std::min(f_dim_high, f_high);

	return true;
}

bool LineAABBIntersection(const AABB & aabbBox, const Vector & v0, const Vector & v1, Vector & vecIntersection, float & flFraction)
{
	float f_low = 0.0f;
	float f_high = 1.0f;

	// 0 Axis x
	if (!ClipLine(0, aabbBox, v0, v1, f_low, f_high))
		return false;

	if (!ClipLine(1, aabbBox, v0, v1, f_low, f_high))
		return false;

	if (!ClipLine(2, aabbBox, v0, v1, f_low, f_high))
		return false;

	// MFGD 16
	/*
	B = v1 - v0
	I = v0 + B * fraction
	*/

	Vector b = v1 - v0;
	vecIntersection = v0 + b * f_low;

	flFraction = f_low;

	return true;
}

