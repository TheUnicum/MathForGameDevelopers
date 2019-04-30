#pragma once

#include "mfgd/mfgd_Classes.h"
#include "mfgd/mfgd_EulerAngle.h"

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
	void Boundaries();

	inline Vector GetPosition() const { return m_position; }
	inline glm::vec3 GetPosVec3() const { return glm::vec3(m_position.x, m_position.y, m_position.z); }
	inline glm::vec3 GetScaleVec3() const { return glm::vec3(m_scale.x, m_scale.y, m_scale.z); }

private:
public:
	Vector m_velocity;
	Vector m_position;

	Vector m_scale;
};

class Player : public Box
{
public:
	Player();
	Player(Vector position, Vector scale);

	void SetEAngle(float pitch, float yaw, float roll);
private:
public:
	EAngle m_EAngle;
	
public:
	Vector m_velocity;
	Vector m_movement;
	Vector m_GoalVelocity;
	Vector m_LerpVelocity = Vector(5, 5, 5);
	Vector m_Cam_Position = Vector(0, 0, 0);
};