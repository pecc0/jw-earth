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



