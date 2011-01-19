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
 * ReadBuffer.cpp
 *
 *  Created on: Nov 29, 2010
 *      Author: Petko
 */

#include "irrlitch/ReadBuffer.h"
#include "irrMath.h"

ReadBuffer::ReadBuffer(void* buffer, int size) :
	m_Position(0), m_Size(size), m_ptrBuffer(buffer)
{

}

ReadBuffer::~ReadBuffer()
{
}

s32 ReadBuffer::read(void *buffer, u32 sizeToRead)
{
	sizeToRead = core::min_(m_Size - m_Position, sizeToRead);
	memcpy(buffer, ((char*) m_ptrBuffer) + m_Position, sizeToRead);
	return sizeToRead;
}

bool ReadBuffer::seek(long finalPos, bool relativeMovement)
{
	u32 newPosition;
	if (relativeMovement)
	{
		newPosition = m_Position + finalPos;
	}
	else
	{
		newPosition = finalPos;
	}
	if (newPosition >= 0 && newPosition < m_Size)
	{
		m_Position = newPosition;
		return true;
	}
	else
	{
		return false;
	}
}

long ReadBuffer::getPos() const
{
	return m_Position;
}

io::path g_Path;

const io::path & ReadBuffer::getFileName() const
{
	return g_Path;
}



long ReadBuffer::getSize() const
{
	return m_Size;
}



