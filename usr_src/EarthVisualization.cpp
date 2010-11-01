/*
 * JWSphere.cpp
 *
 *  Created on: Oct 18, 2010
 *      Author: Petko
 */

#include "EarthVisualization.h"

EarthVisualization::EarthVisualization() :
	scene::ISceneNode(0, 0, 0)
{
	init();
}

EarthVisualization::EarthVisualization(scene::ISceneNode* parent,
		scene::ISceneManager* mgr, s32 id) :
	scene::ISceneNode(parent, mgr, id)
{

	m_Material.Wireframe = true;
	m_Material.Lighting = false;

	m_vVerteces = jw::AutoCleanHashMap<video::S3DVertex>(1000, 1.1, 0.0);
	m_vVerteces.init();

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

	generateMesh();
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

int g_TrCount;
u32 g_TrianglesBuf[1000];

void EarthVisualization::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(m_Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	driver->drawVertexPrimitiveList(m_vVerteces.getPtrPool(),
			m_vVerteces.capacity(), m_vIndices, g_TrCount, video::EVT_STANDARD,
			scene::EPT_TRIANGLES, video::EIT_32BIT);

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

void EarthVisualization::generateMesh()
{

	clear();

	int level = 7;

	g_TrCount = m_Sphere.getTilesSquare(0b001, level, 2, 50, 6, 6,
			g_TrianglesBuf);
	if (g_TrCount > 1000) {
		return;
	}

	m_vIndices = new u32[g_TrCount * 3];

	int k = 0;

	for (int i = 0; i < g_TrCount; i++)
	{
		for (int j = 2; j >= 0; --j)
		{
			u32 vertexId = m_Sphere.getTriangleVertex(g_TrianglesBuf[i], level,
					j, false);
			core::vector3df* ptrVertPos = m_Sphere.getVertex(vertexId);
			video::S3DVertex vert(*ptrVertPos, *ptrVertPos,
					video::SColor(255, 0, 255, 0), core::vector2d<f32>(0, 0));
			vert.Normal.normalize();
			int hash = m_vVerteces.hash(vertexId);
			if (m_vVerteces.getPtrKeys()[hash] == EMPTY_KEY)
			{
				m_vVerteces.put(vertexId, &vert);
			}
			m_vIndices[k++] = hash;
		}
	}
}

