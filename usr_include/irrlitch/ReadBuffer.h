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
 * ReadBuffer.h
 *
 *  Created on: Nov 29, 2010
 *      Author: Petko
 */

#ifndef READBUFFER_H_
#define READBUFFER_H_

#include "IReadFile.h"
#include "irrTypes.h"

using namespace irr;

/**
 * Implementation of IReadFile which reads from a buffer
 */
class ReadBuffer: public virtual io::IReadFile
{
	u32 m_Position;
	u32 m_Size;
	void* m_ptrBuffer;
public:
	ReadBuffer(void* buffer, int size);
	virtual ~ReadBuffer();

	virtual s32 read(void* buffer, u32 sizeToRead);

	virtual bool seek(long finalPos, bool relativeMovement = false);

	virtual long getSize() const;

	virtual long getPos() const;

	virtual const io::path& getFileName() const;
};

#endif /* READBUFFER_H_ */
