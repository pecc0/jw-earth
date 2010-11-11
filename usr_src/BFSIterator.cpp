/*
 * BFSIterator.cpp
 *
 *  Created on: Nov 11, 2010
 *      Author: PetkoLtd
 */

#include "JWSphere.h"

namespace jw
{

JWSphere::BFSIterator::BFSIterator(JWSphere* sphere, u32 start, int level) :
	m_sphere(sphere), m_level(level)
{
	m_trQueue.push(start);
	m_trUsed.insert(start);
}

int JWSphere::BFSIterator::getLevel() const
{
	return m_level;
}

void JWSphere::BFSIterator::setLevel(int m_level)
{
	this->m_level = m_level;
}

bool JWSphere::BFSIterator::next(u32* result)
{
	if (m_trQueue.empty())
	{
		return false;
	}
	*result = m_trQueue.front();
	m_trQueue.pop();
	return true;
}

void JWSphere::BFSIterator::accept(u32 triangle)
{
	JWTriangle* tr = m_sphere->getTriangle(triangle, m_level);
	for (int i = 0; i < 3; i++)
	{
		u32 neigh = tr->getNeighbour(i);

		if (m_trUsed.find(neigh) == m_trUsed.end())
		{
			m_trQueue.push(neigh);
			m_trUsed.insert(neigh);

		}
	}
}

}
