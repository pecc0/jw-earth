/* AUTORIGHTS
Copyright (C) 2010,2011 Petko Petkov (petkodp@gmail.com
      
This file is part of JWorld.

JWorld is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/
/*
 * BFSIterator.cpp
 *
 *  Created on: Nov 11, 2010
 *      Author: PetkoLtd
 */

#include "JWSphere.h"

namespace jw
{

BFSIterator::BFSIterator(JWSphere* sphere, int level) :
	m_sphere(sphere), m_level(level)
{

}

BFSIterator::BFSIterator(JWSphere* sphere, u32 start, int level) :
	m_sphere(sphere), m_level(level)
{
	start = JWTriangle::cropToLevel(start, level);
	m_trQueue.push(start);
	setUsed(start);
}

BFSIterator::BFSIterator(BFSIterator * old, int level) :
	m_sphere(old->m_sphere), m_level(level)
{

}

bool BFSIterator::isUsed(u32 tr)
{
	return m_trUsed.find(tr) != m_trUsed.end();
}

void BFSIterator::setUsed(u32 tr)
{
	m_trUsed.insert(tr);
}

void BFSIterator::push(u32 triangle)
{
	m_trQueue.push(triangle);
}

int BFSIterator::getLevel() const
{
	return m_level;
}

void BFSIterator::setLevel(int m_level)
{
	this->m_level = m_level;
}

bool BFSIterator::next(u32* result)
{
	if (m_trQueue.empty())
	{
		return false;
	}
	*result = m_trQueue.front();
	m_trQueue.pop();
	return true;
}

void BFSIterator::accept(u32 triangle)
{
	JWTriangle* tr = m_sphere->getTriangle(triangle, m_level);
	for (int i = 0; i < 3; i++)
	{
		u32 neigh = tr->getNeighbour(i);

		if (!isUsed(neigh))
		{
			m_trQueue.push(neigh);
			setUsed(neigh);
		}
	}
}

}
