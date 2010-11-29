/*
 * JWSphere.cpp
 *
 *  Created on: Oct 18, 2010
 *      Author: Petko
 */

#include "SphereVisualization.h"
#include <cmath>
#include "irrMath.h"

SphereVisualization::SphereVisualization(scene::ISceneNode* parent,
		scene::ISceneManager* mgr, s32 id, int level,
		const core::vector3df& center, f32 radius) :
	scene::ISceneNode(parent, mgr, id), log(jw::LoggerFactory::getLogger(
			"com.jw.EarthVisualization")), m_Sphere(radius), m_nLevel(level),
			m_vrtCenter(center), m_fRadius(radius), m_isMeshGenerated(true)
{

	m_Material.Wireframe = false;
	m_Material.Lighting = false;

	m_mapVerteces = jw::AutoCleanHashMap<video::S3DVertex>(10000);
	m_mapVerteces.init();

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
	m_vIndices = 0;
	//m_nLevel = 14;
	m_uTriangleUnderUs = 1;
	setTriangleUnderUs(JWTriangle::cropToLevel(
			0b01010101010101010101010101010000, m_nLevel));

	//generateMesh();
}

void SphereVisualization::clear()
{
	delete[] m_vIndices;
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

	driver->setMaterial(m_Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	driver->drawVertexPrimitiveList(m_mapVerteces.getPtrPool(),
			m_mapVerteces.capacity(), m_vIndices, m_nTrCount,
			video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_32BIT);

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
		const core::vector3df& v) const
{
	return core::vector2d<f32>(atan2(v.Z, v.X) / (core::PI * 2), acos(v.Y
			/ m_fRadius) / (core::PI));
}

void SphereVisualization::addTriangleToMesh(u32 triangle, int level)
{
	for (int j = 2; j >= 0; --j)
	{
		u32 vertexId = m_Sphere.getTriangleVertex(triangle, level, j, false);

		int hash = m_mapVerteces.hash(vertexId);
		if (m_mapVerteces.getPtrKeys()[hash] == EMPTY_KEY)
		{
			core::vector3df* ptrVertPos = m_Sphere.getVertex(vertexId);
			video::S3DVertex vert(*ptrVertPos + m_vrtCenter, *ptrVertPos,
					video::SColor(255, 200, 200, 200), getSphericalCoordinates(
							*ptrVertPos));
			vert.Normal.normalize();
			//paintVertex(vertexId, &vert);
			m_mapVerteces.put(vertexId, &vert);
			//log->debug("%f,%f", vert.TCoords.X, vert.TCoords.Y);
		}
		video::S3DVertex* v = m_mapVerteces.getPtrPool() + hash;
		if (v->TCoords.X == 0.5 && !(triangle & 0b11))
		{
			//The border case - the X Tcoordinate of some of the other triangle points is negative.
			//Will add the same point but with X Tcoordinate = -0.5, so set the first bit (which is not used) of the ID
			vertexId |= 0x80000000;
			hash = m_mapVerteces.hash(vertexId);
			if (m_mapVerteces.getPtrKeys()[hash] == EMPTY_KEY)
			{
				m_mapVerteces.put(vertexId, v);
			}
			v = m_mapVerteces.getPtrPool() + hash;
			v->TCoords.X = -0.5;
			//remove the first bit to work for painting
			vertexId &= 0x7FFFFFFF;
		}
		paintVertex(vertexId, v);

		m_vIndices[m_nCurrentIndex++] = hash;
	}
}

void SphereVisualization::generateMesh()
{
	if (!m_isMeshGenerated)
	{
		return;
	}

	clear();

	/*m_nTrCount = m_Sphere.getTilesSquare(JWTriangle::getParentTriangle(
	 0b01010101010101010101010101010000, m_nLevel + 1), m_nLevel, 16,
	 36, 8, 18, g_TrianglesBuf);
	 if (m_nTrCount > 3000)
	 {
	 return;
	 }
	 */
	m_nTrCount = 0;

	m_vIndices = new u32[2000 * 3];

	m_nCurrentIndex = 0;
	/*
	 for (int i = 0; i < m_nTrCount; i++)
	 {
	 addTriangleToMesh(g_TrianglesBuf[i]);
	 }
	 */

	int level = MAX_TRIANGLE_LEVELS;
	jw::BFSIterator* i = m_Sphere.bfs(m_uTriangleUnderUs, level);
	u32 triangle = m_uTriangleUnderUs;
	core::vector3df startPt = m_vertViewerPoint;
	//startPt.setLength(m_fRadius);
	//log->debug("generation started");
	bool addingStarted = false;
	while (m_nTrCount < 2000)
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
			m_nTrCount++;
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
				do
				{
					addTriangleToMesh(triangle, level + 1);
					m_nTrCount++;
					if (m_nTrCount >= 2000)
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
							m_nTrCount++;
							if (m_nTrCount >= 2000)
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
					if (m_nTrCount >= 2000)
					{
						break;
					}

					if (!newIterator->isUsed(triangle))
					{
						newIterator->setUsed(triangle);
						newIterator->accept(triangle);
					}

				} while (i->next(&triangle));

				set<u32>::iterator setiter;
				for (setiter = i->getUsedSet()->begin(); setiter
						!= i->getUsedSet()->end(); ++setiter)
				{
					newIterator->setUsed(JWTriangle::cropToLevel(*setiter,
							level));
				}

				i->drop();
				i = newIterator;
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

void SphereVisualization::paintVertex(u32 vertexId, video::S3DVertex *v)
{
	v->Color = m_Material.Wireframe ? video::SColor(255, 0, 255, 0)
			: video::SColor(255, 200, 200, 200);
	for (int i = 0; i < 3; i++)
	{
		if (m_vTriangleUnderUsPoints[i] == vertexId)
		{
			v->Color = m_Material.Wireframe ? video::SColor(255, 255, 255, 0)
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
}

