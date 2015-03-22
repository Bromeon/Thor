/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2015 Jan Haller
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

/// @file
/// @brief Functions to triangulate set of points

#ifndef THOR_TRIANGULATION_HPP
#define THOR_TRIANGULATION_HPP

#include <SFML/System/Vector2.hpp>

#include <Thor/Math/TriangulationFigures.hpp>

#include <Aurora/Tools/ForEach.hpp>
#include <Aurora/SmartPtr/CopiedPtr.hpp>

#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <iterator>
#include <type_traits>
#ifndef NDEBUG
 #include <typeinfo> // for detail::AdvancedVertex::getUserVertex<V>()
#endif


namespace thor
{

/// @addtogroup Math
/// @{

/// @brief Delaunay Triangulation
/// @details Triangulates a set of points in a way such that every resulting triangle's circumcircle contains no other than
///  the own three points. This condition leads to a "beautiful" result, the triangles appear balanced.
/// @param verticesBegin,verticesEnd Iterator range to the points being triangulated. The element type V can be any type as long as
///  thor::TriangulationTraits<V> is specialized.
///  @n Note that the triangulation may meet problems at 4 co-circular points or at 3 or more collinear points.
/// @param trianglesOut Output iterator which is used to store the computed triangles. The elements shall be of type @ref thor::Triangle "thor::Triangle<V>",
///  where V is your (maybe const-qualified) vertex type. The resulting triangles reference the original vertices in [verticesBegin, verticesEnd[, so they must
///  not be destroyed as long as you access the triangles.
/// @return Output iterator after the last element written.
/// @details Const-correctness is propagated through the algorithm. That is, if @a InputIterator is a const_iterator, the triangle's template argument shall be const.
template <typename InputIterator, typename OutputIterator>
OutputIterator				triangulate(InputIterator verticesBegin, InputIterator verticesEnd, OutputIterator trianglesOut);

/// @brief Constrained Delaunay Triangulation
/// @details Performs a Delaunay triangulation while taking constraining edges into account. "Constrained" means edges
///  which are supposed to be part of the triangulation, locally ignoring the Delaunay condition.
/// @param verticesBegin,verticesEnd Iterator range to the points being triangulated. The element type V can be any type as long as
///  thor::TriangulationTraits<V> is specialized.
///  @n Note that the triangulation may meet problems at 4 co-circular points or at 3 or more collinear points.
/// @param constrainedEdgesBegin,constrainedEdgesEnd Iterator range to the constrained edges. The element type shall be @ref thor::Edge "thor::Edge<V>",
///  where T specifies your vertex type. The edges must refer to vertices inside the range [verticesBegin, verticesEnd[.
///  To get expected results, edges may not intersect (except at the end points; containing the same vertex is allowed).
/// @param trianglesOut Output iterator which is used to store the computed triangles. The elements shall be of type @ref thor::Triangle "thor::Triangle<V>",
///  where V is your vertex type. The resulting triangles reference the original vertices in [verticesBegin, verticesEnd[, so the vertices may
///  not be destroyed as long as you access the triangles.
/// @return Output iterator after the last element written.
/// @details Const-correctness is propagated through the algorithm. That is, if @a InputIterator is a const_iterator, the triangle's template argument shall be const.
template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator				triangulateConstrained(InputIterator1 verticesBegin, InputIterator1 verticesEnd,
								InputIterator2 constrainedEdgesBegin, InputIterator2 constrainedEdgesEnd, OutputIterator trianglesOut);

/// @brief Polygon Delaunay Triangulation
/// @details Computes a Delaunay triangulation of the inside of a polygon.
/// @param verticesBegin,verticesEnd Iterator range to the points being triangulated. The element type V can be any type as long as
///  thor::TriangulationTraits<V> is specialized. The order of the vertices is important, as the constrained edges are formed between consecutive
///  points (and between the last and first point). If the vertices lead to crossing edges, the result is undefined.
/// @param trianglesOut Output iterator which is used to store the computed triangles. The elements shall be of type @ref thor::Triangle "thor::Triangle<V>",
///  where V is your vertex type. The resulting triangles reference the original vertices in [verticesBegin, verticesEnd[, so the vertices may
///  not be destroyed as long as you access the triangles.
/// @return Output iterator after the last element written.
/// @details Const-correctness is propagated through the algorithm. That is, if @a InputIterator is a const_iterator, the triangle's template argument shall be const.
template <typename InputIterator, typename OutputIterator>
OutputIterator				triangulatePolygon(InputIterator verticesBegin, InputIterator verticesEnd, OutputIterator trianglesOut);

/// @brief Polygon Delaunay Triangulation
/// @details Computes a Delaunay triangulation of the inside of a polygon.
/// @param verticesBegin,verticesEnd Iterator range to the points being triangulated. The element type V can be any type as long as
///  thor::TriangulationTraits<V> is specialized. The order of the vertices is important, as the constrained edges are formed between consecutive
///  points (and between the last and first point). If the vertices lead to crossing edges, the result is undefined.
/// @param trianglesOut Output iterator which is used to store the computed triangles. The elements shall be of type @ref thor::Triangle "thor::Triangle<V>",
///  where V is your vertex type. The resulting triangles reference the original vertices in [verticesBegin, verticesEnd[, so the vertices may
///  not be destroyed as long as you access the triangles.
/// @param edgesOut Output iterator which can be used to store the outline of the polygon. Beginning at the edge between @a verticesBegin and
///  @a verticesBegin+1, every edge is sequentially written to @a edgesOut (the edge between the last and first point included).
/// @return Output iterator after the last element written.
/// @details Const-correctness is propagated through the algorithm. That is, if @a InputIterator is a const_iterator, the triangle's and edge's
///  template arguments shall be const.
template <typename InputIterator, typename OutputIterator1, typename OutputIterator2>
OutputIterator1				triangulatePolygon(InputIterator verticesBegin, InputIterator verticesEnd, OutputIterator1 trianglesOut, OutputIterator2 edgesOut);

/// @}

} // namespace thor

#include <Thor/Math/Detail/Triangulation.inl>
#endif // THOR_TRIANGULATION_HPP
