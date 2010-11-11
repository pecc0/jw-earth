/*
 * JWSphere.h
 *
 *  Created on: Oct 18, 2010
 *      Author: Petko
 */

#ifndef EARTHVIS_H_
#define EARTHVIS_H_

#include "ISceneNode.h"
#include <irrlicht.h>
#include "JWTriangle.h"
#include "JWSphere.h"
#include "AutoCleanHashMap.h"

using namespace irr;
using namespace __gnu_cxx;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif



class EarthVisualization: public irr::scene::ISceneNode
{
	jw::JWSphere m_Sphere;

	jw::AutoCleanHashMap<video::S3DVertex> m_mapVerteces;

	int m_nTrCount;
	int m_nCurrentIndex;
	u32 * m_vIndices;

	core::aabbox3d<f32> m_Box;

    video::SMaterial m_Material;

    u32 m_uTriangleUnderUs;
    u32 m_vTriangleUnderUsPoints[3];

    int m_nLevel;

    core::vector3df m_vertViewerPoint;

    void init();
    void clear();
    void addTriangleToMesh(u32 triangle);
public:
    EarthVisualization();
    EarthVisualization(scene::ISceneNode *parent, scene::ISceneManager *mgr, s32 id);
    virtual ~EarthVisualization();
    virtual void OnRegisterSceneNode();
    virtual void render();
    const virtual core::aabbox3d<f32>& getBoundingBox() const;

	virtual u32 getMaterialCount();

	virtual video::SMaterial& getMaterial(u32 i);


	void generateMesh();
	const core::vector3df& getViewerPoint() const;
    void setViewerPoint(const core::vector3df& viewerPoint);
    u32 getUTriangleUnderUs() const;
    void setTriangleUnderUs(u32 m_uTriangleUnderUs);
    void paintVertex(u32 vertexId, video::S3DVertex* v);
};

#endif /* EARTHVIS_H_ */
