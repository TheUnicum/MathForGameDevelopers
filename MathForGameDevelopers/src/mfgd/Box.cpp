#include "Box.h"

Box::Box() {}
Box::Box(Point position, Vector scale, float angle, Vector rotation)
	: m_position(position), m_scale(scale), m_f_angle(angle), m_v3_rotation(rotation)
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

void Box::TakeDamage(int iDamage)
{

	m_iHealth -= iDamage;

	if (m_iHealth <= 0)
	{
		//if (m_bTakesDamage)
		if(true)
		{
			std::cout << " regenerate " << std::endl;

			Box* nNew = Box::CreateCharacter(
				Point((float)(rand() % 20) - 10, 0, (float)(rand() % 20) - 10),
				Vector(0.5f, 0.5f, 0.5f),
				(float)(rand() % 360),
				Vector(1.0f, 1.0f, 1.0f));


			// We're at zero health, time do die.
			Box::RemoveCharacter(this);

		}
	}
	
}

Box * Box::CreateCharacter(Point position, Vector scale, float angle, Vector rotation)
{
	size_t iSpot = ~0;

	// Find a spot in my entity list that's empty
	for (size_t i = 0; i < MAX_CHARACTERS; i++)
	{
		if (!Box::m_apEntityList[i])
		{
			iSpot = i;
			break;
		}
	}

	if (iSpot == ~0)
		// Couldn't find a spot for the new guy! Return null instead.
		return nullptr;

	Box::m_apEntityList[iSpot] = new Box(position, scale, angle, rotation);

	static int iParity = 0;

	Box::m_apEntityList[iSpot]->m_iParity = iParity++;
	Box::m_apEntityList[iSpot]->m_iIndex = iSpot;

	return Box::m_apEntityList[iSpot];
}

void Box::RemoveCharacter(Box * pCharacter)
{

	size_t iSpot = ~0;

	// Find a spot in my entity list that's empty.
	for (size_t i = 0; i < MAX_CHARACTERS; i++)
	{
		if (Box::m_apEntityList[i] == pCharacter)
		{
			std::cout << "Trovato" << std::endl;
			iSpot = i;
			break;
		}
	}

	if (iSpot == ~0)
		// Couldn't find this guy in our entity list! Do nothing.
		return;

	delete m_apEntityList[iSpot];
	m_apEntityList[iSpot] = nullptr;
}

Box* Box::m_apEntityList[MAX_CHARACTERS];



// Player
// -------------------------------
Player::Player()
	: Box()
{
}

Player::Player(Point position, Vector scale, float angle, Vector rotation)
	:Box(position, scale, angle, rotation)
{
}

void Player::SetEAngle(float pitch, float yaw, float roll)
{
	m_EAngle.p = pitch;
	m_EAngle.y = yaw;
	m_EAngle.r = roll;
}
