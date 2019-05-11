#include "Frustum.h"

CFrustum::CFrustum(const Matrix4x4 & m)
{
	// It'll be explain all this junk in a future video.
	p[FustumType::RIGHT].n.x = m.m[0][3] - m.m[0][0];
	p[FustumType::RIGHT].n.y = m.m[1][3] - m.m[1][0];
	p[FustumType::RIGHT].n.z = m.m[2][3] - m.m[2][0];
	p[FustumType::RIGHT].d = m.m[3][3] - m.m[3][0];

	p[FustumType::LEFT].n.x = m.m[0][3] + m.m[0][0];
	p[FustumType::LEFT].n.y = m.m[1][3] + m.m[1][0];
	p[FustumType::LEFT].n.z = m.m[2][3] + m.m[2][0];
	p[FustumType::LEFT].d = m.m[3][3] + m.m[3][0];

	p[FustumType::DOWN].n.x = m.m[0][3] + m.m[0][1];
	p[FustumType::DOWN].n.y = m.m[1][3] + m.m[1][1];
	p[FustumType::DOWN].n.z = m.m[2][3] + m.m[2][1];
	p[FustumType::DOWN].d = m.m[3][3] + m.m[3][1];

	p[FustumType::UP].n.x = m.m[0][3] - m.m[0][1];
	p[FustumType::UP].n.y = m.m[1][3] - m.m[1][1];
	p[FustumType::UP].n.z = m.m[2][3] - m.m[2][1];
	p[FustumType::UP].d = m.m[3][3] - m.m[3][1];

	p[FustumType::FAR].n.x = m.m[0][3] - m.m[0][2];
	p[FustumType::FAR].n.y = m.m[1][3] - m.m[1][2];
	p[FustumType::FAR].n.z = m.m[2][3] - m.m[2][2];
	p[FustumType::FAR].d = m.m[3][3] - m.m[3][2];

	p[FustumType::NEAR].n.x = m.m[0][3] + m.m[0][2];
	p[FustumType::NEAR].n.y = m.m[1][3] + m.m[1][2];
	p[FustumType::NEAR].n.z = m.m[2][3] + m.m[2][2];
	p[FustumType::NEAR].d = m.m[3][3] + m.m[3][2];

	// Normalize all plane normals
	for (int i = 0; i < 6; i++)
		p[i].Normalize();

}


bool CFrustum::SphereInterception(const Vector& vecCenter, float flRadius)
{
	// Loop through each plane that comprises the frustum
	for (int i = 0; i < 6; i++)
	{

		// MFGD 29;
		// p*n + d + r >= 0;
		// Plane-sphere intersection test. If p*n + d + r < 0 then we're outside the plane.
		if (vecCenter.Dot(p[i].n) + p[i].d + flRadius <= 0)
			return false;
	}

	// If none of the planes had the entity lying on its "negative" side then it must be
	// on the "positive" side for all of them. Thus the entity is inside or touching the frustum.
	return true;
}
