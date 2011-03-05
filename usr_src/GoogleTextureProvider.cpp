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
 * JWTextureProvider.cpp
 *
 *  Created on: Dec 4, 2010
 *      Author: PetkoLtd
 */

#include "GoogleTextureProvider.h"
#include "IJWLogger.h"

#define BOOST_THREAD_USE_LIB

#include <boost/thread.hpp>
#include "LoggerFactory.h"
#include <sstream>
#include "irrlitch/ImageCImg.h"

GoogleTextureProvider::GoogleTextureProvider() :
	log(jw::log::LoggerFactory::getLogger("com.jw.GoogleTextureProvider"))
{

}
GoogleTextureProvider::~GoogleTextureProvider()
{
}

void GoogleTextureProvider::getTexture(const irr::core::rect<f32>& boundary,
		int level, video::IVideoDriver* driver)
{
	m_Boundary = boundary;
	m_nLevel = level;
	m_Driver = driver;
	boost::thread thrd(*this);

}

void GoogleTextureProvider::operator()()
{
	int maxTiles = 1 >> m_nLevel;
	irr::core::rect<s32> tilesBoundary;
	tilesBoundary.LowerRightCorner.X = (s32) floor(maxTiles
			* m_Boundary.LowerRightCorner.X);
	tilesBoundary.LowerRightCorner.Y = (s32) floor(maxTiles
			* m_Boundary.LowerRightCorner.Y);

	tilesBoundary.UpperLeftCorner.X = (s32) ceil(maxTiles
			* m_Boundary.UpperLeftCorner.X);
	tilesBoundary.UpperLeftCorner.Y = (s32) ceil(maxTiles
			* m_Boundary.UpperLeftCorner.Y);

	GoogleTilesLoader tileLoader;
	CImg<pixelFormat>* result = new CImg<pixelFormat> (GOOGLE_TILE_SIZE
			* tilesBoundary.getWidth(), GOOGLE_TILE_SIZE
			* tilesBoundary.getHeight(), 1, 3);

	for (int x = tilesBoundary.LowerRightCorner.X; x
			< tilesBoundary.UpperLeftCorner.X; x++)
	{
		for (int y = tilesBoundary.LowerRightCorner.Y; y
				< tilesBoundary.UpperLeftCorner.Y; y++)
		{
			CImg<pixelFormat>* img = tileLoader.loadTile(x, y, 2);
			result->draw_image(x * GOOGLE_TILE_SIZE, y * GOOGLE_TILE_SIZE, *img);
			delete img;
		}
	}

	GoogleVideoDriverWrapper driver(result);

	driver.setWrapped(m_Driver);

	std::ostringstream textureName;
	textureName << "CImg:z=" << m_nLevel << ",x1="
			<< tilesBoundary.LowerRightCorner.X << ",y1="
			<< tilesBoundary.LowerRightCorner.Y << ",x2="
			<< tilesBoundary.UpperLeftCorner.X << ",y2="
			<< tilesBoundary.UpperLeftCorner.Y;


	ITexture* texture = driver.getTexture(textureName.str().c_str());

	//result->

	delete result;

	m_TextureReceiver->receiveTexture(texture);
}

void GoogleTextureProvider::setTextureReceiver(
		ITextureReceiver *textureReceiver)
{
	m_TextureReceiver = textureReceiver;
}

GoogleVideoDriverWrapper::GoogleVideoDriverWrapper(CImg<unsigned char>* img) :
	m_Img(img)
{

}

IImage* GoogleVideoDriverWrapper::createImageFromFile(const io::path& filename)
{
	if (filename == "CImg")
	{
		ImageCImg * result = new ImageCImg(m_Img);
		return result;
	}
	else
	{
		return m_wrapped->createImageFromFile(filename);
	}
}
