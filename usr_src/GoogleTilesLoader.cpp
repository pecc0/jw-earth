/*
 * GoogleTilesLoader.cpp
 *
 *  Created on: Feb 11, 2011
 *      Author: Petko
 */

#include "GoogleTilesLoader.h"
#include <sstream>
#include <iostream>
#include <fstream>
#define CURL_STATICLIB
#include "curl/curl.h"

using namespace std;

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
	std::ostringstream coordinatesStr;
	coordinatesStr << "x=" << x << "&y=" << y << "&z=" << z;

	std::ostringstream cacheFileStream;
	cacheFileStream << m_sTmpDir << "\\" << coordinatesStr.str();

	string cacheFileStr(cacheFileStream.str());

	const char* filename = cacheFileStr.c_str();
	ifstream cacheFile(filename);

	if (!cacheFile)
	{
		//download
		std::ostringstream url;
		//TODO url in parameter
		url << "http://khm0.google.com/kh/v=78&" << coordinatesStr << "&s=Galileo.jpg";

		CURL *curl;
		CURLcode res;
		curl = curl_easy_init();
		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
			res = curl_easy_perform(curl);

			/* always cleanup */
			curl_easy_cleanup(curl);
		}
	}

	return new CImg<pixelFormat> (cacheFileStr.c_str());
}

