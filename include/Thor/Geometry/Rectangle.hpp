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
/// @brief Class thor::Rectangle

#ifndef THOR_RECTANGLE_HPP
#define THOR_RECTANGLE_HPP

#include <Thor/Geometry/Zone.hpp>

#include <SFML/Graphics/Rect.hpp>


namespace thor
{

/// @addtogroup Geometry
/// @{

/// @brief Geometric rectangle class.
/// 
class THOR_API Rectangle : public Zone
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor from SFML rect
		/// @param rect %Rectangle to adapt.
		/// @param rotation Initial rotation of the rectangle.
		explicit					Rectangle(const sf::FloatRect& rect, float rotation = 0.f);

		/// @brief Constructor from center and size
		/// @param centerPosition Position of the rectangle's center.
		/// @param size Width and height of the whole rectangle.
		/// @param rotation Initial rotation of the rectangle.
									Rectangle(sf::Vector2f centerPosition, sf::Vector2f size, float rotation = 0.f);
	
		/// @brief Constructor from center and size
		/// @param centerX,centerY Position of the rectangle's center.
		/// @param width,height Size of the whole rectangle.
		/// @param rotation Initial rotation of the rectangle.
									Rectangle(float centerX, float centerY, float width, float height, float rotation = 0.f);

		/// @brief Returns the size of the rectangle.
		/// 
		void						setSize(sf::Vector2f size);

		/// @brief Returns the size of the rectangle.
		/// 
		sf::Vector2f				getSize() const;

		virtual	sf::Vector2f		getRandomPoint() const;

		virtual Zone*				clone() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		sf::Vector2f				mSize;
};

/// @}

} // namespace thor

#endif // THOR_RECTANGLE_HPP
