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
