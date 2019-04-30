#include "Box.h"

Box::Box() {}
Box::Box(Vector position, Vector scale)
	: m_position(position), m_scale(scale) {}

void Box::Update(float dt)
{
	m_position = m_position + m_velocity * dt;
}

void Box::Gravity(glm::vec3 gravity, float dt)
{
	Vector Vgravity = Vector(gravity.x, gravity.y, gravity.z);
	m_velocity = m_velocity +  Vgravity * dt;
	if (m_position.y <= m_scale.y)
	{
		m_position.y = m_scale.y;
		m_velocity.y = - m_velocity.y;
	}

}

void Box::Boundaries()
{
	if (m_position.y < m_scale.y)
	{
		m_position.y = m_scale.y;
	}
}

Player::Player()
	: Box()
{
}

Player::Player(Vector position, Vector scale)
	:Box(position, scale)
{
}

void Player::SetEAngle(float pitch, float yaw, float roll)
{
	m_EAngle.p = pitch;
	m_EAngle.y = yaw;
	m_EAngle.r = roll;
}
