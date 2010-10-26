/*
 * DecapsulatedHashtable.h
 *
 *  Created on: Oct 25, 2010
 *      Author: PetkoLtd
 */

#ifndef DECAPSULATEDHASHTABLE_H_
#define DECAPSULATEDHASHTABLE_H_

namespace jw
{
//prime number
//#define UNDE 131072
//#define SIZE 1046527



template<class K, class D>
class DecapsulatedHashtable
{
	int m_nSize;
	int m_nUndeletable;
	D* m_ptrPool;
public:
	DecapsulatedHashtable();
	virtual ~DecapsulatedHashtable();
};

}

#endif /* DECAPSULATEDHASHTABLE_H_ */
