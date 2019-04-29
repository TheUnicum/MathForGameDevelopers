#pragma once

#include "mfgd/mfgd_Classes.h"

#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/type_ptr.hpp"


class Box
{
public:
	Box();
	Box(Vector position, Vector scale);

	void Update(float dt);
	void Gravity(glm::vec3 gravity, float dt);

	inline Vector GetPosition() const { return m_position; }
	inline glm::vec3 GetPosVec3() const { return glm::vec3(m_position.x, m_position.y, m_position.z); }
	inline glm::vec3 GetScaleVec3() const { return glm::vec3(m_scale.x, m_scale.y, m_scale.z); }

private:
	Vector m_velocity;
	Vector m_position;

	Vector m_scale;
};