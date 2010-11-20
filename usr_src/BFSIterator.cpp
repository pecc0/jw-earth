/*
 * BFSIterator.cpp
 *
 *  Created on: Nov 11, 2010
 *      Author: PetkoLtd
 */

#include "JWSphere.h"

namespace jw
{

JWSphere::BFSIterator::BFSIterator(JWSphere* sphere, int level) :
	m_sphere(sphere), m_level(level)
{

}

JWSphere::BFSIterator::BFSIterator(JWSphere* sphere, u32 start, int level) :
	m_sphere(sphere), m_level(level)
{
	start = JWTriangle::cropToLevel(start, level);
	m_trQueue.push(start);
	m_trUsed.insert(start);
}

JWSphere::BFSIterator::BFSIterator(JWSphere::BFSIterator * old, int level) :
	m_sphere(old->m_sphere), m_level(level)
{
	set<u32>::iterator i;
	for (i = old->m_trUsed.begin(); i != old->m_trUsed.end(); ++i)
	{
		u32 tr = JWTriangle::cropToLevel(*i, level);
		m_trUsed.insert(tr);
	}

	while (!old->m_trQueue.empty())
	{
		u32 tr = JWTriangle::cropToLevel(old->m_trQueue.front(), level);
		old->m_trQueue.pop();
		if (m_trUsed.find(tr) == m_trUsed.end())
		{
			m_trQueue.push(tr);
			m_trUsed.insert(tr);
		}

	}
}

bool JWSphere::BFSIterator::isUsed(u32 tr)
{
	return m_trUsed.find(tr) == m_trUsed.end();
}

void JWSphere::BFSIterator::setUsed(u32 tr)
{
	m_trUsed.insert(tr);
}

void JWSphere::BFSIterator::push(u32 triangle)
{
	m_trQueue.push(triangle);
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
