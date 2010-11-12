/*
 * JWSphere.cpp
 *
 *  Created on: Oct 26, 2010
 *      Author: Petko
 */

#include "LoggerFactory.h"
#include "JWSphere.h"

using namespace jw;

bool isVertexUndeletable(KEY key)
{
	return key <= (8 << 14); //8 << 14 = 8 << (PERSISTENT_CACHED_LEVEL * 2)
}

jw::JWSphere::JWSphere(f32 radius) :
	log(LoggerFactory::getLogger("com.jw.JWSphere")), m_fRadius(radius)
{
	log->info("Initialize JWSphere");
	m_mapVertices = AutoCleanHashMap<core::vector3df> (1046527, 0.8, 0.2, 8
			<< 14, &isVertexUndeletable);
	m_mapVertices.init();

	m_mapVertices.put(0, core::vector3df(0, m_fRadius, 0));
	m_mapVertices.put(1, core::vector3df(-m_fRadius, 0, 0));
	m_mapVertices.put(2, core::vector3df(0, 0, -m_fRadius));
	m_mapVertices.put(3, core::vector3df(0, 0, m_fRadius));
	//OctahedronVertices[4] = NULL;
	//OctahedronVertices[5] = NULL;
	m_mapVertices.put(6, core::vector3df(m_fRadius, 0, 0));
	m_mapVertices.put(7, core::vector3df(0, -m_fRadius, 0));
	//Vertices[3].Color = video::SColor(255,
	//		0, 0, 255);

	//Triangles which will not be deleted
	m_vmapTriangles[0] = AutoCleanHashMap<JWTriangle> (8, 2.0); //2.0 assure that auto clearing is disabled
	m_vmapTriangles[1] = AutoCleanHashMap<JWTriangle> (8 << 2, 2.0);
	m_vmapTriangles[2] = AutoCleanHashMap<JWTriangle> (8 << 4, 2.0);
	m_vmapTriangles[3] = AutoCleanHashMap<JWTriangle> (8 << 6, 2.0);
	m_vmapTriangles[4] = AutoCleanHashMap<JWTriangle> (8 << 8, 2.0);
	m_vmapTriangles[5] = AutoCleanHashMap<JWTriangle> (8 << 10, 2.0);
	m_vmapTriangles[6] = AutoCleanHashMap<JWTriangle> (8 << 12, 2.0);
	//8 << 14 = 8 << (PERSISTENT_CACHED_LEVEL * 2)
	m_vmapTriangles[7] = AutoCleanHashMap<JWTriangle> (8 << 14, 2.0);
	m_vmapTriangles[8] = AutoCleanHashMap<JWTriangle> (150008);
	m_vmapTriangles[9] = AutoCleanHashMap<JWTriangle> (150009);
	m_vmapTriangles[10] = AutoCleanHashMap<JWTriangle> (150010);
	m_vmapTriangles[11] = AutoCleanHashMap<JWTriangle> (150011);
	m_vmapTriangles[12] = AutoCleanHashMap<JWTriangle> (150012);
	m_vmapTriangles[13] = AutoCleanHashMap<JWTriangle> (150013);
	m_vmapTriangles[14] = AutoCleanHashMap<JWTriangle> (150014);
	//End of Triangles which will not be deleted

	for (int i = 0; i <= MAX_TRIANGLE_LEVELS; i++)
	{
		m_vmapTriangles[i].init();
	}

	AutoCleanHashMap<JWTriangle> & octahedronTriangles = m_vmapTriangles[0];

	octahedronTriangles.put(0, JWTriangle(0b001, 0b100, 0b010));
	octahedronTriangles.put(1, JWTriangle(0b000, 0b011, 0b101));
	octahedronTriangles.put(2, JWTriangle(0b110, 0b011, 0b000));
	octahedronTriangles.put(3, JWTriangle(0b001, 0b010, 0b111));
	octahedronTriangles.put(4, JWTriangle(0b000, 0b101, 0b110));
	octahedronTriangles.put(5, JWTriangle(0b111, 0b100, 0b001));
	octahedronTriangles.put(6, JWTriangle(0b010, 0b100, 0b111));
	octahedronTriangles.put(7, JWTriangle(0b101, 0b011, 0b110));

	log->info("Initialize JWSphere ended");
}

jw::JWSphere::~JWSphere()
{
}

core::vector3df* jw::JWSphere::getVertex(u32 key)
{
	core::vector3df *result = m_mapVertices.get(key);
	if (!result)
	{
		//find the level of the biggest triangle in which the vertex is at internal position 0
		//The ID of the triangle is equal to the ID (key) of the vertex
		int level = MAX_TRIANGLE_LEVELS;
		//u32 triangleId = key;

		while (level > 0)
		{
			if (((key >> (level * 2 + 1)) & 0b11) == 0b00)
			{
				level--;
			}
			else
			{
				break;
			}
		}
		//level 0 should be cached
		if (level == 0)
		{
			log->error("Vertex %d should be in cache and never deleted", key);
			return NULL;
		}

		//up one level - will divide the parent triangle
		--level;

		//The ID of the parent of the triangle whose index the vertex received
		u32 parentTriangleId = JWTriangle::getParentTriangle(key, level);

		divideTriangle(parentTriangleId, level);

		//now it must be in the map
		result = m_mapVertices.get(key);

	}
	return result;
}

JWTriangle* jw::JWSphere::getTriangle(u32 key, int level)
{
	AutoCleanHashMap<JWTriangle> & trianglesMap = m_vmapTriangles[level];
	JWTriangle* result;
	result = trianglesMap.get(key);
	if (result == NULL)
	{
		u32 parent = JWTriangle::getParentTriangle(key, level);
		divideTriangle(parent, level - 1);
		return trianglesMap.get(key);
	}
	else
	{
		return result;
	}
}

u32 jw::JWSphere::getTriangleVertex(u32 triangle, int level, int i,
		bool preventGeneration)
{
	if (preventGeneration && level != 0)
	{
		int trNumber = JWTriangle::getTriangleNumber(triangle, level);
		if (i == trNumber)
		{
			//the vertex is shared between our triangle and its parent
			return getTriangleVertex(JWTriangle::getParentTriangle(triangle,
					level), level - 1, i, false);
		}
		else
		{
			//here I try to find the vertex ID without generating the neighbors
			//(calling getTriangle) at level <level>
			//This is needed while dividing the a triangle at level-1
			//This is the case when the vertex is at the edge of the parent triangle.
			//So, let's find the edge.
			int edge = JWTriangle::getChildVertexEdge(i, trNumber);
			int parentLevel = level - 1;
			u32 parentIndex = JWTriangle::getParentTriangle(triangle, level);

			u32 candidateParent = JWTriangle::getChildIndex(parentIndex,
					JWTriangle::getEdgeRepresentor(edge), parentLevel);

			JWTriangle * parent = getTriangle(parentIndex, parentLevel);
			u32 neighbIndex = parent->getNeighbour(edge);
			JWTriangle * neighbour = getTriangle(neighbIndex, parentLevel);
			int nghbEdge = neighbour->getNeighborInternalIndex(parentIndex);
			u32 candidateNeighbour = JWTriangle::getChildIndex(neighbIndex,
					JWTriangle::getEdgeRepresentor(nghbEdge), parentLevel);
			return min(candidateParent, candidateNeighbour);
		}
	}
	else
	{
		//go around the vertex using the neighbours
		JWTriangle * tr = getTriangle(triangle, level);
		int edge = i;
		u32 bestIndex = 0xFFFFFFFF;
		u32 startTriangle = triangle;
		//Max 6 triangles around the vertex
		for (int j = 0; j < 6; j++)
		{
			if (edge == 0)
			{
				if (bestIndex > triangle)
				{
					bestIndex = triangle;
				}
			}
			u32 neighbIndex = tr->getNeighbour(edge);
			if (startTriangle == neighbIndex)
			{
				//we are back to the start triangle
				break;
			}
			JWTriangle * neighb = getTriangle(neighbIndex, level); //getTrNeighbour(tr, level, edge);

			//what is the index of current triangle in the neighbor's list
			int nghbEdge = neighb->getNeighborInternalIndex(triangle);

			//go counterclockwise in and continue with the neighbor.
			//This ensures we rotate around the same vertex
			edge = (nghbEdge + 1) % 3;
			triangle = neighbIndex;
			tr = neighb;
		}
		//So we found a triangle for which the searched vertex is at index 0 and the triangle
		//is with minimal index
		return bestIndex;
	}
}

void jw::JWSphere::divideTriangle(u32 triangle, int level)
{

	if (level < MAX_TRIANGLE_LEVELS)
	{
		JWTriangle* tr = getTriangle(triangle, level);
		JWTriangle* nghs[3];
		u32 nghsIndexes[3];
		core::vector3df* verteces[3];
		for (int edge = 0; edge < 3; edge++)
		{
			nghsIndexes[edge] = tr->getNeighbour(edge);
			nghs[edge] = getTriangle(nghsIndexes[edge], level);
			verteces[edge]
					= getVertex(getTriangleVertex(triangle, level, edge));

		}

		u32 newTrianglesIndexes[4];
		for (int i = 0; i < 4; i++)
		{
			newTrianglesIndexes[i] = JWTriangle::getChildIndex(triangle, i,
					level);
		}
		for (int edge = 0; edge < 3; edge++)
		{
			int edge2 = (edge + 2) % 3;
			JWTriangle* ngh1 = nghs[edge];
			JWTriangle* ngh2 = nghs[edge2];

			//in most cases nghEdge will be equal to edge, but in some border cases
			//it is not, anyway we need to handle that cases too :)
			int nghEdge1 = ngh1->getNeighborInternalIndex(triangle);
			int nghEdge2 = ngh2->getNeighborInternalIndex(triangle);

			u32 subNgh1 = JWTriangle::getChildIndex(nghsIndexes[edge],
					(nghEdge1 + 1) % 3, level);
			u32 subNgh2 = JWTriangle::getChildIndex(nghsIndexes[edge2],
					nghEdge2, level);

			//u32 iV1 = tr->getChildIndex(edge, level);
			//u32 iV2 = tr->getChildIndex((edge + 1) % 3, level);

			JWTriangle newTriangle;
			newTriangle.setNeighbor(subNgh1, edge);
			newTriangle.setNeighbor(newTrianglesIndexes[0b11], (edge + 1) % 3);
			newTriangle.setNeighbor(subNgh2, (edge + 2) % 3);

			m_vmapTriangles[level + 1].put(newTrianglesIndexes[edge],
					&newTriangle);

			core::vector3df* v1 = verteces[edge];
			core::vector3df* v2 = verteces[(edge + 1) % 3];

			core::vector3df newVertex = (*v1) + (*v2);
			newVertex.setLength(m_fRadius);

			u32 vertexIndex = getTriangleVertex(newTrianglesIndexes[3], level
					+ 1, (edge + 2) % 3, true);

			m_mapVertices.put(vertexIndex, &newVertex);
		}
		JWTriangle middleTriangle(newTrianglesIndexes[2],
				newTrianglesIndexes[0], newTrianglesIndexes[1]);

		m_vmapTriangles[level + 1].put(newTrianglesIndexes[3], &middleTriangle);
	}
	else
	{
		log->warn("Trying to divide triangle %X at "
			"level %d > MAX_TRIANGLE_LEVELS", triangle, level);
	}

}

//--------------------- rectangular part of the sphere ---------------------

int jw::JWSphere::getTilesRow(u32 startTriangle, int level, int left,
		int right, u32 *result)
{
	u32 *resultBase = result;
	u32 currentTr = startTriangle;
	*result = startTriangle;
	++result;
	Direction dir = DIR_LEFT;
	while (left)
	{
		currentTr = getNeighborTriangle(currentTr, level, dir);
		*result = currentTr;
		++result;
		--left;
	}
	currentTr = startTriangle;
	dir = DIR_RIGHT;
	while (right)
	{
		currentTr = getNeighborTriangle(currentTr, level, dir);
		*result = currentTr;
		++result;
		--right;
	}
	return result - resultBase;
}

int jw::JWSphere::getTilesSquare(u32 startTriangle, int level, int left,
		int right, int up, int down, u32 *result)
{
	u32 *resultBase = result;
	u32 currentTr = startTriangle;
	int added = getTilesRow(currentTr, level, left, right, result);
	result += added;
	Direction dir = DIR_UP;
	while (up)
	{
		currentTr = getNeighborTriangle(currentTr, level, dir);
		added = getTilesRow(currentTr, level, left, right, result);
		result += added;
		--up;
	}
	currentTr = startTriangle;
	dir = DIR_DOWN;
	while (down)
	{
		currentTr = getNeighborTriangle(currentTr, level, dir);
		added = getTilesRow(currentTr, level, left, right, result);
		result += added;
		--down;
	}
	return result - resultBase;
}

u32 jw::JWSphere::getNeighborTriangle(u32 triangle, int level,
		Direction& rwDirection)
{
	Direction dir = rwDirection;
	JWTriangle* tr = getTriangle(triangle, level);
	int leadVertex = JWTriangle::getLeadVertex(triangle, level);
	int isUpside = leadVertex >> 2;
	leadVertex &= 0b11;
	if (dir & 0b10)
	{
		//up/down
		if ((isUpside ^ dir) & 0b01)
		{
			//if the triangle is upside and we go up, or the triangle is downside and we go down

			int passedTriangles = 3;
			int atPole = JWTriangle::checkPole(triangle, level);
			if (atPole)
			{
				passedTriangles = 2;
				rwDirection = dir ^ 0b01; //if we were going up, we must start going down and vice versa
			}

			//go round the lead vertex
			int edge = leadVertex;
			for (int j = 0; j < passedTriangles; j++)
			{
				u32 neighbIndex = tr->getNeighbour(edge);
				JWTriangle * neighb = getTriangle(neighbIndex, level);

				//what is the index of current triangle in the neighbor's list
				int nghbEdge = neighb->getNeighborInternalIndex(triangle);

				//go counterclockwise in and continue with the neighbor.
				//This ensures we rotate around the same vertex
				edge = (nghbEdge + 1) % 3;
				triangle = neighbIndex;
				tr = neighb;
			}
			return triangle;

		}
		else
		{
			//the edge opposit to the lead vertex
			return tr->getNeighbour((leadVertex + 1) % 3);

		}
	}
	else
	{
		//left/right

		//TODO collect this in a single operation
		if (isUpside ^ dir)
		{
			//if the triangle is upside and we go left,
			//or the triangle is downside and we go right
			return tr->getNeighbour(leadVertex);
		}
		else
		{
			return tr->getNeighbour((leadVertex + 2) % 3);

		}
	}
}

//--------------------- end of rectangular part of the sphere ---------------------

jw::JWSphere::BFSIterator *jw::JWSphere::bfs(u32 startTr, int startLevel)
{
	return new BFSIterator(this, startTr, startLevel);
}

//--------------------- Triangle under point ---------------------
u32 jw::JWSphere::octahedronTriangleUnderPoint(const core::vector3df & point)
{
	//TODO translate the point if the sphere center is not at (0,0,0)
	u32 result = 0;

	//TODO optimize
	if (point.Z > 0)
	{
		result |= 0b001;
	}
	if (point.X > 0)
	{
		result |= 0b010;
	}
	if (point.Y < 0)
	{
		result |= 0b100;
	}
	return result;
}

f32 g_BorderFunc[14] =
{ 0.605, 0.531, 0.5006, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5 };

u32 jw::JWSphere::getSubtriangleUnderPoint(u32 triangle, int level,
		const core::vector3df & point)
{
	core::vector3df* r1 = getVertex(
			getTriangleVertex(triangle, level, 1, false));
	core::vector3df* r2 = getVertex(
			getTriangleVertex(triangle, level, 2, false));
	core::vector3df* r3;
	//The vertex 0 will be r4
	core::vector3df* r4 = getVertex(
			getTriangleVertex(triangle, level, 0, false));

	u32 t00 = 0;
	u32 t01 = 1;
	u32 t10 = 2;

	f32 d0 = r4->getDistanceFromSQ(*r1);
	f32 d1 = r4->getDistanceFromSQ(*r2);
	//The triangles are either equilateral, or isosceles. In case
	//of isosceles, we choose the point between the equal sides
	//to be the origin of the barycentric coordinate system
	if (fabsf(d0 - d1) > 1.e-5)
	{
		f32 d2 = r1->getDistanceFromSQ(*r2);
		if (fabsf(d2 - d0) < 1.e-5)
		{
			core::vector3df* buf = r4;
			r4 = r1;
			r1 = r2;
			r2 = buf;

			t00 = 1;
			t01 = 2;
			t10 = 0;
		}
		else
		{
			core::vector3df* buf = r4;
			r4 = r2;
			r2 = r1;
			r1 = buf;

			t00 = 2;
			t01 = 0;
			t10 = 1;
		}
	}

	core::vector3df bufR3;
	{
		//I'll calculate r3 so that r3 - r4 is orthogonal to the triangle surface, and |r3-r4| = radius.
		//This way in the Z berycentric coordinate I receive the distance from the point to the triangle.
		//If the point is on the sphere, the distance to the triangle will be used to correct the border value
		core::vector3df r1r4 = (*r1) - (*r4);
		core::vector3df r2r4 = (*r2) - (*r4);
		bufR3 = r2r4.crossProduct(r1r4);
		//bufR3.normalize();
		bufR3.setLength(m_fRadius);
		bufR3 += (*r4);
	}
	r3 = &bufR3;

	core::matrix4 matrT;

	buildTetrahedronBarycentricMatrix(matrT, r1, r2, r3, r4);

	core::vector3df pointBarycentric;

	matrT.transformVect(pointBarycentric, point);

	//pointBarycentric.Z = pointBarycentric.Z / m_fRadius;

	f32 correction = pointBarycentric.Z * 0.576689 / (2 << level);

	if (level == 1)
	{
		//log->info("cor=%f", correction);
		log->info("(%f, %f, %f. c=%f r=%f)", pointBarycentric.X,
				pointBarycentric.Y, pointBarycentric.Z, correction,
				point.getLength());
	}
	//Border between the triangles. In flat case this value should be 0.5, but due to
	//we are in a sphere, we have to correct the value.
	f32 border = 0.5 + correction;

	if (pointBarycentric.X + pointBarycentric.Y < 1. - border)
	{
		return t00;
	}
	else
	{
		//assert pointBarycentric.Y + pointBarycentric.Y < 1 - else the point is outside the triangle
		if (pointBarycentric.X > border)
		{
			return t01;
		}
		else if (pointBarycentric.Y > border)
		{
			return t10;
		}
		else
		{
			//both are < tresshold.
			return 3;
		}
	}

}

void jw::JWSphere::buildTetrahedronBarycentricMatrix(core::matrix4 & matrT,
		const core::vector3df *r1, const core::vector3df *r2,
		const core::vector3df *r3, const core::vector3df *r4)
{
	//See http://en.wikipedia.org/wiki/Barycentric_coordinate_system_(mathematics)#Barycentric_coordinates_on_tetrahedra
	//T^(-1) . (r - r4) is equal to adding r4 to a 4th row in T before inversing, and then just multiplying T^(-1) . r
	matrT[0] = r1->X - r4->X;
	matrT[1] = r1->Y - r4->Y;
	matrT[2] = r1->Z - r4->Z;
	matrT[3] = 0.;

	matrT[4] = r2->X - r4->X;
	matrT[5] = r2->Y - r4->Y;
	matrT[6] = r2->Z - r4->Z;
	matrT[7] = 0.;

	matrT[8] = r3->X - r4->X;
	matrT[9] = r3->Y - r4->Y;
	matrT[10] = r3->Z - r4->Z;
	matrT[11] = 0.;

	matrT[12] = r4->X;
	matrT[13] = r4->Y;
	matrT[14] = r4->Z;
	matrT[15] = 1.;

	matrT.makeInverse();
}

u32 jw::JWSphere::getTriangleUnderPoint(int level,
		const core::vector3df & inPoint)
{
	core::vector3df point(inPoint);
	point.setLength(m_fRadius);
	u32 result = octahedronTriangleUnderPoint(point);

	for (int l = 0; l < level; l++)
	{
		u32 subtr = getSubtriangleUnderPoint(result, l, point);
		result |= subtr << (2 * l + 3);
	}
	return result;
}

//--------------------- end of Triangle under point ---------------------
