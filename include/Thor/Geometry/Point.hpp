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
/// @brief Class thor::Point

#ifndef THOR_POINT_HPP
#define THOR_POINT_HPP

#include <Thor/Geometry/Zone.hpp>


namespace thor
{

/// @addtogroup Geometry
/// @{

/// @brief Geometric point class.
/// 
class THOR_API Point : public Zone
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param position Position of the point.
		explicit					Point(sf::Vector2f position);

		/// @brief Constructor
		/// @param x,y Position of the point.
									Point(float x, float y);

		virtual	sf::Vector2f		getRandomPoint() const;

		virtual Zone*				clone() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		float						mRadius;
};

/// @}

} // namespace thor

#endif // THOR_POINT_HPP
