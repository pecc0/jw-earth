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
 * JWSphere.cpp
 *
 *  Created on: Oct 18, 2010
 *      Author: Petko
 */

#include "SphereVisualization.h"
#include "GoogleTextureProvider.h"
#include <cmath>
#include "irrMath.h"

SphereVisualization::SphereVisualization(scene::ISceneNode* parent,
		scene::ISceneManager* mgr, video::IVideoDriver* driver, s32 id,
		int level, const core::vector3df& center, f32 radius) :
	scene::ISceneNode(parent, mgr, id), log(jw::LoggerFactory::getLogger(
			"com.jw.EarthVisualization")), m_Sphere(radius), m_nLevel(level),
			m_vrtCenter(center), m_fRadius(radius), m_isMeshGenerated(true),
			m_Texture(0), m_Driver(driver)
{

	getMaterial().Wireframe = false;
	getMaterial().Lighting = false;

	m_mapVerteces[0] = jw::AutoCleanHashMap<video::S3DVertex>(10000);
	m_mapVerteces[0].init();

	for (int i = FIRST_DYNAMIC_LEVEL; i <= MAX_TRIANGLE_LEVELS; i++)
	{
		m_mapVerteces[i] = jw::AutoCleanHashMap<video::S3DVertex>(4000);
		m_mapVerteces[i].init();
	}

	init();

	/*
	 The Irrlicht Engine needs to know the bounding box of a scene node.
	 It will use it for automatic culling and other things. Hence, we
	 need to create a bounding box from the 4 vertices we use.
	 If you do not want the engine to use the box for automatic culling,
	 and/or don't want to create the box, you could also call
	 irr::scene::ISceneNode::setAutomaticCulling() with irr::scene::EAC_OFF.
	 */

	m_Box.reset(*(m_Sphere.getVertex(0)));
	for (s32 i = 1; i < 8; ++i)
	{
		core::vector3df* v = m_Sphere.getVertex(i);
		if (v != NULL)
		{
			m_Box.addInternalPoint(*v);
		}
	}
}

void SphereVisualization::init()
{
	for (int i = 0; i <= MAX_TRIANGLE_LEVELS; i++)
	{
		m_vIndices[i] = 0;
	}
	//m_nLevel = 14;
	m_uTriangleUnderUs = 1;
	setTriangleUnderUs(JWTriangle::cropToLevel(
			0b01010101010101010101010101010000, m_nLevel));

	//generateMesh();
}

void SphereVisualization::clear()
{
	for (int i = 0; i <= MAX_TRIANGLE_LEVELS; i++)
	{
		delete[] m_vIndices[i];
		m_vIndices[i] = 0;
		textureBinds[i].LowerRightCorner.X = UNINITED_TEXTURE_COORDINATES;
	}

}

SphereVisualization::~SphereVisualization()
{
	clear();
}

void SphereVisualization::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

u32 g_TrianglesBuf[3000];

void SphereVisualization::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->setMaterial(getMaterial());

	driver->drawVertexPrimitiveList(
			getVerticesMap(STATIC_LEVEL_ID)->getPtrPool(), getVerticesMap(
					STATIC_LEVEL_ID)->capacity(), getIndices(STATIC_LEVEL_ID),
			getTrCount(STATIC_LEVEL_ID), video::EVT_STANDARD,
			scene::EPT_TRIANGLES, video::EIT_32BIT);
	for (int level = FIRST_DYNAMIC_LEVEL; level <= m_nLevel; level++)
	{
		driver->drawVertexPrimitiveList(getVerticesMap(level)->getPtrPool(),
				getVerticesMap(level)->capacity(), getIndices(level),
				getTrCount(level), video::EVT_STANDARD, scene::EPT_TRIANGLES,
				video::EIT_32BIT);
	}
}

const core::aabbox3d<f32>& SphereVisualization::getBoundingBox() const
{
	return m_Box;
}

u32 SphereVisualization::getMaterialCount()
{
	return 1;
}

video::SMaterial& SphereVisualization::getMaterial()
{
	return m_Material;
}

const core::vector2d<f32> SphereVisualization::getSphericalCoordinates(
		const core::vector3df& normalVector) const
{
	return core::vector2d<f32>(atan2(normalVector.Z, normalVector.X)
			/ (core::PI * 2), acos(normalVector.Y) / (core::PI));
}

void SphereVisualization::addTriangleToMesh(u32 triangle, int level)
{
	irr::core::rect<f32>* binds = &(textureBinds[level]);
	for (int j = 2; j >= 0; --j)
	{
		u32 vertexId = m_Sphere.getTriangleVertex(triangle, level, j, false);

		jw::AutoCleanHashMap<video::S3DVertex> * currentLevelVerticesMap =
				getVerticesMap(level);

		int hash = currentLevelVerticesMap->hash(vertexId);
		if (currentLevelVerticesMap->getPtrKeys()[hash] == EMPTY_KEY)
		{
			video::S3DVertex vert;

			video::S3DVertex* staticVertex = 0;
			if (level >= FIRST_DYNAMIC_LEVEL)
			{
				jw::AutoCleanHashMap<video::S3DVertex> * staticVerticesMap =
						getVerticesMap(STATIC_LEVEL_ID);
				staticVertex = staticVerticesMap->get(vertexId);
			}

			if (staticVertex)
			{
				vert = *staticVertex;
			}
			else
			{
				core::vector3df* ptrVertPos = m_Sphere.getVertex(vertexId);
				vert.Pos = *ptrVertPos + m_vrtCenter;
				vert.Normal = *ptrVertPos;
				vert.Normal.normalize();
			}

			vert.TCoords = getSphericalCoordinates(vert.Normal);

			currentLevelVerticesMap->put(vertexId, &vert);
		}
		video::S3DVertex* v = currentLevelVerticesMap->getPtrPool() + hash;

		if (level >= FIRST_DYNAMIC_LEVEL)
		{
			if (binds->LowerRightCorner.X == UNINITED_TEXTURE_COORDINATES)
			{
				binds->LowerRightCorner = v->TCoords;
				binds->UpperLeftCorner = v->TCoords;
			}
			else
			{
				core::vector2df tCoords = v->TCoords;
				if (binds->LowerRightCorner.X < -0.25 && tCoords.X > 0.25)
				{
					binds->LowerRightCorner.X += 1;
					binds->UpperLeftCorner.X += 1;
				}
				else if (binds->UpperLeftCorner.X > 0.25 && tCoords.X < -0.25)
				{
					tCoords.X += 1;
				}

				binds->addInternalPoint(tCoords);
			}
		}

		if (v->TCoords.X == 0.5 && !(triangle & 0b11))
		{
			//The border case - the X Tcoordinate of some of the other triangle points is negative.
			//Will add the same point but with X Tcoordinate = -0.5, so set the first bit (which is not used) of the ID
			vertexId |= 0x80000000;
			hash = currentLevelVerticesMap->hash(vertexId);
			if (currentLevelVerticesMap->getPtrKeys()[hash] == EMPTY_KEY)
			{
				currentLevelVerticesMap->put(vertexId, v);
			}
			v = currentLevelVerticesMap->getPtrPool() + hash;
			v->TCoords.X = -0.5;
			//remove the first bit to work for painting
			vertexId &= 0x7FFFFFFF;
		}
		paintVertex(vertexId, v, level);

		getIndices(level)[incCurrentIndex(level)] = hash;
	}
}

void SphereVisualization::generateMesh()
{
	if (!m_isMeshGenerated)
	{
		return;
	}

	clear();

	setTrCount(0);

	zeroCurrentIndex();

	int level = MAX_TRIANGLE_LEVELS;
	jw::BFSIterator* i = m_Sphere.bfs(m_uTriangleUnderUs, level);
	u32 triangle = m_uTriangleUnderUs;
	core::vector3df startPt = m_vertViewerPoint;
	//startPt.setLength(m_fRadius);
	//log->debug("generation started");
	bool addingStarted = false;
	while (getTotalTrCount() < 2000)
	{
		u32 vertexId = m_Sphere.getTriangleVertex(triangle, level, 0, false);
		core::vector3df* ptrVertPos = m_Sphere.getVertex(vertexId);
		if (isPointVisibleAtLevel(ptrVertPos->getDistanceFrom(startPt), level))
		{
			if (!addingStarted)
			{
				m_nLevel = level;
				//log->debug("addingStarted %d", level);
			}
			addingStarted = true;
			addTriangleToMesh(triangle, level);
			i->accept(triangle);

			if (!i->next(&triangle))
			{
				break;
			}
			incTrCount(level);
		}
		else
		{
			--level;
			if (level < 0)
			{
				break;
			}

			if (addingStarted)
			{
				//log->debug("New level %d", level);
				//i = m_Sphere.bfs(i, level);
				jw::BFSIterator* newIterator = new jw::BFSIterator(&m_Sphere,
						level);

				//u32 remain = triangle;
				set<u32>::iterator setiter;
				for (setiter = i->getUsedSet()->begin(); setiter
						!= i->getUsedSet()->end(); ++setiter)
				{
					newIterator->setUsed(JWTriangle::cropToLevel(*setiter,
							level));
				}

				//Empty the queue of the current BFS
				do
				{
					addTriangleToMesh(triangle, level + 1);
					if (incTrCount(level + 1) >= 2000)
					{
						break;
					}
					i->setUsed(triangle);
					//core::stringc str;
					//str.printBinary(triangle, 32, L'0', L'1');
					//log->debug("++%s", str.c_str());
					triangle = JWTriangle::cropToLevel(triangle, level);
					for (int subtr = 0; subtr < 4; subtr++)
					{
						u32 childTr = JWTriangle::getChildIndex(triangle,
								subtr, level);
						//str = "";
						//str.printBinary(childTr, 32, L'0', L'1');
						if (!i->isUsed(childTr))
						{

							//log->debug(" +%s", str.c_str());
							addTriangleToMesh(childTr, level + 1);
							if (incTrCount(level + 1) >= 2000)
							{
								break;
							}
							i->setUsed(childTr);
						}
						else
						{
							//log->debug(" -%s", str.c_str());
						}
					}
					if (getTotalTrCount() >= 2000)
					{
						break;
					}

					newIterator->accept(triangle);

				} while (i->next(&triangle));

				i->drop();
				i = newIterator;

				if (!i->next(&triangle))
				{
					break;
				}
			}
			else
			{
				triangle = JWTriangle::cropToLevel(triangle, level);
				i->drop();
				i = m_Sphere.bfs(triangle, level);
			}
		}
	}

	i->drop();
}

const core::vector3df& SphereVisualization::getViewerPoint() const
{
	return m_vertViewerPoint;
}

bool SphereVisualization::isPointVisibleAtLevel(f32 distance, int level)
{
	return distance < 200000 >> level;
}

void SphereVisualization::setViewerPoint(const core::vector3df& viewerPoint)
{
	this->m_vertViewerPoint = viewerPoint - m_vrtCenter; //translate to sphere coordinates - center is at 0,0,0

	int level;
	//int level = (m_vertViewerPoint.getLength() - m_fRadius)
	f32 distanceToGround = m_vertViewerPoint.getLength() - m_fRadius;
	//TODO calculate without loop
	for (level = MAX_TRIANGLE_LEVELS; level >= 0; --level)
	{
		if (isPointVisibleAtLevel(distanceToGround, level))
		{
			break;
		}
	}

	u32 triangleUnderUs = m_Sphere.getTriangleUnderPoint(level,
			m_vertViewerPoint);

	if (triangleUnderUs != m_uTriangleUnderUs || m_nLevel != level)
	{
		setTriangleUnderUs(triangleUnderUs);
		for (int i = 0; i < 3; i++)
		{
			m_vTriangleUnderUsPoints[i] = m_Sphere.getTriangleVertex(
					m_uTriangleUnderUs, level, i, false);
		}
		generateMesh();
		reloadTexture();
	}

}

u32 SphereVisualization::getUTriangleUnderUs() const
{
	return m_uTriangleUnderUs;
}

void SphereVisualization::setTriangleUnderUs(u32 triangleUnderUs)
{
	m_uTriangleUnderUs = triangleUnderUs;
}

void SphereVisualization::paintVertex(u32 vertexId, video::S3DVertex *v,
		int level)
{
	v->Color = getMaterial().Wireframe ? //
	(level < FIRST_DYNAMIC_LEVEL || level % 2 == 1 ? //
	video::SColor(255, 0, 255, 0)
			: video::SColor(255, 255, 0, 0))
			: video::SColor(255, 200, 200, 200);
	for (int i = 0; i < 3; i++)
	{
		if (m_vTriangleUnderUsPoints[i] == vertexId)
		{
			v->Color
					= getMaterial().Wireframe ? video::SColor(255, 255, 255, 0)
							: video::SColor(255, 255, 255, 255);
			break;
		}
	}
}

int SphereVisualization::getLevel() const
{
	return m_nLevel;
}

void SphereVisualization::setLevel(int level)
{
	this->m_nLevel = level;
	m_uTriangleUnderUs = JWTriangle::cropToLevel(m_uTriangleUnderUs, level);
	generateMesh();
	reloadTexture();
}

void SphereVisualization::setWireframe(bool wireFrame)
{
	getMaterial().Wireframe = wireFrame;
	getMaterial().setTexture(0, wireFrame ? 0 : m_Texture);
}

void SphereVisualization::reloadTexture()
{
	if (m_Texture)
	{
		return;
		m_Driver->removeTexture(m_Texture);
		//m_Texture->drop();
	}
	GoogleTextureProvider provider;
	m_Texture = m_Driver->getTexture("media/earth.jpg");
	getMaterial().setTexture(0, m_Texture);
	//provider.
}

void SphereVisualization::receiveTexture(video::ITexture* texture)
{

}
