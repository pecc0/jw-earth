/*
 * ITextureReceiver.h
 *
 *  Created on: Feb 20, 2011
 *      Author: PetkoLtd
 */

#ifndef ITEXTURERECEIVER_H_
#define ITEXTURERECEIVER_H_

#include  "irrlicht.h"

using namespace irr;

class ITextureReceiver
{
public:
	virtual void receiveTexture(video::ITexture * texture) = 0;
};

#endif /* ITEXTURERECEIVER_H_ */
