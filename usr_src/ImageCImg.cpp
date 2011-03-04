/*
 * ImageCImg.cpp
 *
 *  Created on: Mar 4, 2011
 *      Author: Petko
 */

#include "irrlitch/ImageCImg.h"

//virtual ([^\(]*) ([a-zA-Z]+)\(([^\)]*)\)([ const]*);

//\1 ImageCImg::\2(\3) \4 {}

ImageCImg::ImageCImg(const CImg<wrappedPixelFormat>& wrapped) :
	m_wrapped(wrapped)
{

}

ImageCImg::~ImageCImg()
{
}

void* ImageCImg::lock()
{
	return m_wrapped.data();
}

//! Unlock function.
/** Should be called after the pointer received by lock() is not
 needed anymore. */
void ImageCImg::unlock()
{
}

//! Returns width and height of image data.
const core::dimension2d<u32>& ImageCImg::getDimension() const
{
	return core::dimension2d<u32>(m_wrapped.width(), m_wrapped.height());
}

//! Returns bits per pixel.
u32 ImageCImg::getBitsPerPixel() const
{
	return getBytesPerPixel() * 8;
}

//! Returns bytes per pixel
u32 ImageCImg::getBytesPerPixel() const
{
	return m_wrapped.spectrum();
}

//! Returns image data size in bytes
u32 ImageCImg::getImageDataSizeInBytes() const
{
	return m_wrapped.width() * m_wrapped.height() * getBytesPerPixel();
}

//! Returns image data size in pixels
u32 ImageCImg::getImageDataSizeInPixels() const
{
	return getImageDataSizeInBytes() * 8;
}

//! Returns a pixel
SColor ImageCImg::getPixel(u32 x, u32 y) const
{
	return SColor(255, m_wrapped(x, y, 0, 0), m_wrapped(x, y, 0, 1), m_wrapped(x, y, 0, 2));
}

//! Sets a pixel
void ImageCImg::setPixel(u32 x, u32 y, const SColor &color, bool blend)
{
	const wrappedPixelFormat colorArr[] = { color.getRed(), color.getGreen(), color.getBlue() };
	m_wrapped.draw_point(x, y, colorArr);
}

//! Returns the color format
ECOLOR_FORMAT ImageCImg::getColorFormat() const
{
	return ECF_R8G8B8;
}

//! Returns mask for red value of a pixel
u32 ImageCImg::getRedMask() const
{
	return 0x00FF0000;
}

//! Returns mask for green value of a pixel
u32 ImageCImg::getGreenMask() const
{
	return 0x0000FF00;
}

//! Returns mask for blue value of a pixel
u32 ImageCImg::getBlueMask() const
{
	return 0x000000FF;
}

//! Returns mask for alpha value of a pixel
u32 ImageCImg::getAlphaMask() const
{
	return 0x0;
}

//! Returns pitch of image
u32 ImageCImg::getPitch() const
{
	return m_wrapped.width() * getBytesPerPixel();
}

//! Copies the image into the target, scaling the image to fit
void ImageCImg::copyToScaling(void* target, u32 width, u32 height, ECOLOR_FORMAT format, u32 pitch)
{
}

//! Copies the image into the target, scaling the image to fit
void ImageCImg::copyToScaling(IImage* target)
{
}

//! copies this surface into another
void ImageCImg::copyTo(IImage* target, const core::position2d<s32>& pos)
{
}

//! copies this surface into another
void ImageCImg::copyTo(IImage* target, const core::position2d<s32>& pos, const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect)
{
}

//! copies this surface into another, using the alpha mask, an cliprect and a color to add with
void ImageCImg::copyToWithAlpha(IImage* target, const core::position2d<s32>& pos, const core::rect<s32>& sourceRect, const SColor &color, const core::rect<s32>* clipRect)
{
}

//! copies this surface into another, scaling it to fit, appyling a box filter
void ImageCImg::copyToScalingBoxFilter(IImage* target, s32 bias, bool blend)
{
}

//! fills the surface with black or white
void ImageCImg::fill(const SColor &color)
{
}
