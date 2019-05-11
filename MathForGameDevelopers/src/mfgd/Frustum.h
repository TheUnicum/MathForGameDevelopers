#pragma once

#include "mfgd/plane.h"

//class Matrix4x4
#include "mfgd/matrix.h"


enum FustumType
{
	NEAR = 0, FAR = 1, LEFT = 2, RIGHT = 3, UP = 4, DOWN = 5
};




// Frustum class defined by six planes enclosing the frustum. The normals face inward.
class CFrustum
{
public:
	CFrustum(const Matrix4x4& m);

public:
	bool SphereInterception(const Vector& vecCenter, float flRadius);

public:
	CPlane p[6];
};




