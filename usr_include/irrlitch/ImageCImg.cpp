/*
 * ImageCImg.cpp
 *
 *  Created on: Mar 4, 2011
 *      Author: Petko
 */

#include "ImageCImg.h"

ImageCImg::ImageCImg(const IImage& wrapped) :
	m_wrapped(wrapped)
{

}

ImageCImg::~ImageCImg()
{
}

