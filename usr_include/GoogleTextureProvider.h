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
 * JWTextureProvider.h
 *
 *  Created on: Dec 4, 2010
 *      Author: PetkoLtd
 */

#ifndef JWTEXTUREPROVIDER_H_
#define JWTEXTUREPROVIDER_H_


#include "ITextureReceiver.h"

using namespace irr;

class GoogleTextureProvider
{
private:
	ITextureReceiver * m_TextureReceiver;
public:

	virtual ~GoogleTextureProvider();

	void getTexture(core::vector2d<f32>& ceterCoordinates);

	void setTextureReceiver(ITextureReceiver * textureReceiver);
};

#endif /* JWTEXTUREPROVIDER_H_ */
