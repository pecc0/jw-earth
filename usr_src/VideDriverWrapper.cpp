/*
 * VideDriverWrapper.cpp
 *
 *  Created on: Feb 25, 2011
 *      Author: Petko
 */

#include "irrlitch/VideDriverWrapper.h"

//0 params	^[^\(]+VideDriverWrapper::([\w]+)\(\)[^\{]*\{
//1 param	^[^\(]+VideDriverWrapper::([\w]+)\([^,\)]*[ \*]+([\w]+)\)[^\{]*\{
//2 params	^[^\(]+VideDriverWrapper::([\w]+)\([^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+)\)[^\{]*\{
//3 params	^[^\(]+VideDriverWrapper::([\w]+)\([^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+)\)[^\{]*\{
//4 params	^[^\(]+VideDriverWrapper::([\w]+)\([^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+)\)[^\{]*\{
//5 params	^[^\(]+VideDriverWrapper::([\w]+)\([^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+)\)[^\{]*\{
//6 params	^[^\(]+VideDriverWrapper::([\w]+)\([^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+)\)[^\{]*\{
//7 params	^[^\(]+VideDriverWrapper::([\w]+)\([^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+)\)[^\{]*\{
//8 params	^[^\(]+VideDriverWrapper::([\w]+)\([^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+), [^,\)]*[ \*]+([\w]+)\)[^\{]*\{

//0 params	\0 return m_wrapped->\1();
//1 param	\0 return m_wrapped->\1(\2);
//2 params	\0 return m_wrapped->\1(\2, \3);
//3 params	\0 return m_wrapped->\1(\2, \3, \4);
//4 params	\0 return m_wrapped->\1(\2, \3, \4, \5);
//5 params	\0 return m_wrapped->\1(\2, \3, \4, \5, \6);
//6 params	\0 return m_wrapped->\1(\2, \3, \4, \5, \6, \7);
//7 params	\0 return m_wrapped->\1(\2, \3, \4, \5, \6, \7, \8);
//8 params	\0 return m_wrapped->\1(\2, \3, \4, \5, \6, \7, \8, \9);

void VideDriverWrapper::setWrapped(video::IVideoDriver * wrapped) {
	m_wrapped = wrapped;
}

IImage *VideDriverWrapper::createImageFromFile(io::IReadFile *file) {
	return m_wrapped->createImageFromFile(file);
}

io::IAttributes *VideDriverWrapper::createAttributesFromMaterial(const video::SMaterial & material) {
	return m_wrapped->createAttributesFromMaterial(material);
}

const wchar_t *VideDriverWrapper::getName() const {
	return m_wrapped->getName();
}

void VideDriverWrapper::draw2DVertexPrimitiveList(const void *vertices, u32 vertexCount, const void *indexList, u32 primCount, E_VERTEX_TYPE vType, scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType) {
	return m_wrapped->draw2DVertexPrimitiveList(vertices, vertexCount, indexList, primCount, vType, pType, iType);
}

bool VideDriverWrapper::writeImageToFile(IImage *image, const io::path & filename, u32 param) {
	return m_wrapped->writeImageToFile(image, filename, param);
}

void VideDriverWrapper::enableClipPlane(u32 index, bool enable) {
	return m_wrapped->enableClipPlane(index, enable);
}

void VideDriverWrapper::draw2DPolygon(core::position2d<s32> center, f32 radius, video::SColor color, s32 vertexCount) {
	return m_wrapped->draw2DPolygon(center, radius, color, vertexCount);
}

void VideDriverWrapper::draw2DImage(const video::ITexture *texture, const core::position2d<s32> & destPos) {
	return m_wrapped->draw2DImage(texture, destPos);
}

IImage *VideDriverWrapper::createImage(IImage *imageToCopy, const core::position2d<s32> & pos, const core::dimension2d<u32> & size) {
	return m_wrapped->createImage(imageToCopy, pos, size);
}

void VideDriverWrapper::addExternalImageLoader(IImageLoader *loader) {
	return m_wrapped->addExternalImageLoader(loader);
}

bool VideDriverWrapper::getTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag) const {
	return m_wrapped->getTextureCreationFlag(flag);
}

ITexture *VideDriverWrapper::getTexture(const io::path & filename) {
	return m_wrapped->getTexture(filename);
}

void VideDriverWrapper::makeColorKeyTexture(video::ITexture *texture, video::SColor color, bool zeroTexels) const {
	return m_wrapped->makeColorKeyTexture(texture, color, zeroTexels);
}

void VideDriverWrapper::removeTexture(ITexture *texture) {
	return m_wrapped->removeTexture(texture);
}

bool VideDriverWrapper::setRenderTarget(E_RENDER_TARGET target, bool clearTarget, bool clearZBuffer, SColor color) {
	return m_wrapped->setRenderTarget(target, clearTarget, clearZBuffer, color);
}

void VideDriverWrapper::draw2DRectangleOutline(const core::recti & pos, SColor color) {
	return m_wrapped->draw2DRectangleOutline(pos, color);
}

void VideDriverWrapper::removeAllTextures() {
	return m_wrapped->removeAllTextures();
}

u32 VideDriverWrapper::getPrimitiveCountDrawn(u32 mode) const {
	return m_wrapped->getPrimitiveCountDrawn(mode);
}

const core::dimension2d<u32> &VideDriverWrapper::getScreenSize() const {
	return m_wrapped->getScreenSize();
}

void VideDriverWrapper::draw2DImage(const video::ITexture *texture, const core::rect<s32> & destRect, const core::rect<s32> & sourceRect, const core::rect<s32> *clipRect, const video::SColor * const colors, bool useAlphaChannelOfTexture) {
	return m_wrapped->draw2DImage(texture, destRect, sourceRect, clipRect, colors, useAlphaChannelOfTexture);
}

const core::dimension2d<u32> &VideDriverWrapper::getCurrentRenderTargetSize() const {
	return m_wrapped->getCurrentRenderTargetSize();
}

IImage *VideDriverWrapper::createImage(ITexture *texture, const core::position2d<s32> & pos, const core::dimension2d<u32> & size) {
	return m_wrapped->createImage(texture, pos, size);
}

void VideDriverWrapper::draw2DImageBatch(const video::ITexture *texture, const core::array<core::position2d<s32> > & positions, const core::array<core::rect<s32> > & sourceRects, const core::rect<s32> *clipRect, SColor color, bool useAlphaChannelOfTexture) {
	return m_wrapped->draw2DImageBatch(texture, positions, sourceRects, clipRect, color, useAlphaChannelOfTexture);
}

void VideDriverWrapper::drawStencilShadowVolume(const core::vector3df *triangles, s32 count, bool zfail) {
	return m_wrapped->drawStencilShadowVolume(triangles, count, zfail);
}

void VideDriverWrapper::removeAllHardwareBuffers() {
	return m_wrapped->removeAllHardwareBuffers();
}

void VideDriverWrapper::drawVertexPrimitiveList(const void *vertices, u32 vertexCount, const void *indexList, u32 primCount, E_VERTEX_TYPE vType, scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType) {
	return m_wrapped->drawVertexPrimitiveList(vertices, vertexCount, indexList, primCount, vType, pType, iType);
}

bool VideDriverWrapper::checkDriverReset() {
	return m_wrapped->checkDriverReset();
}

s32 VideDriverWrapper::getFPS() const {
	return m_wrapped->getFPS();
}

u32 VideDriverWrapper::getImageLoaderCount() const {
	return m_wrapped->getImageLoaderCount();
}

IImageWriter *VideDriverWrapper::getImageWriter(u32 n) {
	return m_wrapped->getImageWriter(n);
}

ITexture *VideDriverWrapper::getTextureByIndex(u32 index) {
	return m_wrapped->getTextureByIndex(index);
}

IImage *VideDriverWrapper::createImage(ECOLOR_FORMAT format, const core::dimension2d<u32> & size) {
	return m_wrapped->createImage(format, size);
}

void VideDriverWrapper::draw3DBox(const core::aabbox3d<f32> & box, SColor color) {
	return m_wrapped->draw3DBox(box, color);
}

void VideDriverWrapper::addExternalImageWriter(IImageWriter *writer) {
	return m_wrapped->addExternalImageWriter(writer);
}

void VideDriverWrapper::setFog(SColor color, E_FOG_TYPE fogType, f32 start, f32 end, f32 density, bool pixelFog, bool rangeFog) {
	return m_wrapped->setFog(color, fogType, start, end, density, pixelFog, rangeFog);
}

ITexture *VideDriverWrapper::addTexture(const core::dimension2d<u32> & size, const io::path & name, ECOLOR_FORMAT format) {
	return m_wrapped->addTexture(size, name, format);
}

bool VideDriverWrapper::setRenderTarget(video::ITexture *texture, bool clearBackBuffer, bool clearZBuffer, SColor color) {
	return m_wrapped->setRenderTarget(texture, clearBackBuffer, clearZBuffer, color);
}

bool VideDriverWrapper::setRenderTarget(const core::array<video::IRenderTarget> & texture, bool clearBackBuffer, bool clearZBuffer, SColor color) {
	return m_wrapped->setRenderTarget(texture, clearBackBuffer, clearZBuffer, color);
}

void VideDriverWrapper::OnResize(const core::dimension2d<u32> & size) {
	return m_wrapped->OnResize(size);
}

const core::rect<s32> &VideDriverWrapper::getViewPort() const {
	return m_wrapped->getViewPort();
}

void VideDriverWrapper::getFog(SColor & color, E_FOG_TYPE & fogType, f32 & start, f32 & end, f32 & density, bool & pixelFog, bool & rangeFog) {
	return m_wrapped->getFog(color, fogType, start, end, density, pixelFog, rangeFog);
}

void VideDriverWrapper::fillMaterialStructureFromAttributes(video::SMaterial & outMaterial, io::IAttributes *attributes) {
	return m_wrapped->fillMaterialStructureFromAttributes(outMaterial, attributes);
}

void VideDriverWrapper::draw3DTriangle(const core::triangle3df & triangle, SColor color) {
	return m_wrapped->draw3DTriangle(triangle, color);
}

E_DRIVER_TYPE VideDriverWrapper::getDriverType() const {
	return m_wrapped->getDriverType();
}

void VideDriverWrapper::makeColorKeyTexture(video::ITexture *texture, core::position2d<s32> colorKeyPixelPos, bool zeroTexels) const {
	return m_wrapped->makeColorKeyTexture(texture, colorKeyPixelPos, zeroTexels);
}

void VideDriverWrapper::drawStencilShadow(bool clearStencilBuffer, video::SColor leftUpEdge, video::SColor rightUpEdge, video::SColor leftDownEdge, video::SColor rightDownEdge) {
	return m_wrapped->drawStencilShadow(clearStencilBuffer, leftUpEdge, rightUpEdge, leftDownEdge, rightDownEdge);
}

void VideDriverWrapper::disableFeature(E_VIDEO_DRIVER_FEATURE feature, bool flag) {
	return m_wrapped->disableFeature(feature, flag);
}

bool VideDriverWrapper::setClipPlane(u32 index, const core::plane3df & plane, bool enable) {
	return m_wrapped->setClipPlane(index, plane, enable);
}

void VideDriverWrapper::renameTexture(ITexture *texture, const io::path & newName) {
	return m_wrapped->renameTexture(texture, newName);
}

void VideDriverWrapper::setAllowZWriteOnTransparent(bool flag) {
	return m_wrapped->setAllowZWriteOnTransparent(flag);
}

const c8 *VideDriverWrapper::getMaterialRendererName(u32 idx) const {
	return m_wrapped->getMaterialRendererName(idx);
}

u32 VideDriverWrapper::getMaterialRendererCount() const {
	return m_wrapped->getMaterialRendererCount();
}

u32 VideDriverWrapper::getDynamicLightCount() const {
	return m_wrapped->getDynamicLightCount();
}

IImage *VideDriverWrapper::createScreenShot() {
	return m_wrapped->createScreenShot();
}

void VideDriverWrapper::makeNormalMapTexture(video::ITexture *texture, f32 amplitude) const {
	return m_wrapped->makeNormalMapTexture(texture, amplitude);
}

s32 VideDriverWrapper::addDynamicLight(const SLight & light) {
	return m_wrapped->addDynamicLight(light);
}

void VideDriverWrapper::drawMeshBuffer(const scene::IMeshBuffer *mb) {
	return m_wrapped->drawMeshBuffer(mb);
}

void VideDriverWrapper::clearZBuffer() {
	return m_wrapped->clearZBuffer();
}

ECOLOR_FORMAT VideDriverWrapper::getColorFormat() const {
	return m_wrapped->getColorFormat();
}

void VideDriverWrapper::draw2DImage(const video::ITexture *texture, const core::position2d<s32> & destPos, const core::rect<s32> & sourceRect, const core::rect<s32> *clipRect, SColor color, bool useAlphaChannelOfTexture) {
	return m_wrapped->draw2DImage(texture, destPos, sourceRect, clipRect, color, useAlphaChannelOfTexture);
}

void VideDriverWrapper::setMinHardwareBufferVertexCount(u32 count) {
	return m_wrapped->setMinHardwareBufferVertexCount(count);
}

const SLight &VideDriverWrapper::getDynamicLight(u32 idx) const {
	return m_wrapped->getDynamicLight(idx);
}

u32 VideDriverWrapper::getMaximalDynamicLightAmount() const {
	return m_wrapped->getMaximalDynamicLightAmount();
}

void VideDriverWrapper::setMaterial(const SMaterial & material) {
	return m_wrapped->setMaterial(material);
}

video::ITexture *VideDriverWrapper::findTexture(const io::path & filename) {
	return m_wrapped->findTexture(filename);
}

void VideDriverWrapper::deleteAllDynamicLights() {
	return m_wrapped->deleteAllDynamicLights();
}

void VideDriverWrapper::draw3DLine(const core::vector3df & start, const core::vector3df & end, SColor color) {
	return m_wrapped->draw3DLine(start, end, color);
}

bool VideDriverWrapper::writeImageToFile(IImage *image, io::IWriteFile *file, u32 param) {
	return m_wrapped->writeImageToFile(image, file, param);
}

bool VideDriverWrapper::queryFeature(E_VIDEO_DRIVER_FEATURE feature) const {
	return m_wrapped->queryFeature(feature);
}

IMaterialRenderer *VideDriverWrapper::getMaterialRenderer(u32 idx) {
	return m_wrapped->getMaterialRenderer(idx);
}

bool VideDriverWrapper::beginScene(bool backBuffer, bool zBuffer, SColor color, const SExposedVideoData & videoData, core::rect<s32> *sourceRect) {
	return m_wrapped->beginScene(backBuffer, zBuffer, color, videoData, sourceRect);
}

void VideDriverWrapper::setTransform(E_TRANSFORMATION_STATE state, const core::matrix4 & mat) {
	return m_wrapped->setTransform(state, mat);
}

const SExposedVideoData &VideDriverWrapper::getExposedVideoData() {
	return m_wrapped->getExposedVideoData();
}

ITexture *VideDriverWrapper::addTexture(const io::path & name, IImage *image, void *mipmapData) {
	return m_wrapped->addTexture(name, image, mipmapData);
}

ITexture *VideDriverWrapper::addRenderTargetTexture(const core::dimension2d<u32> & size, const io::path & name, const ECOLOR_FORMAT format) {
	return m_wrapped->addRenderTargetTexture(size, name, format);
}

void VideDriverWrapper::removeHardwareBuffer(const scene::IMeshBuffer *mb) {
	return m_wrapped->removeHardwareBuffer(mb);
}

const core::matrix4 &VideDriverWrapper::getTransform(E_TRANSFORMATION_STATE state) const {
	return m_wrapped->getTransform(state);
}

void VideDriverWrapper::turnLightOn(s32 lightIndex, bool turnOn) {
	return m_wrapped->turnLightOn(lightIndex, turnOn);
}

bool VideDriverWrapper::endScene() {
	return m_wrapped->endScene();
}

core::stringc VideDriverWrapper::getVendorInfo() {
	return m_wrapped->getVendorInfo();
}

u32 VideDriverWrapper::getTextureCount() const {
	return m_wrapped->getTextureCount();
}

IImage *VideDriverWrapper::createImageFromData(ECOLOR_FORMAT format, const core::dimension2d<u32> & size, void *data, bool ownForeignMemory, bool deleteMemory) {
	return m_wrapped->createImageFromData(format, size, data, ownForeignMemory, deleteMemory);
}

void VideDriverWrapper::drawPixel(u32 x, u32 y, const SColor & color) {
	return m_wrapped->drawPixel(x, y, color);
}

IImageLoader *VideDriverWrapper::getImageLoader(u32 n) {
	return m_wrapped->getImageLoader(n);
}

SOverrideMaterial &VideDriverWrapper::getOverrideMaterial() {
	return m_wrapped->getOverrideMaterial();
}

void VideDriverWrapper::enableMaterial2D(bool enable) {
	return m_wrapped->enableMaterial2D(enable);
}

scene::IMeshManipulator *VideDriverWrapper::getMeshManipulator() {
	return m_wrapped->getMeshManipulator();
}

IImage *VideDriverWrapper::createImageFromFile(const io::path & filename) {
	return m_wrapped->createImageFromFile(filename);
}

u32 VideDriverWrapper::getImageWriterCount() const {
	return m_wrapped->getImageWriterCount();
}

void VideDriverWrapper::draw2DImageBatch(const video::ITexture *texture, const core::position2d<s32> & pos, const core::array<core::rect<s32> > & sourceRects, const core::array<s32> & indices, s32 kerningWidth, const core::rect<s32> *clipRect, SColor color, bool useAlphaChannelOfTexture) {
	return m_wrapped->draw2DImageBatch(texture, pos, sourceRects, indices, kerningWidth, clipRect, color, useAlphaChannelOfTexture);
}

void VideDriverWrapper::draw2DLine(const core::position2d<s32> & start, const core::position2d<s32> & end, SColor color) {
	return m_wrapped->draw2DLine(start, end, color);
}

u32 VideDriverWrapper::getMaximalPrimitiveCount() const {
	return m_wrapped->getMaximalPrimitiveCount();
}

void VideDriverWrapper::draw2DRectangle(SColor color, const core::rect<s32> & pos, const core::rect<s32> *clip) {
	return m_wrapped->draw2DRectangle(color, pos, clip);
}

s32 VideDriverWrapper::addMaterialRenderer(IMaterialRenderer *renderer, const c8 *name) {
	return m_wrapped->addMaterialRenderer(renderer, name);
}

core::dimension2du VideDriverWrapper::getMaxTextureSize() const {
	return m_wrapped->getMaxTextureSize();
}

void VideDriverWrapper::setTextureCreationFlag(E_TEXTURE_CREATION_FLAG flag, bool enabled) {
	return m_wrapped->setTextureCreationFlag(flag, enabled);
}

void VideDriverWrapper::setMaterialRendererName(s32 idx, const c8 *name) {
	return m_wrapped->setMaterialRendererName(idx, name);
}

SMaterial &VideDriverWrapper::getMaterial2D() {
	return m_wrapped->getMaterial2D();
}

void VideDriverWrapper::setViewPort(const core::rect<s32> & area) {
	return m_wrapped->setViewPort(area);
}

void VideDriverWrapper::setAmbientLight(const SColorf & color) {
	return m_wrapped->setAmbientLight(color);
}

IImage *VideDriverWrapper::createImage(ECOLOR_FORMAT format, IImage *imageToCopy) {
	return m_wrapped->createImage(format, imageToCopy);
}

ITexture *VideDriverWrapper::getTexture(io::IReadFile *file) {
	return m_wrapped->getTexture(file);
}

void VideDriverWrapper::draw2DRectangle(const core::rect<s32> & pos, SColor colorLeftUp, SColor colorRightUp, SColor colorLeftDown, SColor colorRightDown, const core::rect<s32> *clip) {
	return m_wrapped->draw2DRectangle(pos, colorLeftUp, colorRightUp, colorLeftDown, colorRightDown, clip);
}

IGPUProgrammingServices *VideDriverWrapper::getGPUProgrammingServices() {
	return m_wrapped->getGPUProgrammingServices();
}

