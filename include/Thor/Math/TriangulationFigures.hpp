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

/// @file
/// @brief Class templates thor::Edge, thor::Triangle, thor::TriangulationTraits

#ifndef THOR_TRIANGULATIONFIGURES_HPP
#define THOR_TRIANGULATIONFIGURES_HPP

#include <Thor/Config.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <array>
#include <cassert>


namespace thor
{

/// @addtogroup Math
/// @{

/// @brief %Edge that contains two vertices (its endpoints or corners)
/// @tparam V The type of the vertices, for which the template thor::TriangulationTraits shall be specialized.
///  If @a V is const-qualified, the corners cannot be changed through this edge.
template <typename V>
class Edge
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief %Edge constructor
		/// @details The vertices startPoint and endPoint form the two "corners" of the edge. They are only referenced and not copied.
									Edge(V& corner0, V& corner1);

		/// @brief Accesses a corner of the edge.
		/// @param cornerIndex The index of the corner. Must be either 0 or 1.
		/// @return Reference to the vertex at the specified corner.
		V&							operator[] (std::size_t cornerIndex);

		/// @brief Accesses a corner of the edge (const overload).
		/// @param cornerIndex The index of the corner. Must be either 0 or 1.
		/// @return Const-reference to the vertex at the specified corner.
		const V&					operator[] (std::size_t cornerIndex) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Protected variables
	protected:
		std::array<V*, 2>			mCorners;
};


/// @brief %Triangle class consisting of 3 vertices (its corners).
/// @details The invariant of this class is that the corners 0, 1, 2 are aligned in clockwise order.
/// @tparam V The type of the vertices, for which the template thor::TriangulationTraits shall be specialized.
///  If @a V is const-qualified, the corners cannot be changed through this triangle.
template <typename V>
class Triangle
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief %Triangle constructor.
		/// @param corner0,corner1,corner2 form the corners of the triangle. They are only referenced and not copied.
		///  The corners shall always be clockwise-oriented.
									Triangle(V& corner0, V& corner1, V& corner2);

		/// @brief Accesses a corner of the triangle.
		/// @param cornerIndex The index of the corner. Must be 0, 1 or 2.
		/// @return Reference to the vertex at the specified corner.
		V&							operator[] (std::size_t cornerIndex);

		/// @brief Accesses a corner of the triangle (const overload).
		/// @param cornerIndex The index of the corner. Must be 0, 1 or 2.
		/// @return Const-reference to the vertex at the specified corner.
		const V&					operator[] (std::size_t cornerIndex) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::array<V*, 3>			mCorners;
};


/// @brief Traits template to implement the Vertex concept
/// @details Specializations of this class template shall contain a method with a signature compatible to the following one.
///  The returned value represents the position of the user-defined vertex.
/// @code
///  static sf::Vector2f getPosition(const V& vertex);
/// @endcode
/// By default, a member function <i>V::getPosition()</i> is invoked.
/// @n The traits template is already specialized for sf::Vector2f.
template <typename V>
struct TriangulationTraits
{
	static sf::Vector2f getPosition(const V& vertex)
	{
		return vertex.getPosition();
	}
};

// Triangulation traits: Specialization for sf::Vector2f
template <>
struct TriangulationTraits<sf::Vector2f>
{
	static sf::Vector2f getPosition(sf::Vector2f vertex)
	{
		return vertex;
	}
};

/// @}

} // namespace thor

#include <Thor/Math/Detail/TriangulationFigures.inl>
#endif // THOR_TRIANGULATIONFIGURES_HPP
