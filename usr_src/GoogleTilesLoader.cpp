/*
 * GoogleTilesLoader.cpp
 *
 *  Created on: Feb 11, 2011
 *      Author: Petko
 */

#include "GoogleTilesLoader.h"
#include <sstream>

GoogleTilesLoader::GoogleTilesLoader() :
	m_sTmpDir("downloaded")
{

}

GoogleTilesLoader::GoogleTilesLoader(string& tmpDir) :
	m_sTmpDir(tmpDir)
{
}

GoogleTilesLoader::~GoogleTilesLoader()
{
	// TODO Auto-generated destructor stub
}

CImg<pixelFormat> *GoogleTilesLoader::loadTile(int x, int y, int z)
{
	std::ostringstream params;
	params << "v=78&x=" << x << "&y=" << y << "&z=" << z
			<< "&s=Galileo.jpg";
	std::ostringstream cacheFile;
	cacheFile<<m_sTmpDir<<"\\"<<params.str();
	return new CImg<pixelFormat> (cacheFile.str().c_str());
}

