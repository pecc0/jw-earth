/*
 * JWSphere.cpp
 *
 *  Created on: Oct 18, 2010
 *      Author: Petko
 */

#include "EarthVisualization.h"

EarthVisualization::EarthVisualization(scene::ISceneNode* parent,
		scene::ISceneManager* mgr, s32 id, int level,
		const core::vector3df& center, f32 radius) :
	scene::ISceneNode(parent, mgr, id), m_Sphere(radius), m_nLevel(level),
			m_vrtCenter(center), m_fRadius(radius)
{

	m_Material.Wireframe = true;
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

void EarthVisualization::init()
{
	m_vIndices = 0;
	//m_nLevel = 14;
	m_uTriangleUnderUs = 1;
	setTriangleUnderUs(JWTriangle::cropToLevel(
			0b01010101010101010101010101010000, m_nLevel));

	//generateMesh();
}

void EarthVisualization::clear()
{
	delete[] m_vIndices;
}

EarthVisualization::~EarthVisualization()
{
	clear();
}

void EarthVisualization::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

u32 g_TrianglesBuf[3000];

void EarthVisualization::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(m_Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	driver->drawVertexPrimitiveList(m_mapVerteces.getPtrPool(),
			m_mapVerteces.capacity(), m_vIndices, m_nTrCount,
			video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_32BIT);

}

const core::aabbox3d<f32>& EarthVisualization::getBoundingBox() const
{
	return m_Box;
}

u32 EarthVisualization::getMaterialCount()
{
	return 1;
}

video::SMaterial& EarthVisualization::getMaterial(u32 i)
{
	return m_Material;
}

void EarthVisualization::addTriangleToMesh(u32 triangle, int level)
{
	for (int j = 2; j >= 0; --j)
	{
		u32 vertexId = m_Sphere.getTriangleVertex(triangle, level, j, false);

		int hash = m_mapVerteces.hash(vertexId);
		if (m_mapVerteces.getPtrKeys()[hash] == EMPTY_KEY)
		{
			core::vector3df* ptrVertPos = m_Sphere.getVertex(vertexId);
			video::S3DVertex vert(*ptrVertPos + m_vrtCenter, *ptrVertPos,
					video::SColor(255, 0, 255, 0), core::vector2d<f32>(0, 0));
			vert.Normal.normalize();
			paintVertex(vertexId, &vert);
			m_mapVerteces.put(vertexId, &vert);
		}
		else
		{
			video::S3DVertex* v = m_mapVerteces.getPtrPool() + hash;
			paintVertex(vertexId, v);
		}
		m_vIndices[m_nCurrentIndex++] = hash;
	}
}

void EarthVisualization::generateMesh()
{

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

	int level = m_nLevel;
	jw::JWSphere::BFSIterator* i = m_Sphere.bfs(m_uTriangleUnderUs, level);
	u32 triangle = m_uTriangleUnderUs;
	core::vector3df startPt = m_vertViewerPoint;
	//startPt.setLength(m_fRadius);

	while (m_nTrCount < 2000)
	{
		u32 vertexId = m_Sphere.getTriangleVertex(triangle, level, 0, false);
		core::vector3df* ptrVertPos = m_Sphere.getVertex(vertexId);
		if (ptrVertPos->getDistanceFrom(startPt) < 200000 >> level)
		{
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
			//i = m_Sphere.bfs(i, level);
			jw::JWSphere::BFSIterator* newIterator =
					new jw::JWSphere::BFSIterator(&m_Sphere, level);
			//u32 remain = triangle;
			do
			{
				triangle = JWTriangle::cropToLevel(triangle, level);
				for (int subtr = 0; subtr < 4; subtr++)
				{
					u32 childTr = JWTriangle::getChildIndex(triangle, subtr,
							level);
					if (!i->isUsed(childTr))
					{
						addTriangleToMesh(childTr, level + 1);
						i->setUsed(childTr);
					}
				}
				if (!newIterator->isUsed(triangle))
				{
					newIterator->push(triangle);
					newIterator->setUsed(triangle);
				}

			} while (i->next(&triangle));

			i->drop();
			i = newIterator;
		}
	}

	i->drop();
}

const core::vector3df& EarthVisualization::getViewerPoint() const
{
	return m_vertViewerPoint;
}

void EarthVisualization::setViewerPoint(const core::vector3df& viewerPoint)
{
	this->m_vertViewerPoint = viewerPoint - m_vrtCenter; //translate to sphere coordinates - center is at 0,0,0
	u32 triangleUnderUs = m_Sphere.getTriangleUnderPoint(m_nLevel,
			m_vertViewerPoint);
	setTriangleUnderUs(triangleUnderUs);

}

u32 EarthVisualization::getUTriangleUnderUs() const
{
	return m_uTriangleUnderUs;
}

void EarthVisualization::setTriangleUnderUs(u32 triangleUnderUs)
{
	if (triangleUnderUs != m_uTriangleUnderUs)
	{
		m_uTriangleUnderUs = triangleUnderUs;
		for (int i = 0; i < 3; i++)
		{
			m_vTriangleUnderUsPoints[i] = m_Sphere.getTriangleVertex(
					m_uTriangleUnderUs, m_nLevel, i, false);
		}
		generateMesh();
	}

}

void EarthVisualization::paintVertex(u32 vertexId, video::S3DVertex *v)
{
	v->Color.setRed(0);
	for (int i = 0; i < 3; i++)
	{
		if (m_vTriangleUnderUsPoints[i] == vertexId)
		{
			v->Color.setRed(255);
			break;
		}
	}
}

int EarthVisualization::getLevel() const
{
	return m_nLevel;
}

void EarthVisualization::setLevel(int level)
{
	this->m_nLevel = level;
	m_uTriangleUnderUs = JWTriangle::cropToLevel(m_uTriangleUnderUs, level);
	generateMesh();
}

