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
/// @brief Algebraic operations on two-dimensional vectors (dot product, vector length and angles, ...)
/// @details Here is an overview over the functionality:
/// <table>
///  <tr><th>Operation</th>				<th>Translation</th>		<th>Scale</th>				<th>Rotation</th>				</tr>
///	 <tr><td>Get</td>					<td>v</td>					<td>length(v)</td>			<td>polarAngle(v)</td>			</tr>
///	 <tr><td>Set, absolute</td>			<td>v = w</td>				<td>setLength(v,l)</td>		<td>setPolarAngle(v,a)</td>		</tr>
///	 <tr><td>Set, relative</td>			<td>v += w</td>				<td>v *= l</td>				<td>rotate(v,a)</td>			</tr>
///	 <tr><td>Copy, relative</td>		<td>v + w</td>				<td>v * l</td>				<td>rotatedVector(v,a)</td>		</tr>
/// </table>
/// Other functions:
/// <table>
///	 <tr><td>dotProduct(v,w)</td>		<td>unitVector(v)</td>					<td>squaredLength(v)</td>		</tr>
///	 <tr><td>crossProduct(v,w)</td>		<td>perpendicularVector(v)</td>			<td>signedAngle(v,w)</td>		</tr>
///	 <tr><td>cwiseProduct(v,w)</td>		<td>cwiseQuotient(v,w)</td>				<td>projectedVector(v,w)</td>	</tr>
/// </table>

#ifndef THOR_VECTORALGEBRA2D_HPP
#define THOR_VECTORALGEBRA2D_HPP

#include <Thor/Math/Trigonometry.hpp>

#include <SFML/System/Vector2.hpp>

#include <cassert>


namespace thor
{

/// @addtogroup Vectors
/// @{

/// @brief Returns the length of the 2D vector.
///
template <typename T>
T							length(const sf::Vector2<T>& vector);

/// @brief Returns the square of @a vector's length.
/// @details Suitable for comparisons, more efficient than length().
template <typename T>
T							squaredLength(const sf::Vector2<T>& vector);

/// @brief Adapts @a vector so that its length is |@a newLength| after this operation.
/// @details If @a newLength is less than zero, the vector's direction changes.
/// @pre @a vector is no zero vector.
template <typename T>
void						setLength(sf::Vector2<T>& vector, T newLength);

/// @brief Returns a vector with same direction as the argument, but with length 1.
/// @pre @a vector is no zero vector.
template <typename T>
sf::Vector2<T>				unitVector(const sf::Vector2<T>& vector);

/// @brief Returns the polar angle.
/// @details The vector (1,0) corresponds 0 degrees, (0,1) corresponds 90 degrees.
/// @return Angle in degrees in the interval [-180,180].
/// @pre @a vector is no zero vector.
template <typename T>
T							polarAngle(const sf::Vector2<T>& vector);

/// @brief Sets the polar angle of the specified vector.
/// @details The vector (1,0) corresponds 0 degrees, (0,1) corresponds 90 degrees.
template <typename T>
void						setPolarAngle(sf::Vector2<T>& vector, T newAngle);

/// @brief Rotates the vector by the given angle (in degrees).
/// @details The vector (1,0) corresponds 0 degrees, (0,1) corresponds 90 degrees.
template <typename T>
void						rotate(sf::Vector2<T>& vector, T angle);

/// @brief Returns a copy of the vector, rotated by @a angle degrees.
/// @details The vector (1,0) corresponds 0 degrees, (0,1) corresponds 90 degrees.
template <typename T>
sf::Vector2<T>				rotatedVector(const sf::Vector2<T>& vector, T angle);

/// @brief Returns a perpendicular vector.
/// @details Returns @a vector turned by 90 degrees counter clockwise; (x,y) becomes (-y,x).
///  For example, the vector (1,0) is transformed to (0,1).
template <typename T>
sf::Vector2<T>				perpendicularVector(const sf::Vector2<T>& vector);

// ---------------------------------------------------------------------------------------------------------------------------


/// @brief Computes the signed angle from @a lhs to @a rhs.
/// @return Angle in degrees in the interval [-180,180]. The angle determines how much you have to turn @a lhs
///  until it points to the same direction as @a rhs.
/// @pre Neither @a lhs nor @a rhs is a zero vector.
template <typename T>
T							signedAngle(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs);

/// @brief Computes the dot product of two 2D vectors.
///
template <typename T>
T							dotProduct(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs);

/// @brief Computes the cross product of two 2D vectors (Z component only).
/// @details Treats the operands as 3D vectors, computes their cross product and returns the result's Z component
///  (X and Y components are always zero).
template <typename T>
T							crossProduct(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs);

/// @brief Returns the component-wise product of @a lhs and @a rhs.
/// @details Computes <i>(lhs.x*rhs.x, lhs.y*rhs.y)</i>. Component-wise multiplications are mainly used for scales.
template <typename T>
sf::Vector2<T>				cwiseProduct(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs);

/// @brief Returns the component-wise quotient of @a lhs and @a rhs.
/// @details Computes <i>(lhs.x/rhs.x, lhs.y/rhs.y)</i>. Component-wise divisions are mainly used for scales.
/// @pre Neither component of @a rhs is zero.
template <typename T>
sf::Vector2<T>				cwiseQuotient(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs);

/// @brief Returns the projection of @a vector onto @a axis.
/// @param vector Vector to project onto another.
/// @param axis Vector being projected onto. Need not be a unit vector, but may not have length zero.
template <typename T>
sf::Vector2<T>				projectedVector(const sf::Vector2<T>& vector, const sf::Vector2<T>& axis);

/// @}

} // namespace thor

#include <Thor/Vectors/Detail/VectorAlgebra2D.inl>
#endif // THOR_VECTORALGEBRA2D_HPP
