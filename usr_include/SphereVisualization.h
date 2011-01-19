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
#include "IJWLogger.h"

using namespace irr;
using namespace __gnu_cxx;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

class SphereVisualization: public irr::scene::ISceneNode
{
	jw::IJWLogger* log;

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

	core::vector3df m_vrtCenter;
	f32 m_fRadius;

	bool m_isMeshGenerated;

	void init();
	void clear();
	void addTriangleToMesh(u32 triangle, int level);
public:
	SphereVisualization(scene::ISceneNode *parent = 0,
			scene::ISceneManager *mgr = 0, s32 id = 0, int level = 2,
			const core::vector3df& center = core::vector3df(0.0f, 0.0f, 0.0f),
			f32 radius = 12742.);
	virtual ~SphereVisualization();
	virtual void OnRegisterSceneNode();
	virtual void render();
	const virtual core::aabbox3d<f32>& getBoundingBox() const;

	virtual u32 getMaterialCount();

	virtual video::SMaterial& getMaterial();

	void generateMesh();
	const core::vector3df& getViewerPoint() const;
	void setViewerPoint(const core::vector3df& viewerPoint);
	u32 getUTriangleUnderUs() const;
	void setTriangleUnderUs(u32 m_uTriangleUnderUs);
	void paintVertex(u32 vertexId, video::S3DVertex* v);
	int getLevel() const;
	void setLevel(int level);

	void setMeshGenerated(bool meshGenerated) {
		m_isMeshGenerated = meshGenerated;
	}

	bool isPointVisibleAtLevel(f32 distance, int level);

	/**
	 *	See
	 *	http://www.euclideanspace.com/maths/geometry/space/coordinates/polar/spherical/index.htm
	 */
	const core::vector2d<f32> getSphericalCoordinates(const core::vector3df& v) const;
};

#endif /* EARTHVIS_H_ */
