/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2012 Jan Haller
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////

#include <Thor/Math/Triangulation.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>
#include <Thor/Config.hpp>

#include <functional>
#include <numeric>
#include <stack>


namespace thor
{
namespace detail
{

	// Type definitions
	typedef std::array<TriangleIterator, 3>					TriangleItrArray;
	typedef std::pair<TriangleIterator, TriangleIterator>	TriangleItrPair;
	typedef std::pair<unsigned int, unsigned int>			UintPair;

	// ---------------------------------------------------------------------------------------------------------------------------


	// Functor operator() definitions
	bool CompareVertexPtrs::operator() (const AdvancedVertex* lhs, const AdvancedVertex* rhs) const
	{
		return std::make_pair(lhs->getPosition().x, lhs->getPosition().y)
			<  std::make_pair(rhs->getPosition().x, rhs->getPosition().y);
	}

	bool CompareEdges::operator() (const AdvancedEdge& lhs, const AdvancedEdge& rhs) const
	{
		// Automatically compare each coordinate, delegate work to std::pair
		return std::make_pair(
			std::make_pair(lhs[0].getPosition().x, lhs[0].getPosition().y),
			std::make_pair(lhs[1].getPosition().x, lhs[1].getPosition().y))
		< std::make_pair(
			std::make_pair(rhs[0].getPosition().x, rhs[0].getPosition().y),
			std::make_pair(rhs[1].getPosition().x, rhs[1].getPosition().y));
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	OptTriangleIterator::OptTriangleIterator()
	: valid(false)
	, target()
	{
	}
	
	OptTriangleIterator::OptTriangleIterator(TriangleIterator target)
	: valid(true)
	, target(target)
	{
	}

	OptTriangleIterator::OptTriangleIterator(const OptTriangleIterator& origin)
	: valid(origin.valid)
	, target()
	{
		// Don't copy target if invalid, since the iterator is singular (default-constructed)
		// and almost all operations on singular iterators evoke undefined behavior.
		if (valid)
			target = origin.target;
	}
	
	OptTriangleIterator& OptTriangleIterator::operator= (const OptTriangleIterator& origin)
	{
		// Don't assign target if invalid, since the iterator is singular (default-constructed)
		// and almost all operations on singular iterators evoke undefined behavior.
		valid = origin.valid;
		if (valid)
			target = origin.target;
	
		return *this;
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	Circle::Circle(sf::Vector2f midPoint, float squaredRadius)
	: midPoint(midPoint)
	, squaredRadius(squaredRadius)
	{
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	CoordVertexHolder::CoordVertexHolder(float x, float y)
	: mPosition(x, y)
	{
	}

	sf::Vector2f CoordVertexHolder::getPosition() const
	{
		return mPosition;
	}
	
	// ---------------------------------------------------------------------------------------------------------------------------


	AdvancedVertex::AdvancedVertex(float x, float y)
	: mVertexHolder(new CoordVertexHolder(x, y))
	, mSurroundingTriangle()
	{
		// mSurroundingTriangle is a singular iterator. setSurroundingTriangle() must called before other operations,
		// especially before assignments and copies of AdvancedVertex objects.
		// Can't initialize surroundingTriangle here: In case of boundary vertices, the vertices themselves form the triangle.
	}

	sf::Vector2f AdvancedVertex::getPosition() const
	{
		return mVertexHolder->getPosition();
	}
	
	void AdvancedVertex::setSurroundingTriangle(TriangleIterator target)
	{
		mSurroundingTriangle = target;
	}
	
	TriangleIterator AdvancedVertex::getSurroundingTriangle() const
	{
		assert(mSurroundingTriangle.valid);
		return mSurroundingTriangle.target;
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	AdvancedEdge::AdvancedEdge(AdvancedVertex& startPoint, AdvancedVertex& endPoint)
	: Edge<AdvancedVertex>(startPoint, endPoint)
	{
		orderCorners();
	}


	void AdvancedEdge::orderCorners()
	{
		if (CompareVertexPtrs()(mCorners[1], mCorners[0]))
		{
			std::swap(mCorners[0], mCorners[1]);
		}
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	AdvancedTriangle::AdvancedTriangle(AdvancedVertex& corner0, AdvancedVertex& corner1, AdvancedVertex& corner2)
	: Triangle<AdvancedVertex>(corner0, corner1, corner2)
	, mRemainingVertices()
	, mAdjacentTriangles()
	, mFlagged(false)
	{
	}
	
	void AdvancedTriangle::addVertex(AdvancedVertex& vertexRef)
	{
		mRemainingVertices.insert(&vertexRef);
	}
	
	void AdvancedTriangle::removeVertex(AdvancedVertex& vertexRef)
	{
		unsigned int erased = mRemainingVertices.erase(&vertexRef);
		assert(erased == 1);
	}
	
	void AdvancedTriangle::removeVertex(VertexPtrIterator vertexItr)
	{
		unsigned int size = mRemainingVertices.size();
		mRemainingVertices.erase(vertexItr);
		assert(size == mRemainingVertices.size()+1);
	}
	
	VertexPtrIterator AdvancedTriangle::begin()
	{
		return mRemainingVertices.begin();
	}
	
	VertexPtrIterator AdvancedTriangle::end()
	{
		return mRemainingVertices.end();
	}
	
	void AdvancedTriangle::setAdjacentTriangle(unsigned int index, const OptTriangleIterator& adjacentTriangle)
	{
		mAdjacentTriangles[index] = adjacentTriangle;
	}
	
	OptTriangleIterator AdvancedTriangle::getAdjacentTriangle(unsigned int index) const
	{
		return mAdjacentTriangles[index];
	}
	
	void AdvancedTriangle::setFlagged(bool flagged)
	{
		mFlagged = flagged;
	}
	
	bool AdvancedTriangle::isFlagged() const
	{
		return mFlagged;
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	// Returns the position of the triangle's vertex identified by index.
	sf::Vector2f at(const AdvancedTriangle& triangle, unsigned int index)
	{
		return triangle[index].getPosition();
	}

	bool isClockwiseOriented(sf::Vector2f v0, sf::Vector2f v1, sf::Vector2f v2)
	{
		return crossProduct(v1 - v0, v2 - v0).z <= 0;
	}

	Circle computeCircumcircle(const AdvancedTriangle& triangle)
	{
		assert(at(triangle, 0) != at(triangle, 1) && at(triangle, 0) != at(triangle, 2));
		
		// Compute midpoint of two sides
		sf::Vector2f p = 0.5f * (at(triangle, 0) + at(triangle, 1));
		sf::Vector2f q = 0.5f * (at(triangle, 0) + at(triangle, 2));
		
		// Compute perpendicular bisectors of the sides
		sf::Vector2f v = perpendicularVector(p - at(triangle, 0));
		sf::Vector2f w = perpendicularVector(q - at(triangle, 0));
	
		// Now we have the lines p + s*v and q + t*w with s and t being real numbers. The intersection is:
		sf::Vector2f intersection(
			v.x * (p.y * w.x + q.x * w.y - q.y * w.x) - p.x * v.y * w.x,
			w.y * (p.y * v.x + q.x * v.y - p.x * v.y) - q.y * v.y * w.x);
		intersection /= v.x * w.y - v.y * w.x;

		// Alternative to calculating intersection (slower):
		//	sf::Vector3f cross = crossProduct(v,w);
		//	sf::Vector2f intersection = p + v * dotProduct(crossProduct(q-p, w), cross) / squaredLength(cross);

		return Circle(intersection, squaredLength(intersection - at(triangle, 0)));
	}

	// Checks two edges for intersection (if one of the endpoints is equal, this doesn't count as intersection).
	// Returns true, if the edges intersect anywhere except at the endpoints.
	bool intersection(const AdvancedEdge& lhs, const AdvancedEdge& rhs)
	{
		sf::Vector2f a = lhs[0].getPosition(); //    c
		sf::Vector2f b = lhs[1].getPosition(); // a--+------b
		sf::Vector2f c = rhs[0].getPosition(); //    |
		sf::Vector2f d = rhs[1].getPosition(); //    d
	
		if (a == c || a == d || b == c || b == d)
			return false;
	
		return detail::isClockwiseOriented(a, c, d) != detail::isClockwiseOriented(b, c, d)
			&& detail::isClockwiseOriented(a, b, c) != detail::isClockwiseOriented(a, b, d);
	}

	// Checks if edge intersects any constrained edge in constrainedEdges.
	bool intersectsEdge(const AdvancedEdge& edge, const EdgeSet& constrainedEdges)
	{
		AURORA_CITR_FOREACH(EdgeSet, constrainedEdges, itr)
		{
			if (intersection(edge, *itr))
				return true;
		}
	
		return false;
	}

	// Inserts a new triangle built of the 3 corners and returns an iterator to it.
	TriangleIterator insertTriangle(TriangleList& triangles, AdvancedVertex& corner0, AdvancedVertex& corner1, AdvancedVertex& corner2)
	{
		triangles.push_back(AdvancedTriangle(corner0, corner1, corner2));
		return --triangles.end();
	}

	// Checks whether vertex is inside the triangle (center,corner1,corner2).                       c2
	// To be exact, this function only checks if vertex is beyond the two vectors                   /    
	// corner1-center, corner2-center, but since the original triangle is split                v   /
	// up into three new triangles, this doesn't matter. The example returns true.        c1-----ce
	bool isVertexInSection(const AdvancedVertex& vertex, sf::Vector2f center, sf::Vector2f corner1, sf::Vector2f corner2)
	{
		assert(isClockwiseOriented(corner1, corner2, center));

		return crossProduct(corner1 - center, vertex.getPosition() - center).z <  0.f
			&& crossProduct(corner2 - center, vertex.getPosition() - center).z >= 0.f;
	}

	// The same as above, but with only 2 sections. Returns true when the vertex
	// is located on the "left" of the vector corner2-corner1                                    v
	// The example on the right would return true.                                           c1------c2
	bool isVertexInSection(const AdvancedVertex& vertex, sf::Vector2f corner1, sf::Vector2f corner2)
	{
		return crossProduct(corner2 - corner1, vertex.getPosition() - corner1).z >= 0.f;
	}

	// Moves the vertex pointed by sourceItr in sourceTriangle
	void transferVertex(TriangleIterator sourceTriangle, VertexPtrIterator sourceItr, TriangleIterator destTriangle)
	{
		// Let the vertex point to the new triangle
		(*sourceItr)->setSurroundingTriangle(destTriangle);
	
		// Move vertex from source to dest
		destTriangle->addVertex(**sourceItr);
		sourceTriangle->removeVertex(sourceItr);
	}

	// Updates the adjacent triangle's references back to oldTriangle. They are replaced by references to newTriangle (which can be invalid).
	// @param other: The adjacent triangle of which the back-references are updated. May be invalid.
	void updateAdjacentBackReferences(TriangleIterator oldTriangle, OptTriangleIterator newTriangle, OptTriangleIterator other)
	{
		if (other.valid)
		{
			// Find out the index of other's adjacent that points to this triangle.
			for (unsigned int i = 0; i < 3; ++i)
			{
				OptTriangleIterator thisOne = other.target->getAdjacentTriangle(i);
				if (thisOne.valid && thisOne.target == oldTriangle)
				{
					other.target->setAdjacentTriangle(i, newTriangle);
					return;
				}
			}
				
			assert(false);
		}
	}

	// Sets up the adjacent triangles of each element in newTriangles according to the old triangle
	// (before the split into three new ones). Updates also the adjacents' back-references.
	void initializeAdjacents(TriangleItrArray& newTriangles, unsigned int index, TriangleIterator oldTriangle)
	{
		unsigned int index1 = (index+1) % 3;
		unsigned int index2 = (index+2) % 3;
	
		OptTriangleIterator other = oldTriangle->getAdjacentTriangle(index2);
	
		newTriangles[index]->setAdjacentTriangle(0, newTriangles[index1]);
		newTriangles[index]->setAdjacentTriangle(1, newTriangles[index2]);
		newTriangles[index]->setAdjacentTriangle(2, other);
	
		updateAdjacentBackReferences(oldTriangle, newTriangles[index], other);
	}

	// Move all vertices in oldTriangle into the 3 new ones in newTriangles, according to their position.
	void transferVertices3(TriangleIterator oldTriangleItr, TriangleItrArray& newTriangles, sf::Vector2f& newCornerPosition)
	{
		AdvancedTriangle& oldTriangle = *oldTriangleItr;

		// Determine in which sub-triangle all the remaining vertices of currentTriangle are transferred.
		// The transfer invalidates the current iterator itr, that's why it is incremented before the call.
		for (VertexPtrIterator itr = oldTriangle.begin(); itr != oldTriangle.end(); )
		{
			if (isVertexInSection(**itr, newCornerPosition, at(oldTriangle, 0), at(oldTriangle, 1)))
			{
				transferVertex(oldTriangleItr, itr++, newTriangles[0]);
			}
			else if (isVertexInSection(**itr, newCornerPosition, at(oldTriangle, 1), at(oldTriangle, 2)))
			{
				transferVertex(oldTriangleItr, itr++, newTriangles[1]);
			}
			else
			{
				assert(isVertexInSection(**itr, newCornerPosition,  at(oldTriangle, 2),  at(oldTriangle, 0)));
				transferVertex(oldTriangleItr, itr++, newTriangles[2]);
			}
		}
	}

	// Of two given adjacent triangles, this function finds out which two corners are shared by both triangles, and which two are
	// contained by only one triangle.
	//
	// The output parameter sharedCornerIndices1 contains the subscripts (referring to the triangles) of the first shared corner,
	// sharedCornerIndices2 to the second shared corner, disjointCornerIndices to the corners that aren't part of the common edge.
	// The member first refers for each pair to the first triangle, second to the second one.
	void arrangeCorners(const AdvancedTriangle& first, const AdvancedTriangle& second, 
		UintPair& sharedCornerIndices1, UintPair& sharedCornerIndices2, UintPair& disjointCornerIndices)
	{
		std::array<bool, 3> match;
	
		// The triangle's corners are numbered in clockwise order. For example, to compare ABC and BAD,          A
		// we need to reverse BAD to DAB. Here, the AB subsequences are equal in ABC and DAB.                 C  |  D
		// The variable j determines by how many elements the first sequence is rotated.                         B
		for (unsigned int j = 0; j < 3; ++j)
		{
			// Rotate vertex sequence in first until two of them are equal to second
			for (unsigned int i = 0; i < 3; ++i)
			{
				// j determines rotation, 2-i is the reversed second sequence
				match[i] = (at(first, (j+i) % 3) == at(second, 2-i));
			}
		
			// If 2 of 3 corners are equal, then we know the corner arrangement.
			if (std::accumulate(match.begin(), match.end(), 0) == 2)
			{
				unsigned int nbSharedCorners = 0;

				// Fill output parameters with the correct vertices (according to the function's interface description)
				for (unsigned int i = 0; i < 3; ++i)
				{
					unsigned int firstIndex = (j+i) % 3;
					unsigned int secondIndex = 2-i;
				
					// A corner that both adjacent triangles (first and second) have in common
					if (match[i])
					{
						assert(&first[firstIndex] == &second[secondIndex]);
					
						if (nbSharedCorners++ == 0)
						{
							sharedCornerIndices1.first  = firstIndex;
							sharedCornerIndices1.second = secondIndex;
						}
						else
						{
							sharedCornerIndices2.first  = firstIndex;
							sharedCornerIndices2.second = secondIndex;
						}
					}
				
					// A disjoint corner (contained in either first or second)
					else
					{
						assert(&first[firstIndex] != &second[secondIndex]);
					
						disjointCornerIndices.first  = firstIndex;
						disjointCornerIndices.second = secondIndex;
					}
				}
			
				// Ensure that the indices are clockwise oriented for both triangles.                       (sc1.f)|(sc1.s)
				// first:  sc1 -> sc2 -> dc                                               first ->   (dc.f)        |        (dc.s)   <- second
				// second: sc2 -> sc1 -> dc                                                                 (sc2.f)|(sc2.s)
				if (!isClockwiseOriented(
					at(first, sharedCornerIndices1.first), 
					at(first, sharedCornerIndices2.first),
					at(first, disjointCornerIndices.first)))
				{
					std::swap(sharedCornerIndices1, sharedCornerIndices2);
				}
				
				return;
			}
		}
	
		// We get here when the triangles are not adjacent (and thus don't share two corners); this should not happen.
		assert(false);
	}

	// Helper function for TransferVertices2()
	void transferVertices2Impl(TriangleIterator oldFirst, TriangleIterator oldSecond, TriangleIterator newFirst, TriangleIterator newSecond,
		const UintPair& disjointCornerIndices, TriangleIterator oldTriangle)
	{
		// Find out on which side of the new edge each vertex is located and push it into the appropriate new triangle.
		for (VertexPtrIterator itr = oldTriangle->begin(); itr != oldTriangle->end(); )
		{
			if (isVertexInSection(**itr, at(*oldFirst, disjointCornerIndices.first), at(*oldSecond, disjointCornerIndices.second)))
			{
				transferVertex(oldTriangle, itr++, newFirst);
			}
			else
			{
				assert(isVertexInSection(**itr, at(*oldSecond, disjointCornerIndices.second), at(*oldFirst, disjointCornerIndices.first)));
				transferVertex(oldTriangle, itr++, newSecond);
			}
		}	
	}

	// Moves all vertices in oldTriangle to either newFirst or newSecond, depending on which side of the new
	// edge they are situated.
	void transferVertices2(TriangleIterator oldFirst, TriangleIterator oldSecond, TriangleIterator newFirst, TriangleIterator newSecond,
		const UintPair& disjointCornerIndices)
	{
		transferVertices2Impl(oldFirst, oldSecond, newFirst, newSecond, disjointCornerIndices, oldFirst);
		transferVertices2Impl(oldFirst, oldSecond, newFirst, newSecond, disjointCornerIndices, oldSecond);	
	}

	// Copies the adjacent triangle from oldTriangle at oldIndex to newTriangle at newIndex (only one reference).
	// Additionally, the referencee is updated so that it points to newTriangle instead of oldTriangle.
	void updateAdjacentRelation(TriangleIterator oldTriangle, unsigned int oldIndex, TriangleIterator newTriangle, unsigned int newIndex)
	{
		OptTriangleIterator other = oldTriangle->getAdjacentTriangle(oldIndex);

		// Update this triangle's references to adjacent triangles.
		newTriangle->setAdjacentTriangle(newIndex, other);
	
		// Update adjacent triangles's references to this triangle.
		updateAdjacentBackReferences(oldTriangle, newTriangle, other);	
	}

	// Performs an edge flip, i.e. both triangles are merged and the resulting quadrilateral is split again, but into two different 
	// triangles  (choose the other diagonal as cutting edge).
	// @param oldFirst				Iterator to the old first triangle. This iterator is invalidated during the operation.
	// @param oldSecond				Like oldFirst, but iterator to the second triangle.
	// @param sharedCornerIndices1	Indices of the corners in oldFirst and oldSecond that refer to the shared corners.
	// @param sharedCornerIndices2	Like 1, but the other shared corner.
	// @param disjointCornerIndices Indices of the non-shared, disjoint corners.
	// @return						Pair of the newly created triangles
	TriangleItrPair flipEdges(TriangleList& triangles, TriangleIterator oldFirst, TriangleIterator oldSecond,
		const UintPair& sharedCornerIndices1, const UintPair& sharedCornerIndices2, const UintPair& disjointCornerIndices)
	{
		// Create the new triangles which are going to outlive this function
		TriangleIterator newFirst = insertTriangle(triangles, 
			(*oldFirst)[sharedCornerIndices1.first],			// (sc1)
			(*oldSecond)[disjointCornerIndices.second],			// (dc.s)
			(*oldFirst)[disjointCornerIndices.first]);			// (dc.f)
	
		TriangleIterator newSecond = insertTriangle(triangles,
			(*oldSecond)[sharedCornerIndices2.second],			// (sc2)
			(*oldFirst)[disjointCornerIndices.first],			// (dc.f)
			(*oldSecond)[disjointCornerIndices.second]);		// (dc.s)
		
		// Move each vertex to the new corresponding triangle
		transferVertices2(oldFirst, oldSecond, newFirst, newSecond, disjointCornerIndices);
	
		// Adapt referenced adjacents - note that the old indices                     //                       (dc.f)              edge      
		// (non-shared corners) now form the end points of the new edge.              // oldFirst    (sc2.f)          (sc1.f)      flip            (1)|(2)
		updateAdjacentRelation(oldFirst, sharedCornerIndices1.first, newSecond, 2);	  //             ------------------------       ->       (0)      |      (0)
		updateAdjacentRelation(oldFirst, sharedCornerIndices2.first, newFirst, 1);    // oldSecond   (sc2.s)          (sc1.s)                      (2)|(1)     
		updateAdjacentRelation(oldSecond, sharedCornerIndices1.second, newSecond, 1); //                       (dc.s)
		updateAdjacentRelation(oldSecond, sharedCornerIndices2.second, newFirst, 2);  //                                                     newSecond|newFirst
	
		// Of course, the new triangles are adjacent to each other.
		newFirst->setAdjacentTriangle(0, newSecond);
		newSecond->setAdjacentTriangle(0, newFirst);
		
		// Mark old triangles for removal; they're not needed anymore.
		oldFirst->setFlagged(true);
		oldSecond->setFlagged(true);
	
		return TriangleItrPair(newFirst, newSecond);
	}

	// Returns true if the triangle contains the passed vertex as a corner.
	bool hasCorner(const AdvancedTriangle& triangle, const AdvancedVertex& corner)
	{
		// Note: Comparing addresses requires that vertices be stored only once and in a non-invalidating container.
		return &triangle[0] == &corner
			|| &triangle[1] == &corner
			|| &triangle[2] == &corner;
	}

	// Returns true if triangle contains at least one of three corners in the corners triangle
	bool has1Of3Corners(const AdvancedTriangle& triangle, const AdvancedTriangle& corners)
	{
		return hasCorner(triangle, corners[0])
			|| hasCorner(triangle, corners[1])
			|| hasCorner(triangle, corners[2]);
	}

	// Returns true if any of the boundary (dummy) vertices is part of first/second's shared edge.
	bool isSharedBoundary(const AdvancedTriangle& boundaryTriangle, const AdvancedTriangle& first,
		const UintPair& sharedCornerIndices1, const UintPair& sharedCornerIndices2)
	{
		return hasCorner(boundaryTriangle, first[sharedCornerIndices1.first])
			|| hasCorner(boundaryTriangle, first[sharedCornerIndices2.first]);
	}

	// Returns true if any of the boundary (dummy) vertices is either first's or second's disjoint corner.
	bool isDisjointBoundary(const AdvancedTriangle& boundaryTriangle, const AdvancedTriangle& first, const AdvancedTriangle& second,
		const UintPair& disjointCornerIndices)
	{
		return hasCorner(boundaryTriangle, first[disjointCornerIndices.first])
			|| hasCorner(boundaryTriangle, second[disjointCornerIndices.second]);
	}

	bool ensureLocalDelaunay(TriangleList& triangles, TriangleIterator first, TriangleIterator second, const AdvancedTriangle& boundaryTriangle,
		const EdgeSet& constrainedEdges);

	// Applies the check for the Delaunay condition recursively to the triangles neighbors.
	// @return true if an edge flip is performed, false otherwise.
	bool ensureLocalDelaunayAdjacent(TriangleList& triangles, TriangleIterator triangleItr, unsigned int adjacentIndex, const AdvancedTriangle& boundaryTriangle,
		const EdgeSet& constrainedEdges)
	{
		OptTriangleIterator itr = triangleItr->getAdjacentTriangle(adjacentIndex);
	
		return itr.valid && ensureLocalDelaunay(triangles, triangleItr, itr.target, boundaryTriangle, constrainedEdges);
	}

	// Flips edges and enforces the Delaunay condition at adjacent triangles
	void changeEdgeSituation(TriangleList& triangles, TriangleIterator first, TriangleIterator second, const AdvancedTriangle& boundaryTriangle,
		const EdgeSet& constrainedEdges, const UintPair& sharedCornerIndices1, const UintPair& sharedCornerIndices2, const UintPair& disjointCornerIndices)
	{
		TriangleItrPair newTriangles = flipEdges(triangles, first, second, sharedCornerIndices1, sharedCornerIndices2, disjointCornerIndices);	
	
		// Ensure that the adjacent triangles locally conform to Delaunay, as well.
		// If one function call returns true, don't execute the others because iterators are invalidated.
		// On average, the recursion stops on O(1) time, the adjacent triangles are already Delaunay-conforming.
		ensureLocalDelaunayAdjacent(triangles, newTriangles.first, 1, boundaryTriangle, constrainedEdges);
		ensureLocalDelaunayAdjacent(triangles, newTriangles.first, 2, boundaryTriangle, constrainedEdges);
		ensureLocalDelaunayAdjacent(triangles, newTriangles.second, 1, boundaryTriangle, constrainedEdges);
		ensureLocalDelaunayAdjacent(triangles, newTriangles.second, 2, boundaryTriangle, constrainedEdges);		
	}

	// Checks whether the shared edge of two triangles must be moved (to the other diagonal of the quadrilateral)
	// and performs the necessary actions in this case, so that the triangulation locally conforms Delaunay.
	// Also, the adjacent triangles are checked and edge-flipped, if necessary. On average, this requires constant
	// time because the surrounding triangles already satisfy Delaunay before the flip of this triangle.
	// If an edge flip is performed, true is returned and the new triangles are stored in the input/output parameters first, second.
	// @return true if an edge flip is performed, false otherwise.
	bool ensureLocalDelaunay(TriangleList& triangles, TriangleIterator first, TriangleIterator second, const AdvancedTriangle& boundaryTriangle,
		const EdgeSet& constrainedEdges)
	{
		// Note: If the merged quadrilateral is concave, the Delaunay condition will locally already be satisfied.	

		// Flagged triangles are going to be removed, don't take them into account
		if (first->isFlagged() || second->isFlagged())
			return false;

		// Find out which triangle indices refer to the corners that are shared by both triangles, and which to the disjoint ones.
		UintPair sharedCornerIndices1;
		UintPair sharedCornerIndices2;
		UintPair disjointCornerIndices;
		arrangeCorners(*first, *second, sharedCornerIndices1, sharedCornerIndices2, disjointCornerIndices);
	
		// Check if we must flip edges because of the boundaries (the triangles there don't have to conform Delaunay, but the triangles inside do)
		bool disjointBoundary = isDisjointBoundary(boundaryTriangle, *first, *second, disjointCornerIndices);
		bool sharedBoundary = isSharedBoundary(boundaryTriangle, *first, sharedCornerIndices1, sharedCornerIndices2);
	
		// The following additional checks are not required if constrained edges are always part of a merged quadrilateral (=two adjacent triangles).
		// But in general, we may have constrained edges that span many triangles, and the local Delaunay condition doesn't capture them.
		bool sharedBlocking = intersectsEdge(AdvancedEdge((*first)[sharedCornerIndices1.first], (*first)[sharedCornerIndices2.first]), constrainedEdges);
		bool disjointBlocking = intersectsEdge(AdvancedEdge((*first)[disjointCornerIndices.first], (*second)[disjointCornerIndices.second]), constrainedEdges);
	
		// These two bools express whether the disjoint edge respectively the shared edge MUST be flipped.
		bool disjointEdgeEnforced = disjointBoundary || disjointBlocking;
		bool sharedEdgeEnforced = sharedBoundary || sharedBlocking;
	
		// If the Delaunay test concerns one of the initial vertices, we pretend that those vertices are never inside the circumcircle.
		// This is required because we don't want to perform edge flips at the boundary of the very big outer triangle.
		// The same applies to constrained edges as input of the Constrained Delaunay Triangulation.
		if (disjointEdgeEnforced && !sharedEdgeEnforced)
			return false;
	
		if (sharedEdgeEnforced && !disjointEdgeEnforced)
		{
			// If the merged quadrilateral isn't convex, we may of course not flip edges (since the new edge would be located outside both triangles).
			if (isClockwiseOriented(at(*first, disjointCornerIndices.first), at(*second, disjointCornerIndices.second), at(*first, sharedCornerIndices1.first))
			 || isClockwiseOriented(at(*second, disjointCornerIndices.second), at(*first, disjointCornerIndices.first), at(*first, sharedCornerIndices2.first)))
				return false;
	 
			changeEdgeSituation(triangles, first, second, boundaryTriangle, constrainedEdges, sharedCornerIndices1, sharedCornerIndices2, disjointCornerIndices);
			return true;
		}
	
		// If the vertex of the other triangle is inside this triangle's circumcircle, the Delaunay condition is locally breached and we need to flip edges.
		// Independently, there can be an enforced edge flip (at the boundary, or because of the constraints).
		// The second && operand is actually not necessary, since the Delaunay condition is symmetric to both triangles. However, rounding errors may occur
		// at close points.
		Circle circle = computeCircumcircle(*first);
		Circle circle2 = computeCircumcircle(*second);
		if (squaredLength(at(*second, disjointCornerIndices.second) - circle.midPoint) < circle.squaredRadius
		 && squaredLength(at(*first, disjointCornerIndices.first) - circle2.midPoint) < circle2.squaredRadius)
		{
			changeEdgeSituation(triangles, first, second, boundaryTriangle, constrainedEdges, sharedCornerIndices1, sharedCornerIndices2, disjointCornerIndices);
			return true;
		}
	
		// Otherwise, the triangles are Delaunay at the moment and no edge flip is required.
		return false;
	}

	// Inserts the specified vertex into the list of triangles.
	void insertPoint(TriangleList& triangles, AdvancedVertex& vertex, const AdvancedTriangle& boundaryTriangle, const EdgeSet& constrainedEdges)
	{
		TriangleIterator oldTriangleItr = vertex.getSurroundingTriangle();
		AdvancedTriangle& oldTriangle = *oldTriangleItr;
	
		assert(isClockwiseOriented(at(oldTriangle, 0), at(oldTriangle, 1), at(oldTriangle,2)));
	
		// Split triangle up into three new sub-triangles, each consisting of two old corners and the new vertex
		TriangleItrArray newTriangles = {
			insertTriangle(triangles, oldTriangle[0], oldTriangle[1], vertex),
			insertTriangle(triangles, oldTriangle[1], oldTriangle[2], vertex),
			insertTriangle(triangles, oldTriangle[2], oldTriangle[0], vertex)};

		// Assign the adjacent triangles to the new triangles
		for (unsigned int i = 0; i < 3; ++i)
			initializeAdjacents(newTriangles, i, oldTriangleItr);

		// Remove current vertex - as soon as it forms a triangle corner, it counts no longer as remaining vertex
		sf::Vector2f newCornerPosition = vertex.getPosition();
		oldTriangle.removeVertex(vertex);
	
		// Move each vertex to its corresponding new surrounding triangle
		transferVertices3(oldTriangleItr, newTriangles, newCornerPosition);
	
		// Remove the old big triangle, we have three new ones
		triangles.erase(oldTriangleItr);
	
		// For each newly created triangle, we must ensure that the Delaunay condition with its adjacent is kept up.
		// The variable adjacent (third argument of EnsureLocalDelaunay()) is the adjacent of the old triangle.
		// Corner number 2 is always the vertex inserted in this function, so the triangle on the opposite is the sought one.
		for (unsigned int i = 0; i < 3; ++i)
		{
			OptTriangleIterator adjacent = newTriangles[i]->getAdjacentTriangle(2);
		
			if (adjacent.valid)
				ensureLocalDelaunay(triangles, newTriangles[i], adjacent.target, boundaryTriangle, constrainedEdges);
		}
	
		// Remove currently flagged triangles. Don't do this earlier because of iterator invalidations.
		triangles.remove_if(std::mem_fn(&AdvancedTriangle::isFlagged));
	}

	// Creates 3 "dummy" vertices that form a huge triangle which later includes all other vertices
	// (every element of allVertices).
	void createBoundaryPoints(VertexCtr& allVertices, TriangleList& triangles)
	{
		assert(allVertices.empty());
		assert(triangles.empty());

		// Add boundary vertices (choose arbitrary, clockwise oriented positions)
		allVertices.push_back(AdvancedVertex(-1, 0));
		allVertices.push_back(AdvancedVertex( 0,+1));
		allVertices.push_back(AdvancedVertex(+1, 0));

		// First triangle in list is the one containing the three boundary vertices
		triangles.push_back(AdvancedTriangle(allVertices[0], allVertices[1], allVertices[2]));
	
		for (unsigned int i = 0; i < 3; ++i)
			allVertices[i].setSurroundingTriangle(triangles.begin());
	}

	// Sets the initial point positions so that the triangle of dummy vertices includes all other vertices.
	// Like this, we can start the algorithm seamlessly.
	void setBoundaryPositions(const VertexCtr& allVertices, AdvancedTriangle& boundaryTriangle)
	{
		// Find maximal coordinate in any direction
		float maxCoord = 0.f;
		AURORA_CITR_FOREACH(VertexCtr, allVertices, itr)
		{
			sf::Vector2f position = itr->getPosition();

			maxCoord = std::max(maxCoord, std::abs(position.x));
			maxCoord = std::max(maxCoord, std::abs(position.y));
		}

		// Reduce probability to have a 3 collinear points (which can't be triangulated) by slightly moving boundary points. Take arbitrary value...
		const float epsilon = 0.000372f;

		// Overwrite 3 dummy vertices so that the resulting triangle certainly surrounds all other vertices.
		maxCoord *= 4.f;
		boundaryTriangle[0] = AdvancedVertex(epsilon, maxCoord-epsilon);
		boundaryTriangle[1] = AdvancedVertex(maxCoord+epsilon, -epsilon);
		boundaryTriangle[2] = AdvancedVertex(-maxCoord-epsilon, -maxCoord+epsilon);
	}

	// Checks whether the edge formed of the two specified points is contained in constrainedEdges.
	// Copies vertex parameters, but they're not further used.
	bool isEdgeConstrained(const EdgeSet& constrainedEdges, AdvancedVertex startPoint, AdvancedVertex endPoint)
	{
		AdvancedEdge adv(startPoint, endPoint);
		EdgeSet::const_iterator candidate = constrainedEdges.find(adv);
	
		// Just to make sure the set predicate really works. Otherwise: return candidate != ..end() && .. && ..;
		assert(candidate == constrainedEdges.end()
		 || adv[0].getPosition() == (*candidate)[0].getPosition()
		 && adv[1].getPosition() == (*candidate)[1].getPosition());
	
		return candidate != constrainedEdges.end();
	}

	// Checks if the adjacent triangle is in use (i.e. inside the polygon) and returns a valid OptTriangleIterator in this case.
	// Otherwise, the latter is invalid.
	OptTriangleIterator hasUnusedAdjacent(const AdvancedTriangle& triangle, unsigned int index, const EdgeSet& constrainedEdges)
	{
		if (isEdgeConstrained(constrainedEdges, triangle[(index+1) % 3], triangle[(index+2) % 3]))
			return OptTriangleIterator();
		else
			return triangle.getAdjacentTriangle(index);
	}

	// Iterative implementation of RemoveUnusedTriangles
	void removeOuterPolygonTrianglesImpl(TriangleIterator current, std::stack<TriangleIterator>& stack, const EdgeSet& constrainedEdges)
	{
		// Flagged triangles have already been passed, skip them
		if (current->isFlagged())
			return;
		
		current->setFlagged(true);

		OptTriangleIterator adjacent;
		for (unsigned int i = 0; i < 3; ++i)
		{
			if ((adjacent = hasUnusedAdjacent(*current, i, constrainedEdges)).valid)
				stack.push(adjacent.target);
		}
	}

	// Removes the triangles that are "unused" (i.e. outside the polygon).
	// The parameter current refers to any triangle touching at least one boundary point. Starting at this triangle, adjacent triangles
	// are walked through recursively. When we reach an edge of the polygon (which is always a constrained edge, and vice versa),
	// we stop here and complete the remaining directions. Finally, all triangles that are outside of the polygon bounds are removed.
	void removeOuterPolygonTriangles(TriangleList& triangles, TriangleIterator current, const EdgeSet& constrainedEdges)
	{
		std::stack<TriangleIterator> stack;
		stack.push(current);

		while (!stack.empty())
		{
			// get the top element of the stack (next triangle to traverse) and pop it
			current = stack.top();
			stack.pop();
		
			// Call function that may push new triangles onto the stack
			removeOuterPolygonTrianglesImpl(current, stack, constrainedEdges);
		}
	
		// Remove all triangles marked as unused.
		// We can't erase() during the stack iteration, because triangles' flags are still polled, so the iterators must be valid.
		triangles.remove_if(std::mem_fn(&AdvancedTriangle::isFlagged));
	}

	// Removes all triangles that are not directly required for the resulting triangulation (algorithm-supporting data).
	// In case that a polygon is triangulated, all triangles at the outside of the polygon are removed.
	void removeUnusedTriangles(TriangleList& triangles, const AdvancedTriangle& boundaryTriangle, const EdgeSet& constrainedEdges, bool limitToPolygon)
	{
		for (TriangleList::iterator itr = triangles.begin(); itr != triangles.end(); )
		{
			// If the current triangle is located at the boundary (touches one of the boundary corners)
			if (has1Of3Corners(*itr, boundaryTriangle))
			{
				// When we just want to triangulate the inside of a polygon, we must cut off all outer triangles, beginning
				// at any boundary-touching triangle.
				if (limitToPolygon)
					return removeOuterPolygonTriangles(triangles, itr, constrainedEdges);

				// Otherwise, only the very outer triangles (those touching the boundary vertices) must be removed
				else
					itr = triangles.erase(itr);
			}
		
			// Triangle is retained, continue
			else
			{
				++itr;
			}
		}
	}

} // namespace detail
} // namespace thor
