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
	cacheFileStream << m_sTmpDir << "\\" << coordinatesStr.str() << ".jpg";

	string cacheFileStr(cacheFileStream.str());

	const char* filename = cacheFileStr.c_str();
	ifstream cacheStream(filename);

	if (!cacheStream)
	{
		//download
		std::ostringstream url;
		//TODO url in parameter
		url << "http://khm0.google.com/kh/v=80&" << coordinatesStr.str()
				<< "&s=Galileo.jpg";

		CURL *curl;
		CURLcode res;
		curl = curl_easy_init();
		if (curl)
		{
			string urlString = url.str();

			cout << "URL=" << urlString.c_str() << endl;

			struct curl_slist *chunk = NULL;

			chunk
					= curl_slist_append(
							chunk,
							"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13");
			//chunk = curl_slist_append(chunk, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");

			curl_easy_setopt(curl, CURLOPT_URL, urlString.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
			FILE* cacheFile = fopen(filename, "w+b");

			curl_easy_setopt(curl, CURLOPT_WRITEDATA, cacheFile);

			res = curl_easy_perform(curl);

			fclose(cacheFile);

			/* always cleanup */
			curl_easy_cleanup(curl);

		}
	}

	return new CImg<pixelFormat> (cacheFileStr.c_str());
}

