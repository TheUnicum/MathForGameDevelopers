#include "Box.h"

Box::Box() {}
Box::Box(Point position, Vector scale)
	: m_position(position), m_scale(scale)
{
	m_aabbSize.vecMin = Vector(-1.0f, -1.0f, -1.0f);
	m_aabbSize.vecMax = Vector(+1.0f, +1.0f, +1.0f);
}

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

void Box::Gravity(Vector gravity, float dt)
{

	std::cout << m_velocity.y << std::endl; 

	m_velocity = m_velocity + gravity * dt;
	if (m_position.y <= m_scale.y)
	{
		m_position.y = m_scale.y;
		m_velocity.y = 0;
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

Player::Player(Point position, Vector scale)
	:Box(position, scale)
{
}

void Player::SetEAngle(float pitch, float yaw, float roll)
{
	m_EAngle.p = pitch;
	m_EAngle.y = yaw;
	m_EAngle.r = roll;
}
