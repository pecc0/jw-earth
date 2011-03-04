/*
 * ImageCImg.h
 *
 *  Created on: Mar 4, 2011
 *      Author: Petko
 */

#ifndef IMAGECIMG_H_
#define IMAGECIMG_H_

#include "irrlicht.h"

using namespace irr;
using namespace irr::video;

/**
 * Implementation of IImage with CImg library
 */
class ImageCImg: public IImage
{
public:
	ImageCImg(const IImage& wrapped);
	virtual ~ImageCImg();

	virtual void* lock();

	//! Unlock function.
	/** Should be called after the pointer received by lock() is not
	 needed anymore. */
	virtual void unlock();

	//! Returns width and height of image data.
	virtual const core::dimension2d<u32>& getDimension() const;

	//! Returns bits per pixel.
	virtual u32 getBitsPerPixel() const;

	//! Returns bytes per pixel
	virtual u32 getBytesPerPixel() const;

	//! Returns image data size in bytes
	virtual u32 getImageDataSizeInBytes() const;

	//! Returns image data size in pixels
	virtual u32 getImageDataSizeInPixels() const;

	//! Returns a pixel
	virtual SColor getPixel(u32 x, u32 y) const;

	//! Sets a pixel
	virtual void
	setPixel(u32 x, u32 y, const SColor &color, bool blend = false);

	//! Returns the color format
	virtual ECOLOR_FORMAT getColorFormat() const;

	//! Returns mask for red value of a pixel
	virtual u32 getRedMask() const;

	//! Returns mask for green value of a pixel
	virtual u32 getGreenMask() const;

	//! Returns mask for blue value of a pixel
	virtual u32 getBlueMask() const;

	//! Returns mask for alpha value of a pixel
	virtual u32 getAlphaMask() const;

	//! Returns pitch of image
	virtual u32 getPitch() const;

	//! Copies the image into the target, scaling the image to fit
	virtual void copyToScaling(void* target, u32 width, u32 height,
			ECOLOR_FORMAT format = ECF_A8R8G8B8, u32 pitch);

	//! Copies the image into the target, scaling the image to fit
	virtual void copyToScaling(IImage* target);

	//! copies this surface into another
	virtual void copyTo(IImage* target,
			const core::position2d<s32>& pos = core::position2d<s32>(0, 0));

	//! copies this surface into another
	virtual void copyTo(IImage* target, const core::position2d<s32>& pos,
			const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect =
					0);

	//! copies this surface into another, using the alpha mask, an cliprect and a color to add with
	virtual void copyToWithAlpha(IImage* target,
			const core::position2d<s32>& pos,
			const core::rect<s32>& sourceRect, const SColor &color,
			const core::rect<s32>* clipRect);

	//! copies this surface into another, scaling it to fit, appyling a box filter
	virtual void copyToScalingBoxFilter(IImage* target, s32 bias, bool blend =
			false);

	//! fills the surface with black or white
	virtual void fill(const SColor &color);
private:
	IImage m_wrapped;
};

#endif /* IMAGECIMG_H_ */
