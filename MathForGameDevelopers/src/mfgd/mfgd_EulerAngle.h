#pragma once

#include "mfgd/mfgd_Classes.h"

class EAngle
{
public:
	EAngle();
	EAngle(float pitch, float yaw, float roll);

public:
	Vector ToVector() const;
	void ToAxisAngle(Vector& vecAxis, float& flAngle) const;
	void Normalize();

public:
	float p;
	float y;
	float r;
};