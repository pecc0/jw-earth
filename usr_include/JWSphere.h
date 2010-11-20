/*
 * JWSphere.h
 *
 *  Created on: Oct 26, 2010
 *      Author: Petko
 */

#ifndef JWSPHERE_H_
#define JWSPHERE_H_

#include <queue>
#include <set>
#include <irrlicht.h>
#include "AutoCleanHashMap.h"
#include "JWTriangle.h"
#include "IJWLogger.h"
#include "matrix4.h"

using namespace irr;

//At what level triangles and vertexes are not deleted
#define PERSISTENT_CACHED_LEVEL 7

typedef int Direction;

#define DIR_LEFT 0b00
#define DIR_RIGHT 0b01
#define DIR_UP 0b10
#define DIR_DOWN 0b11

namespace jw
{

using namespace log;

/**
 * We will tessellate the sphere into triangles. We start the tessellation from an octahedron.
 * Then we start to recursively divide each of the triangles into 4 little triangles.
 * We have a clever style of indexing the triangles - see JWTriangle. We must also store
 * the vertexes of the mesh, but they are only linked functionally to the triangles.
 * For each triangle, we store only its neighbor triangles, ordered 0-2. We number the edges
 * of the triangle again with 0-2, and according to the index of the neighbour with which the
 * edge is shared. We internally number the vertexes of the triangle with 0-2, so that
 * the internal index of the vertex is equal to the index of the edge counter clockwise
 * from the vertex. We globally index the vertextes in following way:
 * The vertex receives its index from the triangle for which this vertex is at number 0 in the
 * internal numbering. In most cases, there are 2 triangles for which the vertex is at number 0.
 * So then we choose the triangle with lower index of the 2.
 */

class BFSIterator;

class JWSphere
{

	AutoCleanHashMap<core::vector3df> m_mapVertices;
	AutoCleanHashMap<JWTriangle> m_vmapTriangles[MAX_TRIANGLE_LEVELS + 1];
	IJWLogger *log;
	f32 m_fRadius;
public:

	JWSphere(f32 radius);
	virtual ~JWSphere();
	core::vector3df *getVertex(u32 key);
	JWTriangle *getTriangle(u32 key, int level);
	u32 getTriangleVertex(u32 triangle, int level, int i,
			bool preventGeneration = false);
	void divideTriangle(u32 triangle, int level);

	int getTilesRow(u32 startTriangle, int level, int left, int right,
			u32* result);

	/**
	 * Returns the triangles from a part of the sphere surface
	 * \param startTriangle Triangle inside the required area
	 * \param left, right, up, down How many tiles to each of the directions from the startTriangle are required
	 * \param result Pointer to receive the triangle IDs
	 * \return the number of triangles returned
	 */
	int getTilesSquare(u32 startTriangle, int level, int left, int right,
			int up, int down, u32* result);

	/**
	 * Returns a neighbor triangle of certain triangle at certain level.
	 * If the neighbor triangle is beyond a pole (happens only in case we go up or down),
	 * will also change the direction accordingly
	 */
	u32 getNeighborTriangle(u32 triangle, int level, Direction& rwDirection);

	BFSIterator* bfs(u32 startTr, int startLevel);

	BFSIterator* bfs(jw::BFSIterator * old, int startLevel);

	/**
	 * Build a matrix for transformin into tetrahedron barycentric coordinate system
	 * \param matrix reveicer
	 * \param r1
	 * \param r2
	 * \param r3
	 * \param r4
	 */
	static void buildTetrahedronBarycentricMatrix(core::matrix4& matrix,
			const core::vector3df* r1, const core::vector3df* r2,
			const core::vector3df* r3, const core::vector3df* r4);

	u32 octahedronTriangleUnderPoint(const core::vector3df & point);

	u32 getSubtriangleUnderPoint(u32 triangle, int level,
			const core::vector3df & point);

	u32 getTriangleUnderPoint(int level, const core::vector3df & point);

};

class BFSIterator: virtual public IReferenceCounted
{
	JWSphere* m_sphere;
	queue<u32> m_trQueue;
	set<u32> m_trUsed;
	int m_level;
public:
	BFSIterator(JWSphere* sphere, int level);
	BFSIterator(JWSphere* sphere, u32 start, int level);
	BFSIterator(BFSIterator* old, int level);
	virtual ~BFSIterator()
	{

	}
	int getLevel() const;
	void setLevel(int m_level);
	bool next(u32* result);
	void accept(u32 triangle);
	/**
	 * Checks whether the triangle is in the used triangles set.
	 */
	bool isUsed(u32 tr);
	void setUsed(u32 tr);
	void push(u32 triangle);

	friend class JWSphere;
};

}

#endif /* JWSPHERE_H_ */
