/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2022 Jan Haller
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

	struct OptTriangleIterator;
	struct OptTriangleItrArray;

	// Functor to sort advanced vertices by their positions, needed to store them inside std::set.
	struct THOR_API CompareVertexPtrs
	{
		bool operator() (const AdvancedVertex* lhs, const AdvancedVertex* rhs) const;
	};

	// Functor to sort constrained edges by their positions, needed to store them inside std::set.
	struct THOR_API CompareEdges
	{
		bool operator() (const AdvancedEdge& lhs, const AdvancedEdge& rhs) const;
	};

	// Containers and iterators which don't depend on type directly
	typedef std::set<AdvancedVertex*, CompareVertexPtrs>	VertexPtrSet;
	typedef VertexPtrSet::iterator							VertexPtrIterator;

	// ---------------------------------------------------------------------------------------------------------------------------


	// Class to represent a vertex for algorithm internals
	class THOR_API AdvancedVertex
	{
		// Note: Public interface contains OptTriangleIterator instead of TriangleIterator, although iterators are always valid.
		// The reason is to avoid dependency on the STL container (TriangleIterator requires full typedef)
		public:
			template <typename V>
										AdvancedVertex(V& userVertex, OptTriangleIterator surroundingTriangle);
										AdvancedVertex(float x, float y);

			sf::Vector2f				getPosition() const;
			void						setSurroundingTriangle(OptTriangleIterator target);
			OptTriangleIterator			getSurroundingTriangle() const;

			template <typename V>
			V&							getUserVertex() const;

		private:
			void*									mUserVertex;
			sf::Vector2f							mPosition;
			aurora::CopiedPtr<OptTriangleIterator>	mSurroundingTriangle;
#ifndef NDEBUG
			const std::type_info*					mUserType;
#endif
	};

	// Meta-information edge, for algorithm internals
	class THOR_API AdvancedEdge : public Edge<AdvancedVertex>
	{
		public:
										AdvancedEdge(AdvancedVertex& startPoint, AdvancedVertex& endPoint);

		private:
			// Sorts the corners by vector-component-sum to allow faster access inside std::set.
			void						orderCorners();
	};

	// A triangle that carries advanced information in order to efficiently support the algorithm.
	class THOR_API AdvancedTriangle : public Triangle<AdvancedVertex>
	{
		public:
										AdvancedTriangle(AdvancedVertex& corner0, AdvancedVertex& corner1, AdvancedVertex& corner2);

			void						addVertex(AdvancedVertex& vertexRef);

			void						removeVertex(AdvancedVertex& vertexRef);
			void						removeVertex(VertexPtrIterator vertexItr);

			VertexPtrIterator			begin();
			VertexPtrIterator			end();

			// Sets/returns the adjacent triangle on the opposite side of the corner #index.
			void						setAdjacentTriangle(std::size_t index, const OptTriangleIterator& adjacentTriangle);
			OptTriangleIterator			getAdjacentTriangle(std::size_t index) const;

			// Marks this triangle (or removes mark)
			void						setFlagged(bool flagged);
			bool						isFlagged() const;

		private:
			VertexPtrSet							mRemainingVertices;
			aurora::CopiedPtr<OptTriangleItrArray>	mAdjacentTriangles;
			bool									mFlagged;
	};

	// ---------------------------------------------------------------------------------------------------------------------------


	// Type definitions of containers
	typedef std::deque<AdvancedVertex>						VertexCtr;
	typedef std::set<AdvancedEdge, CompareEdges>			EdgeSet;
	typedef std::list<AdvancedTriangle>						TriangleList;

	// Type definitions of often used iterators
	typedef TriangleList::iterator							TriangleIterator;

	// ---------------------------------------------------------------------------------------------------------------------------


	// Iterator to a triangle which can be explicitly invalid ("opt" stands for "optional").
	struct THOR_API OptTriangleIterator
	{
									OptTriangleIterator();
									OptTriangleIterator(TriangleIterator target);
									OptTriangleIterator(const OptTriangleIterator& origin);
		OptTriangleIterator&		operator= (const OptTriangleIterator& origin);

		bool						valid;
		TriangleIterator			target;
	};

	// Circle class, needed for circumcircle
	struct Circle
	{
									Circle(sf::Vector2f midPoint, float squaredRadius);

		sf::Vector2f				midPoint;
		float						squaredRadius;
	};

	// Metafunction to get a CV-qualified iterator value type (std::iterator_traits<T>::value_type is not const)
	template <typename T>
	struct DereferencedIterator
	{
		typedef typename std::remove_pointer<
			typename std::iterator_traits<T>::pointer
		>::type value_type;
	};

	// ---------------------------------------------------------------------------------------------------------------------------


	template <typename V>
	AdvancedVertex::AdvancedVertex(V& userVertex, OptTriangleIterator surroundingTriangle)
	: mUserVertex(const_cast<typename std::remove_const<V>::type*>(&userVertex))
	, mPosition(getVertexPosition(userVertex))
	, mSurroundingTriangle(aurora::makeCopied<OptTriangleIterator>(surroundingTriangle))
#ifndef NDEBUG
	, mUserType(&typeid(V))
#endif
	{
	}

	template <typename V>
	V& AdvancedVertex::getUserVertex() const
	{
		assert(typeid(V) == *mUserType);
		return *static_cast<V*>(mUserVertex);
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	// Function declarations required by the header
	void 				THOR_API insertPoint(TriangleList& triangles, AdvancedVertex& vertex, const AdvancedTriangle& boundaryTriangle,
									const EdgeSet& constrainedEdges);

	void				THOR_API removeUnusedTriangles(TriangleList& triangles, const AdvancedTriangle& boundaryTriangle,
									const EdgeSet& constrainedEdges, bool limitToPolygon);

	void 				THOR_API createBoundaryPoints(VertexCtr& allVertices, TriangleList& triangles);

	void 				THOR_API setBoundaryPositions(const VertexCtr& allVertices, AdvancedTriangle& boundaryTriangle);

	// ---------------------------------------------------------------------------------------------------------------------------


	// Policy class for small differences in triangulation - here for triangulateConstrained()
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

	// Policy class for small differences in triangulation - here for triangulatePolygon() without edgesOut parameter
	struct PolygonTrDetails
	{
		static const bool isPolygon = true;
	};

	// Policy class for small differences in triangulation - here for triangulatePolygon() with edgesOut parameter
	template <typename OutputIterator, typename UserVertex>
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


	// Returns the position of a user vertex.
	template <typename V>
	sf::Vector2f getVertexPosition(const V& vertex)
	{
		return TriangulationTraits<V>::getPosition(vertex);
	}

	// Sort out vertices according to their "importance". Vertices that are part of a constrained edge shall be inserted first.
	// Adds constrained edges as well.
	template <typename InputIterator1, typename InputIterator2>
	void collateVerticesConstrained(TriangleIterator firstTriangle, VertexCtr& allVertices, EdgeSet& constrainedEdges,
		InputIterator1 verticesBegin, InputIterator1 verticesEnd, InputIterator2 constrainedEdgesBegin, InputIterator2 constrainedEdgesEnd)
	{
		typedef typename DereferencedIterator<InputIterator1>::value_type UserVertex;
		typedef typename std::iterator_traits<InputIterator2>::value_type UserEdge;
		typedef std::set<UserVertex*> RawVertexPtrSet;

		// Store pointers to important vertices in importantVertices set
		RawVertexPtrSet importantVertices;
		for (InputIterator2 itr = constrainedEdgesBegin; itr != constrainedEdgesEnd; ++itr)
		{
			UserEdge& edge = *itr;

			importantVertices.insert(&edge[0]);
			importantVertices.insert(&edge[1]);
		}

		// Insert important advanced vertices, link to base triangle, fill user->advanced map
		std::map<UserVertex*, AdvancedVertex*> map;
		AURORA_FOREACH(UserVertex* userVertex, importantVertices)
		{
			allVertices.push_back(AdvancedVertex(*userVertex, firstTriangle));

			AdvancedVertex& advancedVertex = allVertices.back();
			firstTriangle->addVertex(advancedVertex);

			map.insert(std::make_pair(userVertex, &advancedVertex));
		}

		// Insert advanced edges, using the user->advanced map
		for (InputIterator2 itr = constrainedEdgesBegin; itr != constrainedEdgesEnd; ++itr)
		{
			UserEdge& userEdge = *itr;

			AdvancedEdge advancedEdge(*map[&userEdge[0]], *map[&userEdge[1]]);
			constrainedEdges.insert(advancedEdge);
		}

		// Insert other vertices, link to base triangle
		for (; verticesBegin != verticesEnd; ++verticesBegin)
		{
			UserVertex& userVertex = *verticesBegin;

			if (importantVertices.find(&userVertex) == importantVertices.end())
			{
				allVertices.push_back(detail::AdvancedVertex(userVertex, firstTriangle));
				firstTriangle->addVertex(allVertices.back());
			}
		}
	}

	// Helper function for collateVerticesPolygon(); constructs an edge and adds it to the container and output iterator.
	inline void addEdge(EdgeSet& constrainedEdges, AdvancedVertex* previousVertex, AdvancedVertex& currentVertex,
		PolygonTrDetails)
	{
		if (previousVertex != nullptr)
			constrainedEdges.insert(AdvancedEdge(*previousVertex, currentVertex));
	}

	// Overload for PolygonOutputTrDetails to write in an output iterator
	template <typename OutputIterator, typename UserVertex>
	void addEdge(EdgeSet& constrainedEdges, AdvancedVertex* previousVertex, AdvancedVertex& currentVertex,
		PolygonOutputTrDetails<OutputIterator, UserVertex> details)
	{
		if (previousVertex != nullptr)
		{
			*details.edgesOut++ = Edge<UserVertex>(
				previousVertex->getUserVertex<UserVertex>(),
				currentVertex.getUserVertex<UserVertex>());

			constrainedEdges.insert(AdvancedEdge(*previousVertex, currentVertex));
		}
	}

	// collateVertices() - Implementation for polygons
	template <typename InputIterator, class AdditionalDetails>
	void collateVerticesPolygon(TriangleIterator firstTriangle, VertexCtr& allVertices, EdgeSet& constrainedEdges,
		InputIterator verticesBegin, InputIterator verticesEnd, const AdditionalDetails& details)
	{
		// Empty vertex range: Do nothing
		if (verticesBegin == verticesEnd)
			return;

		AdvancedVertex* previousVertex = nullptr;

		for (; verticesBegin != verticesEnd; ++verticesBegin)
		{
			// Add the vertex to allVertices, link with firstTriangle
			allVertices.push_back(AdvancedVertex(*verticesBegin, firstTriangle));
			AdvancedVertex& vertex = allVertices.back();
			firstTriangle->addVertex(vertex);

			// Add edge of adjacent vertices
			addEdge(constrainedEdges, previousVertex, vertex, details);
			previousVertex = &vertex;
		}

		// If at least one vertex is contained, insert edge from last to first vertex, so that the boundary is closed.
		// First vertex is the one directly inserted after the three boundary vertices.
		AdvancedVertex& firstVertex = allVertices[3];
		addEdge(constrainedEdges, previousVertex, firstVertex, details);
	}

	// Indirect overload for ConstrainedTrDetail<InputIterator2>
	template <typename InputIterator1, typename InputIterator2>
	void collateVertices(TriangleIterator firstTriangle, VertexCtr& allVertices, EdgeSet& constrainedEdges,
		InputIterator1 verticesBegin, InputIterator1 verticesEnd, const ConstrainedTrDetails<InputIterator2>& details)
	{
		collateVerticesConstrained(firstTriangle, allVertices, constrainedEdges, verticesBegin, verticesEnd,
			details.constrainedEdgesBegin, details.constrainedEdgesEnd);
	}

	// Indirect overload for PolygonTrDetails
	template <typename InputIterator>
	void collateVertices(TriangleIterator firstTriangle, VertexCtr& allVertices, EdgeSet& constrainedEdges,
		InputIterator verticesBegin, InputIterator verticesEnd, const PolygonTrDetails& details)
	{
		collateVerticesPolygon(firstTriangle, allVertices, constrainedEdges, verticesBegin, verticesEnd, details);
	}

	// Indirect overload for PolygonOutputTrDetails
	template <typename InputIterator, typename OutputIterator, typename UserVertex>
	void collateVertices(TriangleIterator firstTriangle, VertexCtr& allVertices, EdgeSet& constrainedEdges,
		InputIterator verticesBegin, InputIterator verticesEnd, const PolygonOutputTrDetails<OutputIterator, UserVertex>& details)
	{
		collateVerticesPolygon(firstTriangle, allVertices, constrainedEdges, verticesBegin, verticesEnd, details);
	}

	template <typename UserVertex, typename InputIterator, typename OutputIterator>
	OutputIterator transformTriangles(InputIterator begin, InputIterator end, OutputIterator out)
	{
		for (; begin != end; ++begin)
		{
			const AdvancedTriangle& current = *begin;

			// Downcast to original vertex type (during the algorithm, we abandoned full type information)
			*out++ = Triangle<UserVertex>(
				current[0].getUserVertex<UserVertex>(),
				current[1].getUserVertex<UserVertex>(),
				current[2].getUserVertex<UserVertex>());
		}

		return out;
	}

	template <typename InputIterator, typename OutputIterator, class AdditionalDetails>
	OutputIterator triangulateImpl(InputIterator verticesBegin, InputIterator verticesEnd, OutputIterator trianglesOut, const AdditionalDetails& details)
	{
		// Create container for all vertices with additional implementation data (allVertices) and for triangles
		VertexCtr		allVertices;
		TriangleList	triangles;
		EdgeSet			constrainedEdges;

		// Avoid reallocations (and thus iterator and reference invalidations), add first three boundary (dummy, as they're removed afterwards) vertices
		createBoundaryPoints(allVertices, triangles);
		AdvancedTriangle boundaryTriangle = triangles.front();

		// Bring vertices in ideal order for constrained Delaunay triangulation, and add constrained edges
		collateVertices(triangles.begin(), allVertices, constrainedEdges, verticesBegin, verticesEnd, details);

		// Set the positions of the boundary vertices, according to the spread of the internal vertices
		setBoundaryPositions(allVertices, boundaryTriangle);

		// Insert each vertex. This invalidates the iterator first (because the old triangle is removed (split)).
		for (VertexCtr::iterator itr = allVertices.begin() + 3, end = allVertices.end(); itr != end; ++itr)
			insertPoint(triangles, *itr, boundaryTriangle, constrainedEdges);

		// Remove triangles that are not contained in the final triangulation
		removeUnusedTriangles(triangles, boundaryTriangle, constrainedEdges, AdditionalDetails::isPolygon);

		// Transform from algorithm-specific data structures to user interface
		typedef typename DereferencedIterator<InputIterator>::value_type UserVertex;
		return transformTriangles<UserVertex>(triangles.begin(), triangles.end(), trianglesOut);
	}

} // namespace detail

// ---------------------------------------------------------------------------------------------------------------------------


template <typename InputIterator, typename OutputIterator>
OutputIterator triangulate(InputIterator verticesBegin, InputIterator verticesEnd, OutputIterator trianglesOut)
{
	// Delaunay Triangulation == Constrained Delaunay Triangulation without constraining edges
	typedef typename detail::DereferencedIterator<InputIterator>::value_type UserVertex;

	std::vector<Edge<UserVertex>> noEdges;
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
		detail::PolygonOutputTrDetails<OutputIterator2, typename detail::DereferencedIterator<InputIterator>::value_type>(edgesOut));
}

} // namespace thor
