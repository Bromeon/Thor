/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2013 Jan Haller
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
/// @brief Functors to create random distributions of geometric shapes

#ifndef THOR_DISTRIBUTIONS_HPP
#define THOR_DISTRIBUTIONS_HPP

#include <Thor/Math/Distribution.hpp>
#include <Thor/Config.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>


namespace thor
{

/// @addtogroup Math
/// @{ 

/// @brief Namespace for some predefined distribution functions
/// 
namespace Distributions
{

	/// @brief %Uniform random distribution in a float interval
	///
	thor::Distribution<float> THOR_API			uniform(float begin, float end);

	/// @brief %Uniform random distribution in a time interval
	///
	thor::Distribution<sf::Time> THOR_API		uniform(sf::Time begin, sf::Time end);

	/// @brief %Uniform random distribution in a rectangle
	/// 
	thor::Distribution<sf::Vector2f> THOR_API	rect(sf::Vector2f center, sf::Vector2f halfSize);

	/// @brief %Uniform random distribution in a circle
	/// 
	thor::Distribution<sf::Vector2f> THOR_API	circle(sf::Vector2f center, float radius);

	/// @brief Vector rotation with a random angle
	/// 
	thor::Distribution<sf::Vector2f> THOR_API	deflect(sf::Vector2f direction, float maxRotation);

} // namespace Distributions

/// @}

} // namespace thor

#endif // THOR_DISTRIBUTIONS_HPP
