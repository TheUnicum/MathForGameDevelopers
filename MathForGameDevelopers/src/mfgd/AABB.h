#pragma once

#include "mfgd/mfgd_Classes.h"
#include <algorithm>

//  axis - aligned bounding box
class AABB
{
public:
	AABB operator+(const Point& p) const;

public:
	Vector vecMin;
	Vector vecMax;
};

bool ClipLine(int d, const AABB& aabbBox, const Vector& v0, const Vector& v1, float& f_low, float& f_high);

// Find the intersection of a line from v0 to v1 and an axis-aligned bounding box
bool LineAABBIntersection(const AABB& aabbBox, const Vector& v0, const Vector& v1,
	Vector& vecIntersection, float & flFraction);

