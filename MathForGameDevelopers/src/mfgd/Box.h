#pragma once

#include "mfgd/mfgd_Classes.h"
#include "mfgd/mfgd_EulerAngle.h"
#include "mfgd/AABB.h"
#include "mfgd/matrix.h"
#include "mfgd/quaternion.h"

#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/type_ptr.hpp"

#include <vector>

//148.42
#define MAX_CHARACTERS 1000

class Box
{
public:
	Box();
	Box(Point position, Vector scale, float fangle, Vector rotation);

	void SetRotation(const EAngle& angRotation);
	void SetRotation(const Quaternion& qRotation);

	void Update(float dt);
	void Gravity(glm::vec3 gravity, float dt);
	void Gravity(Vector gravity, float dt);
	void Boundaries();

	inline Vector GetPosition() const { return m_position; }
	inline glm::vec3 GetPosVec3() const { return glm::vec3(m_position.x, m_position.y, m_position.z); }
	inline glm::vec3 GetScaleVec3() const { return glm::vec3(m_scale.x, m_scale.y, m_scale.z); }

	void TakeDamage(int iDamage);

private:
public:
	int m_iIndex;
	int m_iParity;

	Point m_position;
	Vector m_scale;
	float m_f_angle;
	Vector m_v3_rotation;


	AABB m_aabbSize;

	Vector m_velocity;

	//utility
	Matrix4x4 m_Transform;
	Matrix4x4 m_TransformInverse;

	bool m_bTakesDamage;
	int m_iHealth;

public:
	static Box* m_apEntityList[];
	static Box* CreateCharacter(Point position, Vector scale, float angle, Vector rotation);
	static void RemoveCharacter(Box* pCharacter);
	static void MergeSortRenderSubList(std::vector<Box*>& apRenderList, size_t iStart, size_t iEnd, Vector camPos);
};


class Player : public Box
{
public:
	Player();
	Player(Point position, Vector scale, float fangle, Vector rotation);

	void SetEAngle(float pitch, float yaw, float roll);
private:
public:
	EAngle m_EAngle;
	
public:
	Vector m_movement;
	Vector m_GoalVelocity;
	Vector m_LerpVelocity = Vector(5, 5, 5);
	Vector m_Cam_Position = Vector(0, 0, 0);
	Vector m_Gravity = Vector(0, -20, 0);
};
