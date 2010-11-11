/*
 * JWTriangle.cpp
 *
 *  Created on: Oct 19, 2010
 *      Author: Petko
 */

#include "JWTriangle.h"

int JWTriangle::s_vOctahedronTrianglesLeadVerteces[] =
{ 0b100, 0b101, 0b110, 0b101, 0b010, 0b001, 0b010, 0b000 };

JWTriangle::JWTriangle()
{
}
JWTriangle::JWTriangle(u32 ngh0, u32 ngh1, u32 ngh2)
{
	setNeighbours(ngh0, ngh1, ngh2);
}

void JWTriangle::setNeighbor(u32 ngh, int i)
{
	this->m_vNeighbours[i] = ngh;
}

void JWTriangle::setNeighbours(u32 v0, u32 v1, u32 v2)
{
	this->m_vNeighbours[0] = v0;
	this->m_vNeighbours[1] = v1;
	this->m_vNeighbours[2] = v2;
}

JWTriangle::~JWTriangle()
{
}

u32 JWTriangle::getNeighbour(int i)
{
	return this->m_vNeighbours[i % 3]; // & 0x7FFFFFFF
}

bool JWTriangle::isNeighbourGenerated(int i)
{
	return !(this->m_vNeighbours[i % 3] & 0xA0000000);
}

int JWTriangle::getNeighborInternalIndex(u32 trNeighbor)
{
	int result;
	for (result = 0; result < 3; result++)
	{
		if (getNeighbour(result) == trNeighbor)
		{
			return result;
		}
	}
	return -13;
}

bool JWTriangle::isUpside(u32 trIndex, int level)
{
	bool result = (trIndex & 0b100) == 0;
	int i = level;
	while (i > 0)
	{
		//i * 2 + 1 = 3 + (i - 1) * 2
		if ((trIndex >> (i * 2 + 1) & 0b11) == 0b11)
		{
			result = !result;
		}
		--i;
	}
	return result;
}

u32 JWTriangle::getChildIndex(u32 trIndex, u32 internalChildIndex, int level)
{
	return trIndex | (internalChildIndex << (level * 2 + 3));
}

int JWTriangle::getTriangleNumber(u32 trIndex, int level)
{
	if (level == 0)
	{
		return -1;
	}
	else
	{
		return (trIndex >> (level * 2 + 1)) & 0b11;
	}
}

u32 JWTriangle::getParentTriangle(u32 trIndex, int level)
{
	//assert that level > 0
	return trIndex & (0x7FFFFFFF >> (MAX_TRIANGLE_LEVELS * 2 - level * 2 + 2));
}

void JWTriangle::getSubtrianglesAtEdge(u32 trIndex, int level, int edge,
		u32 *result)
{
	result[0] = getChildIndex(trIndex, edge, level);
	result[1] = getChildIndex(trIndex, (edge + 1) % 3, level);
}

int JWTriangle::getChildVertexEdge(int vertex, int child)
{
	//I watch the picture at http://www.gamedev.net/reference/articles/article2074.asp
	//and find the rules this way
	if (child == 0b11)
	{
		return (vertex + 1) % 3;
	}
	else
	{
		//vertex != child
		//so by watching the picture, I construct a function f(vertex, child)->edge:
		//(0,1)=0
		//(0,2)=2
		//(1,0)=0
		//(1,2)=1
		//(2,0)=2
		//(2,1)=1
		//more simply, there is a rule for the sum vertex+child :
		int sum = vertex + child;
		if (sum == 1)
		{
			return 0;
		}
		else if (sum == 2)
		{
			return 2;
		}
		else
		{ //sum == 3
			return 1;
		}
	}
}

int JWTriangle::getEdgeRepresentor(int edge)
{
	//By watching at the picture http://images.gamedev.net/features/programming/procplanet1/structure.jpg,
	//I create a function f(edge)=tr for finding the sub-triangle in which the vertex is at position 0
	//f(0) = 1
	//f(1) = 3
	//f(2) = 2
	if (edge == 0)
	{
		return 1;
	}
	else if (edge == 1)
	{
		return 3;
	}
	else
	{ //edge == 2
		return 2;
	}
}

int JWTriangle::getLeadVertex(u32 trIndex, int level)
{
	//The 3rd bit set in the static array
	int result = s_vOctahedronTrianglesLeadVerteces[trIndex & 0b111];

	while (level > 0)
	{
		if (((trIndex >> (level * 2 + 1)) & 0b11) == 0b11)
		{
			result ^= 0b100; //flip 3rd bit
		}
		--level;
	}
	return result;
}

int JWTriangle::checkPole(u32 trIndex, int level)
{
	u32 octahedronTriangle = trIndex & 0b111; //level 0 parent of this triangle
	u32 topVertex = s_vOctahedronTrianglesLeadVerteces[octahedronTriangle] & 0b11;
	while (level > 0)
	{
		//near the pole, all subtriangle indexes must be equal to the lead vertex of the
		//octahedron triangle
		if (((trIndex >> (level * 2 + 1)) & 0b11) != topVertex) {
			return 0;
		}
		--level;
	}
	return octahedronTriangle & 0b100 ? -1 : 1; //in the southern hemisphere the triangles has 3rd bit set
}

