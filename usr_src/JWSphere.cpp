/*
 * JWSphere.cpp
 *
 *  Created on: Oct 18, 2010
 *      Author: Petko
 */

#include "JWSphere.h"

JWSphere::JWSphere() :
	scene::ISceneNode(0, 0, 0)
{
	init();
}

JWSphere::JWSphere(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id) :
	scene::ISceneNode(parent, mgr, id)
{

	m_Material.Wireframe = true;
	m_Material.Lighting = false;
	init();

	/*
	 The Irrlicht Engine needs to know the bounding box of a scene node.
	 It will use it for automatic culling and other things. Hence, we
	 need to create a bounding box from the 4 vertices we use.
	 If you do not want the engine to use the box for automatic culling,
	 and/or don't want to create the box, you could also call
	 irr::scene::ISceneNode::setAutomaticCulling() with irr::scene::EAC_OFF.
	 */
	m_Box.reset(m_OctahedronVertices[0].Pos);
	for (s32 i = 1; i < 4; ++i)
	{
		m_Box.addInternalPoint(m_OctahedronVertices[i].Pos);
	}
}

void JWSphere::init()
{
	m_vIndices = 0;
	m_OctahedronVertices[0] = video::S3DVertex(0, 20, 0, 0, 1, 0,
			video::SColor(255, 0, 0, 0), 0, 0);
	m_OctahedronVertices[1] = video::S3DVertex(-20, 0, 0, -1, 0, 0,
			video::SColor(255, 0, 255, 0), 0, 0);
	m_OctahedronVertices[2] = video::S3DVertex(0, 0, -20, 0, 0, -1,
			video::SColor(255, 0, 255, 0), 0, 0);
	m_OctahedronVertices[3] = video::S3DVertex(0, 0, 20, 0, 0, 1,
			video::SColor(255, 0, 255, 0), 0, 0);
	//OctahedronVertices[4] = NULL;
	//OctahedronVertices[5] = NULL;
	m_OctahedronVertices[6] = video::S3DVertex(20, 0, 0, 1, 0, 0,
			video::SColor(255, 0, 255, 0), 0, 0);
	m_OctahedronVertices[7] = video::S3DVertex(0, -20, 0, 0, -1, 0,
			video::SColor(255, 0, 255, 0), 0, 0);
	//Vertices[3].Color = video::SColor(255,
	//		0, 0, 255);
	m_mapOctahedronTriangles[0] = JWTriangle(0b000, 0b001, 0b100, 0b010);
	m_mapOctahedronTriangles[1] = JWTriangle(0b001, 0b000, 0b011, 0b101);
	m_mapOctahedronTriangles[2] = JWTriangle(0b010, 0b110, 0b011, 0b000);
	m_mapOctahedronTriangles[3] = JWTriangle(0b011, 0b001, 0b010, 0b111);
	m_mapOctahedronTriangles[4] = JWTriangle(0b100, 0b000, 0b101, 0b110);
	m_mapOctahedronTriangles[5] = JWTriangle(0b101, 0b111, 0b100, 0b001);
	m_mapOctahedronTriangles[6] = JWTriangle(0b110, 0b010, 0b100, 0b111);
	m_mapOctahedronTriangles[7] = JWTriangle(0b111, 0b101, 0b011, 0b110);

	generateIndeces();
}

void JWSphere::clear()
{
	delete[] m_vIndices;
}

JWSphere::~JWSphere()
{
	clear();
}

void JWSphere::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void JWSphere::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(m_Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawVertexPrimitiveList(&m_OctahedronVertices[0], 8, m_vIndices,
			m_mapOctahedronTriangles.size(), video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_32BIT);

}

const core::aabbox3d<f32>& JWSphere::getBoundingBox() const
{
	return m_Box;
}

u32 JWSphere::getMaterialCount()
{
	return 1;
}

video::SMaterial& JWSphere::getMaterial(u32 i)
{
	return m_Material;
}

u32 JWSphere::getTriangleVertex(u32 triangle, int i)
{
	JWTriangle * tr = &(m_mapOctahedronTriangles[triangle]);
	int edge = i;
	JWTriangle * best;
	//Max 6 triangles around the vertex
	for (int j = 0; j < 6; j++)
	{
		if (edge == 0)
		{
			best = tr;
			if (tr->isUpside())
			{
				break;
			}
		}
		JWTriangle * neighb =
				&(m_mapOctahedronTriangles[tr->getNeighbour(edge)]);
		int nghbEdge;

		//what is the index of current triangle in the neighbor's list
		for (nghbEdge = 0; nghbEdge < 3; nghbEdge++)
		{
			if (neighb->getNeighbour(nghbEdge) == tr->getTrIndex())
			{
				break;
			}
		}
		//go counterclockwise in and continue with the neighbor.
		//This ensures we rotate around the same vertex
		edge = (nghbEdge + 1) % 3;
		tr = neighb;
	}
	//So, in worst case, we found a triangle for which the searched vertex is at index 0,
	//and in the best case, this triangle is upside
	return best->getTrIndex();
}

void JWSphere::generateIndeces()
{
	clear();
	m_vIndices = new u32[m_mapOctahedronTriangles.size() * 3];

	int i = 0;

	TrianglesMap::iterator it = m_mapOctahedronTriangles.begin();
	for (; it != m_mapOctahedronTriangles.end(); ++it)
	{
		//it->first
		for (int j = 2; j >= 0; --j) {
			m_vIndices[i++] = getTriangleVertex(it->first, j);
		}
	}
}

