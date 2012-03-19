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

namespace thor
{
namespace detail
{

	// Forward declarations
	class AdvancedTriangle;
	class AdvancedVertex;
	class AdvancedEdge;

	// Functor to compare Vertex pointers (the original instance must be kept, no copies) in std::set.
	struct THOR_API CompareRawVertexPtrs
	{
		bool operator() (const Vertex* lhs, const Vertex* rhs) const;
	};

	// The same for advanced vertices. Since AdvancedVertex is not derived from Vertex, this is necessary.
	struct THOR_API CompareVertexPtrs
	{
		bool operator() (const AdvancedVertex* lhs, const AdvancedVertex* rhs) const;
	};

	// Functor to sort constrained edges by their positions, needed to store them inside std::set.
	struct THOR_API CompareEdges
	{
		bool operator() (const AdvancedEdge& lhs, const AdvancedEdge& rhs) const;
	};

	// ---------------------------------------------------------------------------------------------------------------------------


	// Type definitions of containers
	typedef std::vector<Vertex>								VertexCtr;
	typedef std::deque<AdvancedVertex>						AdvancedVertexCtr;
	typedef std::set<const Vertex*, CompareRawVertexPtrs>	RawVertexPtrSet;
	typedef std::set<AdvancedVertex*, CompareVertexPtrs>	VertexPtrSet;
	typedef std::set<AdvancedEdge, CompareEdges>			EdgeSet; 
	typedef std::list<AdvancedTriangle>						TriangleList;

	// Type definitions of often used iterators
	typedef VertexPtrSet::iterator							VertexPtrIterator;
	typedef TriangleList::iterator							TriangleIterator;
	
	// ---------------------------------------------------------------------------------------------------------------------------


	// Circle class, needed for circumcircle
	struct Circle
	{
									Circle(sf::Vector2f midPoint, float squaredRadius);

		sf::Vector2f				midPoint;
		float						squaredRadius;
	};

	// Meta-information vertex, for algorithm internals
	class THOR_API AdvancedVertex 
	{
		public:
										AdvancedVertex(const Vertex& userVertex, TriangleIterator surroundingTriangle);
			sf::Vector2f				getPosition() const;
			void						setSurroundingTriangle(TriangleIterator target);
			TriangleIterator			getSurroundingTriangle() const;
			const Vertex&				getUserVertex() const;
		
		private:
			const Vertex*				mUserVertex;
			TriangleIterator			mSurroundingTriangle;
	};

	// Meta-information edge, for algorithm internals
	class THOR_API AdvancedEdge : public Edge<Vertex>
	{
		public:
										AdvancedEdge(const Vertex& startPoint, const Vertex& endPoint);
		
			template <typename UserVertex>
										AdvancedEdge(const Edge<UserVertex>& userEdge)
			: Edge<Vertex>(userEdge[0], userEdge[1])
			{
				orderCorners();
			}
	
		private:
			// Sorts the corners by vector-component-sum to allow faster access inside std::set.
			void orderCorners();
	};

	// Iterator to a triangle which can be explicitly invalid ("opt" stands for "optional").
	struct THOR_API OptTriangleIterator
	{
									OptTriangleIterator();
									OptTriangleIterator(TriangleIterator target);
									OptTriangleIterator(const OptTriangleIterator& origin);

		OptTriangleIterator&		operator= (const OptTriangleIterator& origin);

		// Members
		bool						valid;
		TriangleIterator			target;
	};

	// A triangle that carries advanced information in order to efficiently support the algorithm.
	class THOR_API AdvancedTriangle : public Triangle<Vertex>
	{	
		public:
										AdvancedTriangle(const Vertex& corner0, const Vertex& corner1, const Vertex& corner2);
		
			void						addVertex(AdvancedVertex& vertexRef);

			void						removeVertex(AdvancedVertex& vertexRef);	
			void						removeVertex(VertexPtrIterator vertexItr);
		
			VertexPtrIterator			Begin();		
			VertexPtrIterator			End();
		
			// Sets/returns the adjacent triangle on the opposite side of the corner #index.
			void						setAdjacentTriangle(unsigned int index, const OptTriangleIterator& adjacentTriangle);
			OptTriangleIterator			getAdjacentTriangle(unsigned int index) const;

			// Marks this triangle (or removes mark)
			void						setFlagged(bool flagged);
			bool						isFlagged() const;

		private:		
			VertexPtrSet							mRemainingVertices;
			std::tr1::array<OptTriangleIterator, 3>	mAdjacentTriangles;
			bool									mFlagged;
	};

	// ---------------------------------------------------------------------------------------------------------------------------


	// Function declarations
	float				THOR_API sumVectorComponents(sf::Vector2f vector);
	void 				THOR_API insertPoint(TriangleList& triangles, AdvancedVertex& vertex, const VertexCtr& boundaryVertices, const EdgeSet& constrainedEdges);
	void 				THOR_API createBoundaryPoints(AdvancedVertexCtr& allVertices, VertexCtr& boundaryVertices, TriangleList& triangles);
	void 				THOR_API setBoundaryPositions(const AdvancedVertexCtr& allVertices, VertexCtr& boundaryVertices);
	bool 				THOR_API has1Of3Corners(const AdvancedTriangle& triangle, const VertexCtr& corners);
	bool 				THOR_API isClockwiseOriented(sf::Vector2f v0, sf::Vector2f v1, sf::Vector2f v2);
	bool 				THOR_API isEdgeConstrained(const EdgeSet& constrainedEdges, const Vertex& startPoint, const Vertex& endPoint);
	OptTriangleIterator THOR_API hasUnusedAdjacent(const AdvancedTriangle& triangle, unsigned int index, const VertexCtr& boundaryVertices, const EdgeSet& constrainedEdges);
	void				THOR_API removeOuterPolygonTriangles(TriangleList& triangles, TriangleIterator current, const EdgeSet& constrainedEdges);
	void				THOR_API removeUnusedTriangles(TriangleList& triangles, const VertexCtr& boundaryVertices, const EdgeSet& constrainedEdges, bool limitToPolygon);

	// ---------------------------------------------------------------------------------------------------------------------------
	

	// Policy class for small differences in triangulation - here for TriangulateConstrained()
	template <typename InputIterator>
	struct ConstrainedTrDetails
	{
		ConstrainedTrDetails(InputIterator constrainedEdgesBegin, InputIterator constrainedEdgesEnd)
		: constrainedEdgesBegin(constrainedEdgesBegin)
		, constrainedEdgesEnd(constrainedEdgesEnd)
		{
		}

		InputIterator constrainedEdgesBegin;
		InputIterator constrainedEdgesEnd;
	
		static const bool isPolygon = false;
	};

	// Policy class for small differences in triangulation - here for TriangulatePolygon() without edgesOut parameter
	struct PolygonTrDetails
	{
		static const bool isPolygon = true;
	};

	// Policy class for small differences in triangulation - here for TriangulatePolygon() with edgesOut parameter
	template <typename OutputIterator, class VertexType>
	struct PolygonOutputTrDetails
	{
		PolygonOutputTrDetails(OutputIterator edgesOut)
		: edgesOut(edgesOut)
		{
		}
	
		OutputIterator edgesOut;

		static const bool isPolygon = true;
	};

	// ---------------------------------------------------------------------------------------------------------------------------


	// Sort out vertices according to their "importance". Vertices that are part of a constrained edge shall be inserted first.
	// Adds constrained edges as well.
	template <typename InputIterator1, typename InputIterator2>
	void collateVerticesConstrained(TriangleIterator firstTriangle, AdvancedVertexCtr& allVertices, EdgeSet& constrainedEdges,
		InputIterator1 verticesBegin, InputIterator1 verticesEnd, InputIterator2 constrainedEdgesBegin, InputIterator2 constrainedEdgesEnd)
	{
		// Sort out "important" vertices
		RawVertexPtrSet importantVertices;
		for (; constrainedEdgesBegin != constrainedEdgesEnd; ++constrainedEdgesBegin)
		{
			typedef typename std::iterator_traits<InputIterator2>::value_type UserEdge;
			const UserEdge& edge = *constrainedEdgesBegin;
		
			importantVertices.insert(&edge[0]);
			importantVertices.insert(&edge[1]);
			constrainedEdges.insert(edge);
		}
	
		// Insert important vertices, link to base triangle
		AURORA_CITR_FOREACH(RawVertexPtrSet, importantVertices, itr)
		{
			allVertices.push_back(AdvancedVertex(**itr, firstTriangle));
			firstTriangle->addVertex(allVertices.back());
		}

		// Insert other vertices, link to base triangle
		for (; verticesBegin != verticesEnd; ++verticesBegin)
		{
			const Vertex& vertex = *verticesBegin;
	
			if (importantVertices.find(&vertex) == importantVertices.end())
			{
				allVertices.push_back(detail::AdvancedVertex(vertex, firstTriangle));
				firstTriangle->addVertex(allVertices.back());
			}
		}
	}

	// Helper function for CollateVerticesPolygon(); constructs an edge and adds it to the container and output iterator.
	inline void addEdge(EdgeSet& constrainedEdges, const Vertex* previousVertex, const Vertex& currentVertex, const PolygonTrDetails&)
	{
		if (previousVertex != NULL)
			constrainedEdges.insert(AdvancedEdge(*previousVertex, currentVertex));
	}

	// Overload for PolygonTrDetails<OutputIterator> to write in an output iterator
	template <typename OutputIterator, class VertexType>
	void addEdge(EdgeSet& constrainedEdges, const Vertex* previousVertex, const Vertex& currentVertex, PolygonOutputTrDetails<OutputIterator, VertexType> details)
	{
		if (previousVertex != NULL)
		{	
			*details.edgesOut++ = Edge<VertexType>(
				static_cast<const VertexType&>(*previousVertex),
				static_cast<const VertexType&>(currentVertex));
		
			constrainedEdges.insert(AdvancedEdge(*previousVertex, currentVertex));
		}
	}

	// CollateVertices Implementation for polygons
	template <typename InputIterator, class AdditionalDetails>
	void CollateVerticesPolygon(TriangleIterator firstTriangle, AdvancedVertexCtr& allVertices, EdgeSet& constrainedEdges,
		InputIterator verticesBegin, InputIterator verticesEnd, const AdditionalDetails& details)
	{
		// Empty vertex range: Do nothing
		if (verticesBegin == verticesEnd)
			return;

		const Vertex& firstVertex = *verticesBegin;
		const Vertex* previousVertex = NULL;
	
		for (; verticesBegin != verticesEnd; ++verticesBegin)
		{	
			// Add the vertex to allVertices, link with firstTriangle
			AdvancedVertex vertex(*verticesBegin, firstTriangle);
			allVertices.push_back(vertex);
			firstTriangle->addVertex(allVertices.back()); 

			// Add edge of adjacent vertices
			addEdge(constrainedEdges, previousVertex, vertex.getUserVertex(), details);
			previousVertex = &vertex.getUserVertex();
		}
	
		// If at least one vertex is contained, insert edge from last to first vertex, so that the boundary is closed.
		addEdge(constrainedEdges, previousVertex, firstVertex, details);
	}

	// Indirect overload for ConstrainedTrDetail<InputIterator2>
	template <typename InputIterator1, typename InputIterator2>
	void collateVertices(TriangleIterator firstTriangle, AdvancedVertexCtr& allVertices, EdgeSet& constrainedEdges,
		InputIterator1 verticesBegin, InputIterator1 verticesEnd, const ConstrainedTrDetails<InputIterator2>& details)
	{
		collateVerticesConstrained(firstTriangle, allVertices, constrainedEdges, verticesBegin, verticesEnd,
			details.constrainedEdgesBegin, details.constrainedEdgesEnd);
	}

	// Indirect overload for PolygonTrDetails
	template <typename InputIterator>
	void collateVertices(TriangleIterator firstTriangle, AdvancedVertexCtr& allVertices, EdgeSet& constrainedEdges,
		InputIterator verticesBegin, InputIterator verticesEnd, const PolygonTrDetails& details)
	{
		CollateVerticesPolygon(firstTriangle, allVertices, constrainedEdges, verticesBegin, verticesEnd, details);
	}

	// Indirect overload for PolygonTrDetails<OutputIterator>
	template <typename InputIterator, typename OutputIterator, class VertexType>
	void collateVertices(TriangleIterator firstTriangle, AdvancedVertexCtr& allVertices, EdgeSet& constrainedEdges,
		InputIterator verticesBegin, InputIterator verticesEnd, const PolygonOutputTrDetails<OutputIterator, VertexType>& details)
	{
		CollateVerticesPolygon(firstTriangle, allVertices, constrainedEdges, verticesBegin, verticesEnd, details);
	}

	template <class VertexType, typename InputIterator, typename OutputIterator>
	OutputIterator transformTriangles(InputIterator begin, InputIterator end, OutputIterator out)
	{
		for (; begin != end; ++begin)
		{
			const AdvancedTriangle& current = *begin;
		
			// Downcast to original vertex type (during the algorithm, we abandoned full type information)
			*out++ = Triangle<VertexType>(
				static_cast<const VertexType&>(current[0]),
				static_cast<const VertexType&>(current[1]),
				static_cast<const VertexType&>(current[2]));
		}
	
		return out;
	}

	template <typename InputIterator, typename OutputIterator, class AdditionalDetails>
	OutputIterator triangulateImpl(InputIterator verticesBegin, InputIterator verticesEnd, OutputIterator trianglesOut, const AdditionalDetails& details)
	{
		// Create container for all vertices with additional implementation data (allVertices) and for triangles
		VertexCtr			boundaryVertices;
		AdvancedVertexCtr	allVertices;
		TriangleList		triangles;
		EdgeSet				constrainedEdges;
	
		// Avoid reallocations (and thus iterator and reference invalidations), add first three boundary (dummy, as they're removed afterwards) vertices
		createBoundaryPoints(allVertices, boundaryVertices, triangles);
	
		// Bring vertices in ideal order for constrained Delaunay triangulation, and add constrained edges
		collateVertices(triangles.begin(), allVertices, constrainedEdges, verticesBegin, verticesEnd, details);

		// Set the positions of the boundary vertices, according to the spread of the internal vertices
		setBoundaryPositions(allVertices, boundaryVertices);
	
		// Insert each vertex. This invalidates the iterator first (because the old triangle is removed (split)).
		for (AdvancedVertexCtr::iterator itr = allVertices.begin() + 3, end = allVertices.end(); itr != end; ++itr) 
			insertPoint(triangles, *itr, boundaryVertices, constrainedEdges);
	
		// Remove triangles that are not contained in the final triangulation
		removeUnusedTriangles(triangles, boundaryVertices, constrainedEdges, AdditionalDetails::isPolygon);

		// Transform from algorithm-specific data structures to user interface
		typedef typename std::iterator_traits<InputIterator>::value_type UserVertex;
		return transformTriangles<UserVertex>(triangles.begin(), triangles.end(), trianglesOut);
	}

} // namespace detail

// ---------------------------------------------------------------------------------------------------------------------------


template <typename InputIterator, typename OutputIterator>
OutputIterator triangulate(InputIterator verticesBegin, InputIterator verticesEnd, OutputIterator trianglesOut)
{
	// Delaunay Triangulation == Constrained Delaunay Triangulation without constraining edges
	detail::EdgeSet noEdges;
	return triangulateConstrained(verticesBegin, verticesEnd, noEdges.begin(), noEdges.end(), trianglesOut);
}

template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator triangulateConstrained(InputIterator1 verticesBegin, InputIterator1 verticesEnd,
	InputIterator2 constrainedEdgesBegin, InputIterator2 constrainedEdgesEnd, OutputIterator trianglesOut)
{
	return detail::triangulateImpl(verticesBegin, verticesEnd, trianglesOut,
		detail::ConstrainedTrDetails<InputIterator2>(constrainedEdgesBegin, constrainedEdgesEnd));
}

template <typename InputIterator, typename OutputIterator>
OutputIterator triangulatePolygon(InputIterator verticesBegin, InputIterator verticesEnd, OutputIterator trianglesOut)
{
	return detail::triangulateImpl(verticesBegin, verticesEnd, trianglesOut,
		detail::PolygonTrDetails());
}

template <typename InputIterator, typename OutputIterator1, typename OutputIterator2>
OutputIterator1 triangulatePolygon(InputIterator verticesBegin, InputIterator verticesEnd, OutputIterator1 trianglesOut, OutputIterator2 edgesOut)
{
	return detail::triangulateImpl(verticesBegin, verticesEnd, trianglesOut, 
		detail::PolygonOutputTrDetails<OutputIterator2, typename std::iterator_traits<InputIterator>::value_type>(edgesOut));
}

} // namespace thor
