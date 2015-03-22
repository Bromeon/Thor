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
/// @brief Class template thor::PolarVector2

#ifndef THOR_POLARVECTOR2_HPP
#define THOR_POLARVECTOR2_HPP

#include <Thor/Vectors/VectorAlgebra2D.hpp>
#include <Thor/Math/Trigonometry.hpp>


namespace thor
{

/// @addtogroup Vectors
/// @{

/// @brief Vector in polar coordinate system
/// @details 2D vector which stores its components in polar instead of cartesian coordinates.
template <typename T>
struct PolarVector2
{
	T	r;				///< Radius
	T	phi;			///< Angle in degrees

	/// @brief Default constructor
	/// @details Creates a zero vector. The angle @a phi measures 0 degrees.
	PolarVector2();

	/// @brief Constructs a polar vector with specified radius and angle.
	/// @param radius The radial component (length of the vector).
	/// @param angle The angular component in degrees.
	PolarVector2(T radius, T angle);

	/// @brief Constructs a polar vector from a cartesian SFML vector.
	/// @param vector Cartesian (x,y) vector being converted to polar coordinates.
	///  Zero vectors are allowed and result in a polar vector with r=0 and phi=0.
	PolarVector2(const sf::Vector2<T>& vector);

	/// @brief Converts the polar vector into a cartesian SFML vector.
	/// @return Equivalent (x,y) vector in cartesian coordinates.
	operator sf::Vector2<T> () const;
};

/// @brief Type definition for float polar vectors
///
typedef PolarVector2<float> PolarVector2f;

/// @relates PolarVector2
/// @brief Returns the length of a polar vector.
/// @details The returned value is @a vector.r.
template <typename T>
T length(const PolarVector2<T>& vector);

/// @relates PolarVector2
/// @brief Returns the angle of a polar vector in degrees.
/// @details The returned value is @a vector.phi.
template <typename T>
T polarAngle(const PolarVector2<T>& vector);

/// @}

} // namespace thor

#include <Thor/Vectors/Detail/PolarVector2.inl>
#endif // THOR_POLARVECTOR2_HPP
