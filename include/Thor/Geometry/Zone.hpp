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
/// @brief Class thor::Zone

#ifndef THOR_ZONE_HPP
#define THOR_ZONE_HPP

#include <Thor/Geometry/Attributes2D.hpp>
#include <Thor/Config.hpp>

#include <SFML/System/Vector2.hpp>


namespace thor
{

/// @addtogroup Geometry
/// @{

/// @brief Abstract base class for geometric zones.
/// @details A zone represents an geometric figure on the screen, e.g. a rectangle, circle or just a point. Derive from this class
///  to implement your own zone.
class THOR_API Zone : public Positionable, public Rotatable
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Virtual destructor
		/// 
		virtual						~Zone();

		/// @brief Returns a random point inside the zone (uniformly distributed).
		/// @return %Point in global coordinate system (translation, rotation already applied).
		virtual	sf::Vector2f		getRandomPoint() const = 0;

		/// @brief Returns a copy of the dynamic object type, allocated with the new operator.
		///
		virtual Zone*				clone() const = 0;

	
	// ---------------------------------------------------------------------------------------------------------------------------
	// Protected member functions
	protected:
		/// @brief Base class constructor
		/// 
									Zone(sf::Vector2f position, float rotation);
		
		/// @brief Transforms a point according to the position and rotation of the zone.
		/// @param point %Point in unit coordinate system with origin (0, 0).
		/// @return %Point in a coordinate system rotated by the zone's rotation and offset by the zone's position (in this order).
		sf::Vector2f				transformToGlobal(sf::Vector2f point) const;
};

/// @}

} // namespace thor

#endif // THOR_ZONE_HPP
