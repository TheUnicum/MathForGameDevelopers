#include "Box.h"

Box::Box() {}
Box::Box(Point position, Vector scale, float angle, Vector rotation)
	: m_position(position), m_scale(scale), m_f_angle(angle), m_v3_rotation(rotation)
{
	m_aabbSize.vecMin = Vector(-1.0f, -1.0f, -1.0f);
	m_aabbSize.vecMax = Vector(+1.0f, +1.0f, +1.0f);
}


void Box::SetRotation(const EAngle & angRotation)
{
	//float m_f_angle;
	//Vector m_v3_rotation;
	angRotation.ToAxisAngle(m_v3_rotation, m_f_angle);
}


void Box::SetRotation(const Quaternion & qRotation)
{
	qRotation.ToAxisAngle(m_v3_rotation, m_f_angle);
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
		if(false)
		{
			std::cout << " regenerate " << std::endl;

			Box* nNew = Box::CreateCharacter(
				Point((float)(rand() % 20) - 10, 0, (float)(rand() % 20) - 10),
				//Vector(0.5f, 0.5f, 0.5f),
				Vector(1.0f, 1.0f, 1.0f),
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

void Box::MergeSortRenderSubList(std::vector<Box*>& apRenderList, size_t iStart, size_t iEnd, Vector camPos)
{
	// iStart the index of the first index that we are to sort. iEnd is the index+1 of the last index we are to sort
	size_t iLength = iEnd - iStart;

	if (iLength <= 1)
	{
		// We are in a base case of one item. We're sorted! Return.
		return;
	}
	else if (iLength == 2)
	{
		// We are in a base case of two items. If the first one i bigger than the second, swap them.
		float flLeftDistanceSqr = (apRenderList[iStart]->m_position - camPos).LengthSqr();
		float flRightDistanceSqr = (apRenderList[iStart + 1 ]->m_position - camPos).LengthSqr();

		// We can compare square distances just like regular disatance, ant they're faster to calculate.
		if (flLeftDistanceSqr > flRightDistanceSqr)
			std::swap(apRenderList[iStart], apRenderList[iStart + 1]);

		// Now we're sorted!
		return;
	}

	//0, 1, 2, 3, 4
	//0+(4+1)/2 = 2
	//	0-1
	//	2-3-4

	// We aren't in a base case yet. Split the list in two.
	size_t iMiddle = (iStart - iEnd) / 2;
	MergeSortRenderSubList(apRenderList, iStart, iMiddle, camPos);
	MergeSortRenderSubList(apRenderList, iMiddle, iEnd, camPos);

	// Merge the two sub-lists together by plucking off the lowest element.
	// First make a copy of the list
	std::vector<Box*> apRenderListCopy = apRenderList;
	
	size_t iLeft = iStart;
	size_t iRight = iMiddle;
	size_t iOutPut = iStart;
	while (true)
	{
		float flLeftDistanceSqr = (apRenderListCopy[iLeft]->m_position - camPos).LengthSqr();
		float flRightDistanceSqr = 0;
		if (iRight != iEnd)
			flRightDistanceSqr = (apRenderListCopy[iRight]->m_position - camPos).LengthSqr();

		// We can compare square distances just like regular disatance, ant they're faster to calculate.
		bool bUseLeft = flLeftDistanceSqr < flRightDistanceSqr;
		if (iRight == iEnd)
			bUseLeft = true;
		else if (iLeft == iMiddle)
			bUseLeft = false;

		if (bUseLeft)
			apRenderList[iOutPut++] = apRenderListCopy[iLeft++];
		else
			apRenderList[iOutPut++] = apRenderListCopy[iRight++];

		// If we're reached the end of both sub-lists, break form the loop.
		if (iLeft == iMiddle && iRight == iEnd)
			break;
	}

	// Our sub-list is sorted! Return

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
