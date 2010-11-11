/*
 * JWTriangle.h
 *
 *  Created on: Oct 19, 2010
 *      Author: Petko
 */

#ifndef JWTRIANGLE_H_
#define JWTRIANGLE_H_

#include "irrTypes.h"

using namespace irr;

#define MAX_TRIANGLE_LEVELS 14

/**
 * We will tessellate the earth into triangles. We start the tessellation from an octahedron.
 * Then we start to recursively divide each of the triangles into 4 little triangles.
 * The "history" of each received triangle is stored into its TrIndex. The first 8 triangles
 * of the octahedron has TrIndex 0b000 - 0b111. Then at each iteration, we occupy 2 more bits
 * of the u32 type. The child triangles are numbered 0b00 - 0b11. The center triangle is
 * always 0b11, the other triangles are numbered according to the vertex they share with
 * the parent. I don't store the value TrIndex here because it is used as a key in the map, so we have it there.
 * The level of the triangle is the iteration at which we have received the
 * triangle. The triangles of the octahedron have level=0. We don't store this value because
 * it is too common and we could take it from outside. A triangle is considered
 * "upside" if 1 of its edges is above the 2 others. 4 of the octahedron triangles are "upside".
 * When we divide a triangle, only the central one changes its direction.
 * So we can easily determine whether a triangle is upside by having its TrIndex and its level.
 * In m_vNeighbours we store the indexes of the neighbour triangles.
 *
 * @see JWTriangle::isUpside
 */
class JWTriangle
{
	/**
	 * indexes of the neighbour triangles
	 */
	u32 m_vNeighbours[3];

	/**
	 * For each of the 8 octahedron triangles, I precache the "lead vertex"
	 * TODO explain "lead vertex"
	 */
	static int s_vOctahedronTrianglesLeadVerteces[8];
public:
	JWTriangle();
	JWTriangle(u32 ngh0, u32 ngh1, u32 ngh2);
	virtual ~JWTriangle();

	/**
	 * Set the IDs of neighbour triangles.
	 * Watch out: if you know ID of triangle one level above the current,
	 * you must also set the 1st bit of the u32 you give
	 */
	void setNeighbours(u32 v0, u32 v1, u32 v2);

	/**
	 * See #setNeighbours
	 */
	void setNeighbor(u32 ngh, int i);

	/**
	 * Returns the ID of a neighbour triangle.
	 * It is possible that the triangle is not from the same level, but one level above.
	 * This is stored in the first bit of the triangle ID. Use
	 * sNeighbourGenerated to check whether the neighbour is from same level
	 * \param i internal index of the neighbour - 0-2
	 */
	u32 getNeighbour(int i);

	/**
	 * Returns whether the ID of neighbour we have is a triangle from same level,
	 * or from one level above
	 *  \param i internal index of the neighbour - 0-2
	 *  @deprecated Not used
	 */
	bool isNeighbourGenerated(int i);

	/**
	 * Returns the internal index of a neighbor triangle,
	 * -1 if the triangle is not found in the neighbors
	 *
	 * \param trNeighbor Global index of a triangle
	 */
	int getNeighborInternalIndex(u32 trNeighbor);

	/**
	 * Returnes whether a triangle is "upside". A triangle is considered "upside" if 1 of his edges is above the 2 others.
	 *
	 * \param trIndex Global index of the triangle
	 * \param level of the triangle
	 */
	static bool isUpside(u32 trIndex, int level);

	/**
	 * Returns the global ID of a child of this triangle.
	 * Perform only bit operations on the ID of the current triangle.
	 * Could be a macro
	 *
	 * \param trIndex Global index of the triangle
	 * \param internalChildIndex Internal index inside the triangle (0b00 - 0b11)
	 * \param level The level of the current triangle (because I don't want to
	 * save it in this class)
	 */
	static u32 getChildIndex(u32 trIndex, u32 internalChildIndex, int level);

	/**
	 * Returns the number of the triangle inside the parent - 0b00 - 0b11;
	 *  -1 if the triangle has not parrent
	 */
	static int getTriangleNumber(u32 trIndex, int level);

	/**
	 * Returns the ID of the parent triangle. level must be > 0
	 *
	 * \param level Level of the current triangle. The parent level is <level> - 1
	 */
	static u32 getParentTriangle(u32 trIndex, int level);

	/**
	 * Returns the global IDs of the 2 sub-triangles of a triangle which share
	 * edge with certain parent's edge.
	 */
	static void getSubtrianglesAtEdge(u32 trIndex, int level, int edge, u32* result);

	/**
	 * For a child triangle <child> , finds at which edge in the parent is the vertex
	 * with internal index for the child is <vertex>.
	 * precondition: child !=  vertex - else the vertex is shared between the parent
	 * and the child
	 *
	 * \param vertex interlan index of the vertex inside the child triangle [0, 2]
	 * \param child the triangle number of the child [0, 3]
	 */
	static int getChildVertexEdge(int vertex, int child);

	/**
	 * For an edge in a triangle, find the subtriangle for which the middle point of the edge
	 * is at position 0
	 */
	static int getEdgeRepresentor(int edge);

	/**
	 * Return the lead vertex of the triangle in first 2 bits.
	 * If the triangle is upside, will set the 3rd bit. So the possible results are
	 * 0-2, 4-6
	 */
	static int getLeadVertex(u32 trIndex, int level);

	/**
	 * Returns 0 if the triangle is not around the pole, -1 if the triangle is around the north pole,
	 * 1 if the triangle is around the south pole
	 */
	static int checkPole(u32 trIndex, int level);
};

#endif /* JWTRIANGLE_H_ */
