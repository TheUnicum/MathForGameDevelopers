#pragma once

#include "mfgd/mfgd_Classes.h"

// Plane class defined by normal vector which should always be unit length and a "distance" to the origin
// Also con be thought as a ax + by + cz +d = 0, the equation of a plane in 3D space, where a b and c are the x, y, z of the normal n.

class CPlane
{
public:
	void Normalize()
	{
		// It helps a ton if our plane are normalized, meaning n is unit length,
		// To normalize the plane, we do this operation:
		// s(ax + by + cz + d) = s(0)
		// We calculate s by using 1/|n|, and it gets us the number we must scale n by to meke it unit length.
		// Notice how d needs to be scaled also.

		float flScale = 1 / n.Length();
		n.x *= flScale;
		n.y *= flScale;
		n.z *= flScale;
		d *= flScale;
	}

public:
	Vector n; // The normal
	float d; // The "distance" to the origin.
};