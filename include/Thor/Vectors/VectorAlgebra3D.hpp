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
/// @brief Algebraic operations on three-dimensional vectors (dot and cross product, vector length and angles, ...)
/// @details Here is an overview over the functionality:
/// <table>
///	 <tr><td>dotProduct(v,w)</td>				<td>length(v)</td>				<td>unitVector(v)</td>				</tr>
///	 <tr><td>crossProduct(v,w)</td>				<td>squaredLength(v)</td>		<td>toVector3(v)</td>				</tr>
///	 <tr><td>cwiseProduct(v,w)</td>		<td>polarAngle(v)</td>												</tr>
///	 <tr><td>cwiseQuotient(v,w)</td>	<td>elevationAngle(v)</td>											</tr>
/// </table>

#ifndef THOR_VECTORALGEBRA3D_HPP
#define THOR_VECTORALGEBRA3D_HPP

#include <Thor/Math/Trigonometry.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <cassert>


namespace thor
{

/// @addtogroup Vectors
/// @{

/// @brief Returns the length of the 3D vector.
///
template <typename T>
T							length(const sf::Vector3<T>& vector);

/// @brief Returns the square of @a vector's length.
/// @details Suitable for comparisons, more efficient than length().
template <typename T>
T							squaredLength(const sf::Vector3<T>& vector);

/// @brief Returns a vector with same direction as the argument, but with length 1.
/// @pre @a vector is no zero vector.
template <typename T>
sf::Vector3<T>				unitVector(const sf::Vector3<T>& vector);

/// @brief Returns the angle between the projection of @a vector onto the XY-plane and the X-axis.
/// @details Example: The vector (1,0,z) corresponds 0 degrees, (0,1,z) corresponds 90 degrees, where z is arbitrary.
/// @return Angle in degrees in the interval [-180,180].
/// @pre @a vector is no zero vector.
template <typename T>
T							polarAngle(const sf::Vector3<T>& vector);

/// @brief Returns the elevation angle of @a vector.
/// @details The elevation angle is the angle between the vector and the XY-plane. It has the same sign as @a vector.z.
/// @return Elevation angle in degrees in the interval [-90,90].
/// @pre @a vector is no zero vector.
template <typename T>
T							elevationAngle(const sf::Vector3<T>& vector);

// ---------------------------------------------------------------------------------------------------------------------------


/// @brief Computes the dot product of two 3D vectors.
///
template <typename T>
T							dotProduct(const sf::Vector3<T>& lhs, const sf::Vector3<T>& rhs);

/// @brief Computes the cross product of two 3D vectors.
///
template <typename T>
sf::Vector3<T>				crossProduct(const sf::Vector3<T>& lhs, const sf::Vector3<T>& rhs);

/// @brief Returns the component-wise product of @a lhs and @a rhs.
/// @details Computes <i>(lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z)</i>.
///  Component-wise multiplications are mainly used for scales.
template <typename T>
sf::Vector3<T>				cwiseProduct(const sf::Vector3<T>& lhs, const sf::Vector3<T>& rhs);

/// @brief Returns the component-wise quotient of @a lhs and @a rhs.
/// @details Computes <i>(lhs.x/rhs.x, lhs.y/rhs.y, lhs.z/rhs.z)</i>.
///  Component-wise divisions are mainly used for scales.
/// @pre Neither component of @a rhs is zero.
template <typename T>
sf::Vector3<T>				cwiseQuotient(const sf::Vector3<T>& lhs, const sf::Vector3<T>& rhs);

// ---------------------------------------------------------------------------------------------------------------------------


/// @brief Constructs a 3D vector from a 2D vector by setting the Z component to zero.
///
template <typename T>
sf::Vector3<T>				toVector3(const sf::Vector2<T>& vector2);

/// @}

} // namespace thor

#include <Thor/Vectors/Detail/VectorAlgebra3D.inl>
#endif // THOR_VECTORALGEBRA3D_HPP
