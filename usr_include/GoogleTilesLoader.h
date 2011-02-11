/*
 * GoogleTilesLoader.h
 *
 *  Created on: Feb 11, 2011
 *      Author: Petko
 */

#ifndef GOOGLETILESLOADER_H_
#define GOOGLETILESLOADER_H_

#define cimg_use_jpeg

#include <string>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

typedef unsigned char pixelFormat;

class GoogleTilesLoader
{
private:
	string m_sTmpDir;
public:
	GoogleTilesLoader();
	GoogleTilesLoader(string& tmpDir);
	virtual ~GoogleTilesLoader();

	CImg<pixelFormat>* loadTile(int x, int y, int z);
};

#endif /* GOOGLETILESLOADER_H_ */
