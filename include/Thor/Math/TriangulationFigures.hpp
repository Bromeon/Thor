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

/// @file
/// @brief Classes/templates thor::Vertex, thor::Edge, thor::Triangle

#ifndef THOR_TRIANGULATIONFIGURES_HPP
#define THOR_TRIANGULATIONFIGURES_HPP

#include <Thor/Config.hpp>

#include <SFML/System/Vector2.hpp>

#include AURORA_TR1_HEADER(array)
#include <cassert>


namespace thor
{

/// @addtogroup Math
/// @{

/// @brief Class that represents a point with X and Y coordinates.
/// @details Vertices are needed for algorithms like triangulations.
///  You can inherit this class, if you want to associate more informations than just the position.
class THOR_API Vertex
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructs a vertex with the specified position.
		///
		explicit					Vertex(sf::Vector2f position);
		
		/// @brief Constructs a vertex with the specified position.
		///
									Vertex(float x, float y);
	
		/// @brief Returns the vertex's position.
		///
		sf::Vector2f				getPosition() const;
			

	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		sf::Vector2f				mPosition;
};


/// @brief %Edge that contains two vertices (its endpoints)
/// @tparam VertexType The type of the vertices. Should be thor::Vertex or derived from thor::Vertex.
template <class VertexType>
class Edge
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief %Edge constructor	
		/// @details The vertices startPoint and endPoint form the two "corners" of the edge. They are only referenced and not copied.
									Edge(const VertexType& startPoint, const VertexType& endPoint);
				
		/// @brief Accesses a "corner" of the edge.
		/// @param cornerIndex The index of the corner. Must be either 0 or 1.
		/// @return Const-reference to the vertex at the specified corner.
		const VertexType&			operator[] (unsigned int cornerIndex) const;
		

	// ---------------------------------------------------------------------------------------------------------------------------
	// Protected variables
	protected:
		std::tr1::array<const VertexType*, 2> mCorners;
};


/// @brief %Triangle class consisting of 3 vertices. 
/// @details The invariant of this class is that the corners 0, 1, 2 are aligned in clockwise order.
/// @tparam VertexType The type of the vertices. Should be thor::Vertex or derived from thor::Vertex.
template <class VertexType>
class Triangle
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief %Triangle constructor.
		/// @details The vertices v0, v1 and v2 form the corners of the triangle. They are only referenced and not copied.
		///  v0, v1 and v2 shall always be clockwise-oriented.
									Triangle(const VertexType& v0, const VertexType& v1, const VertexType& v2);
			
		/// @brief Accesses a corner of the triangle.
		/// @param cornerIndex The index of the corner. Must be 0, 1 or 2.
		/// @return Const-reference to the vertex at the specified corner.
		const VertexType&			operator[] (unsigned int cornerIndex) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::tr1::array<const VertexType*, 3>	mCorners;
};

/// @}

} // namespace thor

#include <Thor/Detail/TriangulationFigures.inl>
#endif // THOR_TRIANGULATIONFIGURES_HPP
