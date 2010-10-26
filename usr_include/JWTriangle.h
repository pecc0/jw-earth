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

/**
 * We will tessellate the earth into triangles. We start the tessellation from an octahedron.
 * Then we start to recursively divide each of the triangles into 4 little triangles.
 * The "history" of each received triangle is stored into its TrIndex field. The first 8 triangles
 * of the octahedron has TrIndex 0b000 - 0b111. Then at each iteration, we occupy 2 more bits
 * of the u32 type. The child triangles are numbered 0b00 - 0b11. The center triangle is
 * always 0b11, the other triangles are numbered according to the vertex they share with
 * the parent. In m_nIteration we store the iteration at which we have received the
 * triangles. The triangles of the octahedron have m_nIteration=0. A triangle is considered
 * "upside" if 1 of his edges is above the 2 others. 4 of the octahedron triangles are "upside".
 * When we divide a triangle, only the central one changes its direction.
 * So we can easily determine whether a triangle is upside by having its TrIndex.
 * In m_vNeighbours we store the indexes of the neighbour triangles.
 *
 * @see JWTriangle::isUpside
 */
class JWTriangle
{
	int m_nIteration;
	u32 m_u32TrIndex;
	/**
	 * indexes of the neighbour triangles
	 */
	u32 m_vNeighbours[3];
public:
	JWTriangle();
	JWTriangle(u32 indx, u32 v0, u32 v1, u32 v2);
	virtual ~JWTriangle();
	/**
	 * Returns the index of the triangle. This value contains the "history" of the triangle. See
	 * JWTriangle
	 */
	u32 getTrIndex() const;
	void setTileIndex(u32 TileIndex);
	void setNeighbours(u32 v0, u32 v1, u32 v2);
	u32 getNeighbour(int i);
	/**
	 * Returnes whether a triangle is "upside". A triangle is considered "upside" if 1 of his edges is above the 2 others.
	 */
	bool isUpside();
	int getIteration() const;
	void setIteration(int Iteration);
};

#endif /* JWTRIANGLE_H_ */
