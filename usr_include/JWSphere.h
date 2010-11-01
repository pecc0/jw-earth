/*
 * JWSphere.h
 *
 *  Created on: Oct 26, 2010
 *      Author: Petko
 */

#ifndef JWSPHERE_H_
#define JWSPHERE_H_

#include <irrlicht.h>
#include "AutoCleanHashMap.h"
#include "JWTriangle.h"
#include "IJWLogger.h"

using namespace irr;


//At what level triangles and vertexes are not deleted
#define PERSISTENT_CACHED_LEVEL 7

#define SPHERE_RADIUS 20.0

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

class JWSphere
{
	AutoCleanHashMap<core::vector3df> m_mapVertices;
	AutoCleanHashMap<JWTriangle> m_vmapTriangles[MAX_TRIANGLE_LEVELS];
    IJWLogger *log;
public:
    JWSphere();
    virtual ~JWSphere();
    core::vector3df *getVertex(u32 key);
    JWTriangle *getTriangle(u32 key, int level);
    u32 getTriangleVertex(u32 triangle, int level, int i, bool preventGeneration = false);
    void divideTriangle(u32 triangle, int level);

    int getTilesRow(u32 startTriangle, int level, int left, int right, u32* result);

    /**
     * Returns the triangles from a part of the sphere surface
     * \param startTriangle Triangle inside the required area
     * \param left, right, up, down How many tiles to each of the directions from the startTriangle are required
     * \param result Pointer to receive the triangle IDs
     * \return the number of triangles returned
     */
    int getTilesSquare(u32 startTriangle, int level, int left, int right, int up, int down, u32* result);

    u32 getNeighborTriangle(u32 triangle, int level, Direction dir);
};
}
#endif /* JWSPHERE_H_ */
